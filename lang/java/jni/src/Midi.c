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
/* MidiShare/Java home page : http://www.grame.fr/MidiShare/Develop/Java.html
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
/*
/*****************************************************************************/

#ifdef __Macintosh__
#include "MidiSharePPC.h"
#endif


#include "Midi.h"
#include <Memory.h>   // for NewPtr and DisposPtr

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

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AddField
  (JNIEnv *, jclass, jint event, jint val) {

	MidiAddField((MidiEvPtr)event,val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AddSeq
  (JNIEnv *, jclass, jint seq, jint ev){

	MidiAddSeq((MidiSeqPtr)seq,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ApplySeq
  (JNIEnv *, jclass, jint, jint){
	/* not implemented */
}
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_AvailEv
  (JNIEnv *, jclass, jint refnum) {
  
 	return (jint) MidiAvailEv(refnum);
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Call
  (JNIEnv *, jclass, jint, jint, jint, jint, jint, jint){

 	 /* not implemented */
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ClearSeq
  (JNIEnv *, jclass, jint seq) {
  
 	MidiClearSeq((MidiSeqPtr)seq);
 	
 }
 
 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Close
  (JNIEnv *, jclass, jint refnum){

 	 MidiClose(refnum);
 }

 /*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Connect
  (JNIEnv *, jclass, jint src, jint dst, jint state){
  
 	MidiConnect(src,dst,state);
 }


/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CopyEv
  (JNIEnv *, jclass, jint ev) {
  
	return (jint)MidiCopyEv((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountAppls
  (JNIEnv *, jclass){

	return MidiCountAppls();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountDTasks
  (JNIEnv *, jclass, jint refnum){
  
	return MidiCountDTasks (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountEvs
  (JNIEnv *, jclass, jint refnum) {

	return MidiCountEvs(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_CountFields
  (JNIEnv *, jclass, jint ev){

	return MidiCountFields((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_DTask
  (JNIEnv *, jclass, jint, jint, jint, jint, jint, jint){

	 /* not implemented */
	
 	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Exec1DTask
  (JNIEnv *, jclass, jint refnum){

	MidiExec1DTask(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Ext2IntTime
  (JNIEnv *, jclass, jint time){
  
	return MidiExt2IntTime (time);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FlushDTasks
  (JNIEnv *, jclass, jint refnum){

	MidiFlushDTasks (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FlushEvs
  (JNIEnv *, jclass, jint refnum){

	MidiFlushEvs (refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_ForgetTask
  (JNIEnv *, jclass, jint){

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeCell
  (JNIEnv *, jclass, jint ev){

	MidiFreeCell ((MidiEvPtr) ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeEv
  (JNIEnv *, jclass, jint ev){

	MidiFreeEv ((MidiEvPtr) ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeSeq
  (JNIEnv *, jclass, jint seq){
  
	MidiFreeSeq ((MidiSeqPtr) seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_FreeSpace
  (JNIEnv *, jclass){

	return MidiFreeSpace();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetApplAlarm
  (JNIEnv *, jclass, jint){

	/* not implemented */
	return 0;	
}
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetEv
  (JNIEnv *, jclass, jint refnum){
  
	return (jint) MidiGetEv(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetExtTime
  (JNIEnv *, jclass){

	 return MidiGetExtTime();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetField
  (JNIEnv *, jclass, jint ev, jint i){
  
	return MidiGetField((MidiEvPtr)ev,i);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetFilter
  (JNIEnv *, jclass, jint refnum){

	return (long)MidiGetFilter(refnum);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetIndAppl
  (JNIEnv *, jclass, jint refnum){

	return (jint)MidiGetIndAppl(refnum);
}

/*--------------------------------------------------------------------------*/


JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetInfo
  (JNIEnv *, jclass, jint){
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetNameAux
  (JNIEnv *, jclass, jint refnum){
  
	char *  buffer = NewPtr(128);
	
	if (buffer) {
		MidiName midiname =  MidiGetName(refnum);
		pTocCopy (buffer, midiname);
	}

	return (long) buffer;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetNamedApplAux
  (JNIEnv *, jclass, jint midiname) {
  
	unsigned char buffer [128];
	cTopCopy (buffer,(char*)midiname);
	
	return (long)MidiGetNamedAppl (buffer);
}

/*--------------------------------------------------------------------------*/


JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetPortState
  (JNIEnv *, jclass, jint port){

	return MidiGetPortState(port);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetRcvAlarm
  (JNIEnv *, jclass, jint){

	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_GetSyncInfo
  (JNIEnv * inEnv, jclass, jobject syncinfo){

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
  (JNIEnv *, jclass){

	return MidiGetTime();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetVersion
  (JNIEnv *, jclass){
	return MidiGetVersion();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GrowSpace
  (JNIEnv *, jclass, jint val){

	return MidiGrowSpace (val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Int2ExtTime
  (JNIEnv *, jclass, jint time){
  
	return MidiInt2ExtTime (time);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_IsConnected
  (JNIEnv *, jclass, jint src, jint dst) {
  
	return MidiIsConnected(src,dst);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewCell
  (JNIEnv *, jclass) {

	return (jint)MidiNewCell();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewEv
  (JNIEnv *, jclass, jint type) {
  
	return (jint)MidiNewEv(type);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewSeq
  (JNIEnv *, jclass){

	return (jint) MidiNewSeq();
}
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_OpenAux
  (JNIEnv *, jclass, jint str) {

	unsigned char buffer [128];
	char * midiname = (char*)str;
	cTopCopy (buffer, midiname);
	return  (long)MidiOpen(buffer);

}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_ReadSync
  (JNIEnv *, jclass, jint){
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/


JNIEXPORT void JNICALL Java_grame_midishare_Midi_Send
  (JNIEnv *, jclass, jint ref, jint ev){

	MidiSend(ref,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SendAt
  (JNIEnv *, jclass, jint ref , jint ev , jint date){
  
	MidiSendAt(ref,(MidiEvPtr)ev,date);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SendIm
  (JNIEnv *, jclass, jint ref, jint ev){ 
  
	MidiSendIm(ref,(MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetApplAlarm
  (JNIEnv *, jclass, jint, jint){

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetField
  (JNIEnv *, jclass, jint ev , jint i , jint val){
  
	MidiSetField((MidiEvPtr)ev,i,val);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetFilter
  (JNIEnv *, jclass, jint ref, jint filter){

	MidiSetFilter(ref,(FilterPtr)filter);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetInfo
  (JNIEnv *, jclass, jint, jint){

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetNameAux
  (JNIEnv *, jclass, jint ref, jint midiname){
  
	unsigned char buffer [128];
	cTopCopy (buffer, (char*)midiname);
	MidiSetName(ref,buffer);
}

/*--------------------------------------------------------------------------*/


JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetPortState
  (JNIEnv *, jclass, jint port, jint state){
  
	 MidiSetPortState(port,state);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetRcvAlarm
  (JNIEnv *, jclass, jint, jint){

	/* not implemented */
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_SetSyncMode
  (JNIEnv *, jclass, jint) {
  
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Share
  (JNIEnv *, jclass){

	return MidiShare();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_Smpte2Time
  (JNIEnv *inEnv , jclass, jobject smpteloc) {
	
	
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
  (JNIEnv *, jclass, jint, jint, jint, jint, jint, jint){
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_Time2Smpte
  (JNIEnv * inEnv, jclass, jint time, jint format, jobject smpteloc){

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
  (JNIEnv *, jclass){
  
	return MidiTotalSpace();
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_WriteSync
  (JNIEnv *, jclass, jint, jint){
	/* not implemented */
	return 0;
}

/*--------------------------------------------------------------------------*/

/* FOR COMMON FIELDS MANAGEMENT */

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetLink
  (JNIEnv *, jclass, jint ev){

	return (long)Link((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetLink
  (JNIEnv *, jclass, jint ev , jint dst){

	Link((MidiEvPtr)ev) = (MidiEvPtr)dst;	
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetDate
  (JNIEnv *, jclass, jint ev){
  
	return (long)Date((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/


JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetDate
  (JNIEnv *, jclass, jint ev, jint date){

	Date((MidiEvPtr)ev) = date;
}

/*--------------------------------------------------------------------------*/


JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetRefnum
  (JNIEnv *, jclass, jint ev){
  
	return (long)RefNum((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetRefnum
  (JNIEnv *, jclass, jint ev, jint ref){

	RefNum((MidiEvPtr)ev) = ref;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetType
  (JNIEnv *, jclass, jint ev){

	return(jint)EvType((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetType
  (JNIEnv *, jclass, jint ev, jint type){

	EvType((MidiEvPtr)ev) = type;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetChan
  (JNIEnv *, jclass, jint ev){

	return (long)Chan((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetChan
  (JNIEnv *, jclass, jint ev, jint chan){

	Chan((MidiEvPtr)ev) = chan;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetPort
  (JNIEnv *, jclass, jint ev) { 

	return (long)Port((MidiEvPtr)ev);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetPort
  (JNIEnv *, jclass, jint ev, jint port){

	Port((MidiEvPtr)ev) = port;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData0
  (JNIEnv *, jclass, jint ev){

 	return (long)Data((MidiEvPtr)ev)[0];
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData1
  (JNIEnv *, jclass, jint ev){

 	return (long)Data((MidiEvPtr)ev)[1];
 }
 
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData2
  (JNIEnv *, jclass, jint ev){

 	return (long)Data((MidiEvPtr)ev)[2];
  }
  
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetData3
  (JNIEnv *, jclass, jint ev){

 	return (long)Data((MidiEvPtr)ev)[3];
  }
  
/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData0
  (JNIEnv *, jclass, jint ev, jint val){


 	Data((MidiEvPtr)ev)[0] = val;
  }

/*--------------------------------------------------------------------------*/

 JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData1
  (JNIEnv *, jclass, jint ev, jint val){

 	Data((MidiEvPtr)ev)[1] = val;
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData2
  (JNIEnv *, jclass, jint ev, jint val){

 	Data((MidiEvPtr)ev)[2] = val;
  }

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetData3
  (JNIEnv *, jclass, jint ev, jint val){

 	Data((MidiEvPtr)ev)[3] = val;
  }


/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetTextAux
  (JNIEnv *, jclass, jint ev){

  	MidiEvPtr tmp = (MidiEvPtr) ev;
	long i, n = MidiCountFields(tmp);
	char * buffer;
	 	
	buffer = NewPtr(n+1);
	
	if (buffer) {
		for (i = 0; i < n; i++) {
	 		buffer[i] = MidiGetField(tmp, i);
		}
		buffer[i] = 0; // stops the string
	}
	return (long)buffer;
  }


/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetTextAux
  (JNIEnv *, jclass, jint ev, jint str){

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
  (JNIEnv *, jclass, jint seq){

	return (jint) FirstEv((MidiSeqPtr)seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetFirstEv
  (JNIEnv *, jclass, jint seq, jint ev){

	FirstEv((MidiSeqPtr)seq) = (MidiEvPtr)ev;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_GetLastEv
  (JNIEnv *, jclass, jint seq){

	return (long) LastEv((MidiSeqPtr)seq);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_SetLastEv
  (JNIEnv *, jclass, jint seq, jint ev){

	LastEv((MidiSeqPtr)seq) = (MidiEvPtr)ev;
}

/*--------------------------------------------------------------------------*/
/* FOR FILTER MANAGEMENT */
/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewFilter
  (JNIEnv *, jclass){

	return (long) NewPtr (sizeof (TFilter));
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeFilter
  (JNIEnv *, jclass, jint filter){

	DisposePtr ((Ptr)filter);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptPort
  (JNIEnv *, jclass, jint filter, jint val, jint c){

	if (filter) {
		if (c)
			AcceptBit (((FilterPtr)filter)->port, (Byte)val);
		else
			RejectBit (((FilterPtr)filter)->port, (Byte)val);
	}
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptChan
  (JNIEnv *, jclass, jint filter, jint val, jint c){

	if (filter) {
		if (c)
			AcceptBit (((FilterPtr)filter)->channel, (Byte)val);
		else
			RejectBit (((FilterPtr)filter)->channel, (Byte)val);
	}
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_AcceptType
  (JNIEnv *, jclass, jint filter, jint val, jint c){

	if (filter) {
		if (c)
			AcceptBit (((FilterPtr)filter)->evType, (Byte)val);
		else
			RejectBit (((FilterPtr)filter)->evType, (Byte)val);
	}
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_WriteEv
  (JNIEnv * , jclass, jobject  ){
  
 	/* not implemented */
 	
 	return 0;
 }


/*--------------------------------------------------------------------------*/
JNIEXPORT jint JNICALL Java_grame_midishare_Midi_ReadEv
  (JNIEnv * , jclass, jobject ){
 	
 	/* not implemented */
 	
 	return 0; 	
 }

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewString
  (JNIEnv *, jclass){
  
	return (long)NewPtr(128);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeString
  (JNIEnv *, jclass, jint str){

	DisposePtr((Ptr) str);
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_Midi_WriteChar
  (JNIEnv *, jclass, jint str, jint index, jbyte character){

	char * str1 = (char*) str;
	str1+=index;
	*str1 =  (char)character;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jbyte JNICALL Java_grame_midishare_Midi_ReadChar
  (JNIEnv *, jclass, jint str, jint index){
  
	char * str1 = (char*) str;
	str1+=index;
	return (long)*str1;
}



