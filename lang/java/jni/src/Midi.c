/*

  Copyright © Grame 1999-2000

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr

*/

/*****************************************************************************/
/* Midi.c : interface to MidiShare 
/*
/* MidiShare home page 		: http://www.grame.fr/MidiShare/
/* MidiShare/Java home page 	: http://www.grame.fr/MidiShare/Develop/Java.html
/*
/* Bug & comments report 	: MidiShare@rd.grame.fr
/*	
/* History : 1/06/96  version 1.00
/*           16/07/96 version 1.01 gestion du package MidiShare
/*   		 20/07/96 fonctions de lecture des ev text : MidiGetText et MidiSetText 
/*            ....... version 1.03
/*           8/10/96  version 1.04 nom des packages en minuscule
/*           9/01/97  version 1.05 correction bug ReadSysEx (buffer vide apres creation d'une cellule)
/*           28/11/97 version 1.06 compatible JNI
/*           14/02/01 version 1.07 fonctions pour la gestion des drivers
/*
/*****************************************************************************/

#ifdef __Macintosh__
	#ifdef __MacOS9__
		#include <MidiShare.h>
		#include <Memory.h>   // for NewPtr and DisposePtr
	#else
		#include <MidiShare.h>
		#include <stdlib.h>
		#define NewPtr(v) malloc((v))
		#define DisposePtr(ptr) free((Ptr)(ptr))
	#endif
#endif

#ifdef __Linux__
	#include <MidiShare.h>
	#include <stdlib.h>
		
	#define NewPtr(v) malloc((v))
	#define DisposePtr(ptr) free((Ptr)(ptr))
	
	#define MSALARMAPI
#endif

#ifdef WIN32
	#include <MidiShare.h>
#endif

#include "Midi.h"


/*--------------------------------------------------------------------------*/
 void pTocCopy(  char *dest,  unsigned char * src)  /* chaine pascal dans chaine c */
{
	register short i;
	
	i= *src++;
	while( i--)
		*dest++= *src++;
	*dest= 0;
}

/*--------------------------------------------------------------------------*/
 void cTocCopy(  char *dest,  char * src) /* chaine c dans chaine c */
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;	
}

/*--------------------------------------------------------------------------*/
 void cTopCopy(  unsigned char *dest,  char * src) /* chaine c dans chaine p */
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i+1] = src[i];
		i++;
	}
	dest[0] = i;	
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AddField
  (JNIEnv * env, jclass  cl, jint event, jint val) {

	MidiAddField((MidiEvPtr)event,val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AddSeq
  (JNIEnv * env, jclass cl, jint seq, jint ev){

	MidiAddSeq((MidiSeqPtr)seq,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ApplySeq
  (JNIEnv * env, jclass cl, jint a1 , jint a2){
	/* not implemented */
}
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_AvailEv
  (JNIEnv * env, jclass cl, jint refnum) {
  
 	return (jint) MidiAvailEv(refnum);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Call
  (JNIEnv * env, jclass cl, jint a0, jint a1, jint a2, jint a3, jint a4, jint a5){

 	 /* not implemented */
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ClearSeq
  (JNIEnv * env, jclass cl, jint seq) {
  
 	MidiClearSeq((MidiSeqPtr)seq);
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Close
  (JNIEnv * env, jclass cl, jint refnum){

 	 MidiClose(refnum);
 }

 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Connect
  (JNIEnv * env, jclass cl, jint src, jint dst, jint state){
  
 	MidiConnect(src,dst,state);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CopyEv
  (JNIEnv * env, jclass cl, jint ev) {
  
	return (jint)MidiCopyEv((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountAppls
  (JNIEnv * env, jclass cl){

	return MidiCountAppls();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountDTasks
  (JNIEnv * env, jclass cl, jint refnum){
  
	return MidiCountDTasks (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountEvs
  (JNIEnv * env, jclass cl, jint refnum) {

	return MidiCountEvs(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountFields
  (JNIEnv * env, jclass cl, jint ev){

	return MidiCountFields((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_DTask
  (JNIEnv * env, jclass cl,jint a0, jint a1, jint a2, jint a3, jint a4, jint a5){

	 /* not implemented */
	
 	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Exec1DTask
  (JNIEnv * env, jclass cl, jint refnum){

	MidiExec1DTask(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Ext2IntTime
  (JNIEnv * env, jclass cl, jint time){
  
	return MidiExt2IntTime (time);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FlushDTasks
  (JNIEnv * env, jclass cl, jint refnum){

	MidiFlushDTasks (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FlushEvs
  (JNIEnv * env, jclass cl, jint refnum){

	MidiFlushEvs (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ForgetTask
  (JNIEnv * env, jclass cl, jint a0){

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeCell
  (JNIEnv * env, jclass cl, jint ev){

	MidiFreeCell ((MidiEvPtr) ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeEv
  (JNIEnv * env, jclass cl, jint ev){

	MidiFreeEv ((MidiEvPtr) ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeSeq
  (JNIEnv * env, jclass cl, jint seq){
  
	MidiFreeSeq ((MidiSeqPtr) seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_FreeSpace
  (JNIEnv * env, jclass cl){

	return MidiFreeSpace();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetApplAlarm
  (JNIEnv * env, jclass cl, jint a0){

	/* not implemented */
	return 0;	
}
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetEv
  (JNIEnv * env, jclass cl, jint refnum){
  
	return (jint) MidiGetEv(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetExtTime
  (JNIEnv * env, jclass cl){

	 return MidiGetExtTime();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetField
  (JNIEnv * env, jclass cl, jint ev, jint i){
  
	return MidiGetField((MidiEvPtr)ev,i);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetFilter
  (JNIEnv * env, jclass cl, jint refnum){

	return (jint)MidiGetFilter(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetIndAppl
  (JNIEnv * env, jclass cl, jint refnum){

	return (jint)MidiGetIndAppl(refnum);
}

/*--------------------------------------------------------------------------*/


JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetInfo
  (JNIEnv * env, jclass cl, jint a0){
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetNameAux
  (JNIEnv * env, jclass cl, jint refnum){
  
	char *  buffer = (char *)NewPtr(128);
	
	if (buffer) {
		MidiName midiname =  MidiGetName(refnum);
		
		#if defined(__Macintosh__) && defined(__MacOS9__)
			pTocCopy (buffer, midiname);
		#else
			cTocCopy (buffer, midiname);
		#endif	
	}

	return (jint) buffer;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetNamedApplAux
  (JNIEnv * env, jclass cl, jint midiname) {
  
	#if defined(__Macintosh__) && defined(__MacOS9__)
		unsigned char buffer [128];
		cTopCopy (buffer,(char*)midiname);
	#else
		char buffer [128];
		cTocCopy (buffer,(char*)midiname);		
	#endif
	
	return (jint)MidiGetNamedAppl (buffer);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetPortState
  (JNIEnv * env, jclass cl, jint port){

	/* no more implemented */
	//return MidiGetPortState(port);
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetRcvAlarm
  (JNIEnv * env, jclass cl, jint a0){

	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_GetSyncInfo
  (JNIEnv *  inEnv, jclass cl, jobject syncinfo){

	TSyncInfo info;
	jfieldID time,reenter,syncMode,syncLocked,syncPort,syncStart,syncStop,syncOffset,syncSpeed,syncBreaks,syncFormat;
 	jclass syncinfoclass = (*inEnv)->GetObjectClass(inEnv, syncinfo);
 
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

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetTime
  (JNIEnv * env, jclass cl){

	return MidiGetTime();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetVersion
  (JNIEnv * env, jclass cl){
  
	return MidiGetVersion();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GrowSpace
  (JNIEnv * env, jclass cl, jint val){

	return MidiGrowSpace (val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Int2ExtTime
  (JNIEnv * env, jclass cl, jint time){
  
	return MidiInt2ExtTime (time);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsConnected
  (JNIEnv * env, jclass cl, jint src, jint dst) {
  
	return MidiIsConnected(src,dst);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewCell
  (JNIEnv * env, jclass cl) {

	return (jint)MidiNewCell();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewEv
  (JNIEnv * env, jclass cl, jint type) {
  
	return (jint)MidiNewEv(type);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewSeq
  (JNIEnv * env, jclass cl){

	return (jint) MidiNewSeq();
}
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_OpenAux
  (JNIEnv * env, jclass cl, jint str) {
		
	#if defined(__Macintosh__) && defined(__MacOS9__)
		unsigned char buffer [128];
		char * midiname = (char*)str;
		cTopCopy (buffer, midiname);
	#else
		char buffer [128];
		char * midiname = (char*)str;
		cTocCopy (buffer, midiname);
	#endif	
	
	return  (jint)MidiOpen(buffer);

}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_ReadSync
  (JNIEnv * env, jclass cl, jint a0){
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Send
  (JNIEnv * env, jclass cl, jint ref, jint ev){

	MidiSend(ref,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SendAt
  (JNIEnv * env, jclass cl, jint ref , jint ev , jint date){
  
	MidiSendAt(ref,(MidiEvPtr)ev,date);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SendIm
  (JNIEnv * env, jclass cl, jint ref, jint ev){ 
  
	MidiSendIm(ref,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetApplAlarm
  (JNIEnv * env, jclass cl, jint a0, jint a1){

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetField
  (JNIEnv * env, jclass cl, jint ev , jint i , jint val){
  
	MidiSetField((MidiEvPtr)ev,i,val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetFilter
  (JNIEnv * env, jclass cl, jint ref, jint filter){

	MidiSetFilter(ref,(MidiFilterPtr)filter);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetInfo
  (JNIEnv * env, jclass cl, jint a0, jint a1){

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetNameAux
  (JNIEnv * env, jclass cl, jint ref, jint midiname){
  
	#if defined(__Macintosh__) && defined(__MacOS9__)
		unsigned char buffer [128];
		cTopCopy (buffer, (char*)midiname);
	#else
		char buffer [128];
		cTocCopy (buffer, (char*)midiname);
	#endif	
		
	MidiSetName(ref,buffer);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetPortState
  (JNIEnv * env, jclass cl, jint port, jint state){
  
  	/* no more implemented */
	//MidiSetPortState(port,state);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetRcvAlarm
  (JNIEnv * env, jclass cl, jint a0, jint a1){

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
  (JNIEnv * env, jclass cl){

	return MidiShare();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Smpte2Time
  (JNIEnv * inEnv , jclass cl, jobject smpteloc) {
	
	TSmpteLocation loc;
 	jfieldID fformat,fhours,fminutes,fseconds,fframes,ffracs;
 	
 	jclass smpteposclass = (*inEnv)->GetObjectClass(inEnv, smpteloc);
 	
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

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Task
  (JNIEnv * env, jclass cl, jint a0, jint a1, jint a2, jint a3, jint a4, jint a5){
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Time2Smpte
  (JNIEnv *  inEnv, jclass cl, jint time, jint format, jobject smpteloc){

	TSmpteLocation loc;
 	jfieldID fformat,fhours,fminutes,fseconds,fframes,ffracs;
 	jclass smpteposclass = (*inEnv)->GetObjectClass(inEnv, smpteloc);
 	
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
  (JNIEnv * env, jclass cl){
  
	return MidiTotalSpace();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_WriteSync
  (JNIEnv * env, jclass cl, jint a0, jint a1){
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

/* FOR COMMON FIELDS MANAGEMENT */

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetLink
  (JNIEnv * env, jclass cl, jint ev){

	return (jint)Link((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetLink
  (JNIEnv * env, jclass cl, jint ev , jint dst){

	Link((MidiEvPtr)ev) = (MidiEvPtr)dst;	
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetDate
  (JNIEnv * env, jclass cl, jint ev){
  
	return (jint)Date((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/


JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetDate
  (JNIEnv * env, jclass cl, jint ev, jint date){

	Date((MidiEvPtr)ev) = date;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetRefnum
  (JNIEnv * env, jclass cl, jint ev){
  
	return (jint)RefNum((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetRefnum
  (JNIEnv * env, jclass cl, jint ev, jint ref){

	RefNum((MidiEvPtr)ev) = ref;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetType
  (JNIEnv * env, jclass cl, jint ev){

	return(jint)EvType((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetType
  (JNIEnv * env, jclass cl, jint ev, jint type){

	EvType((MidiEvPtr)ev) = type;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetChan
  (JNIEnv * env, jclass cl, jint ev){

	return (jint)Chan((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetChan
  (JNIEnv * env, jclass cl, jint ev, jint chan){

	Chan((MidiEvPtr)ev) = chan;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetPort
  (JNIEnv * env, jclass cl, jint ev) { 

	return (jint)Port((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetPort
  (JNIEnv * env, jclass cl, jint ev, jint port){

	Port((MidiEvPtr)ev) = port;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData0
  (JNIEnv * env, jclass cl, jint ev){

 	return (jint)Data((MidiEvPtr)ev)[0];
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData1
  (JNIEnv * env, jclass cl, jint ev){

 	return (jint)Data((MidiEvPtr)ev)[1];
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData2
  (JNIEnv * env, jclass cl, jint ev){

 	return (jint)Data((MidiEvPtr)ev)[2];
  }
  
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData3
  (JNIEnv * env, jclass cl, jint ev){

 	return (jint)Data((MidiEvPtr)ev)[3];
  }
  
/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData0
  (JNIEnv * env, jclass cl, jint ev, jint val){


 	Data((MidiEvPtr)ev)[0] = val;
  }

/*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData1
  (JNIEnv * env, jclass cl, jint ev, jint val){

 	Data((MidiEvPtr)ev)[1] = val;
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData2
  (JNIEnv * env, jclass cl, jint ev, jint val){

 	Data((MidiEvPtr)ev)[2] = val;
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData3
  (JNIEnv * env, jclass cl, jint ev, jint val){

 	Data((MidiEvPtr)ev)[3] = val;
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetTextAux
  (JNIEnv * env, jclass cl, jint ev){

  	MidiEvPtr tmp = (MidiEvPtr) ev;
	long i, n = MidiCountFields(tmp);
	char * buffer;
	 	
	buffer = (char *)NewPtr(n+1);
	
	if (buffer) {
		for (i = 0; i < n; i++) {
	 		buffer[i] = MidiGetField(tmp, i);
		}
		buffer[i] = 0; /* stops the string */
	}
	return (jint)buffer;
  }


/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetTextAux
  (JNIEnv * env, jclass cl, jint ev, jint str){

  	long i = 0;
 	char * midiname = (char*)str;
	
	while (midiname[i]!= 0) {
		MidiAddField((MidiEvPtr)ev,midiname[i]);
		i++; 
	}
	
 }
  

/*--------------------------------------------------------------------------*/
/* FOR SEQUENCES MANAGEMENT */
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetFirstEv
  (JNIEnv * env, jclass cl, jint seq){

	return (jint) FirstEv((MidiSeqPtr)seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetFirstEv
  (JNIEnv * env, jclass cl, jint seq, jint ev){

	FirstEv((MidiSeqPtr)seq) = (MidiEvPtr)ev;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetLastEv
  (JNIEnv * env, jclass cl, jint seq){

	return (jint) LastEv((MidiSeqPtr)seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetLastEv
  (JNIEnv * env, jclass cl, jint seq, jint ev){

	LastEv((MidiSeqPtr)seq) = (MidiEvPtr)ev;
}

/*--------------------------------------------------------------------------*/
/* FOR FILTER MANAGEMENT */
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewFilter
  (JNIEnv * env, jclass cl){

	return (jint)MidiNewFilter();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeFilter
  (JNIEnv * env, jclass cl, jint filter){

	MidiFreeFilter((MidiFilterPtr) filter);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptPort
  (JNIEnv * env, jclass cl, jint filter, jint val, jint c){
	
	MidiAcceptPort ((MidiFilterPtr) filter,val,c);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptChan
  (JNIEnv * env, jclass cl, jint filter, jint val, jint c){

	MidiAcceptChan ((MidiFilterPtr) filter,val,c);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptType
  (JNIEnv * env, jclass cl, jint filter, jint val, jint c){

	MidiAcceptType ((MidiFilterPtr) filter,val,c);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsAcceptedPort
	(JNIEnv * env, jclass cl, jint filter, jint port){
  
 	return MidiIsAcceptedPort((MidiFilterPtr)filter,port);
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsAcceptedChan
	(JNIEnv * env, jclass cl, jint filter, jint chan){
  
 	return MidiIsAcceptedChan((MidiFilterPtr)filter,chan);
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsAcceptedType
	(JNIEnv * env, jclass cl, jint filter, jint type){
  
 	return MidiIsAcceptedType((MidiFilterPtr)filter,type);
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_WriteEv
  (JNIEnv * env , jclass cl, jobject ob ){
  
 	/* not implemented */
 	
 	return 0;
 }

/*--------------------------------------------------------------------------*/
JNIEXPORT jint JNICALL Java_grame_midishare_Midi_ReadEv
  (JNIEnv * env , jclass cl, jobject ob){
 	
 	/* not implemented */
 	
 	return 0; 	
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewString
  (JNIEnv * env, jclass cl){
  
	return (jint)NewPtr(128);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeString
  (JNIEnv * env, jclass cl, jint str){

	DisposePtr((Ptr) str);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_WriteChar
  (JNIEnv * env, jclass cl, jint str, jint index, jbyte character){

	char * str1 = (char*) str;
	str1+=index;
	*str1 =  (char)character;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jbyte JNICALL Java_grame_midishare_Midi_ReadChar
  (JNIEnv * env, jclass cl, jint str, jint index){
  
	char * str1 = (char*) str;
	str1+=index;
	return (jbyte)*str1;
}


/*--------------------------------------------------------------------------*/
/* FOR DRIVER MANAGEMENT */
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountDrivers
	(JNIEnv * env , jclass cl){
	
	return MidiCountDrivers();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetIndDriver
	(JNIEnv * env , jclass cl, jint index){
	
	return MidiGetIndDriver(index);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetDriverInfosAux
	(JNIEnv * inEnv , jclass cl, jint refnum, jobject jinfos){
	
	TDriverInfos infos;
	jfieldID nameAux,version,slots;
    Boolean res;
	char *  buffer = (char *)NewPtr(128);
	jclass infosclass = (*inEnv)->GetObjectClass(inEnv, jinfos);
        
        res = MidiGetDriverInfos(refnum,&infos);
 	
        if (res && buffer) {
            nameAux = (*inEnv)->GetFieldID(inEnv, infosclass, "nameAux",  "I");
            version = (*inEnv)->GetFieldID(inEnv, infosclass, "version",  "S");
            slots = (*inEnv)->GetFieldID(inEnv, infosclass, "slots",  "S");
            #if defined(__Macintosh__) && defined(__MacOS9__)
                    pTocCopy (buffer, infos.name);
            #else
                    cTocCopy (buffer, infos.name);
            #endif	
	
            (*inEnv)->SetIntField(inEnv, jinfos, nameAux, (jint) buffer); /* Set the C string */
            (*inEnv)->SetShortField(inEnv, jinfos, version,infos.version) ;
            (*inEnv)->SetShortField(inEnv, jinfos, slots,infos.slots );
        }
        
        return res;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_AddSlotAux
	(JNIEnv * env , jclass cl, jint refnum, jint slotname, jint slotdirection){
	
	SlotRefNum ref;
	#if defined(__Macintosh__) && defined(__MacOS9__)
		unsigned char buffer [128];
		char * midiname = (char*)slotname;
		cTopCopy (buffer, midiname);
	#else
		char buffer [128];
		char * midiname = (char*)slotname;
		cTocCopy (buffer, midiname);
	#endif	
	
	ref =  MidiAddSlot(refnum, buffer,(SlotDirection)slotdirection);
	return *(jint *)&ref;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetIndSlot
	(JNIEnv * env , jclass cl, jint refnum, jint index){
	
	SlotRefNum ref = MidiGetIndSlot(refnum, index);
	return *(jint *)&ref;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_RemoveSlot
	(JNIEnv * env , jclass cl, jint slotrefnum){
	
	MidiRemoveSlot(*(SlotRefNum *)&slotrefnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetSlotInfosAux
	(JNIEnv * inEnv , jclass cl, jint slotrefnum, jobject jinfos){
	
	TSlotInfos infos;
	jfieldID nameAux,direction,cnxAux;
	char *  buffer1 = (char *)NewPtr(128);
	char *  buffer2 = (char *)NewPtr(128);
	jclass infosclass = (*inEnv)->GetObjectClass(inEnv, jinfos);
    Boolean res;
	int i;
        
        res = MidiGetSlotInfos(*(SlotRefNum *)&slotrefnum,&infos);
        
        if (res && buffer1 && buffer2){
 	
            nameAux = (*inEnv)->GetFieldID(inEnv, infosclass, "nameAux",  "I");
            direction = (*inEnv)->GetFieldID(inEnv, infosclass, "direction",  "I");
            cnxAux = (*inEnv)->GetFieldID(inEnv, infosclass, "cnxAux",  "I"); 
            
            #if defined(__Macintosh__) && defined(__MacOS9__)
                    pTocCopy (buffer1, infos.name);
            #else
                    cTocCopy (buffer1, infos.name);
            #endif	
	
            /* Copy the connection array into a buffer */
            for(i = 0; i< 32; i++) buffer2[i] = infos.cnx[i];
	
            (*inEnv)->SetIntField(inEnv, jinfos, nameAux, (jint) buffer1); /* Set the C string  */
            (*inEnv)->SetShortField(inEnv, jinfos, direction,infos.direction) ;
            (*inEnv)->SetIntField(inEnv, jinfos, cnxAux, (jint) buffer2); /* Set the connection buffer */
        }
        return res;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ConnectSlot
	(JNIEnv * env , jclass cl, jint port, jint slotrefnum, jint state){
		
	MidiConnectSlot(port, *(SlotRefNum *)&slotrefnum, state);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsSlotConnected
	(JNIEnv * env , jclass cl, jint port, jint slotrefnum){
	
	return (jint) MidiIsSlotConnected(port, *(SlotRefNum *)&slotrefnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetSlotNameAux
	(JNIEnv * env , jclass cl, jint slotrefnum, jint slotname){
	
	#if defined(__Macintosh__) && defined(__MacOS9__)
		unsigned char buffer [128];
		char * midiname = (char*)slotname;
		cTopCopy (buffer, midiname);
	#else
		char buffer [128];
		char * midiname = (char*)slotname;
		cTocCopy (buffer, midiname);
	#endif	
	
	MidiSetSlotName(*(SlotRefNum *)&slotrefnum, buffer);
}

int main () {return 0;}
