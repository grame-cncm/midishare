/*    From Jacques.Herry@wanadoo.fr */
/*            Jacques Herry */
/*           35, resid Corlay */
/*           22300 Lannion */
/*            France */
/*  	Grame  MidiPlayer for schemers. */
/*  "Welcome to Grame MidiShare(c)extension player, version 1.6.22 Copyright(c)2001 Jacques Herry" */


#include <sys/unistd.h>
#include "include/MidiShare.h"
#include "include/escheme.h"
#include "include/player.h"
#include "include/mstype.h"


#ifndef __UseMSThunking__

/* here callback for real-time */
/* I initialize one vector of 63 procedure (one for each refNum possible) */
/* myLinkScheme is always the alarmproc */
/* which point on  vector container of box  */
/* which is a scheme procedure unboxed when alarm is here */
/* (midi-set-rcv-alarm ref-num procedure) */
/* is the command, the proc contain no arg  */


void MSALARMAPI  myLinkScheme( short);


/* here we just get the name of the proc for the refNum */
static Scheme_Object *sch_getRcvProc(int refNum)
{ /* return procedure with counterpart (vector-ref midircv-alarm-proc  refNum) */
   if  ((refNum < 0) && (refNum < 63))
    scheme_signal_error("vector 'midi-rcv-alarm-proc' have only 63 slots and refNum <%d> is not valide\n",refNum);
  return (SCHEME_VEC_ELS(scheme_lookup_global(scheme_intern_symbol("midi-rcv-alarm-proc"),
					      scheme_get_env(scheme_config)))[refNum]);
 
} 

 /* this function is allway called by Alarm interrupt with refNum in arg */
/* I have to know better the thread mechanism if I want this to work */
void MSALARMAPI  myLinkScheme( short RefNum)
{ /* we keep refNum and send this to proc in box */
  Scheme_Object *msAlarm;
  msAlarm = (Scheme_Object *)scheme_malloc_atomic(sizeof(msAlarm));
  msAlarm = scheme_thread( SCHEME_BOX_VAL(sch_getRcvProc(RefNum)),NULL);
  SCHEME_VEC_ELS(scheme_lookup_global(scheme_intern_symbol("midi-rcv-alarm-threads"),
				      scheme_get_env(scheme_config)))[RefNum]= msAlarm;
  return;
}

/* just to verify the proc alarm is ok */
/* we run a new thread and we get the threads id for later use */
static  Scheme_Object *sch_testRcvProc(int argc, Scheme_Object **argv)
{ 
  if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-tst-rcv-proc", "integer ref-num", 0, argc, argv);
  
   scheme_eval( SCHEME_BOX_VAL(sch_getRcvProc(SCHEME_INT_VAL(argv[0]))),scheme_get_env(scheme_config));
  return scheme_void;
} 

/* I box the proc for getting the name (without box I have procedure but not the name of proc) in vector */
static Scheme_Object *sch_setRcvAlarm(int argc, Scheme_Object **argv)
{  
  int ref;
  Scheme_Object *box;
  if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-set-rcv-alarm!", "integer ref-num", 0, argc, argv);
   ref = SCHEME_INT_VAL(argv[0]); 
  if (!SCHEME_PAIRP(argv[1]))
   scheme_wrong_type("midi-set-rcv-alarm!", "quoted list of scheme procedure(s) without arg  ", 1, argc, argv);
  if ( ( 62 < ref) && (ref < 0))
  scheme_signal_error("the vector of name : 'midi-rcv-alarm-proc' have 63 slots and refnum <%d> is not valide\n",ref);
/* assign the proc on vector numref */
  box = (Scheme_Object *) scheme_malloc(sizeof(Scheme_Object));
  box = scheme_box(argv[1]); 
  SCHEME_VEC_ELS(scheme_lookup_global(scheme_intern_symbol("midi-rcv-alarm-proc"),
				      scheme_get_env(scheme_config)))[ref]= box; /* , scheme_void;  */
  MidiSetRcvAlarm((short)ref,myLinkScheme);
  return scheme_void;
}

/* we need this because the scheme implementation don't work directly with address pointer */
static  Scheme_Object *sch_stopRcvAlarm(int argc, Scheme_Object **argv)
{ 
   if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-stop-rcv-alarm", "integer ref-num", 0, argc, argv);
   MidiSetRcvAlarm((short)SCHEME_INT_VAL(argv[0]),NULL);
  
   return scheme_void;
} 


static Scheme_Object *sch_getRcvAlarm(int argc, Scheme_Object **argv)
{
   if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-get-rcv-alarm", "integer ref-num", 0, argc, argv);
   return sch_getRcvProc(SCHEME_INT_VAL(argv[0]));
}

#endif


/* adapted from forum PLT on struct implement in C */ 
 
Scheme_Object *new_scheme_struct (Scheme_Env* env, const char* basename, int
num_fields, char** field_names,int flags)
{  int i,count_out;
   
 Scheme_Object *new_type, **vals;
 Scheme_Object **struct_names;

 Scheme_Object** a = (Scheme_Object**)  scheme_malloc(num_fields*sizeof(Scheme_Object*)); 

 for ( i = 0; i< num_fields; ++i){
   a[i] = (Scheme_Object*) scheme_intern_symbol(field_names[i]);
 }
 
 new_type = scheme_make_struct_type(scheme_intern_symbol(basename),
				    NULL /*super_type*/, 
				    num_fields); 
 struct_names =   scheme_make_struct_names(scheme_intern_symbol(basename), 
					   scheme_build_list(num_fields,a), 
					   flags  , 
					   &count_out); 

 vals = scheme_make_struct_values(new_type, struct_names, count_out, flags); 
 for ( i=0; i < count_out; ++i) 
   scheme_add_global_symbol(struct_names[i],vals[i],env);

 return new_type;
}


static Scheme_Object *sch_version(int argc, Scheme_Object **argv)
{
  return scheme_make_integer(MidiGetVersion());
}


static Scheme_Object *sch_midiShare(int argc, Scheme_Object **argv)
{
  return MidiShare() ? scheme_true  : scheme_false;
}


static Scheme_Object *sch_countAppls(int argc, Scheme_Object **argv)
{
  return scheme_make_integer(MidiCountAppls());
}


static Scheme_Object *sch_getIndAppl(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-get-ind-appl", "integer", 0, argc, argv);
  return scheme_make_integer(MidiGetIndAppl((short)SCHEME_INT_VAL(argv[0])));
}


static Scheme_Object *sch_getNamedAppl(int argc, Scheme_Object **argv)
{   MidiName v;
 if (!SCHEME_STRINGP(argv[0]))
   scheme_wrong_type("midi-get-named-appl", "string", 0, argc, argv);
 v = SCHEME_STR_VAL(argv[0]);
 return scheme_make_integer(MidiGetNamedAppl(v));
}

/*----------------------------- SMPTE synchronization -------------------------*/
/* here is the getSyncInfo for the entire midiShare process => no arg*/
/* we don't need pointer here because this is only a readable value */
static Scheme_Object *sch_getSyncInfo(int argc, Scheme_Object **argv)
{
  Scheme_Object  *a[11];
  SyncInfoPtr s = NULL;
  if (argc)
    scheme_signal_error("midi-get-sync-info procedure require no arg and return scheme struct sync-info");
  s = (SyncInfoPtr )scheme_malloc_atomic(sizeof(TSyncInfo));
  MidiGetSyncInfo(s);
  a[0]=scheme_make_integer_value(s->time);
  a[1]= scheme_make_integer_value(s->reenter);
  a[2]=scheme_make_integer_value_from_unsigned((unsigned long) s->syncMode);
  a[3]=scheme_make_integer((long) s->syncLocked);
  a[4]=scheme_make_integer((long) s->syncPort);
  a[5]=scheme_make_integer(s->syncStart);
  a[6]=scheme_make_integer(s->syncStop);
  a[7]=scheme_make_integer(s->syncOffset);
  a[8]=scheme_make_integer(s->syncSpeed);
  a[9]=scheme_make_integer(s->syncBreaks);
  a[10]=scheme_make_integer((long)s->syncFormat);
  return scheme_make_struct_instance(sync_info_struct_type,sizeray(a),a);
}   


static Scheme_Object *sch_setSyncMode(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-set-sync-mode!", "integer mode", 0, argc, argv);
  MidiSetSyncMode((unsigned short)SCHEME_INT_VAL(argv[0]));
  return scheme_void;
}

/* the time of the tape in the current seq */
static Scheme_Object *sch_getExtTime(int argc, Scheme_Object **argv)
{
   if (argc)
    scheme_signal_error("for midi-get-ext-time  no arg please");
  return scheme_make_integer_value(MidiGetExtTime());

}   

static Scheme_Object *sch_int2ExtTime(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-int->ext-time", "integer internal-time", 0, argc, argv);
  return scheme_make_integer_value(MidiInt2ExtTime(SCHEME_INT_VAL(argv[0])));
 
}

static Scheme_Object *sch_ext2IntTime(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-ext->int-time", "integer external-time", 0, argc, argv);
  return scheme_make_integer_value(MidiExt2IntTime(SCHEME_INT_VAL(argv[0]))); 
}

static Scheme_Object *sch_time2Smpte(int argc, Scheme_Object **argv)
{     
   TSmpteLocation *ts;
  Scheme_Object *scmSmpte; 
  shSmpteLoc *locTypePtr;
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-time->smpte", "integer time ", 0, argc, argv);
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-time->smpte", "integer format ", 1, argc, argv);
 if (! scheme_is_struct_instance((Scheme_Object *)  smpte_loc_struct_type, (Scheme_Object *) argv[2]))
   scheme_wrong_type("midi-time->smpte", "smpte-loc", 2, argc, argv);
 locTypePtr = GET_SMPTE(argv[2]);
 ts = locTypePtr->smpte_loc_ptr;
 scmSmpte =argv[2];
  MidiTime2Smpte(SCHEME_INT_VAL(argv[0]),SCHEME_INT_VAL(argv[1]),ts);

  /* we can get now the values from the smpte c-struct filled and set values in scm-struct*/
  /* provided in arg */
  scheme_struct_set(scmSmpte,0,scheme_make_integer(ts->format));
  scheme_struct_set(scmSmpte,1,scheme_make_integer(ts->hours));
  scheme_struct_set(scmSmpte,2,scheme_make_integer(ts->minutes));
  scheme_struct_set(scmSmpte,3,scheme_make_integer(ts->seconds));
  scheme_struct_set(scmSmpte,4,scheme_make_integer(ts->frames));
  scheme_struct_set(scmSmpte,5,scheme_make_integer(ts->fracs));

  locTypePtr->smpte_loc_ptr = ts;
  scheme_struct_set(scmSmpte,6,(Scheme_Object *)locTypePtr);
  return scheme_void;
}

/* you should provide smpte scm-struct to this proc */
/* (midi-smpte->time  your-smpte-struct)) */
static Scheme_Object *sch_smpte2Time(int argc, Scheme_Object **argv)
{    
   shSmpteLoc *locTypePtr;
   if (! scheme_is_struct_instance((Scheme_Object *)  smpte_loc_struct_type, (Scheme_Object *) argv[0]))
   scheme_wrong_type("midi-smpte->time", "smpte-loc", 0, argc, argv);
  return  scheme_make_integer_value(MidiSmpte2Time(GET_SMPTE_PTR(argv[0])));
  
}

/* you should provide the 6 required params for this struct and 7th 0*/
/* I return a previously allocated loc struct for later uses */
static Scheme_Object *sch_makeSmpteLoc(int argc, Scheme_Object **argv)
{  
  Scheme_Object *a[7];
  int i;
  TSmpteLocation *smpte;
  shSmpteLoc *locTypePtr; 
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("make-smpte-loc", "integer format", 0, argc, argv);
if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("make-smpte-loc", "integer hours", 1, argc, argv);
if (!SCHEME_INTP(argv[2]))
   scheme_wrong_type("make-smpte-loc", "integer minutes", 2, argc, argv);
if (!SCHEME_INTP(argv[3]))
   scheme_wrong_type("make-smpte-loc", "integer seconds", 3, argc, argv);
if (!SCHEME_INTP(argv[4]))
   scheme_wrong_type("make-smpte-loc", "integer frames", 4, argc, argv);
if (!SCHEME_INTP(argv[5]))
   scheme_wrong_type("make-smpte-loc", "integer fracs", 5, argc, argv);
  /* first alloc the memory for the c-struct */
  smpte = (TSmpteLocation *) scheme_malloc_atomic(sizeof(TSmpteLocation));
  smpte->format = (short)  SCHEME_INT_VAL(argv[0]);
  smpte->hours = (short)   SCHEME_INT_VAL(argv[1]);
  smpte->minutes = (short) SCHEME_INT_VAL(argv[2]);
  smpte->seconds = (short) SCHEME_INT_VAL(argv[3]);
  smpte->frames = (short)  SCHEME_INT_VAL(argv[4]);
  smpte->fracs = (short)   SCHEME_INT_VAL(argv[5]);

  /* the pointer on the memory location go to the scm-record  */
  /* and the type loc is set */
 locTypePtr=(shSmpteLoc *) scheme_malloc_atomic(sizeof(shSmpteLoc));   
 locTypePtr->type= smpte_loc_type;
 locTypePtr->smpte_loc_ptr = smpte;
/* copy of args on a for making struct */
  for (i=0;i<7;i++) {a[i]=argv[i];}
 /* here the c-struct */
 a[6]=(Scheme_Object *)locTypePtr;
  return scheme_make_struct_instance(smpte_loc_struct_type,sizeray(a),a);
}


static Scheme_Object *sch_open(int argc, Scheme_Object **argv)
{   MidiName v;
 if (!SCHEME_STRINGP(argv[0]))
   scheme_wrong_type("midi-open", "string", 0, argc, argv);
 v = SCHEME_STR_VAL(argv[0]);
 return scheme_make_integer(MidiOpen(v));
}

static Scheme_Object *sch_close(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-close", "integer", 0, argc, argv);
  MidiClose((short)SCHEME_INT_VAL(argv[0]));
  return scheme_void;
}


static Scheme_Object *sch_getName(int argc, Scheme_Object **argv)
{   
 MidiName v;
 short nEnter = (short) SCHEME_INT_VAL(argv[0]);
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-get-name", "integer", 0, argc, argv);
 if ((nEnter < 0) || (nEnter >= MidiCountAppls())) /* test valide nAppl */
   scheme_wrong_type("midi-get-name", "     positif and < MidiCountAppls        ", 0, argc, argv);
 v= MidiGetName(nEnter);
 return scheme_make_string(v);
}


static Scheme_Object *sch_setName(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-set-name!", "integer", 0, argc, argv);
    
  if (!SCHEME_STRINGP(argv[1]))
    scheme_wrong_type("midi-set-name!", "string", 1, argc, argv);

  MidiSetName((short)SCHEME_INT_VAL(argv[0]), SCHEME_STR_VAL(argv[1]));
  return scheme_void;
}


/***** to do : setInfo, getInfo and Filter functions ********/

static Scheme_Object *sch_getFilter(int argc, Scheme_Object **argv)
{  shFilter *sf;
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-get-filter", "integer ref-num", 0, argc, argv);
 sf = (shFilter *)scheme_malloc(sizeof(shFilter));
 sf->type = filter_type;
 sf->filter_ptr = MidiGetFilter((short)SCHEME_INT_VAL(argv[0]));
 return (Scheme_Object *) sf;
}


static Scheme_Object *sch_setFilter(int argc, Scheme_Object **argv)
{  shFilter *sf;
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-set-filter!", "integer ref-num", 0, argc, argv);
 if (SCHEME_TYPE(argv[1]) != filter_type)
   scheme_wrong_type("midi-set-filter!", "midi-filter", 1, argc, argv);
 sf = (shFilter *) argv[1];
 MidiSetFilter((short)SCHEME_INT_VAL(argv[0]), sf->filter_ptr);
 return scheme_void;
}


Scheme_Object *sch_newFilter(int argc, Scheme_Object **argv)
{   
  shFilter *sf;
  if (argc)
    scheme_wrong_type("midi-new-filter", "no arg ", 0, argc, argv);
  sf = (shFilter *)scheme_malloc(sizeof(shFilter));
  sf->type = filter_type;
  sf->filter_ptr = MidiNewFilter();
  return (Scheme_Object *) sf; 
}


static Scheme_Object *sch_freeFilter(int argc, Scheme_Object **argv)
{  
 shFilter *sf;
 if (SCHEME_TYPE(argv[0]) != filter_type)
   scheme_wrong_type("midi-free-filter", "midi-filter", 0, argc, argv);
 sf = (shFilter *) argv[0];
 MidiFreeFilter( sf->filter_ptr);
 sf->type = filter_type_killed ;
 return scheme_void;
}


static Scheme_Object *sch_acceptPort(int argc, Scheme_Object **argv)
{  
 shFilter *sf;
  if (SCHEME_TYPE(argv[0]) != filter_type)
   scheme_wrong_type("midi-accept-port", "midi-filter", 0, argc, argv);
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-accept-port", "integer port", 1, argc, argv);
 if (!SCHEME_BOOLP(argv[2]))
   scheme_wrong_type("midi-accept-port", "boolean", 2, argc, argv);
 sf = (shFilter *) argv[0];
 
 MidiAcceptPort(sf->filter_ptr,(short)SCHEME_INT_VAL(argv[1]), (BOOLEAN)SCHEME_TRUEP(argv[2]) );
 return scheme_void;
}


static Scheme_Object *sch_acceptChan(int argc, Scheme_Object **argv)
{  
 shFilter *sf;
 if (SCHEME_TYPE(argv[0]) != filter_type)
   scheme_wrong_type("midi-accept-chan", "midi-filter", 0, argc, argv);
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-accept-chan", "integer port", 1, argc, argv);
 if (!SCHEME_BOOLP(argv[2]))
   scheme_wrong_type("midi-accept-chan", "boolean", 2, argc, argv);
 sf = (shFilter *) argv[0];
 MidiAcceptChan(sf->filter_ptr,(short)SCHEME_INT_VAL(argv[1]),(BOOLEAN) SCHEME_TRUEP(argv[2]) );
 return scheme_void;
}


static Scheme_Object *sch_acceptType(int argc, Scheme_Object **argv)
{  
 shFilter *sf;
 if (SCHEME_TYPE(argv[0]) != filter_type)
   scheme_wrong_type("midi-accept-type", "midi-filter", 0, argc, argv);
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-accept-type", "integer port", 1, argc, argv);
 if (!SCHEME_BOOLP(argv[2]))
   scheme_wrong_type("midi-accept-type", "boolean", 2, argc, argv);
 sf = (shFilter *) argv[0];
 MidiAcceptType(sf->filter_ptr,(short)SCHEME_INT_VAL(argv[1]),(BOOLEAN) SCHEME_TRUEP(argv[2]) );
 return scheme_void;
}


static Scheme_Object *sch_isAcceptedType(int argc, Scheme_Object **argv)
{  
 shFilter *sf;
 if (SCHEME_TYPE(argv[0]) != filter_type)
   scheme_wrong_type("midi-accepted-type?", "midi-filter", 0, argc, argv);
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-accepted-type?", "integer type", 1, argc, argv);
  sf = (shFilter *) argv[0];
 return (MidiIsAcceptedType(sf->filter_ptr,(short)SCHEME_INT_VAL(argv[1])) ? scheme_true  : scheme_false);
}


static Scheme_Object *sch_isAcceptedChan(int argc, Scheme_Object **argv)
{  
 shFilter *sf;
 if (SCHEME_TYPE(argv[0]) != filter_type)
   scheme_wrong_type("midi-accepted-chan?", "midi-filter", 0, argc, argv);
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-accepted-chan?", "integer chan", 1, argc, argv);
  sf = (shFilter *) argv[0];
 return (MidiIsAcceptedChan(sf->filter_ptr,(short)SCHEME_INT_VAL(argv[1])) ? scheme_true  : scheme_false);
}


static Scheme_Object *sch_isAcceptedPort(int argc, Scheme_Object **argv)
{  
 shFilter *sf;
 if (SCHEME_TYPE(argv[0]) != filter_type)
   scheme_wrong_type("midi-accepted-port?", "midi-filter", 0, argc, argv);
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-accepted-port?", "integer port", 1, argc, argv);
  sf = (shFilter *) argv[0];
 return (MidiIsAcceptedPort(sf->filter_ptr,(short)SCHEME_INT_VAL(argv[1])) ? scheme_true  : scheme_false);
}

static Scheme_Object *sch_connect(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-connect", "integer", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-connect", "integer", 1, argc, argv);

  if (!SCHEME_BOOLP(argv[2]))
    scheme_wrong_type("midi-connect", "boolean", 2, argc, argv);

  MidiConnect((short) SCHEME_INT_VAL(argv[0]),(short) SCHEME_INT_VAL(argv[1]),(BOOLEAN) SCHEME_TRUEP(argv[2]));
  return scheme_void; 
}


static Scheme_Object *sch_isConnected(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-connected?", "integer src ", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-connected?", "integer dest", 1, argc, argv);
  return (MidiIsConnected((short)SCHEME_INT_VAL(argv[0]),
			  (short) SCHEME_INT_VAL(argv[1])) ? scheme_true : scheme_false);
}

static Scheme_Object *sch_getPortState(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-get-port-state", "integer port ", 0, argc, argv);
  return (MidiGetPortState((short)SCHEME_INT_VAL(argv[0])) ? scheme_true : scheme_false);
}


static Scheme_Object *sch_setPortState(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-set-port-state!", "integer port ", 0, argc, argv);
  if (!SCHEME_BOOLP(argv[1]))
    scheme_wrong_type("midi-set-port-state!", "boolean state ", 1, argc, argv);
  MidiSetPortState((short)SCHEME_INT_VAL(argv[0]),(BOOLEAN) SCHEME_TRUEP(argv[1]));
  return scheme_void;
}


static Scheme_Object *sch_freeSpace(int argc, Scheme_Object **argv)
{   
  return scheme_make_integer_value_from_unsigned(MidiFreeSpace());
}

static Scheme_Object *sch_totalSpace(int argc, Scheme_Object **argv)
{   
  return scheme_make_integer_value_from_unsigned(MidiTotalSpace());
}

static Scheme_Object *sch_growSpace(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-grow-space", "integer", 0, argc, argv);
    
  return scheme_make_integer_value_from_unsigned(MidiGrowSpace((long)SCHEME_INT_VAL(argv[0])));
}


 Scheme_Object *sch_newCell(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;
  if (argc)
    scheme_wrong_type("midi-new-cell", "no arg", 0, argc, argv);
  sne = (shNewEv *)scheme_malloc(sizeof(shNewEv));
  sne->type = ev_type;
  sne->midi_ev_ptr = MidiNewCell();
  return (Scheme_Object *) sne;   
}


 Scheme_Object *sch_freeCell(int argc, Scheme_Object **argv)
{   
  shNewEv *oe;
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-free-cell", "midi-event", 0, argc, argv);
  oe = (shNewEv *) argv[0];
  MidiFreeCell(oe->midi_ev_ptr);
  oe->type = ev_type_killed ;
  return scheme_void;    
}


 Scheme_Object *sch_newEv(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;
	
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-new-ev", "integer", 0, argc, argv);
  sne = (shNewEv *)scheme_malloc(sizeof(shNewEv));
  sne->type = ev_type;
  sne->midi_ev_ptr = MidiNewEv((short)SCHEME_INT_VAL(argv[0]));
  return (Scheme_Object *) sne ;    
}


 Scheme_Object *sch_isEv(int argc, Scheme_Object **argv)
{   
  shNewEv *oe;
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-ev?", "midi-event", 0, argc, argv);
  oe = (shNewEv *)argv[0];
  return oe->midi_ev_ptr ? scheme_true : scheme_false ;
}


 Scheme_Object *sch_copyEv(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;
  shNewEv *oe;
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-copy-ev", "midi-event", 0, argc, argv);
  oe = (shNewEv *)argv[0];
  sne = (shNewEv *)scheme_malloc(sizeof(shNewEv));
  sne->type = ev_type;
  sne->midi_ev_ptr = MidiCopyEv(oe->midi_ev_ptr);
  return (Scheme_Object *) sne;
}

/* events memory is managed by midi-share */
  Scheme_Object *sch_freeEv(int argc, Scheme_Object **argv)
{   
  shNewEv *oe;
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-free-ev", "midi-event", 0, argc, argv);
  oe = (shNewEv *) argv[0];
  MidiFreeEv(oe->midi_ev_ptr);
  oe->type = ev_type_killed ;
  return scheme_void;
}

static Scheme_Object *sch_setField(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-set-field!", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-set-field!", "integer  => index number of field", 1, argc, argv);
  if (!SCHEME_INTP(argv[2]))
    scheme_wrong_type("midi-set-field!", "integer  => affected value", 2, argc, argv);
  MidiSetField(sne->midi_ev_ptr, SCHEME_INT_VAL(argv[1]),SCHEME_INT_VAL(argv[2]));
  return scheme_void;
}


static Scheme_Object *sch_getField(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-get-field", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-get-field", "integer  => index number of field", 1, argc, argv);
	
  return scheme_make_integer(MidiGetField(sne->midi_ev_ptr, SCHEME_INT_VAL(argv[1])));    
}


/* mainly for sysex */
static Scheme_Object *sch_addField(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-add-field", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-add-field", "integer  value of field", 1, argc, argv);
  MidiAddField(sne->midi_ev_ptr, SCHEME_INT_VAL(argv[1]));
  return scheme_void;
    
}

static Scheme_Object *sch_countFields(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-count-fields", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  return scheme_make_integer(MidiCountFields(sne->midi_ev_ptr));	
}


static Scheme_Object *sch_getLink(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;
  shNewEv *oe;
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-get-link", "midi-event", 0, argc, argv);
  oe = (shNewEv *)argv[0];
  sne = (shNewEv *)scheme_malloc(sizeof(shNewEv));
  sne->type = ev_type;
  sne->midi_ev_ptr = MidiGetLink(oe->midi_ev_ptr);
  return (Scheme_Object *) sne;    
}


static Scheme_Object *sch_setLink(int argc, Scheme_Object **argv)
{   
  shNewEv *e;
  shNewEv *next;
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-set-link!", "midi-event e ", 0, argc, argv);
  e = (shNewEv *)argv[0];
  if (SCHEME_TYPE(argv[1]) != ev_type)
    scheme_wrong_type("midi-set-link!", "midi-event  next ", 0, argc, argv);
  next = (shNewEv *)argv[1];
  MidiSetLink(e->midi_ev_ptr,next->midi_ev_ptr);
  return scheme_void;    
}

static Scheme_Object *sch_setDate(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-set-date!", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-set-date!", "integer  => date", 1, argc, argv);
  MidiSetDate(sne->midi_ev_ptr, SCHEME_INT_VAL(argv[1]));
  return scheme_void;    
}


static Scheme_Object *sch_getDate(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-date", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer_value(MidiGetDate(sne->midi_ev_ptr));	
}


static Scheme_Object *sch_setRefNum(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-set-ref-num!", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-set-ref-num!", "integer  => refNum", 1, argc, argv);
	
  MidiSetRefNum(sne->midi_ev_ptr, (short)SCHEME_INT_VAL(argv[1]));
  return scheme_void;    
}

static Scheme_Object *sch_getRefNum(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-ref-num", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer(MidiGetRefNum(sne->midi_ev_ptr));	
}


static Scheme_Object *sch_setType(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-set-type!", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-set-type!", "integer  => type", 1, argc, argv);
	
 MidiSetType(sne->midi_ev_ptr,(short) SCHEME_INT_VAL(argv[1]));
 return scheme_void;    
}


static Scheme_Object *sch_getType(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-type", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer(MidiGetType(sne->midi_ev_ptr));	
}



static Scheme_Object *sch_setChan(int argc, Scheme_Object **argv)
{   
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-set-chan!", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-set-chan!", "integer  => chan", 1, argc, argv);
 MidiSetChan(sne->midi_ev_ptr,(short) SCHEME_INT_VAL(argv[1]));
 return scheme_void;    
}


static Scheme_Object *sch_getChan(int argc, Scheme_Object **argv)
{   
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-chan", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer(MidiGetChan(sne->midi_ev_ptr));	
}


static Scheme_Object *sch_setPort(int argc, Scheme_Object **argv)
{   
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-set-port!", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("midi-set-port!", "integer  => port", 1, argc, argv);
	
 MidiSetPort(sne->midi_ev_ptr,(short) SCHEME_INT_VAL(argv[1]));
 return scheme_void;    
}


static Scheme_Object *sch_getPort(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-port", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer(MidiGetPort(sne->midi_ev_ptr));
	
}

static Scheme_Object *sch_getData0(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-data0", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer(MidiGetData0(sne->midi_ev_ptr));
	
}

static Scheme_Object *sch_getData1(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-data1", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer(MidiGetData1(sne->midi_ev_ptr));
	
}

static Scheme_Object *sch_getData2(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-data2", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer(MidiGetData2(sne->midi_ev_ptr));
}

static Scheme_Object *sch_getData3(int argc, Scheme_Object **argv)
{    
 shNewEv *sne;
 if (SCHEME_TYPE(argv[0]) != ev_type)
   scheme_wrong_type("midi-get-data3", "midi-event", 0, argc, argv);
 sne = (shNewEv *) argv[0];
 return scheme_make_integer(MidiGetData3(sne->midi_ev_ptr));
}

static Scheme_Object *sch_setData0(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-set-data0!", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-set-data0!", "integer  => data", 1, argc, argv);
  MidiSetData0(sne->midi_ev_ptr, SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}

static Scheme_Object *sch_setData1(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-set-data1!", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-set-data1!", "(integer) byte  => data1", 1, argc, argv);
  MidiSetData1(sne->midi_ev_ptr, SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}

static Scheme_Object *sch_setData2(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-set-data2!", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-set-data2!", "(integer) byte  => data2", 1, argc, argv);
  MidiSetData2(sne->midi_ev_ptr, SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}

static Scheme_Object *sch_setData3(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != ev_type)
    scheme_wrong_type("midi-set-data3!", "midi-event", 0, argc, argv);
  sne = (shNewEv *) argv[0];
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("midi-set-data3!", "(integer) byte => data3", 1, argc, argv);
  MidiSetData3(sne->midi_ev_ptr, SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}


Scheme_Object *sch_newSeq(int argc, Scheme_Object **argv)
{   
  shNewSeq *sns;
  if (argc)
    scheme_wrong_type("midi-new-seq", "no arg", 0, argc, argv);
  sns = (shNewSeq *)scheme_malloc(sizeof(shNewSeq));
  sns->type = seq_type;
  sns->midi_seq_ptr = MidiNewSeq();
  return (Scheme_Object *) sns;
}


 Scheme_Object *sch_addSeq(int argc, Scheme_Object **argv)
{   
  shNewSeq *os;
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != seq_type)
    scheme_wrong_type("midi-add-seq", "midi-seq", 0, argc, argv);
   if (SCHEME_TYPE(argv[1]) != ev_type)
    scheme_wrong_type("midi-add-seq", "midi-event", 1, argc, argv);
  sne = (shNewEv *) argv[1];
  os = (shNewSeq *) argv[0];
  MidiAddSeq(os->midi_seq_ptr,sne->midi_ev_ptr);
  
  return scheme_void;
}


 Scheme_Object *sch_freeSeq(int argc, Scheme_Object **argv)
{   
  shNewSeq *os;
  if (SCHEME_TYPE(argv[0]) != seq_type)
    scheme_wrong_type("midi-free-seq", "midi-seq", 0, argc, argv);
  os = (shNewSeq *) argv[0];
  MidiFreeSeq(os->midi_seq_ptr);
  os->type = seq_type_killed ;

  return scheme_void;
}

 Scheme_Object *sch_clearSeq(int argc, Scheme_Object **argv)
{   
  shNewSeq *os;
  if (SCHEME_TYPE(argv[0]) != seq_type)
    scheme_wrong_type("midi-clear-seq", "midi-seq", 0, argc, argv);
  os = (shNewSeq *) argv[0];
  MidiClearSeq(os->midi_seq_ptr);
  return scheme_void;
}


 Scheme_Object *sch_getFirstEv(int argc, Scheme_Object **argv)
{   
  shNewSeq *os;
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != seq_type)
    scheme_wrong_type("midi-get-first-ev", "midi-seq", 0, argc, argv);
  os = (shNewSeq *) argv[0];
  sne = (shNewEv *)scheme_malloc(sizeof(shNewEv));
  sne->type = ev_type;
  sne->midi_ev_ptr= MidiGetFirstEv(os->midi_seq_ptr);
  return (Scheme_Object *) sne;
}


 Scheme_Object *sch_setFirstEv(int argc, Scheme_Object **argv)
{   
  shNewSeq *os;
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != seq_type)
    scheme_wrong_type("midi-set-first-ev!", "midi-seq", 0, argc, argv);
  if (SCHEME_TYPE(argv[1]) != ev_type)
    scheme_wrong_type("midi-set-first-ev!", "midi-event", 1, argc, argv);
  sne = (shNewEv *) argv[1];
  os = (shNewSeq *) argv[0];
  MidiSetFirstEv(os->midi_seq_ptr,sne->midi_ev_ptr);
  return scheme_void;
    
}


 Scheme_Object *sch_getLastEv(int argc, Scheme_Object **argv)
{   
  shNewSeq *os;
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != seq_type)
    scheme_wrong_type("midi-get-last-ev", "midi-seq", 0, argc, argv);
  os = (shNewSeq *) argv[0];
  sne = (shNewEv *)scheme_malloc(sizeof(shNewEv));
  sne->type = ev_type;
  sne->midi_ev_ptr= MidiGetLastEv(os->midi_seq_ptr);
  return (Scheme_Object *) sne;
}

 Scheme_Object *sch_setLastEv(int argc, Scheme_Object **argv)
{   
  shNewSeq *os;
  shNewEv *sne;	
  if (SCHEME_TYPE(argv[0]) != seq_type)
    scheme_wrong_type("midi-set-last-ev!", "midi-seq", 0, argc, argv);
  if (SCHEME_TYPE(argv[1]) != ev_type)
    scheme_wrong_type("midi-set-last-ev!", "midi-event", 1, argc, argv);
  sne = (shNewEv *) argv[1];
  os = (shNewSeq *) argv[0];
  MidiSetLastEv(os->midi_seq_ptr,sne->midi_ev_ptr);
  return scheme_void;
}


static Scheme_Object *sch_getTime(int argc, Scheme_Object **argv)
{   
  return scheme_make_integer_value_from_unsigned(MidiGetTime());
}



static Scheme_Object *sch_sendIm(int argc, Scheme_Object **argv)
{   
 shNewEv *sne;
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-send-im", "integer  => refNum ", 0, argc, argv);
 if (SCHEME_TYPE(argv[1]) != ev_type)
   scheme_wrong_type("midi-send-im", "midi-event", 1, argc, argv);
 sne = (shNewEv *) argv[1];
 MidiSendIm((short) SCHEME_INT_VAL(argv[0]),sne->midi_ev_ptr);
 return scheme_void;
}


static Scheme_Object *sch_send(int argc, Scheme_Object **argv)
{   
 shNewEv *sne;
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-send", "integer  => refNum ", 0, argc, argv);
 if (SCHEME_TYPE(argv[1]) != ev_type)
   scheme_wrong_type("midi-send", "midi-event", 1, argc, argv);
 sne = (shNewEv *) argv[1];
 MidiSend((short)SCHEME_INT_VAL(argv[0]),sne->midi_ev_ptr);
 return scheme_void;
}



static Scheme_Object *sch_sendAt(int argc, Scheme_Object **argv)
{   
 shNewEv *sne;
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("midi-send-at", "integer  => refNum ", 0, argc, argv);
 if (SCHEME_TYPE(argv[1]) != ev_type)
   scheme_wrong_type("midi-send-at", "midi-event", 1, argc, argv);
 if (!SCHEME_INTP(argv[2]))
   scheme_wrong_type("midi-send-at", "integer  => date", 2, argc, argv);
 sne = (shNewEv *) argv[1];
 MidiSendAt((short)SCHEME_INT_VAL(argv[0]), sne->midi_ev_ptr,SCHEME_INT_VAL(argv[2]));
 return scheme_void;
}


/** reception ***/
static Scheme_Object *sch_countEvs(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-count-evs", "integer  *ref-num* ", 0, argc, argv);
  return scheme_make_integer_value_from_unsigned(MidiCountEvs((short)SCHEME_INT_VAL(argv[0])));
}


static Scheme_Object *sch_getEv(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-get-ev", "integer  *ref-num*", 0, argc, argv);
  sne = (shNewEv *)scheme_malloc(sizeof(shNewEv));
  sne->type = ev_type;
  sne->midi_ev_ptr = MidiGetEv((short)SCHEME_INT_VAL(argv[0]));
  if (0 !=  sne->midi_ev_ptr)
    {return (Scheme_Object *) sne;}
  else
    {return scheme_false;}
}


 static Scheme_Object *sch_availEv(int argc, Scheme_Object **argv)
{   
  shNewEv *sne;
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-avail-ev", "integer   *ref-num*", 0, argc, argv);
  sne = (shNewEv *)scheme_malloc(sizeof(shNewEv));
  sne->type = ev_type;
  sne->midi_ev_ptr = MidiAvailEv((short)SCHEME_INT_VAL(argv[0]));
  return (Scheme_Object *) sne;
}


 static Scheme_Object *sch_flushEvs(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("midi-flush-evs", "integer *ref-num*", 0, argc, argv);
  MidiFlushEvs((short)SCHEME_INT_VAL(argv[0]));
  return scheme_void;
}


Scheme_Object *scheme_reload(Scheme_Env *env)
{
  scheme_add_global("midi-get-version", scheme_make_prim_w_arity(sch_version, "midi-get-version", 0, 0), env);
  scheme_add_global("midi-share?", scheme_make_prim_w_arity(sch_midiShare, "midi-share?", 0, 0), env);
  scheme_add_global("midi-get-ind-appl", scheme_make_prim_w_arity(sch_getIndAppl,"midi-get-ind-appl", 1, 1), env);
  scheme_add_global("midi-count-appls", scheme_make_prim_w_arity(sch_countAppls, "midi-count-appls", 0, 0), env);
  scheme_add_global("midi-get-named-appl", scheme_make_prim_w_arity(sch_getNamedAppl,"midi-get-named-appl", 1, 1), env);
 
  scheme_add_global("midi-get-sync-info", scheme_make_prim_w_arity(sch_getSyncInfo, "midi-get-sync-info", 0, 0), env);
  scheme_add_global("midi-set-sync-mode!", scheme_make_prim_w_arity(sch_setSyncMode,"midi-set-sync-mode!", 1, 1), env);
  scheme_add_global("midi-get-ext-time", scheme_make_prim_w_arity(sch_getExtTime, "midi-get-ext-time", 0, 0), env);
  scheme_add_global("midi-int->ext-time" , scheme_make_prim_w_arity(sch_int2ExtTime,"midi-int->ext-time", 1, 1), env);
  scheme_add_global("midi-ext->int-time" , scheme_make_prim_w_arity(sch_ext2IntTime,"midi-ext->int-time", 1, 1), env);
  scheme_add_global("midi-time->smpte" , scheme_make_prim_w_arity(sch_time2Smpte,"midi-time->smpte", 3, 3), env);
  scheme_add_global("midi-smpte->time" , scheme_make_prim_w_arity(sch_smpte2Time,"midi-smpte->time", 1, 1), env);
                
  scheme_add_global("make-smpte-loc" , scheme_make_prim_w_arity(sch_makeSmpteLoc,"make-smpte-loc", 6, 6), env);

  scheme_add_global("midi-open", scheme_make_prim_w_arity(sch_open,"midi-open", 1, 1), env);
  scheme_add_global("midi-close", scheme_make_prim_w_arity(sch_close,"midi-close", 1, 1), env);
  scheme_add_global("midi-get-name", scheme_make_prim_w_arity(sch_getName,"midi-get-name", 1, 1), env);
  scheme_add_global("midi-set-name!", scheme_make_prim_w_arity(sch_setName,"midi-set-name!", 2, 2), env);

  scheme_add_global("midi-get-filter", scheme_make_prim_w_arity(sch_getFilter,"midi-get-filter", 1, 1), env);
  scheme_add_global("midi-set-filter!", scheme_make_prim_w_arity(sch_setFilter,"midi-set-filter!", 2, 2), env);
  scheme_add_global("midi-new-filter", scheme_make_prim_w_arity(sch_newFilter,"midi-new-filter", 0, 0), env);
  scheme_add_global("midi-free-filter", scheme_make_prim_w_arity(sch_freeFilter,"midi-free-filter", 1, 1), env);
  scheme_add_global("midi-accept-port", scheme_make_prim_w_arity(sch_acceptPort,"midi-accept-port", 3, 3), env);
  scheme_add_global("midi-accept-chan", scheme_make_prim_w_arity(sch_acceptChan,"midi-accept-chan", 3, 3), env);
  scheme_add_global("midi-accept-type", scheme_make_prim_w_arity(sch_acceptType,"midi-accept-type", 3, 3), env);
  scheme_add_global("midi-accepted-port?", scheme_make_prim_w_arity(sch_isAcceptedPort,"Midi-accepted-port?", 2, 2), env);
  scheme_add_global("midi-is-accepted-port?", scheme_make_prim_w_arity(sch_isAcceptedPort,"Midi-is-accepted-port?", 2, 2), env);
  scheme_add_global("midi-accepted-chan?", scheme_make_prim_w_arity(sch_isAcceptedChan,"midi-accepted-chan?", 2, 2), env);
  scheme_add_global("midi-is-accepted-chan?", scheme_make_prim_w_arity(sch_isAcceptedChan,"midi-is-accepted-chan?", 2, 2), env);
  scheme_add_global("midi-accepted-type?", scheme_make_prim_w_arity(sch_isAcceptedType,"midi-accepted-type?", 2, 2), env);
  scheme_add_global("midi-is-accepted-type?", scheme_make_prim_w_arity(sch_isAcceptedType,"midi-is-accepted-type?", 2, 2), env);

  scheme_add_global("midi-connect", scheme_make_prim_w_arity(sch_connect,"midi-connect", 3, 3), env);
  scheme_add_global("midi-connected?", scheme_make_prim_w_arity(sch_isConnected,"midi-connected?", 2, 2), env);
  scheme_add_global("midi-is-connected?", scheme_make_prim_w_arity(sch_isConnected,"midi-is-connected?", 2, 2), env);
 
  scheme_add_global("midi-set-port-state!", scheme_make_prim_w_arity(sch_setPortState,"midi-set-port-state!", 2, 2), env);
  scheme_add_global("midi-get-port-state", scheme_make_prim_w_arity(sch_getPortState,"midi-port-state?", 1, 1), env);

  scheme_add_global("midi-new-cell", scheme_make_prim_w_arity(sch_newCell,"midi-new-ev", 0, 0), env);
  scheme_add_global("midi-free-cell", scheme_make_prim_w_arity(sch_freeCell,"midi-free-cell", 1, 1), env);
 
  scheme_add_global("midi-free-space", scheme_make_prim_w_arity(sch_freeSpace,"midi-free-space", 0, 0), env);
  scheme_add_global("midi-total-space", scheme_make_prim_w_arity(sch_totalSpace,"midi-total-space", 0, 0), env);
  scheme_add_global("midi-grow-space", scheme_make_prim_w_arity(sch_growSpace,"midi-grow-space", 1, 1), env);
 
  scheme_add_global("midi-new-ev", scheme_make_prim_w_arity(sch_newEv,"midi-new-ev", 1, 1), env);
  scheme_add_global("midi-copy-ev", scheme_make_prim_w_arity(sch_copyEv,"midi-copy-ev", 1, 1), env);
  scheme_add_global("midi-free-ev", scheme_make_prim_w_arity(sch_freeEv,"midi-free-ev", 1, 1), env);
  /* JH for sanity I add the midi-ev? predicat */
  scheme_add_global("midi-ev?", scheme_make_prim_w_arity(sch_isEv,"midi-ev?", 1, 1), env);

  scheme_add_global("midi-set-field!", scheme_make_prim_w_arity(sch_setField,"midi-set-field!", 3, 3), env);
  scheme_add_global("midi-get-field", scheme_make_prim_w_arity(sch_getField,"midi-get-field", 2, 2), env);
  scheme_add_global("midi-add-field", scheme_make_prim_w_arity(sch_addField,"midi-add-field", 2, 2), env);
  scheme_add_global("midi-count-fields", scheme_make_prim_w_arity(sch_countFields,"midi-count-fields", 1, 1), env);
 
  scheme_add_global("midi-set-link!", scheme_make_prim_w_arity(sch_setLink,"midi-set-link!", 2, 2), env);
  scheme_add_global("midi-get-link", scheme_make_prim_w_arity(sch_getLink,"midi-get-link", 1, 1), env);

  scheme_add_global("midi-set-date", scheme_make_prim_w_arity(sch_setDate,"midi-set-date", 2, 2), env);
  scheme_add_global("midi-get-date", scheme_make_prim_w_arity(sch_getDate,"midi-get-date", 1, 1), env);
  scheme_add_global("midi-set-ref-num!", scheme_make_prim_w_arity(sch_setRefNum,"midi-set-ref-num", 2, 2), env);
  scheme_add_global("midi-get-ref-num", scheme_make_prim_w_arity(sch_getRefNum,"midi-get-ref-num", 1, 1), env);
  scheme_add_global("midi-set-type!", scheme_make_prim_w_arity(sch_setType,"midi-set-type!", 2, 2), env);
  scheme_add_global("midi-get-type", scheme_make_prim_w_arity(sch_getType,"midi-get-type", 1, 1), env);
  scheme_add_global("midi-set-chan!", scheme_make_prim_w_arity(sch_setChan,"midi-set-chan!", 2, 2), env);
  scheme_add_global("midi-get-chan", scheme_make_prim_w_arity(sch_getChan,"midi-get-chan", 1, 1), env);
  scheme_add_global("midi-set-port!", scheme_make_prim_w_arity(sch_setPort,"midi-set-port!", 2, 2), env);
  scheme_add_global("midi-get-port", scheme_make_prim_w_arity(sch_getPort,"midi-get-port", 1, 1), env);

  scheme_add_global("midi-set-data0!", scheme_make_prim_w_arity(sch_setData0,"midi-set-data0!", 2, 2), env);
  scheme_add_global("midi-get-data0", scheme_make_prim_w_arity(sch_getData0,"midi-get-data0", 1, 1), env);
  scheme_add_global("midi-set-data1!", scheme_make_prim_w_arity(sch_setData1,"midi-set-data1!", 2, 2), env);
  scheme_add_global("midi-get-data1", scheme_make_prim_w_arity(sch_getData1,"midi-get-data1", 1, 1), env);
  scheme_add_global("midi-set-data2!", scheme_make_prim_w_arity(sch_setData2,"midi-set-data2!", 2, 2), env);
  scheme_add_global("midi-get-data2", scheme_make_prim_w_arity(sch_getData2,"midi-get-data2", 1, 1), env);
  scheme_add_global("midi-set-data3!", scheme_make_prim_w_arity(sch_setData3,"midi-set-data3!", 2, 2), env);
  scheme_add_global("midi-get-data3", scheme_make_prim_w_arity(sch_getData3,"midi-get-data3", 1, 1), env);
 
  scheme_add_global("midi-get-time", scheme_make_prim_w_arity(sch_getTime,"midi-get-time", 0, 0), env);

  scheme_add_global("midi-new-seq", scheme_make_prim_w_arity(sch_newSeq,"midi-new-seq", 0, 0), env);
  scheme_add_global("midi-add-seq", scheme_make_prim_w_arity(sch_addSeq,"midi-add-seq", 2, 2), env);
  scheme_add_global("midi-free-seq", scheme_make_prim_w_arity(sch_freeSeq,"midi-free-seq", 1, 1), env);
  scheme_add_global("midi-clear-seq", scheme_make_prim_w_arity(sch_clearSeq,"midi-clear-seq", 1, 1), env);


  scheme_add_global("midi-send-im", scheme_make_prim_w_arity(sch_sendIm,"midi-send-im", 2, 2), env);
  scheme_add_global("midi-send", scheme_make_prim_w_arity(sch_send,"midi-send", 2, 2), env);
  scheme_add_global("midi-send-at", scheme_make_prim_w_arity(sch_sendAt,"midi-send-at", 3, 3), env);
  scheme_add_global("midi-get-first-ev", scheme_make_prim_w_arity(sch_getFirstEv,"midi-get-first-ev", 1, 1), env);
  scheme_add_global("midi-get-last-ev", scheme_make_prim_w_arity(sch_getLastEv,"midi-get-last-ev", 1, 1), env);
  scheme_add_global("midi-set-first-ev!", scheme_make_prim_w_arity(sch_setFirstEv,"midi-set-first-ev!", 2, 2), env);
  scheme_add_global("midi-set-last-ev!", scheme_make_prim_w_arity(sch_setLastEv,"midi-set-last-ev!", 2, 2), env);

  scheme_add_global("midi-count-evs", scheme_make_prim_w_arity(sch_countEvs,"midi-count-evs", 1, 1), env);
  scheme_add_global("midi-get-ev", scheme_make_prim_w_arity(sch_getEv,"midi-get-ev", 1, 1), env);
  scheme_add_global("midi-avail-ev", scheme_make_prim_w_arity(sch_availEv,"midi-avail-ev", 1, 1), env);
  scheme_add_global("midi-flush-evs", scheme_make_prim_w_arity(sch_flushEvs,"midi-flush-evs", 1, 1), env);
  /* here the value for the types for use with player */
  /* I don't know how get type from player without this */
  scheme_add_global("*<midi-seq>*",scheme_make_integer(seq_type),env);
  scheme_add_global("*<midi-ev>*",scheme_make_integer(ev_type),env);
  scheme_add_global("*<midi-filter>*",scheme_make_integer(filter_type),env);


#ifndef __UseMSThunking__
  /* void vector init for alarm and for max 63 appls*/
   /* her I stock proc */
  scheme_add_global("midi-rcv-alarm-proc", scheme_make_vector(63,scheme_make_integer(0)), env);
  /* here I stock pointers for threads */
  scheme_add_global("midi-rcv-alarm-threads", scheme_make_vector(63,scheme_make_integer(0)), env);

  scheme_add_global("midi-set-rcv-alarm!", scheme_make_prim_w_arity(sch_setRcvAlarm,"midi-set-rcv-alarm!", 2, 2), env);
  scheme_add_global("midi-get-rcv-alarm", scheme_make_prim_w_arity(sch_getRcvAlarm,"midi-get-rcv-alarm", 1, 1), env); 

  scheme_add_global("midi-test-rcv-proc", scheme_make_prim_w_arity(sch_testRcvProc,"midi-test-rcv-proc", 1, 1), env); 
  scheme_add_global("midi-stop-rcv-alarm", scheme_make_prim_w_arity(sch_stopRcvAlarm,"midi-stop-rcv-alarm", 1, 1), env);
 
#endif 

  return scheme_void;

}

Scheme_Object *scheme_initialize(Scheme_Env *env)
{ 
 const char *smpte_field_names[]= {"format","hours","minutes","seconds", "frames", "fracs","smpteLocPtr"}; 
 const char *sync_info_field_names[]= {"time","reenter","syncMode","syncLocked", "syncPort", "syncStart",
				       "syncStop","syncOffset","syncSpeed","syncBreaks","syncFormat"}; 
 ev_type = scheme_make_type("<midi-ev>"); 
 ev_type_killed = scheme_make_type("<midi-ev-killed>");
 filter_type = scheme_make_type("<midi-filter>");	
 filter_type_killed = scheme_make_type("<midi-filter-killed>" );
 seq_type =  scheme_make_type("<midi-seq>");
 smpte_loc_type = scheme_make_type("<midi-smpte-loc>");
 seq_type_killed = scheme_make_type("<midi-seq-killed>");
 smpte_loc_struct_type = new_scheme_struct(env,"smpte-loc", 7, (char**) smpte_field_names,SCHEME_STRUCT_NO_CONSTR);
 sync_info_struct_type = new_scheme_struct(env,"sync-info", 12, (char**) sync_info_field_names,SCHEME_STRUCT_NO_CONSTR);


 return scheme_reload(env);
}
