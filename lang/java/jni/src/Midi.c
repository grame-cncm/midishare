/*
  MidiShare Project
  Copyright (C) Grame 1999-2009

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/*****************************************************************************/
/* Midi.c : interface to MidiShare 
 *
 * MidiShare home page 		: http://midishare.sourceforge.net
 *
 * Bug & comments report 	: MidiShare@grame.fr
 *	
 * History : 01/06/96 version 1.00
 *           16/07/96 version 1.01 gestion du package MidiShare
 *   		 20/07/96 version 1.02 fonctions de lecture des ev text : MidiGetText et MidiSetText 
 *           ??/??/?? version 1.03
 *           08/10/96 version 1.04 nom des packages en minuscule
 *           09/01/97 version 1.05 correction bug ReadSysEx (buffer vide apres creation d'une cellule)
 *           28/11/97 version 1.06 compatible JNI
 *           14/02/01 version 1.07 fonctions pour la gestion des drivers
 *           19/03/02 version 1.08 fonctions MidiGetInfo et MidiSetInfo nŽcessaires pour le thread bloquant sur MacOS9
 *           19/04/02 version 1.09 implementation de MidiForgetTask
 *           06/11/02 version 1.10 test de validitŽ des pointeurs sur des objets Java passŽ au C
 *           ??/??/?? version 1.11 version Intel
 *           01/08/06 version 1.12 utilisation des string JNI
 *
 *****************************************************************************/
#ifdef WIN32
#pragma warning(disable: 4244)
#endif

#include "MidiShare.h"
#include "MidiPtrType.h"


//#ifdef __x86_64__
# include "Midi64.h"
//#else
//# include "Midi.h"
//#endif


/*--------------------------------------------------------------------------*/
 void pTocCopy(char *dest,  unsigned char * src)  /* chaine pascal dans chaine c */
{
	register short i;
	
	i= *src++;
	while( i--)
		*dest++= *src++;
	*dest= 0;
}

/*--------------------------------------------------------------------------*/
 void cTocCopy(char *dest,  char * src) /* chaine c dans chaine c */
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;	
}

/*--------------------------------------------------------------------------*/
 void cTopCopy(unsigned char *dest,  char * src) /* chaine c dans chaine p */
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i+1] = src[i];
		i++;
	}
	dest[0] = i;	
}

/*--------------------------------------------------------------------------*/
#define Check_Pointer(env,ptr)\
    if ((ptr) == NULL) {\
        (*(env))->ThrowNew((env), (*(env))->FindClass((env),"java.lang.NullPointerException"), "");\
        return;\
    }\

/*--------------------------------------------------------------------------*/
#define Check_Pointer0(env,ptr)\
    if ((ptr) == NULL) {\
        (*(env))->ThrowNew((env), (*(env))->FindClass((env),"java.lang.NullPointerException"), "");\
        return 0;\
    }\

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AddField
  (JNIEnv * env, jclass  cl, javaptr event, jint val) {

	MidiAddField ((MidiEvPtr)event,val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AddSeq
  (JNIEnv * env, jclass cl, javaptr seq, javaptr ev) {

	MidiAddSeq ((MidiSeqPtr)seq,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ApplySeq
  (JNIEnv * env, jclass cl, javaptr a1 , javaptr a2) {
	/* not implemented */
}
 
/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_AvailEv
  (JNIEnv * env, jclass cl, jint refnum) {
  
 	return (javaptr) MidiAvailEv(refnum);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Call
  (JNIEnv * env, jclass cl, javaptr a0, javaptr a1, jint a2, jint a3, jint a4, jint a5) {

 	 /* not implemented */
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ClearSeq
  (JNIEnv * env, jclass cl, javaptr seq) {
  
 	MidiClearSeq((MidiSeqPtr)seq);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Close
  (JNIEnv * env, jclass cl, jint refnum) {

	MidiClose(refnum);
 }

 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Connect
  (JNIEnv * env, jclass cl, jint src, jint dst, jint state) {
  
 	MidiConnect(src,dst,state);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_CopyEv
  (JNIEnv * env, jclass cl, javaptr ev) {
  
	return (javaptr)MidiCopyEv((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountAppls
  (JNIEnv * env, jclass cl) {

	return MidiCountAppls();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountDTasks
  (JNIEnv * env, jclass cl, jint refnum) {
  
	return MidiCountDTasks (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountEvs
  (JNIEnv * env, jclass cl, jint refnum) {

	return MidiCountEvs(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountFields
  (JNIEnv * env, jclass cl, javaptr ev) {

	return MidiCountFields((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_DTask
  (JNIEnv * env, jclass cl, javaptr a0, javaptr a1, jint a2, jint a3, jint a4, jint a5) {

	 /* not implemented */
	
 	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Exec1DTask
  (JNIEnv * env, jclass cl, jint refnum) {

	MidiExec1DTask(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_Ext2IntTime
  (JNIEnv * env, jclass cl, javadate time) {
  
	return MidiExt2IntTime (time);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FlushDTasks
  (JNIEnv * env, jclass cl, jint refnum) {

	MidiFlushDTasks (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FlushEvs
  (JNIEnv * env, jclass cl, jint refnum) {

	MidiFlushEvs (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ForgetTask
  (JNIEnv * env, jclass cl, javaptr ev) {

     MidiEvPtr adr = (MidiEvPtr)ev;
     MidiForgetTask(&adr);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeCell
  (JNIEnv * env, jclass cl, javaptr ev) {

	MidiFreeCell ((MidiEvPtr) ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeEv
  (JNIEnv * env, jclass cl, javaptr ev) {

	MidiFreeEv ((MidiEvPtr) ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeSeq
  (JNIEnv * env, jclass cl, javaptr seq) {
  
	MidiFreeSeq ((MidiSeqPtr) seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_FreeSpace
  (JNIEnv * env, jclass cl) {

	return MidiFreeSpace();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetApplAlarm
  (JNIEnv * env, jclass cl, jint a0) {

	/* not implemented */
	return 0;	
}
/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetEv
  (JNIEnv * env, jclass cl, jint refnum) {
  
	return (javaptr) MidiGetEv(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetExtTime
  (JNIEnv * env, jclass cl) {

	 return MidiGetExtTime();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetField
  (JNIEnv * env, jclass cl, javaptr ev, jint i) {
  
	return MidiGetField((MidiEvPtr)ev,i);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetFilter
  (JNIEnv * env, jclass cl, jint refnum) {

	return (javaptr)MidiGetFilter(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetIndAppl
  (JNIEnv * env, jclass cl, jint refnum) {

	return (jint)MidiGetIndAppl(refnum);
}

/*--------------------------------------------------------------------------*/


JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetInfo
  (JNIEnv * env, jclass cl, jint refnum) {
  
	return (javaptr)MidiGetInfo(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jstring JNICALL Java_grame_midishare_Midi_GetName
  (JNIEnv * env, jclass cl, jint refnum) {
  
	const char* midiname = MidiGetName(refnum);
	return (*env)->NewStringUTF(env, (midiname ? midiname : ""));
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetNamedAppl
  (JNIEnv * env, jclass cl, jstring midiname) {

	  const char *applName;
	  jint refNum;

	  applName = (*env)->GetStringUTFChars(env, midiname, NULL);
	  if (applName == NULL) { return 0; /* OutOfMemoryError already thrown */ }
	  refNum = (jint)MidiGetNamedAppl(applName);
	  (*env)->ReleaseStringUTFChars(env, midiname, applName); 
	  return refNum;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetPortState
  (JNIEnv * env, jclass cl, jint port) {

	/* no more implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetRcvAlarm
  (JNIEnv * env, jclass cl, jint a0) {

	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_GetSyncInfo
  (JNIEnv *  inEnv, jclass cl, jobject syncinfo) {

	TSyncInfo info;
	jfieldID time,reenter,syncMode,syncLocked,syncPort,syncStart,syncStop,syncOffset,syncSpeed,syncBreaks,syncFormat;
 	jclass syncinfoclass;
        
        Check_Pointer(inEnv,syncinfo);
        syncinfoclass = (*inEnv)->GetObjectClass(inEnv, syncinfo);
 
	MidiGetSyncInfo (&info);
	
 	time = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "time",  "I");
 	reenter = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "reenter",  "I");
 	syncMode = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncMode",  "I");
 	syncLocked = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncLocked",  "I");
 	syncPort = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncPort",  "I");
 	syncStart = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncStart",  "I");
 	syncStop = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncStop",  "I");
 	syncOffset = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncOffset",  "I");
 	syncSpeed = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncSpeed",  "I");
 	syncBreaks = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncBreaks",  "I");
 	syncFormat = (*inEnv)->GetFieldID(inEnv, syncinfoclass, "syncFormat",  "I");
	
	if ((time == 0) || (reenter == 0) || (syncLocked == 0)|| (syncPort == 0)
 		|| (syncStart == 0)|| (syncStop == 0)|| (syncOffset == 0)|| (syncSpeed == 0)
 		|| (syncBreaks == 0)|| (syncFormat == 0)|| (syncPort == 0)) {
 		return;
 	}
 	
 	(*inEnv)->SetIntField(inEnv, syncinfo, time, info.time);
	(*inEnv)->SetIntField(inEnv, syncinfo, reenter,info.reenter) ;
	(*inEnv)->SetIntField(inEnv, syncinfo, syncMode,info.syncMode );
	(*inEnv)->SetIntField(inEnv, syncinfo, syncLocked,info.syncLocked) ;
	(*inEnv)->SetIntField(inEnv, syncinfo, syncPort,info.syncPort) ;
	(*inEnv)->SetIntField(inEnv, syncinfo, syncStart, info.syncStart) ;
	(*inEnv)->SetIntField(inEnv, syncinfo, syncStop,info.syncStop) ;
	(*inEnv)->SetIntField(inEnv, syncinfo, syncOffset,info.syncOffset) ;
	(*inEnv)->SetIntField(inEnv, syncinfo, syncSpeed, info.syncSpeed);
	(*inEnv)->SetIntField(inEnv, syncinfo, syncBreaks, info.syncBreaks );
	(*inEnv)->SetIntField(inEnv, syncinfo, syncFormat,info.syncFormat );
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetTime
  (JNIEnv * env, jclass cl) {

	return MidiGetTime();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetVersion
  (JNIEnv * env, jclass cl) {
  
	return MidiGetVersion();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GrowSpace
  (JNIEnv * env, jclass cl, jint val) {

	return MidiGrowSpace (val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_Int2ExtTime
  (JNIEnv * env, jclass cl, javadate time) {
  
	return MidiInt2ExtTime (time);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsConnected
  (JNIEnv * env, jclass cl, jint src, jint dst) {
  
	return MidiIsConnected(src,dst);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_NewCell
  (JNIEnv * env, jclass cl) {

	return (javaptr)MidiNewCell();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_NewEv
  (JNIEnv * env, jclass cl, jint type) {
  
	  return (javaptr)MidiNewEv(type);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_NewSeq
  (JNIEnv * env, jclass cl) {

	return (javaptr) MidiNewSeq();
}
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Open
  (JNIEnv * env, jclass cl, jstring str) {

	const char *midiname;
	jint refNum;

	midiname = (*env)->GetStringUTFChars(env, str, NULL);
	if (midiname == NULL) { return 0; /* OutOfMemoryError already thrown */ }
	refNum = (jint) MidiOpen(midiname);
	(*env)->ReleaseStringUTFChars(env, str, midiname); 
	return refNum;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_ReadSync
  (JNIEnv * env, jclass cl, javaptr a0) {
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Send
  (JNIEnv * env, jclass cl, jint ref, javaptr ev) {

	MidiSend(ref,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SendAt
  (JNIEnv * env, jclass cl, jint ref , javaptr ev , javadate date) {
  
	MidiSendAt(ref,(MidiEvPtr)ev,date);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SendIm
  (JNIEnv * env, jclass cl, jint ref, javaptr ev) { 
  
	MidiSendIm(ref,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetApplAlarm
  (JNIEnv * env, jclass cl, jint a0, javaptr a1) {

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetField
  (JNIEnv * env, jclass cl, javaptr ev , jint i , jint val) {
  
	  MidiSetField((MidiEvPtr)ev,i,val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetFilter
  (JNIEnv * env, jclass cl, jint ref, javaptr filter) {

	MidiSetFilter(ref,(MidiFilterPtr)filter);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetInfo
  (JNIEnv * env, jclass cl, jint refnum, javaptr info) {

	MidiSetInfo(refnum,(void*)info);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetName
  (JNIEnv * env, jclass cl, jint ref, jstring str) {
  
	const char *midiname;

	midiname = (*env)->GetStringUTFChars(env, str, NULL);
	if (midiname) {
		MidiSetName(ref,midiname);
		(*env)->ReleaseStringUTFChars(env, str, midiname); 
	}
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetPortState
  (JNIEnv * env, jclass cl, jint port, jint state) {
  
  	/* no more implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetRcvAlarm
  (JNIEnv * env, jclass cl, jint a0, javaptr a1) {

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_SetSyncMode
  (JNIEnv * env, jclass cl, jint a0) {
  
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Share
  (JNIEnv * env, jclass cl) {

	return MidiShare();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Smpte2Time
  (JNIEnv * inEnv , jclass cl, jobject smpteloc) {
	
	TSmpteLocation loc;
 	jfieldID fformat,fhours,fminutes,fseconds,fframes,ffracs;
 	jclass smpteposclass;
        
	Check_Pointer0(inEnv,smpteloc);
	smpteposclass = (*inEnv)->GetObjectClass(inEnv, smpteloc); 
 	
 	fformat = (*inEnv)->GetFieldID(inEnv, smpteposclass, "format",  "S");
 	fhours = (*inEnv)->GetFieldID(inEnv, smpteposclass, "hours",  "S");
 	fminutes = (*inEnv)->GetFieldID(inEnv, smpteposclass, "minutes",  "S");
	fseconds = (*inEnv)->GetFieldID(inEnv, smpteposclass, "seconds",  "S");
 	fframes = (*inEnv)->GetFieldID(inEnv, smpteposclass, "frames",  "S");
 	ffracs = (*inEnv)->GetFieldID(inEnv, smpteposclass, "fracs",  "S");

	if ((fformat== 0) || (fhours== 0) ||(fminutes== 0) ||
		(fseconds== 0) || (fframes== 0) ||(ffracs== 0) ) return 0;
 			
 	loc.format = 	(*inEnv)->GetShortField(inEnv,smpteloc,fformat); 
 	loc.hours = (*inEnv)->GetShortField(inEnv,smpteloc,fhours); 
 	loc.minutes =	(*inEnv)->GetShortField(inEnv,smpteloc,fminutes); 
 	loc.seconds = (*inEnv)->GetShortField(inEnv,smpteloc,fseconds); 
 	loc.frames = (*inEnv)->GetShortField(inEnv,smpteloc,fframes); 
 	loc.fracs = (*inEnv)->GetShortField(inEnv,smpteloc,ffracs); 	
		
	return MidiSmpte2Time(&loc);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_Task
  (JNIEnv * env, jclass cl, javaptr a0, javaptr a1, jint a2, jint a3, jint a4, jint a5) {
  
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Time2Smpte
  (JNIEnv *  inEnv, jclass cl, javadate time, jint format, jobject smpteloc) {

	TSmpteLocation loc;
 	jfieldID fformat,fhours,fminutes,fseconds,fframes,ffracs;
 	jclass smpteposclass;
        
        Check_Pointer(inEnv,smpteloc);
        smpteposclass = (*inEnv)->GetObjectClass(inEnv, smpteloc); 
 
 	MidiTime2Smpte(time,format, &loc);	
 	
 	fformat = (*inEnv)->GetFieldID(inEnv, smpteposclass, "format",  "S");
 	fhours = (*inEnv)->GetFieldID(inEnv, smpteposclass, "hours",  "S");
 	fminutes = (*inEnv)->GetFieldID(inEnv, smpteposclass, "minutes",  "S");
	fseconds = (*inEnv)->GetFieldID(inEnv, smpteposclass, "seconds",  "S");
 	fframes = (*inEnv)->GetFieldID(inEnv, smpteposclass, "frames",  "S");
 	ffracs = (*inEnv)->GetFieldID(inEnv, smpteposclass, "fracs",  "S");

	if ((fformat== 0) || (fhours== 0) ||(fminutes== 0) ||
		(fseconds== 0) || (fframes== 0) ||(ffracs== 0) ) return;
				
 	(*inEnv)->SetShortField(inEnv,smpteloc,fformat,	loc.format); 
 	(*inEnv)->SetShortField(inEnv,smpteloc,fhours,loc.hours); 
 	(*inEnv)->SetShortField(inEnv,smpteloc,fminutes,loc.minutes); 
 	(*inEnv)->SetShortField(inEnv,smpteloc,fseconds,loc.seconds); 
 	(*inEnv)->SetShortField(inEnv,smpteloc,fframes,loc.frames); 
 	(*inEnv)->SetShortField(inEnv,smpteloc,ffracs,loc.fracs); 
}	

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_TotalSpace
  (JNIEnv * env, jclass cl) {
  
	return MidiTotalSpace();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_WriteSync
  (JNIEnv * env, jclass cl, javaptr a0, javaptr a1) {
  
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

/* FOR COMMON FIELDS MANAGEMENT */

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetLink
  (JNIEnv * env, jclass cl, javaptr ev) {

	return (javaptr)Link((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetLink
  (JNIEnv * env, jclass cl, javaptr ev , javaptr dst) {

	Link((MidiEvPtr)ev) = (MidiEvPtr)dst;	
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javadate JNICALL Java_grame_midishare_Midi_GetDate
  (JNIEnv * env, jclass cl, javaptr ev) {
  
	return (javadate)Date((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetDate
  (JNIEnv * env, jclass cl, javaptr ev, javadate date) {

	Date((MidiEvPtr)ev) = date;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetRefnum
  (JNIEnv * env, jclass cl, javaptr ev) {
  
	return (jint)RefNum((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetRefnum
  (JNIEnv * env, jclass cl, javaptr ev, jint ref) {

	RefNum((MidiEvPtr)ev) = ref;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetType
  (JNIEnv * env, jclass cl, javaptr ev) {

	return(jint)EvType((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetType
  (JNIEnv * env, jclass cl, javaptr ev, jint type) {

	EvType((MidiEvPtr)ev) = type;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetChan
  (JNIEnv * env, jclass cl, javaptr ev) {

	return (jint)Chan((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetChan
  (JNIEnv * env, jclass cl, javaptr ev, jint chan) {

	Chan((MidiEvPtr)ev) = chan;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetPort
  (JNIEnv * env, jclass cl, javaptr ev) { 

	return (jint)Port((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetPort
  (JNIEnv * env, jclass cl, javaptr ev, jint port) {

	Port((MidiEvPtr)ev) = port;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData0
  (JNIEnv * env, jclass cl, javaptr ev) {

 	return MidiGetField((MidiEvPtr)ev, 0);
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData1
  (JNIEnv * env, jclass cl, javaptr ev) {

 	return MidiGetField((MidiEvPtr)ev, 1);
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData2
  (JNIEnv * env, jclass cl, javaptr ev) {

 	return MidiGetField((MidiEvPtr)ev, 2);
  }
  
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData3
  (JNIEnv * env, jclass cl, javaptr ev) {

 	return MidiGetField((MidiEvPtr)ev, 3);
  }
  
/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData0
  (JNIEnv * env, jclass cl, javaptr ev, jint val) {


 	MidiSetField((MidiEvPtr)ev, 0, val);
  }

/*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData1
  (JNIEnv * env, jclass cl, javaptr ev, jint val) {

 	MidiSetField((MidiEvPtr)ev, 1, val);
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData2
  (JNIEnv * env, jclass cl, javaptr ev, jint val) {

 	MidiSetField((MidiEvPtr)ev, 2, val);
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData3
  (JNIEnv * env, jclass cl, javaptr ev, jint val) {

 	MidiSetField((MidiEvPtr)ev, 3, val);
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT jstring JNICALL Java_grame_midishare_Midi_GetText
  (JNIEnv * env, jclass cl, javaptr ev) {

  	MidiEvPtr tmp = (MidiEvPtr) ev;
	long i, n = MidiCountFields(tmp);
	char buffer[256];
	
	for (i = 0; i < n; i++) {
		buffer[i] = MidiGetField(tmp, i);
	}
	buffer[i] = 0; /* stops the string */
	return (*env)->NewStringUTF(env,buffer);
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetText
  (JNIEnv * env, jclass cl, javaptr ev, jstring str) {

 	long i = 0;
	const char *text = (*env)->GetStringUTFChars(env, str, NULL);
	if (text == NULL) { return; /* OutOfMemoryError already thrown */ }
	
	while (text[i]!= 0) {
		MidiAddField((MidiEvPtr)ev,text[i]);
		i++; 
	}

	(*env)->ReleaseStringUTFChars(env, str, text); 
 }
  

/*--------------------------------------------------------------------------*/
/* FOR SEQUENCES MANAGEMENT */
/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetFirstEv
  (JNIEnv * env, jclass cl, javaptr seq) {

	return (javaptr) FirstEv((MidiSeqPtr)seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetFirstEv
  (JNIEnv * env, jclass cl, javaptr seq, javaptr ev) {

	FirstEv((MidiSeqPtr)seq) = (MidiEvPtr)ev;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_GetLastEv
  (JNIEnv * env, jclass cl, javaptr seq) {

	return (javaptr) LastEv((MidiSeqPtr)seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetLastEv
  (JNIEnv * env, jclass cl, javaptr seq, javaptr ev) {

	LastEv((MidiSeqPtr)seq) = (MidiEvPtr)ev;
}

/*--------------------------------------------------------------------------*/
/* FOR FILTER MANAGEMENT */
/*--------------------------------------------------------------------------*/

JNIEXPORT javaptr JNICALL Java_grame_midishare_Midi_NewFilter
  (JNIEnv * env, jclass cl) {

	return (javaptr)MidiNewFilter();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeFilter
  (JNIEnv * env, jclass cl, javaptr filter) {

	MidiFreeFilter((MidiFilterPtr) filter);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptPort
  (JNIEnv * env, jclass cl, javaptr filter, jint val, jint c) {
	
	MidiAcceptPort ((MidiFilterPtr) filter,val,c);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptChan
  (JNIEnv * env, jclass cl, javaptr filter, jint val, jint c) {

	MidiAcceptChan ((MidiFilterPtr) filter,val,c);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptType
  (JNIEnv * env, jclass cl, javaptr filter, jint val, jint c) {

	MidiAcceptType ((MidiFilterPtr) filter,val,c);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsAcceptedPort
	(JNIEnv * env, jclass cl, javaptr filter, jint port) {
  
 	return MidiIsAcceptedPort((MidiFilterPtr)filter,port);
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsAcceptedChan
	(JNIEnv * env, jclass cl, javaptr filter, jint chan) {
  
 	return MidiIsAcceptedChan((MidiFilterPtr)filter,chan);
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsAcceptedType
	(JNIEnv * env, jclass cl, javaptr filter, jint type) {
  
 	return MidiIsAcceptedType((MidiFilterPtr)filter,type);
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_WriteEv
  (JNIEnv * env , jclass cl, jobject ob ) {
  
 	/* not implemented */
 	
 	return 0;
 }

/*--------------------------------------------------------------------------*/
JNIEXPORT jint JNICALL Java_grame_midishare_Midi_ReadEv
  (JNIEnv * env , jclass cl, jobject ob) {
 	
 	/* not implemented */
 	
 	return 0; 	
 }

/*--------------------------------------------------------------------------*/
/* FOR DRIVER MANAGEMENT */
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountDrivers
	(JNIEnv * env , jclass cl) {
	
	return MidiCountDrivers();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetIndDriver
	(JNIEnv * env , jclass cl, jint index) {
	
	return MidiGetIndDriver(index);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetDriverInfos
	(JNIEnv * inEnv , jclass cl, jint refnum, jobject jinfos) {

	TDriverInfos infos;
	jfieldID name, version, slots;
	Boolean res;
	jstring infosName;
	jclass infosclass ;

	Check_Pointer0(inEnv,jinfos);
	infosclass = (*inEnv)->GetObjectClass(inEnv, jinfos);

	res = MidiGetDriverInfos(refnum,&infos);

	if (res) {
		name = (*inEnv)->GetFieldID(inEnv, infosclass, "name",  "Ljava/lang/String;");
		version = (*inEnv)->GetFieldID(inEnv, infosclass, "version",  "S");
		slots = (*inEnv)->GetFieldID(inEnv, infosclass, "slots",  "S");
		
		infosName = (*inEnv)->NewStringUTF(inEnv, infos.name);
		if (infosName == NULL) { return 0; /* out of memory */ }
		(*inEnv)->SetObjectField(inEnv, jinfos, name, infosName); /* Set the java String */
		(*inEnv)->SetShortField(inEnv, jinfos, version,infos.version) ;
		(*inEnv)->SetShortField(inEnv, jinfos, slots,infos.drvslots );
		(*inEnv)->DeleteLocalRef(inEnv, infosName);
	}

	(*inEnv)->DeleteLocalRef(inEnv, infosclass);
	return res;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_AddSlot
	(JNIEnv * env , jclass cl, jint refnum, jstring str, jint slotdirection) {
	
	SlotRefNum ref;
 	const char *slotname;

	slotname = (*env)->GetStringUTFChars(env, str, NULL);
	if (slotname == NULL) { return 0; /* OutOfMemoryError already thrown */ }
	
	ref =  MidiAddSlot(refnum, slotname,(SlotDirection)slotdirection);

	(*env)->ReleaseStringUTFChars(env, str, slotname); 
	return *(jint *)&ref;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetIndSlot
	(JNIEnv * env , jclass cl, jint refnum, jint index) {
	
	SlotRefNum ref = MidiGetIndSlot(refnum, index);
	return *(jint *)&ref;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_RemoveSlot
	(JNIEnv * env , jclass cl, jint slotrefnum) {
	
	MidiRemoveSlot(*(SlotRefNum *)&slotrefnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetSlotInfos
	(JNIEnv * inEnv , jclass cl, jint slotrefnum, jobject jinfos) {
	
	TSlotInfos infos;
	jfieldID name,direction,cnx;
	jstring infosName;
	jbyteArray infosCnx;
	jclass infosclass;
    Boolean res;
        
	Check_Pointer0(inEnv,jinfos);
	infosclass = (*inEnv)->GetObjectClass(inEnv, jinfos);
	res = MidiGetSlotInfos(*(SlotRefNum *)&slotrefnum,&infos);
        
	if (res) {

		name = (*inEnv)->GetFieldID(inEnv, infosclass, "name",  "Ljava/lang/String;");
		direction = (*inEnv)->GetFieldID(inEnv, infosclass, "direction",  "I");
		cnx = (*inEnv)->GetFieldID(inEnv, infosclass, "cnx",  "[B"); 
	   
		infosName = (*inEnv)->NewStringUTF(inEnv, infos.name);
		if (infosName == NULL) { return 0; /* out of memory */ }

		infosCnx = (*inEnv)->NewByteArray(inEnv, 32);
		if (infosCnx == NULL) { return 0; /* out of memory */ }
		(*inEnv)->SetByteArrayRegion(inEnv, infosCnx, 0, 32, infos.cnx);

		(*inEnv)->SetObjectField(inEnv, jinfos, name, infosName); /* Set the java String  */
		(*inEnv)->SetIntField(inEnv, jinfos, direction,infos.direction) ;
		(*inEnv)->SetObjectField(inEnv, jinfos, cnx, infosCnx); /* Set the connection byte array */
		(*inEnv)->DeleteLocalRef(inEnv, infosName);
		(*inEnv)->DeleteLocalRef(inEnv, infosCnx);
	}

	(*inEnv)->DeleteLocalRef(inEnv, infosclass);
	return res;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ConnectSlot
	(JNIEnv * env , jclass cl, jint port, jint slotrefnum, jint state) {
		
	MidiConnectSlot(port, *(SlotRefNum *)&slotrefnum, state);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsSlotConnected
	(JNIEnv * env , jclass cl, jint port, jint slotrefnum) {
	
	return (jint) MidiIsSlotConnected(port, *(SlotRefNum *)&slotrefnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetSlotName
	(JNIEnv * env , jclass cl, jint slotrefnum, jstring str) {
	
	const char *slotname;

	slotname = (*env)->GetStringUTFChars(env, str, NULL);
	if (slotname) {
		MidiSetSlotName(*(SlotRefNum *)&slotrefnum, slotname);
		(*env)->ReleaseStringUTFChars(env, str, slotname); 
	}
}

int main () {return 0;}
