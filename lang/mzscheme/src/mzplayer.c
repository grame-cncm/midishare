/*    From Jacques.Herry@wanadoo.fr */
/*            Jacques Herry */
/*           35, resid Corlay */
/*           22300 Lannion */
/*            France */
/*  	Grame  MidiPlayer for schemers. */
/*  "Welcome to Grame MidiShare(c)extension player, version 1.6.22 Copyright(c)2001 Jacques Herry" */

#include "include/escheme.h"
#include "include/player.h"
#include "include/mstype.h"
#include <stdlib.h>


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


static Scheme_Object *sch_openPlayer(int argc, Scheme_Object **argv)
{   
 MidiName v;
 if (!SCHEME_STRINGP(argv[0]))
   scheme_wrong_type("open-player", "string", 0, argc, argv);
 v = SCHEME_STR_VAL(argv[0]);
 return scheme_make_integer(OpenPlayer(v));
}

static Scheme_Object *sch_closePlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("close-player", "integer", 0, argc, argv);
  ClosePlayer((short)SCHEME_INT_VAL(argv[0]));
  return scheme_void;
}

/* transport control */
static Scheme_Object *sch_startPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("start-player", "integer", 0, argc, argv);
  StartPlayer((short)SCHEME_INT_VAL(argv[0]));
  return scheme_void;
}

static Scheme_Object *sch_contPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("cont-player", "integer", 0, argc, argv);
  ContPlayer((short)SCHEME_INT_VAL(argv[0]));
  return scheme_void;
}

static Scheme_Object *sch_stopPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("stop-player", "integer", 0, argc, argv);
  StopPlayer((short)SCHEME_INT_VAL(argv[0]));
  return scheme_void;
}

static Scheme_Object *sch_pausePlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("pause-player", "integer", 0, argc, argv);
  PausePlayer((short)SCHEME_INT_VAL(argv[0]));
  return scheme_void;
}

/* record management */
static Scheme_Object *sch_setRecordModePlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-record-mode-player!", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("set-record-mode-player!", "integer state", 1, argc, argv);
  SetRecordModePlayer((short)SCHEME_INT_VAL(argv[0]),(short) SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}


static Scheme_Object *sch_recordPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("record-player", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("record-player", "integer tracknum", 1, argc, argv);
  RecordPlayer((short)SCHEME_INT_VAL(argv[0]),(short) SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}


static Scheme_Object *sch_setRecordFilterPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-record-filter-player!", "integer refnum ", 0, argc, argv);
    
  if (SCHEME_TYPE(argv[1]) != filter_type)
    scheme_wrong_type("set-record-filter-player!", "filter", 1, argc, argv);
  SetRecordFilterPlayer((short)SCHEME_INT_VAL(argv[0]),
			((shFilter *) argv[1])->filter_ptr);
  return scheme_void;
}


/* 4 args the 5th is for me */
static Scheme_Object *sch_makePosPlayer(int argc, Scheme_Object **argv)
{   
  shPos *newPos;
  PosPtr msPos;
  Scheme_Object *a[4];
  int i;
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("make-pos-player", "integer bar", 0, argc, argv);
 if (!SCHEME_INTP(argv[1]))
   scheme_wrong_type("make-pos-player", "integer beat", 1, argc, argv);
 if (!SCHEME_INTP(argv[2]))
   scheme_wrong_type("make-pos-player", "integer unit", 2, argc, argv);
  /* we create the c struct */
  newPos = (shPos *)scheme_malloc(sizeof(shPos)); 
  msPos = (PosPtr)scheme_malloc_atomic(sizeof(Pos));
  newPos->type = pos_type; 
  msPos->bar = (short) SCHEME_INT_VAL(argv[0]);
  msPos->beat = (short)SCHEME_INT_VAL(argv[1]);
  msPos->unit = (short)SCHEME_INT_VAL(argv[2]);
  newPos->pos_ptr = msPos;
  /* copy of args on a for making struct */
  for (i=0;i<3;i++) {a[i]=argv[i];}
  a[3]= (Scheme_Object *) newPos;
  return scheme_make_struct_instance(pos_player_struct_type,sizeray(a),a);
}


static Scheme_Object *sch_setPosBBUPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-pos-bbu-player!", "integer refnum ", 0, argc, argv);
   if (! scheme_is_struct_instance((Scheme_Object *) pos_player_struct_type, (Scheme_Object *) argv[1]))
     scheme_wrong_type("set-pos-bbu-player!", "pos-player structure ", 0, argc, argv);    
   SetPosBBUPlayer((short)SCHEME_INT_VAL(argv[0]), GET_POS_PTR(argv[1]));
  return scheme_void;
}

static Scheme_Object *sch_setPosMsPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-pos-ms-player!", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_NUMBERP(argv[1]))
    scheme_wrong_type("set-pos-ms-player!", "long date_ms", 1, argc, argv);
  SetPosMsPlayer((short)SCHEME_INT_VAL(argv[0]), SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}


/* loop management */

static Scheme_Object *sch_setLoopPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-loop-player!", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("set-loop-player!", "integer state", 1, argc, argv);
  SetLoopPlayer((short)SCHEME_INT_VAL(argv[0]),(short) SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}



static Scheme_Object *sch_setLoopStartBBUPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-loop-start-bbu-player!", "integer refnum ", 0, argc, argv);
  if (! scheme_is_struct_instance((Scheme_Object *) pos_player_struct_type, (Scheme_Object *) argv[1]))
    scheme_wrong_type("set-loop-start-bbu-player!", "pos-player structure ", 0, argc, argv);    
   return scheme_make_integer_value(SetLoopStartBBUPlayer((short)SCHEME_INT_VAL(argv[0]),GET_POS_PTR(argv[1])));		       
}



static Scheme_Object *sch_setLoopEndBBUPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-loop-end-bbu-player!", "integer refnum ", 0, argc, argv);
  if (! scheme_is_struct_instance((Scheme_Object *) pos_player_struct_type, (Scheme_Object *) argv[1]))
    scheme_wrong_type("set-loop-end-bbu-player!", "pos-player structure ", 0, argc, argv);    
  return scheme_make_integer_value(SetLoopEndBBUPlayer((short)SCHEME_INT_VAL(argv[0]),GET_POS_PTR(argv[1])));
  
}

static Scheme_Object *sch_setLoopStartMsPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-loop-start-ms-player!", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_NUMBERP(argv[1]))
    scheme_wrong_type("set-loop-start-ms-player!", "long date_ms", 1, argc, argv);
  return scheme_make_integer_value(SetLoopStartMsPlayer((short)SCHEME_INT_VAL(argv[0]), 
						  SCHEME_INT_VAL(argv[1])));
  
}


static Scheme_Object *sch_setLoopEndMsPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-loop-end-ms-player!", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_NUMBERP(argv[1]))
    scheme_wrong_type("set-loop-end-ms-player!", "integer date_ms", 1, argc, argv);
  return scheme_make_integer_value(SetLoopEndMsPlayer((short)SCHEME_INT_VAL(argv[0]), 
						  SCHEME_INT_VAL(argv[1])));
  
}


/* synchro */

static Scheme_Object *sch_setSynchroInPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-synchro-in-player!", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("set-synchro-in-player!", "integer state", 1, argc, argv);
  SetSynchroInPlayer((short)SCHEME_INT_VAL(argv[0]),(short) SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}

static Scheme_Object *sch_setSynchroOutPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-synchro-out-player!", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("set-synchro-out-player!", "integer state", 1, argc, argv);
  SetSynchroOutPlayer((short)SCHEME_INT_VAL(argv[0]),(short) SCHEME_INT_VAL(argv[1]));
  return scheme_void;
}


 
/* 
typedef struct TSmpteLocation FAR *SmpteLocPtr;
	
     typedef struct TSmpteLocation
	{
		short	    format;			;* (0:24f/s, 1:25f/s, 2:30DFf/s, 3:30f/s)	
		short		hours;			;* 0..23					
		short		minutes;		;* 0..59					
		short		seconds;		;* 0..59					
		short		frames;			; 0..30 (according to format)	
		short		fracs;			; 0..99 (1/100 of frames)	
	} TSmpteLocation;
*/



/* you can use struct also in scheme */
/* (define my-smpte (make-smpte-loc 1 2 3 4 5 6 )) */
/* we can use this with MidiShare SMPTE synchronisation already defined*/

static Scheme_Object *sch_setSMPTEOffsetPlayer(int argc, Scheme_Object **argv)
{  
  TSmpteLocation *smptePtr;
   if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-smpte-offset-player!", "integer refnum ", 0, argc, argv);
  if (! scheme_is_struct_instance((Scheme_Object *) smpte_loc_struct_type, (Scheme_Object *) argv[1]))
    scheme_wrong_type("set-smpte-offset-player!", "smpte-loc structure receiver )", 0, argc, argv);
 
      smptePtr = GET_SMPTE_PTR(argv[1]);  
      
  smptePtr->format = (short)  SCHEME_INT_VAL(scheme_struct_ref(argv[1],0));
  smptePtr->hours = (short)   SCHEME_INT_VAL(scheme_struct_ref(argv[1],1));
  smptePtr->minutes = (short) SCHEME_INT_VAL(scheme_struct_ref(argv[1],2));
  smptePtr->seconds = (short) SCHEME_INT_VAL(scheme_struct_ref(argv[1],3));
  smptePtr->frames = (short)  SCHEME_INT_VAL(scheme_struct_ref(argv[1],4));
  smptePtr->fracs = (short)   SCHEME_INT_VAL(scheme_struct_ref(argv[1],5));

  SetSMPTEOffsetPlayer((short)SCHEME_INT_VAL(argv[0]), smptePtr);
  /* the pointer on the memory location is in record provided */

  return scheme_void;
}


static Scheme_Object *sch_setTempoPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-tempo-player!", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_NUMBERP(argv[1]))
    scheme_wrong_type("set-tempo-player!", "integer tempo", 1, argc, argv);
 SetTempoPlayer((short)SCHEME_INT_VAL(argv[0]),  SCHEME_INT_VAL(argv[1]));
 return scheme_void;
  
}

/* JH working */
/* State management */
/* return scheme new struct imediatly without arg*/
/* else with arg2 provided fill the struct provided with the values*/

static Scheme_Object *sch_getStatePlayer(int argc, Scheme_Object **argv)
{
  Scheme_Object  *a[13];
  PlayerStatePtr s=NULL;
  int i = 0;
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("get-state-player", "integer refnum ", 0, argc, argv);
  if (argc == 1)
    { s = (PlayerStatePtr) scheme_malloc_atomic(sizeof(PlayerState)); }
  /* else if argc > 1 */
  else if  (! scheme_is_struct_instance((Scheme_Object *) state_player_struct_type, (Scheme_Object *) argv[1]))
    {  scheme_wrong_type("get-state-player", "state-player structure receiver obtained by (get-state-player refNum)", 1, argc, argv); }
  /* now getting the pointer from struct provided */ 
  if (argc == 2)
    s = (PlayerStatePtr) SCHEME_INT_VAL(scheme_struct_ref(argv[1],12)); // I think address is ok
  /* now s is filled by the C function */
  GetStatePlayer((short)SCHEME_INT_VAL(argv[0]),s);
  /* and the s values are writing in scm struct */
  a[0]=scheme_make_integer_value(s->date);
  a[1]= scheme_make_integer_value(s->tempo);
  a[2]=scheme_make_integer(s->tsnum);
  a[3]=scheme_make_integer(s->tsdenom);
  a[4]=scheme_make_integer(s->tsclick);
  a[5]=scheme_make_integer(s->tsquarter);
  a[6]=scheme_make_integer(s->bar);
  a[7]=scheme_make_integer(s->beat);
  a[8]=scheme_make_integer(s->unit);
  a[9]=scheme_make_integer(s->state);
  a[10]=scheme_make_integer(s->syncin);
  a[11]=scheme_make_integer(s->syncout);
  a[12]=scheme_make_integer_value((long)s); /* Here for later use in getStatePLayer */
  /* return argv[1] without new allocated struct */
  if (argc==2){
    for ( i = 0; i< 13; ++i){
      scheme_struct_set(argv[1],i,a[i] ); } 
    return argv[1]; }
  else /* new struct */
    return scheme_make_struct_instance(state_player_struct_type,sizeray(a),a);
}   


static Scheme_Object *sch_getEndScorePlayer(int argc, Scheme_Object **argv)
{ int i = 0;
 Scheme_Object  *a[13];
 PlayerStatePtr s=NULL;
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("get-end-score-player", "integer refnum ", 0, argc, argv);
 if (argc == 1)
   { s = (PlayerStatePtr) scheme_malloc_atomic(sizeof(PlayerState)); }
 /* else if argc > 1 */
 else if  (! scheme_is_struct_instance((Scheme_Object *) state_player_struct_type, (Scheme_Object *) argv[1]))
   {  scheme_wrong_type("get-end-score-player", "state-player structure receiver obtained by (get-end-score-player refNum)", 1, argc, argv); }
 /* now getting the pointer from struct provided */ 
 if (argc == 2)
   s = (PlayerStatePtr) SCHEME_INT_VAL(scheme_struct_ref(argv[1],12)); // I think address is ok
 /* now s is filled by the C function */
 GetEndScorePlayer((short)SCHEME_INT_VAL(argv[0]),s);
 a[0]=scheme_make_integer_value(s->date);
 a[1]= scheme_make_integer_value(s->tempo);
 a[2]=scheme_make_integer(s->tsnum);
 a[3]=scheme_make_integer(s->tsdenom);
 a[4]=scheme_make_integer(s->tsclick);
 a[5]=scheme_make_integer(s->tsquarter);
 a[6]=scheme_make_integer(s->bar);
 a[7]=scheme_make_integer(s->beat);
 a[8]=scheme_make_integer(s->unit);
 a[9]=scheme_make_integer(s->state);
 a[10]=scheme_make_integer(s->syncin);
 a[11]=scheme_make_integer(s->syncout);
 a[12]=scheme_make_integer_value((long)s); /* Here for later use in getStatePLayer */
 /* return argv[1] without new allocated struct */
 if (argc==2){
   for ( i = 0; i< 3; ++i){
     scheme_struct_set(argv[1],i,a[i] ); } 
   return argv[1] ;}
 else /* new struct */   
   return scheme_make_struct_instance(state_player_struct_type,sizeray(a),a);

}


/* step playing */


static Scheme_Object *sch_forwardStepPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("forward-step-player", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("forward-step-player", "integer flag", 1, argc, argv);
  ForwardStepPlayer((short)SCHEME_INT_VAL(argv[0]),(short)SCHEME_INT_VAL(argv[1]));
  return scheme_void; 
  
}

static Scheme_Object *sch_backwardStepPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("backward-step-player", "integer refnum ", 0, argc, argv);
    
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("backward-step-player", "integer flag", 1, argc, argv);
  BackwardStepPlayer((short)SCHEME_INT_VAL(argv[0]),(short)SCHEME_INT_VAL(argv[1]));
  return scheme_void; 
  
}


/* track */
static Scheme_Object *sch_getAllTrackPlayer(int argc, Scheme_Object **argv)
{   
 shNewSeq *seq;
     
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("get-all-track-player", "integer refnum ", 0, argc, argv);
 seq = (shNewSeq *)scheme_malloc(sizeof(shNewSeq));     
 seq->type = seq_type;    
 seq->midi_seq_ptr = GetAllTrackPlayer((short)SCHEME_INT_VAL(argv[0]));
 return (Scheme_Object *)seq; 
  
}

static Scheme_Object *sch_getTrackPlayer(int argc, Scheme_Object **argv)
{   
 shNewSeq *seq;
 if (!SCHEME_INTP(argv[0]))
   scheme_wrong_type("get-track-player", "integer refnum ", 0, argc, argv);
 if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("get-track-player", "integer tracknum", 1, argc, argv);
 seq = (shNewSeq *)scheme_malloc(sizeof(shNewSeq));     
 seq->type = seq_type;    
 seq->midi_seq_ptr = GetTrackPlayer((short)SCHEME_INT_VAL(argv[0]),(short)SCHEME_INT_VAL(argv[1]));
 return (Scheme_Object *)seq; 
  
}


static Scheme_Object *sch_setTrackPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-track-player!", "integer refnum ", 0, argc, argv);
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("set-track-player!", "integer tracknum ", 1, argc, argv);
    
  if (SCHEME_TYPE(argv[2]) != seq_type)
    scheme_wrong_type("set-track-player!", "midi-seq", 2, argc, argv);
  return scheme_make_integer_value(SetTrackPlayer((short)SCHEME_INT_VAL(argv[0]),(short)SCHEME_INT_VAL(argv[1]),
		      ((shNewSeq *) argv[2])->midi_seq_ptr));
}


static Scheme_Object *sch_setAllTrackPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-all-track-player!", "integer refnum ", 0, argc, argv);
    
  if (SCHEME_TYPE(argv[1]) != seq_type)
    scheme_wrong_type("set-all-track-player!", "midi-seq", 1, argc, argv); 

if (!SCHEME_INTP(argv[2]))
    scheme_wrong_type("set-all-track-player!", "integer ticks per quarter ", 1, argc, argv);

 return scheme_make_integer_value(SetAllTrackPlayer((short)SCHEME_INT_VAL(argv[0]),
				 ((shNewSeq *)argv[1])->midi_seq_ptr,
				 SCHEME_INT_VAL(argv[2])));
}


static Scheme_Object *sch_setParamPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("set-param-player!", "integer refnum ", 0, argc, argv);
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("set-param-player!", "integer tracknum ", 1, argc, argv);
  if (!SCHEME_INTP(argv[2]))
    scheme_wrong_type("set-param-player!", "integer param ", 2, argc, argv);
  if (!SCHEME_INTP(argv[3]))
    scheme_wrong_type("set-param-player!", "integer value ", 3, argc, argv);
          
  SetParamPlayer((short)SCHEME_INT_VAL(argv[0]),(short)SCHEME_INT_VAL(argv[1]),
		(short)SCHEME_INT_VAL(argv[2]),(short)SCHEME_INT_VAL(argv[3]));
  return scheme_void; 
  
}


static Scheme_Object *sch_getParamPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("get-param-player", "integer refnum ", 0, argc, argv);
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("get-param-player", "integer tracknum ", 1, argc, argv);
  if (!SCHEME_INTP(argv[2]))
    scheme_wrong_type("get-param-player", "integer param ", 2, argc, argv);
  return scheme_make_integer(GetParamPlayer((short)SCHEME_INT_VAL(argv[0]),(short)SCHEME_INT_VAL(argv[1]),
					    (short)SCHEME_INT_VAL(argv[2])));
}


static Scheme_Object *sch_insertAllTrackPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("insert-all-track-player", "integer refnum ", 0, argc, argv);
    
  if (SCHEME_TYPE(argv[1]) != seq_type)
    scheme_wrong_type("insert-all-track-player", "midi-seq", 1, argc, argv);

  return scheme_make_integer_value(InsertAllTrackPlayer((short)SCHEME_INT_VAL(argv[0]),
				    ((shNewSeq *)argv[1])->midi_seq_ptr));  
}

static Scheme_Object *sch_insertTrackPlayer(int argc, Scheme_Object **argv)
{   
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("insert-track-player", "integer refnum ", 0, argc, argv);
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("insert-track-player", "integer tracknum ", 0, argc, argv);
      
  if (SCHEME_TYPE(argv[2]) != seq_type)
    scheme_wrong_type("insert-track-player", "midi-seq", 1, argc, argv);

  return scheme_make_integer_value(InsertTrackPlayer((short)SCHEME_INT_VAL(argv[0]), 
				  (short)  SCHEME_INT_VAL(argv[1]),
				    ((shNewSeq *) argv[2])->midi_seq_ptr));  
}

/* the info-struct is properly initialized with this proc */
/* just fill the last field with 0 value */
static Scheme_Object *sch_makeMidiFileInfos(int argc, Scheme_Object **argv)
{ 
  shMidiFileInfos *newInfos;
  MidiFileInfosPtr infos;
  Scheme_Object *a[5];
  int i;
  if (!SCHEME_INTP(argv[0]))
    scheme_wrong_type("make-midi-file-infos", "integer format ", 0, argc, argv);
  if (!SCHEME_INTP(argv[1]))
    scheme_wrong_type("make-midi-file-infos", "integer timedef ", 1, argc, argv);
  if (!SCHEME_INTP(argv[2]))
    scheme_wrong_type("make-midi-file-infos", "integer clicks ", 2, argc, argv);
  if (!SCHEME_INTP(argv[3]))
    scheme_wrong_type("make-midi-file-infos", "integer tracks ", 3, argc, argv);

  infos = (MidiFileInfosPtr) scheme_malloc_atomic(sizeof(MidiFileInfos));
  newInfos = (shMidiFileInfos *)scheme_malloc(sizeof(shMidiFileInfos));   
 
  newInfos->type = midi_file_infos_type;
  infos->format = (long)SCHEME_INT_VAL(argv[0]);
  infos->timedef =(long)SCHEME_INT_VAL(argv[1]);
  infos->clicks =(long)SCHEME_INT_VAL(argv[2]);
  infos->tracks =(long)SCHEME_INT_VAL(argv[3]);
  newInfos->midi_file_infos_ptr=infos; 
  for (i=0;i<4;i++)
    a[i]=argv[i];
  a[4]=(Scheme_Object *)newInfos;

  return scheme_make_struct_instance(infos_player_struct_type,sizeray(a),a);
}



/* the info-struct is properly initialized with this proc */
/* just fill the last field with 0 value */
static Scheme_Object *sch_midiFileSave(int argc, Scheme_Object **argv)
{  
  shMidiFileInfos *infoPtr;
  MidiFileInfosPtr infos; 
  if (!SCHEME_STRINGP(argv[0]))
    scheme_wrong_type("midi-file-save", "string name ", 0, argc, argv);
  if (SCHEME_TYPE(argv[1]) != seq_type)
    scheme_wrong_type("midi-file-save", "midi-seq", 1, argc, argv);
  if (! scheme_is_struct_instance((Scheme_Object *)infos_player_struct_type,(Scheme_Object *)  argv[2]))
    scheme_wrong_type("midi-file-save", "midi-infos structure receiver", 2, argc, argv); 
  return scheme_make_integer_value(MidiFileSave(SCHEME_STR_VAL(argv[0]), 
						((shNewSeq *) argv[1])->midi_seq_ptr, 
						GET_INFOS_PTR(argv[2]))); 
}


static Scheme_Object *sch_midiFileLoad(int argc, Scheme_Object **argv)
{  
  Scheme_Object *resultat;
  int i;
  MidiFileInfosPtr info;
  if (!SCHEME_STRINGP(argv[0]))
    scheme_wrong_type("midi-file-load", "string name ", 0, argc, argv);
  if (SCHEME_TYPE(argv[1]) != seq_type)
    scheme_wrong_type("midi-file-load", "midi-seq", 1, argc, argv);
  if (! scheme_is_struct_instance((Scheme_Object *)infos_player_struct_type,(Scheme_Object *)  argv[2]))
    scheme_wrong_type("midi-file-save", "midi-infos structure receiver", 2, argc, argv); 
  info = GET_INFOS_PTR(argv[2]);
  resultat= scheme_make_integer_value(MidiFileLoad(SCHEME_STR_VAL(argv[0]), 
					     ((shNewSeq *) argv[1])->midi_seq_ptr, 
					     info)); 
  /* now fill the scm-struct with the values of infosPtr */
  scheme_struct_set(argv[2],0,scheme_make_integer_value(info->format));
  scheme_struct_set(argv[2],1,scheme_make_integer_value(info->timedef));
  scheme_struct_set(argv[2],2,scheme_make_integer_value(info->clicks));
  scheme_struct_set(argv[2],3,scheme_make_integer_value(info->tracks)); 
 
  return resultat;
}

	
/*****************************/

Scheme_Object *scheme_reload(Scheme_Env *env)
{
  scheme_add_global("open-player", scheme_make_prim_w_arity(sch_openPlayer, "open-player", 1, 1), env);
  scheme_add_global("close-player", scheme_make_prim_w_arity(sch_closePlayer, "close-player", 1, 1), env);  
 
  scheme_add_global("start-player", scheme_make_prim_w_arity(sch_startPlayer, "start-player", 1, 1), env);
  scheme_add_global("cont-player", scheme_make_prim_w_arity(sch_contPlayer, "cont-player", 1, 1), env);
  scheme_add_global("stop-player", scheme_make_prim_w_arity(sch_stopPlayer, "stop-player", 1, 1), env);
  scheme_add_global("pause-player", scheme_make_prim_w_arity(sch_pausePlayer, "pause-player", 1, 1), env);

  scheme_add_global("make-pos-player", scheme_make_prim_w_arity(sch_makePosPlayer, "make-pos-player", 3, 3), env);
  scheme_add_global("set-pos-bbu-player!", scheme_make_prim_w_arity(sch_setPosBBUPlayer, 
								    "set-pos-bbu-player!", 2, 2), env);
  scheme_add_global("set-pos-ms-player!", scheme_make_prim_w_arity(sch_setPosMsPlayer, 
								   "set-pos-ms-player!", 2, 2), env);

  scheme_add_global("set-loop-player!", scheme_make_prim_w_arity(sch_setLoopPlayer, "set-loop-player!", 2, 2), env);
  scheme_add_global("set-loop-start-bbu-player!", scheme_make_prim_w_arity(sch_setLoopStartBBUPlayer, 
									   "set-loop-start-bbu-player!", 2, 2), env);
  scheme_add_global("set-loop-end-bbu-player!", scheme_make_prim_w_arity(sch_setLoopEndBBUPlayer, 
									 "set-loop-end-bbu-player!", 2, 2), env);
  scheme_add_global("set-loop-start-ms-player!", scheme_make_prim_w_arity(sch_setLoopStartMsPlayer, 
									  "set-loop-start-ms-player!", 2, 2), env);
  scheme_add_global("set-loop-end-ms-player!", scheme_make_prim_w_arity(sch_setLoopEndMsPlayer, 
									"set-loop-end-ms-player!", 2, 2), env);

 
  scheme_add_global("set-synchro-in-player!", scheme_make_prim_w_arity(sch_setSynchroInPlayer, 
								       "set-synchro-in-player!", 2, 2), env);
  scheme_add_global("set-synchro-out-player!", scheme_make_prim_w_arity(sch_setSynchroOutPlayer, 
									"set-synchro-out-player!", 2, 2), env);
 
  scheme_add_global("set-smpte-offset-player!", scheme_make_prim_w_arity(sch_setSMPTEOffsetPlayer, 
									 "set-smpte-offset-player!", 2, 2), env);
  scheme_add_global("set-tempo-player!", scheme_make_prim_w_arity(sch_setTempoPlayer, "set-tempo-player!", 2, 2), env);
  ;                                                                                    
  scheme_add_global("get-state-player", scheme_make_prim_w_arity(sch_getStatePlayer, "get-state-player", 1, 2), env);
  scheme_add_global("get-end-score-player", scheme_make_prim_w_arity(sch_getEndScorePlayer, 
								     "get-end-score-player", 1, 2), env);

  scheme_add_global("forward-step-player", scheme_make_prim_w_arity(sch_forwardStepPlayer, 
								    "forward-step-player", 2, 2), env);
  scheme_add_global("backward-step-player", scheme_make_prim_w_arity(sch_backwardStepPlayer, 
								     "backward-step-player", 2, 2), env);

  scheme_add_global("get-all-track-player", scheme_make_prim_w_arity(sch_getAllTrackPlayer, 
								     "get-all-track-player", 1, 1), env);
  scheme_add_global("get-track-player", scheme_make_prim_w_arity(sch_getTrackPlayer, "get-track-player", 2, 2), env);

  scheme_add_global("set-track-player!", scheme_make_prim_w_arity(sch_setTrackPlayer, 
								  "set-track-player!", 3, 3), env);
  scheme_add_global("set-all-track-player!", scheme_make_prim_w_arity(sch_setAllTrackPlayer, 
								      "set-all-track-player!", 3, 3), env);

  scheme_add_global("set-param-player!", scheme_make_prim_w_arity(sch_setParamPlayer, "set-param-player!", 4,4), env);
  scheme_add_global("get-param-player", scheme_make_prim_w_arity(sch_getParamPlayer, "get-param-player", 3,3), env);

  scheme_add_global("insert-all-track-player", scheme_make_prim_w_arity(sch_insertAllTrackPlayer, 
									"insert-all-track-player", 2, 2), env);
  scheme_add_global("insert-track-player", scheme_make_prim_w_arity(sch_insertTrackPlayer, 
								    "insert-track-player", 3, 3), env);
  scheme_add_global("make-midi-file-infos", scheme_make_prim_w_arity(sch_makeMidiFileInfos, 
								  "make-midi-file-infos", 4,4), env);


  scheme_add_global("midi-file-save", scheme_make_prim_w_arity(sch_midiFileSave, "midi-file-save", 3, 3), env);
  scheme_add_global("midi-file-load", scheme_make_prim_w_arity(sch_midiFileLoad, "midi-file-load", 3, 3), env);

  return scheme_void;
}


Scheme_Object *scheme_initialize(Scheme_Env *env)
{
  /* I add ptr for the proc C which eat ptr in arg memory menagement*/
  const char *state_field_names[] = {"date","tempo", "tsnum", "tsdenom","tsclick","tsquarter","bar",
				     "beat","unit","state","syncin","syncout","statePtr"};
 
  const char *pos_field_names[]= {"bar","beat","unit","posPtr"};
  const char *infos_field_names[]= {"format","timedef","clicks","tracks","infoPtr"};
  /* Here struct in scheme */
  state_player_struct_type = new_scheme_struct(env,"state-player", 13, (char**) state_field_names,
					       SCHEME_STRUCT_NO_CONSTR);
  pos_player_struct_type = new_scheme_struct(env,"pos-player", 4, (char**) pos_field_names,SCHEME_STRUCT_NO_CONSTR);
 
  infos_player_struct_type = new_scheme_struct(env,"midi-file-infos", 5, (char**) infos_field_names,SCHEME_STRUCT_NO_CONSTR);
  
  seq_type = SCHEME_INT_VAL(scheme_lookup_global(scheme_intern_symbol("*<midi-seq>*"),env));
  filter_type = SCHEME_INT_VAL(scheme_lookup_global(scheme_intern_symbol("*<midi-filter>*"),env));
  smpte_loc_type= scheme_lookup_global(scheme_intern_symbol("#%struct:smpte-loc"),env);
  pos_type = scheme_make_type("<player-pos>"); 
 
  midi_file_infos_type = scheme_make_type("<midi-file-info>");
 
  return scheme_reload(env);
}
