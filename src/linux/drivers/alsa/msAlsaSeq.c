
/* MidiShare ALSA driver (derived from msRawMidi), 8-May-2004 Albert Graef */

/* Usage: msAlsaSeq [client-name] [-i id:port[/msport] | -o id:port[/msport]
   ...]. See msAlsaSeq(1) for details. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fnmatch.h>
#include <pthread.h> 
#include <alsa/asoundlib.h>

#include "MidiShare.h"
#include "EventToMidiStream.h"
#include "MidiStreamToEvent.h"

#define BUFSZ 1024
#define MAXBUFSZ 0x4000000 /* 64 MB */
#define MAXCONN 512

typedef struct {
  /* ALSA sequencer handle and port numbers */
  snd_seq_t		*seq_handle;
  int			in_port, out_port;

  /* ALSA event processing */
  snd_midi_event_t	*encoder;
  snd_midi_event_t	*decoder;

  int			bufsz;
  unsigned char		*buf;

  /* MidiShare client number, exit flag */
  short			ref;
  short			done;

  /* input thread and input filter */
  pthread_t		inThread;
  MidiFilterPtr		filter;

  /* MidiShare event processing */
  StreamFifo		inFifo;
  ParseMethodTbl	inMthd;
  Status2TypeTbl	inTypes;
	
  Ev2StreamRec		outFifo;
  LinearizeMthTbl	outMthd;

} DrvMem, *DrvMemPtr;

DrvMem gMem = { 0 };
static char *prog, *name;
static inline DrvMemPtr GetData() { return &gMem; }

static int n_inputs, n_outputs;
static char *inputs[MAXCONN], *outputs[MAXCONN];
static snd_seq_addr_t input_addr[MAXCONN], output_addr[MAXCONN];
static int have_ports, input_port[MAXCONN], output_port[MAXCONN];

static void fatal(const char *msg);

static int lookup(snd_seq_t *seq, char *name)
{
  snd_seq_client_info_t *info;
  snd_seq_client_info_alloca(&info);
  while (snd_seq_query_next_client(seq, info) >= 0)
    if (fnmatch(name, snd_seq_client_info_get_name(info), 0) == 0) {
      return snd_seq_client_info_get_client(info);
    }
  return -1;
}

static int parse_addr(snd_seq_t *seq, char *s0, snd_seq_addr_t *addr,
		      int *port)
{
  int id;
  char *s = alloca(strlen(s0)+1), *t, *end;

  strcpy(s, s0);
  if (!(t = strtok(s, ":")) || !*t)
    return -1;
  t = strtok(NULL, "/");
  if (!*t)
    return -1;
  id = strtol(t, &end, 0);
  if (!*end)
    addr->port = id;
  else
    return -1;
  id = strtol(s, &end, 0);
  if (!*end)
    addr->client = id;
  else if ((id = lookup(seq, s)) >= 0)
    addr->client = id;
  else
    return -1;
  t = strtok(NULL, "");
  if (!t || !*t)
    *port = -1;
  else {
    id = strtol(t, &end, 0);
    if (!*end && id >= 0 && id <= 255)
      *port = id;
    else
      return -1;
  }
  return 0;
}

static void subscribe(snd_seq_t *seq, snd_seq_addr_t *sender,
		      snd_seq_addr_t *dest)
{
  snd_seq_port_subscribe_t *subs;
  snd_seq_port_subscribe_alloca(&subs);
  snd_seq_port_subscribe_set_sender(subs, sender);
  snd_seq_port_subscribe_set_dest(subs, dest);
  snd_seq_subscribe_port(seq, subs);
}

static inline void initbuf(DrvMemPtr mem)
{
  mem->bufsz = BUFSZ;
  mem->buf = malloc(mem->bufsz);
  if (!mem->buf)
    fatal("Error creating MIDI buffer.");
  if (snd_midi_event_new(mem->bufsz, &mem->decoder) < 0)
    fatal("Error creating ALSA event decoder.");
  if (snd_midi_event_new(mem->bufsz, &mem->encoder) < 0)
    fatal("Error creating ALSA event encoder.");
}

static inline void checkbuf(DrvMemPtr mem, int sz)
{
  if (sz < 0 || sz > MAXBUFSZ)
    fatal("Error resizing MIDI buffer.");
  if (sz <= mem->bufsz)
    return;
  mem->bufsz = (sz/BUFSZ)*BUFSZ;
  if (sz%BUFSZ > 0) mem->bufsz += BUFSZ;
  mem->buf = realloc(mem->buf, mem->bufsz);
  if (!mem->buf)
    fatal("Error resizing MIDI buffer.");
  if (snd_midi_event_resize_buffer(mem->decoder, mem->bufsz) < 0)
    fatal("Error resizing ALSA event decoder.");
  if (snd_midi_event_resize_buffer(mem->encoder, mem->bufsz) < 0)
    fatal("Error resizing ALSA event encoder.");
}

static void InitAlsa(DrvMemPtr mem, char *name)
{
  snd_seq_client_info_t *info;
  snd_seq_addr_t this;
  int i;
  snd_seq_client_info_alloca(&info);
  /* open ALSA sequencer */
  if (snd_seq_open(&mem->seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0)
    fatal("Error creating ALSA client.");
  snd_seq_set_client_name(mem->seq_handle, name);
  /* create input and output ports (looks like ALSA allows us to create a
     single port with both read and write capability here) */
  if ((mem->in_port = snd_seq_create_simple_port
       (mem->seq_handle, name,
	SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ|
	SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
	SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
    fatal("Error creating ALSA input port.");
  mem->out_port = mem->in_port;
  /* set up event processing */
  initbuf(mem);
  snd_midi_event_no_status(mem->decoder, 1);
  snd_midi_event_no_status(mem->encoder, 1);
  /* establish connections */
  this.client = snd_seq_client_id(mem->seq_handle);
  this.port = mem->in_port;
  for (i = 0; i < n_inputs; i++) {
    input_port[i] = -1;
    if (parse_addr(mem->seq_handle, inputs[i], &input_addr[i],
		   &input_port[i]) ||
	snd_seq_get_any_client_info(mem->seq_handle, input_addr[i].client,
				    info) < 0)
      fprintf(stderr, "WARNING: Unknown input address `%s'.\n", inputs[i]);
    else {
      if (input_port[i] >= 0) {
	have_ports = 1;
	printf("Connecting input \"%s\" (%d:%d) to MidiShare port %d.\n",
	       snd_seq_client_info_get_name(info),
	       input_addr[i].client, input_addr[i].port, input_port[i]);
      } else
	printf("Connecting input \"%s\" (%d:%d).\n",
	       snd_seq_client_info_get_name(info),
	       input_addr[i].client, input_addr[i].port);
      subscribe(mem->seq_handle, &input_addr[i], &this);
    }
  }
  this.port = mem->out_port;
  for (i = 0; i < n_outputs; i++) {
    output_port[i] = -1;
    if (parse_addr(mem->seq_handle, outputs[i], &output_addr[i],
		   &output_port[i]) ||
	snd_seq_get_any_client_info(mem->seq_handle, output_addr[i].client,
				    info) < 0)
      fprintf(stderr, "WARNING: Unknown output address `%s'.\n", outputs[i]);
    else {
      if (output_port[i] >= 0) {
	have_ports = 1;
	printf("Connecting output \"%s\" (%d:%d) to MidiShare port %d.\n",
	       snd_seq_client_info_get_name(info),
	       output_addr[i].client, output_addr[i].port, output_port[i]);
      } else
	printf("Connecting output \"%s\" (%d:%d).\n",
	       snd_seq_client_info_get_name(info),
	       output_addr[i].client, output_addr[i].port);
      subscribe(mem->seq_handle, &this, &output_addr[i]);
    }
  }
}

static inline int midi_in(DrvMemPtr mem)
{
  Ev2StreamPtr f = &mem->outFifo;
  unsigned char c;
  int i = 0;
  /* decode MidiSHare event */
  while (MidiStreamGetByte(f, &c)) {
    checkbuf(mem, i+1);
    mem->buf[i++] = c;
  }
  return i;
}

static inline void alsa_out(DrvMemPtr mem, long count, int port)
{
  snd_seq_event_t ev;
  if (count <= 0) return;
  /* encode as ALSA sequencer event */
  snd_midi_event_reset_encode(mem->encoder);
  if (snd_midi_event_encode(mem->encoder, mem->buf, count, &ev) >= 0) {
    /* send ALSA sequencer event */
    snd_seq_ev_set_source(&ev, mem->out_port);
    snd_seq_ev_set_direct(&ev);
    if (have_ports) {
      int i;
      /* map MidiShare port to ALSA destinations */
      for (i = 0; i < n_outputs; i++)
	if (output_port[i] == port) {
	  snd_seq_ev_set_dest(&ev, output_addr[i].client, output_addr[i].port);
	  snd_seq_event_output_direct(mem->seq_handle, &ev);
	}
    } else {
      snd_seq_ev_set_subs(&ev);
      snd_seq_event_output_direct(mem->seq_handle, &ev);
    }
  }
}

static void KeyOffTask(long date, short refNum, long a1,long a2,long a3)
{
  MidiEvPtr e = (MidiEvPtr)a1;
  DrvMemPtr mem = (DrvMemPtr)a2;
  Ev2StreamPtr f = &mem->outFifo;
  long count;

  MidiStreamPutEvent(f, e);
  count = midi_in(mem);
  alsa_out(mem, count, e->port);
}

static void RcvAlarm(short ref)
{
  DrvMemPtr mem = GetData();
  Ev2StreamPtr f = &mem->outFifo;
  MidiEvPtr off, e = MidiGetEv(ref);
  long count;
  
  while (e) {
    /* decode MidiShare event */
    off = MidiStreamPutEvent(f, e);
    count = midi_in(mem);
    alsa_out(mem, count, e->port);
    if (off) MidiTask(KeyOffTask, Date(off), ref, (long)off, (long)mem, 0);
    e = MidiGetEv(ref);
  }
}

static void InitMidiShare(DrvMemPtr mem, char *name)
{
  short i;
  /* create MidiShare client */
  mem->ref = MidiOpen(name);
  if (mem->ref < 0) fatal("Error creating MidiShare client (MidiOpen).");
  /* create and set up filter */
  mem->filter = MidiNewFilter();
  if (!mem->filter) fatal("Error creating MidiShare filter (MidiNewFilter).");
  for (i = 0; i < 256; i++) {
    MidiAcceptPort(mem->filter, i, true);
    MidiAcceptType(mem->filter, i, false);
  }
  for (i = 0; i < 16; i++)
    MidiAcceptChan(mem->filter, i, true);
  for (i = 0; i <= typeProcess; i++)
    MidiAcceptType(mem->filter, i, true);
  for (i = typeQuarterFrame; i <= typeRegParam; i++)
    MidiAcceptType(mem->filter, i, true);
  MidiSetFilter(mem->ref, mem->filter);
  /* set up event processing */
  MidiParseInitMthTbl(mem->inMthd);
  MidiParseInitTypeTbl(mem->inTypes);
  MidiParseInit(&mem->inFifo, mem->inMthd, mem->inTypes);
  MidiStreamInitMthTbl(mem->outMthd);
  MidiStreamInit(&mem->outFifo, mem->outMthd);
  /* install input handler */
  MidiSetRcvAlarm(mem->ref, RcvAlarm);
}

static void Init(DrvMemPtr mem, char *name)
{
  InitAlsa(mem, name); InitMidiShare(mem, name);
}

static void *inputThread(void *ptr)
{
  DrvMemPtr mem = (DrvMemPtr)ptr;
  StreamFifoPtr f = &mem->inFifo;
  int npfd;
  struct pollfd *pfd;
  snd_seq_event_t *ev;
  long count;

  /* poll ALSA descriptor for incoming MIDI messages */
  npfd = snd_seq_poll_descriptors_count(mem->seq_handle, POLLIN);
  pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
  snd_seq_poll_descriptors(mem->seq_handle, pfd, npfd, POLLIN);
  while (!mem->done) {
    if (poll(pfd, npfd, 100000) > 0) {
      do {
	/* get ALSA sequencer event and decode */
	snd_seq_event_input(mem->seq_handle, &ev);
	snd_midi_event_reset_decode(mem->decoder);
	while ((count = snd_midi_event_decode(mem->decoder, mem->buf,
					      mem->bufsz, ev)) == -ENOMEM &&
	       mem->bufsz <= MAXBUFSZ) {
	  /* try to enlarge the buffer */
	  checkbuf(mem, mem->bufsz+BUFSZ);
	  snd_midi_event_reset_decode(mem->decoder);
	}
	if (count > 0) {
	  /* encode as MidiShare event */
	  int i;
	  for (i = 0; i < count; i++) {
	    MidiEvPtr e = MidiParseByte(f, mem->buf[i]);
	    if (e) {
	      if (have_ports) {
		/* translate ALSA source to corresponding MidiShare port */
		int port = -1, j;
		for (j = 0; j < n_inputs; j++)
		  if (ev->source.client == input_addr[j].client &&
		      ev->source.port == input_addr[j].port &&
		      input_port[j] >= 0) {
		    port = input_port[j];
		    break;
		  }
		if (port >= 0) {
		  e->port = port;
		  MidiSend(mem->ref, e);
		}
	      } else
		MidiSend(mem->ref, e);
	    }
	  }
	}
	snd_seq_free_event(ev);
      } while (snd_seq_event_input_pending(mem->seq_handle, 0) > 0);
    }
  }
  return 0;
}

static void InitThread(DrvMemPtr mem)
{
  struct sched_param param;  
  int  err, ret = pthread_create(&mem->inThread, NULL, inputThread, mem);
  uid_t uid = getuid();

  param.sched_priority = 99; /* 0 to 99  */
  setuid(name_to_uid("root")); 
  err = pthread_setschedparam(mem->inThread, SCHED_RR,  &param); 
  setuid(uid);

  if (ret) {
    char str[256];
    sprintf(str, "Error creating input thread (pthread_create, code = %d).",
	    ret);
    fatal(str);		
  }
}

static void ExitAlsa(DrvMemPtr mem)
{
  if (mem->seq_handle) {
    snd_seq_close(mem->seq_handle);
    mem->seq_handle = NULL;
  }
  if (mem->decoder) {
    snd_midi_event_free(mem->decoder);
    mem->decoder = NULL;
  }
  if (mem->encoder) {
    snd_midi_event_free(mem->encoder);
    mem->encoder = NULL;
  }
}

static void ExitMidiShare(DrvMemPtr mem)
{
  if (mem->ref > 0) {
    MidiClose(mem->ref);
    mem->ref = 0;
  }
  if (mem->filter) {
    MidiFreeFilter(mem->filter);
    mem->filter = NULL;
  }
}

static void Exit(DrvMemPtr mem)
{
  ExitAlsa(mem); ExitMidiShare(mem);
}

static void fatal(const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog, msg);
  Exit(GetData());
  exit(1);
}

static void usage(void)
{
  fprintf(stderr, "Usage: %s [client-name] [-i id:port[/msport] | -o id:port[/msport] ...]\n",
	  prog);
  exit(1);
}

static int check_quit(void)
{
  char c = getc(stdin);
  return (c == 'q') || (c == 'Q');
}

int main(int argc, char *argv[])
{
  DrvMemPtr mem = GetData();
  void *ret;
  int c;
  
  prog = argv[0];
  while ((c = getopt(argc, argv, "hi:o:")) != -1)
    switch (c) {
    case 'h':
      usage();
      break;
    case 'i':
      if (n_inputs >= MAXCONN)
	fatal("Too many inputs.");
      else 
	inputs[n_inputs++] = optarg;
      break;
    case 'o':
      if (n_outputs >= MAXCONN)
	fatal("Too many outputs.");
      else
	outputs[n_outputs++] = optarg;
      break;
    default:
      exit(1);
    }
  if (argc <= optind)
    name = "MidiShare/ALSA Bridge";
  else if (optind+1 < argc)
    usage();
  else
    name = argv[optind];
  /* initialize */
  if (!MidiShare()) fatal("MidiShare is not installed.");
  Init(mem, name);
  InitThread(mem);
  /* interactive loop */
  printf("MidiShare ALSA driver anchored on \"%s\" is running.\n", name);
  printf("Type 'q' to quit.\n");
  while (!mem->done)
    mem->done = check_quit();
  /* finalize */
  pthread_cancel(mem->inThread);
  pthread_join(mem->inThread, &ret);
  Exit(mem);
  return 0;        
}
