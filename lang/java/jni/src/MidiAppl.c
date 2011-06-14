/*

  Copyright � Grame 1999-2009

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


/*****************************************************************************
 MidiAppl.c : interface to MidiShare 

 MidiShare home page 	: http://www.grame.fr/MidiShare/
 MidiShare/Java home page : http://www.grame.fr/MidiShare/Develop/Java.html

 Bug & comments report : MidiShare@rd.grame.fr
	
 History : 20/07/96 Suppression des fonction SetData et GetData : utilisation de MidiSetField
          19/03/02 Thread bloquant sur MacOS9
          17/04/02 Appel direct du code Java dans la ReceiveAlarm sur MacOSX, Windows et Linux
			05/12/03 Utilisation de deux champs JNIEnv * dans ApplContext, un pour le thread callback, un pour le thread de l'application. 
			21/04/09 Dans Java_grame_midishare_MidiAppl_ApplClose, utilisation du parametre JNIEnv * env et pas celui garde dans la structure context.
		
*****************************************************************************/

#include "MidiShare.h"

#ifdef __Macintosh__
	#ifdef __MacOS9__
            static  ProcessSerialNumber gJavaProcess;
            UPPRcvAlarmPtr UPPJRcvAlarmPtr ;
            UPPApplAlarmPtr UPPJApplAlarmPtr ;
	#else
            RcvAlarmPtr UPPJRcvAlarmPtr ;
            ApplAlarmPtr UPPJApplAlarmPtr ;
	#endif
#endif

#ifdef __Linux__
	#define MSALARMAPI
        RcvAlarmPtr UPPJRcvAlarmPtr ;
        ApplAlarmPtr UPPJApplAlarmPtr ;
#endif

#ifdef WIN32
	ApplAlarmPtr UPPJApplAlarmPtr ;
	RcvAlarmPtr UPPJRcvAlarmPtr ;
#endif

#include "MidiAppl.h"
#include <stdlib.h>

#define typeTask 	19
#define typeAlarm 	20

#define kPollingMode 	0
#define kNativeMode 	1

enum { false, true };

/*--------------------------------------------------------------------------*/
typedef struct ApplContext {
   JavaVM * fJvm;
   JNIEnv * fCallbackEnv;
   JNIEnv * fApplEnv;
   jmethodID fMid;
   jobject fObj;
   jclass fClass;
   int fAttached;
   jmethodID fApplAlarm;
}ApplContext;

/*--------------------------------------------------------------------------*/
static int CheckThreadEnv(ApplContext* context)
{
    if (context->fAttached) {
         return true;
    }else if ((*context->fJvm)->AttachCurrentThread(context->fJvm, (void **)&context->fCallbackEnv, NULL) == 0) {
        context->fAttached = true;
        return true;
    }else{
        return false;
    }
}

/*--------------------------------------------------------------------------*/
static void  MSALARMAPI ApplAlarm(short ref,long code)
{	
	ApplContext* context = MidiGetInfo(ref);
	if (context) {
		JNIEnv * env;
		(*context->fJvm)->AttachCurrentThread(context->fJvm, (void **)&env, NULL);
		(*env)->CallIntMethod(env, context->fObj, context->fApplAlarm, code);
	}
	else{
		printf("ApplAlarm error : cannot callback Java ApplAlarm (%d)\n", ref);
	}
/*
	MidiEvPtr e = MidiNewEv(typeAlarm);    
    if (e ) {
		MidiSetField(e,0,code);
		MidiSendIm (ref+128, e);
    }
*/
}
         
/*--------------------------------------------------------------------------*/
static void MSALARMAPI JavaTask(long date, short refNum, void* a1, void* a2, void* a3)
{
    ApplContext* context = MidiGetInfo(refNum);
    jclass class;
    jmethodID mid;
    jobject task;
    jobject appl;
    jfieldID taskptr;
    
    if (context && CheckThreadEnv(context)) {
        appl = (jobject)a1;
        task = (jobject)a2;
        class = (*context->fCallbackEnv)->GetObjectClass(context->fCallbackEnv, task);
        mid = (*context->fCallbackEnv)->GetMethodID(context->fCallbackEnv, class, "Execute", "(Lgrame/midishare/MidiAppl;I)V");
        taskptr = (*context->fCallbackEnv)->GetFieldID(context->fCallbackEnv, class, "taskptr",  "I");
        (*context->fCallbackEnv)->SetIntField(context->fCallbackEnv,task,taskptr,0); 
        (*context->fCallbackEnv)->CallVoidMethod(context->fCallbackEnv, task, mid, appl,date);
        (*context->fCallbackEnv)->DeleteGlobalRef(context->fCallbackEnv, appl);
        (*context->fCallbackEnv)->DeleteGlobalRef(context->fCallbackEnv, task);
    }else{
        printf("ReceiveAlarm error : cannot callback Java task\n");
    }
}
                            
/*--------------------------------------------------------------------------*/
static void MSALARMAPI ReceiveAlarm(short ref)
{
#if defined (__Macintosh__) && defined(__MacOS9__)
	UniversalProcPtr proc = MidiGetInfo(ref);
	
	if (proc) {
		CallUniversalProc(proc, 0);
		WakeUpProcess(&gJavaProcess);
	}
#else
		ApplContext* context = MidiGetInfo(ref);
		if (context && CheckThreadEnv(context)) { 
			(*context->fCallbackEnv)->CallVoidMethod(context->fCallbackEnv, context->fObj, context->fMid);
		}
		else{
			printf("ReceiveAlarm error : cannot callback Java MidiEventLoop (%d)\n", ref);
		}
#endif
}

/*--------------------------------------------------------------------------*/
JNIEXPORT jint JNICALL Java_grame_midishare_MidiAppl_ApplOpen
  (JNIEnv * env, jobject obj, jint ref , jint mode) {
  
	ApplContext* context;
	jclass cls;
	jsize size;
	int res;
	
	context = (ApplContext*) malloc(sizeof(ApplContext));
	if (!context) goto error;        
	
	res = JNI_GetCreatedJavaVMs(&context->fJvm, 1, &size);
	if (res < 0) goto error;
	
	cls = (*env)->GetObjectClass(env, obj);
	context->fClass = (*env)->NewGlobalRef(env,cls);
	context->fMid = (*env)->GetMethodID(env, context->fClass, "MidiEventLoop", "()V");
	context->fApplAlarm = (*env)->GetMethodID(env, context->fClass, "ApplAlarm", "(I)V");
	context->fObj = (*env)->NewGlobalRef(env,obj);
	context->fAttached = false;

	if ((*context->fJvm)->AttachCurrentThread(context->fJvm, (void **)&context->fApplEnv, NULL) != 0) goto error;
	MidiSetInfo(ref,context);
	
#if defined( __Macintosh__) && defined( __MacOS9__)
	UPPJApplAlarmPtr = NewApplAlarmPtr(ApplAlarm);
	UPPJRcvAlarmPtr = NewRcvAlarmPtr(ReceiveAlarm);
	GetCurrentProcess(&gJavaProcess);
#else
	UPPJApplAlarmPtr =  ApplAlarm;
	UPPJRcvAlarmPtr =  ReceiveAlarm ;
#endif 

	// Configure calling mode
	switch (mode) {
		case kNativeMode:
			MidiSetRcvAlarm(ref, UPPJRcvAlarmPtr);
			break;
		 case kPollingMode:
			break;
	}
   
	MidiSetApplAlarm(ref, UPPJApplAlarmPtr);
	return 1;
   
error :
   if (context) free(context);
   return 0;         
}

/*--------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_grame_midishare_MidiAppl_ApplClose
  (JNIEnv * env, jobject obj, jint ref) 
{
	ApplContext* context = MidiGetInfo(ref);
	MidiSetRcvAlarm (ref, 0);
	MidiSetApplAlarm (ref, 0);
	if (context) {
		MidiSetInfo (ref, 0);
		//(*context->fApplEnv)->DeleteGlobalRef(context->fApplEnv, context->fClass);
		//(*context->fApplEnv)->DeleteGlobalRef(context->fApplEnv, context->fObj);
        // 21/04/2009 
		(*env)->DeleteGlobalRef(env, context->fClass);
		(*env)->DeleteGlobalRef(env, context->fObj);
		free(context);
	}
		
#if defined( __Macintosh__) && defined( __MacOS9__)
	if (MidiGetApplAlarm (ref)) DisposeRoutineDescriptor (MidiGetApplAlarm (ref));
	if (MidiGetRcvAlarm (ref)) DisposeRoutineDescriptor (MidiGetRcvAlarm (ref));
#endif
}
 
/*--------------------------------------------------------------------------*/
JNIEXPORT jint JNICALL Java_grame_midishare_MidiAppl_ScheduleTask
	(JNIEnv * env, jobject appl, jobject task, jint date, jint ref, jint mode)
{
	jclass cls;
	jfieldID taskptr;
	MidiEvPtr taskev = 0;
	
#if defined (__Macintosh__) && defined(__MacOS9__)
	taskev = MidiDTask(JavaTask, date, ref, (long)((*env)->NewGlobalRef(env,appl)), (long)((*env)->NewGlobalRef(env,task)), 0);
#else
	
	// Configure calling mode
	switch (mode) {
		case kNativeMode:
			taskev = MidiTask(JavaTask, date, ref, (void*)((*env)->NewGlobalRef(env,appl)), (*env)->NewGlobalRef(env,task), 0);
			break;
		case kPollingMode:
			taskev = MidiDTask(JavaTask, date, ref, (*env)->NewGlobalRef(env,appl), (*env)->NewGlobalRef(env,task), 0);
			break;
	}
		
#endif
	
	cls = (*env)->GetObjectClass(env, task);
	taskptr = (*env)->GetFieldID(env, cls, "taskptr",  "I");
	(*env)->SetIntField(env,task,taskptr,(jint)taskev); 
	return (jint)taskev;
}


