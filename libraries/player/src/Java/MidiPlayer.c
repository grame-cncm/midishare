// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================

/*****************************************************************************/
/* MidiPlayer.c : JNI interface to the Player library
/*
/* Copyright GRAME May 1996-1999
/*
/* MidiShare home page : http://www.grame.fr/MidiShare.html
/*
/* Bug & comments report : MidiShare@rd.grame.fr
/*	
/* History : 1/06/96  version 1.00
/*           15/07/96 version 1.01  correction retour de certaines fonctions
/*           16/07/96 version 1.02  MidiShare package management
/*            ....... version 1.03
/*           8/10/96  version 1.04  Packages names in unsercase
/*          26/07/97  version 1.07  New InsertAllTrack and InsertTrack functions
/*          25/11/97  version 1.08  NewSetTempo function and kExternalSync mode
/*          28/11/97  version 1.09  JNI compatible
/*****************************************************************************/

#ifdef __Macintosh__
		#include <midisharePPC.h>
		/* New type definition */
		typedef struct TFilter * MidiFilterPtr;
#endif

#ifdef __Linux__
        #include "MidiShare.h"
#endif

#include "Player.h"
#include "MidiPlayer.h"


/*--------------------------------------------------------------------------*/

 void pTocCopy(  char *dest,  unsigned char * src);
 void cTocCopy(  char *dest,  char * src);
 void cTopCopy(  unsigned char *dest,  char * src);

/*--------------------------------------------------------------------------*/
 void pTocCopy(  char *dest,  unsigned char * src)  // chaine pascal dans chaine c
{
	register short i;
	
	i= *src++;
	while( i--)
		*dest++= *src++;
	*dest= 0;
}

/*--------------------------------------------------------------------------*/
 void cTocCopy(  char *dest,  char * src) // chaine c dans chaine c
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;	
}

/*--------------------------------------------------------------------------*/
 void cTopCopy(  unsigned char *dest,  char * src) // chaine c dans chaine p
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i+1] = src[i];
		i++;
	}
	dest[0] = i;	
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_OpenAux
  (JNIEnv * inEnv, jclass cl, jint midiname){
  
  	
	#ifdef __Macintosh__
    		unsigned char buffer [128];
    		cTopCopy (buffer, (char*)midiname);
		return  OpenPlayer(buffer);
	#else
    		char buffer [128];
    		cTocCopy (buffer, (char*)midiname);
		return  OpenPlayer(buffer);
	#endif
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_Close
  (JNIEnv * inEnv, jclass cl, jint ref){

 	ClosePlayer(ref);
 }
 
 /*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_Start
  (JNIEnv * inEnv, jclass cl, jint ref){

 	StartPlayer(ref);
 }

 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_Cont
  (JNIEnv * inEnv, jclass cl, jint ref){

 	ContPlayer(ref);
 }

 /*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_Pause
  (JNIEnv * inEnv,  jclass cl, jint ref){

 	PausePlayer(ref);
 }	

 /*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_Stop
  (JNIEnv * inEnv, jclass cl, jint ref){

 	StopPlayer(ref);
 }

 /*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetRecordMode
  (JNIEnv * inEnv, jclass cl, jint refnum, jint mode){

 	SetRecordModePlayer(refnum, mode);
 }
 
 /*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_Record
  (JNIEnv * inEnv, jclass cl, jint refnum, jint tracknum){

 	RecordPlayer(refnum, tracknum);
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetRecordFilter
  (JNIEnv * inEnv, jclass cl, jint refnum, jint filter){

 	SetRecordFilterPlayer(refnum, (MidiFilterPtr)filter);
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetPosBBU
  (JNIEnv * inEnv, jclass cl, jint refnum, jobject pos){

 	Pos tpos;
 	jfieldID fbar,fbeat,funit;
 	
 	jclass posclass = (*inEnv)->GetObjectClass(inEnv, pos);
 	
 	fbar = (*inEnv)->GetFieldID(inEnv, posclass, "bar",  "S");
 	fbeat = (*inEnv)->GetFieldID(inEnv, posclass, "beat",  "S");
 	funit = (*inEnv)->GetFieldID(inEnv, posclass, "unit",  "S");

	if ((fbar== 0) || (fbeat== 0) ||(funit== 0) ) return;
 	
 	tpos.bar = 	(*inEnv)->GetIntField(inEnv,pos,fbar); 
 	tpos.beat = (*inEnv)->GetIntField(inEnv,pos, fbeat); 
 	tpos.unit =  (*inEnv)->GetIntField(inEnv,pos, funit); 
 	
  	SetPosBBUPlayer(refnum, &tpos);
 }

/*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetPosMs
  (JNIEnv * inEnv, jclass cl, jint refnum, jint date_ms){

 	SetPosMsPlayer(refnum, date_ms);
 }
 
 /*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetPosTicks
  (JNIEnv * inEnv, jclass cl, jint refnum, jint date_ticks){

 	SetPosTicksPlayer(refnum, date_ticks);
 }
 
/*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetLoop
  (JNIEnv * inEnv, jclass cl, jint refnum, jint loopstate){

 	SetLoopPlayer(refnum, loopstate);
 }

/*--------------------------------------------------------------------------*/
JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_SetLoopStartBBU
  (JNIEnv * inEnv, jclass cl, jint refnum, jobject pos){
  
 	Pos tpos;
 	jfieldID fbar,fbeat,funit;
 	
 	jclass posclass = (*inEnv)->GetObjectClass(inEnv, pos);
 	
 	fbar = (*inEnv)->GetFieldID(inEnv, posclass, "bar",  "S");
 	fbeat = (*inEnv)->GetFieldID(inEnv, posclass, "beat",  "S");
 	funit = (*inEnv)->GetFieldID(inEnv, posclass, "unit",  "S");

	if ((fbar== 0) || (fbeat== 0) ||(funit== 0) ) return 0;
 	
 	tpos.bar = 	(*inEnv)->GetShortField(inEnv,pos,fbar); 
 	tpos.beat = (*inEnv)->GetShortField(inEnv,pos, fbeat); 
 	tpos.unit =  (*inEnv)->GetShortField(inEnv,pos, funit); 
 	
 	return SetLoopStartBBUPlayer(refnum, &tpos);
 }
 
 /*--------------------------------------------------------------------------*/
JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_SetLoopEndBBU
  (JNIEnv * inEnv, jclass cl, jint refnum , jobject pos){

 	Pos tpos;
 	jfieldID fbar,fbeat,funit;
 	
 	jclass posclass = (*inEnv)->GetObjectClass(inEnv, pos);
 	
 	fbar = (*inEnv)->GetFieldID(inEnv, posclass, "bar",  "S");
 	fbeat = (*inEnv)->GetFieldID(inEnv, posclass, "beat",  "S");
 	funit = (*inEnv)->GetFieldID(inEnv, posclass, "unit",  "S");

	if ((fbar== 0) || (fbeat== 0) ||(funit== 0) ) return 0;
 	
 	tpos.bar = 	(*inEnv)->GetShortField(inEnv,pos,fbar); 
 	tpos.beat = (*inEnv)->GetShortField(inEnv,pos, fbeat); 
 	tpos.unit =  (*inEnv)->GetShortField(inEnv,pos, funit); 
 	

 	return SetLoopEndBBUPlayer(refnum, &tpos);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_SetLoopStartMs
  (JNIEnv * inEnv, jclass cl, jint refnum, jint date){

 	return SetLoopStartMsPlayer(refnum, date);
 }

/*--------------------------------------------------------------------------*/

 JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_SetLoopEndMs
  (JNIEnv * inEnv, jclass cl, jint refnum, jint date){

 	return SetLoopEndMsPlayer(refnum, date);
 }

/*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetSynchroIn
  (JNIEnv * inEnv, jclass cl, jint refnum, jint syncstate){

 	SetSynchroInPlayer(refnum,  syncstate);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetSynchroOut
  (JNIEnv * inEnv, jclass cl, jint refnum, jint syncstate){

 	SetSynchroOutPlayer(refnum,  syncstate);
  }
  
/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetTempo
  (JNIEnv * inEnv, jclass cl, jint refnum, jint tempo){

 	SetTempoPlayer(refnum,  tempo);
 }
 
/*--------------------------------------------------------------------------*/
 
JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetTempoFactor
	(JNIEnv * inEnv, jclass cl, jint refnum, jfloat factor){

 	SetTempoFactorPlayer(refnum,  factor);
 }

 
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetSMPTEOffset
  (JNIEnv * inEnv, jclass cl, jint refnum, jobject smptepos){
  
 	TSmpteLocation tsmptepos;
 	jfieldID fformat,fhours,fminutes,fseconds,fframes,ffracs;
 	
 	jclass smpteposclass = (*inEnv)->GetObjectClass(inEnv, smptepos);
 	
 	fformat = (*inEnv)->GetFieldID(inEnv, smpteposclass, "format",  "S");
 	fhours = (*inEnv)->GetFieldID(inEnv, smpteposclass, "hours",  "S");
 	fminutes = (*inEnv)->GetFieldID(inEnv, smpteposclass, "minutes",  "S");
	fseconds = (*inEnv)->GetFieldID(inEnv, smpteposclass, "seconds",  "S");
 	fframes = (*inEnv)->GetFieldID(inEnv, smpteposclass, "frames",  "S");
 	ffracs = (*inEnv)->GetFieldID(inEnv, smpteposclass, "fracs",  "S");

	if ((fformat== 0) || (fhours== 0) ||(fminutes== 0) ||
		(fseconds== 0) || (fframes== 0) ||(ffracs== 0) ) return;
 			
 	tsmptepos.format = 	(*inEnv)->GetShortField(inEnv,smptepos,fformat); 
 	tsmptepos.hours = (*inEnv)->GetShortField(inEnv,smptepos,fhours); 
 	tsmptepos.minutes =	(*inEnv)->GetShortField(inEnv,smptepos,fminutes); 
 	tsmptepos.seconds = (*inEnv)->GetShortField(inEnv,smptepos,fseconds); 
 	tsmptepos.frames = (*inEnv)->GetShortField(inEnv,smptepos,fframes); 
 	tsmptepos.fracs = (*inEnv)->GetShortField(inEnv,smptepos,ffracs); 
 	
 	SetSMPTEOffsetPlayer(refnum, &tsmptepos);
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_GetState
  (JNIEnv * inEnv, jclass cl, jint refnum, jobject playerstate){

 	PlayerState tstate;
 	jfieldID fdate,ftempo,ftsnum,ftsdenom,ftsclick,ftsquarter,fbar,fbeat,funit,fstate,fsyncin,fsyncout;
 	jclass playerstateclass = (*inEnv)->GetObjectClass(inEnv, playerstate);
 	
 	GetStatePlayer( refnum, &tstate);
 
 	fdate = (*inEnv)->GetFieldID(inEnv, playerstateclass, "date",  "I");
 	ftempo = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tempo",  "I");
 	ftsnum = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tsnum",  "S");
 	ftsdenom = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tsdenom",  "S");
 	ftsclick = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tsclick",  "S");
 	ftsquarter = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tsquarter",  "S");
 	fbar = (*inEnv)->GetFieldID(inEnv, playerstateclass, "bar",  "S");
 	fbeat = (*inEnv)->GetFieldID(inEnv, playerstateclass, "beat",  "S");
 	funit = (*inEnv)->GetFieldID(inEnv, playerstateclass, "unit",  "S");
 	fstate = (*inEnv)->GetFieldID(inEnv, playerstateclass, "state",  "S");
 	fsyncin = (*inEnv)->GetFieldID(inEnv, playerstateclass, "syncin",  "S");
 	fsyncout = (*inEnv)->GetFieldID(inEnv, playerstateclass, "syncout",  "S");
 	
 	if ((fdate == 0) || (ftempo == 0) || (ftsnum == 0)|| (ftsdenom == 0)
 		|| (ftsclick == 0)|| (ftsquarter == 0)|| (fbar == 0)|| (fbeat == 0)
 		|| (funit == 0)|| (fstate == 0)|| (fsyncin == 0)|| (fsyncout == 0)) {
 		//SysBeep(0);
 		return;
 	}
 	
 
 	(*inEnv)->SetIntField(inEnv, playerstate, fdate, tstate.date);
	(*inEnv)->SetIntField(inEnv, playerstate, ftempo, tstate.tempo);
 	(*inEnv)->SetShortField(inEnv, playerstate, ftsnum, tstate.tsnum);
 	(*inEnv)->SetShortField(inEnv, playerstate, ftsdenom, tstate.tsdenom);
 	(*inEnv)->SetShortField(inEnv, playerstate, ftsclick, tstate.tsclick);
 	(*inEnv)->SetShortField(inEnv, playerstate, ftsquarter, tstate.tsquarter);
 	(*inEnv)->SetShortField(inEnv, playerstate, fbar, tstate.bar);
 	(*inEnv)->SetShortField(inEnv, playerstate, fbeat, tstate.beat);
 	(*inEnv)->SetShortField(inEnv, playerstate, funit, tstate.unit);
 	(*inEnv)->SetShortField(inEnv, playerstate, fstate, tstate.state);
 	(*inEnv)->SetShortField(inEnv, playerstate, fsyncin,tstate.syncin);
 	(*inEnv)->SetShortField(inEnv, playerstate, fsyncout, tstate.syncout);
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_GetEndScore
 (JNIEnv * inEnv, jclass cl, jint refnum, jobject playerstate){

 	PlayerState tstate;
 	jfieldID fdate,ftempo,ftsnum,ftsdenom,ftsclick,ftsquarter,fbar,fbeat,funit,fstate,fsyncin,fsyncout;
 	jclass playerstateclass = (*inEnv)->GetObjectClass(inEnv, playerstate);
 	
 	GetEndScorePlayer( refnum,  &tstate);
 
 	fdate = (*inEnv)->GetFieldID(inEnv, playerstateclass, "date",  "I");
 	ftempo = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tempo",  "I");
 	ftsnum = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tsnum",  "S");
 	ftsdenom = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tsdenom",  "S");
 	ftsclick = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tsclick",  "S");
 	ftsquarter = (*inEnv)->GetFieldID(inEnv, playerstateclass, "tsquarter",  "S");
 	fbar = (*inEnv)->GetFieldID(inEnv, playerstateclass, "bar",  "S");
 	fbeat = (*inEnv)->GetFieldID(inEnv, playerstateclass, "beat",  "S");
 	funit = (*inEnv)->GetFieldID(inEnv, playerstateclass, "unit",  "S");
 	fstate = (*inEnv)->GetFieldID(inEnv, playerstateclass, "state",  "S");
 	fsyncin = (*inEnv)->GetFieldID(inEnv, playerstateclass, "syncin",  "S");
 	fsyncout = (*inEnv)->GetFieldID(inEnv, playerstateclass, "syncout",  "S");
 	
 	if ((fdate == 0) || (ftempo == 0) || (ftsnum == 0)|| (ftsdenom == 0)
 		|| (ftsclick == 0)|| (ftsquarter == 0)|| (fbar == 0)|| (fbeat == 0)
 		|| (funit == 0)|| (fstate == 0)|| (fsyncin == 0)|| (fsyncout == 0)) {
 		//SysBeep(0);
 		return;
 	}
 	
 	(*inEnv)->SetIntField(inEnv, playerstate, fdate, tstate.date);
	(*inEnv)->SetIntField(inEnv, playerstate, ftempo, tstate.tempo);
 	(*inEnv)->SetShortField(inEnv, playerstate, ftsnum, tstate.tsnum);
 	(*inEnv)->SetShortField(inEnv, playerstate, ftsdenom, tstate.tsdenom);
 	(*inEnv)->SetShortField(inEnv, playerstate, ftsclick, tstate.tsclick);
 	(*inEnv)->SetShortField(inEnv, playerstate, ftsquarter, tstate.tsquarter);
 	(*inEnv)->SetShortField(inEnv, playerstate, fbar, tstate.bar);
 	(*inEnv)->SetShortField(inEnv, playerstate, fbeat, tstate.beat);
 	(*inEnv)->SetShortField(inEnv, playerstate, funit, tstate.unit);
 	(*inEnv)->SetShortField(inEnv, playerstate, fstate, tstate.state);
 	(*inEnv)->SetShortField(inEnv, playerstate, fsyncin,tstate.syncin);
 	(*inEnv)->SetShortField(inEnv, playerstate, fsyncout, tstate.syncout);
 }

/*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_ForwardStep
  (JNIEnv * inEnv, jclass cl, jint refnum, jint stepstate){

 	ForwardStepPlayer(refnum, stepstate);
 }
 /*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_BackwardStep
   (JNIEnv * inEnv, jclass cl, jint refnum, jint stepstate){


 	BackwardStepPlayer(refnum, stepstate);
 }
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_GetAllTrack
  (JNIEnv * inEnv, jclass cl, jint refnum){

 	return  (jint)GetAllTrackPlayer(refnum);
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_GetTrack
  (JNIEnv * inEnv, jclass cl, jint refnum, jint tracknum){

 	return (jint)GetTrackPlayer(refnum, tracknum);
 }
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_SetTrack
  (JNIEnv *inEnv, jclass cl, jint refnum, jint tracknum, jint seq){

 	return SetTrackPlayer(refnum, tracknum, (MidiSeqPtr)seq);
 }
/*--------------------------------------------------------------------------*/

 JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_SetAllTrack
  (JNIEnv * inEnv, jclass cl, jint refnum , jint seq, jint tpq){

   	return SetAllTrackPlayer(refnum, (MidiSeqPtr)seq, tpq);
 }
 
/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_grame_midishare_player_MidiPlayer_SetParam
  (JNIEnv * inEnv, jclass cl, jint refnum, jint tracknum, jint param, jint value){

 	SetParamPlayer( refnum, tracknum,  param ,  value);
 }
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_GetParam
  (JNIEnv * inEnv, jclass cl, jint refnum, jint tracknum, jint param){

 	return GetParamPlayer(refnum, tracknum, param);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_InsertAllTrack
  (JNIEnv * inEnv, jclass cl, jint refnum, jint seq){

 	return InsertAllTrackPlayer(refnum, (MidiSeqPtr)seq);
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_InsertTrack
  (JNIEnv * inEnv, jclass cl, jint refnum, jint tracknum, jint seq){

 	return InsertTrackPlayer(refnum,tracknum, (MidiSeqPtr)seq);
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_Load
  (JNIEnv * inEnv, jclass cl, jstring str, jint seq, jobject obj){

	/* no more implemented */
	
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_player_MidiPlayer_Save
  (JNIEnv * inEnv, jclass cl, jstring str, jint seq, jobject obj){

	/* no more implemented */
	
	return 0;
}
