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
/* MidiAppl.c : interface to MidiShare 
/*
/* MidiShare home page 		: http://www.grame.fr/MidiShare/
/* MidiShare/Java home page 	: http://www.grame.fr/MidiShare/Develop/Java.html
/*
/* Bug & comments treport 	: MidiShare@rd.grame.fr
/*	
/* History : 20/07/96 suppression des fonction SetData et GetData : utilisation de MidiSetField
/*   		
/*****************************************************************************/

#ifdef __Macintosh__
	#ifdef __MacOS9__
		#include <MidiShare.h>
                UPPRcvAlarmPtr UPPJRcvAlarmPtr ;
                UPPApplAlarmPtr UPPJApplAlarmPtr ;
	#else
		#include <MidiShare.h>
                RcvAlarmPtr UPPJRcvAlarmPtr ;
                ApplAlarmPtr UPPJApplAlarmPtr ;
	#endif
#endif

#ifdef __Linux__
	#include "MidiShare.h"
	#define MSALARMAPI
#endif

#ifdef WIN32
	#include <MidiShare.h>
#endif

#include "MidiAppl.h"

       
void  MSALARMAPI ApplAlarm( short ref,long code);

		
/*--------------------------------------------------------------------------*/

#define typeAlarm 20

void  MSALARMAPI ApplAlarm( short ref,long code)
{
	MidiEvPtr e;

	if ((e = MidiNewEv(typeAlarm))) {
		MidiSetField(e,0,code);
		MidiSendIm (ref+128, e);
	}
}

/*--------------------------------------------------------------------------*/

JNIEXPORT jint JNICALL Java_grame_midishare_MidiAppl_ApplOpen
  (JNIEnv * env , jobject obj, jint ref) {
  
	#if defined( __Macintosh__) && defined( __MacOS9__)
		UPPJApplAlarmPtr =  NewApplAlarmPtr(ApplAlarm);
	#else
		UPPJApplAlarmPtr =  ApplAlarm;
	#endif
	
	MidiSetApplAlarm(ref,UPPJApplAlarmPtr);
	return 1;
}

/*--------------------------------------------------------------------------*/

JNIEXPORT void JNICALL Java_grame_midishare_MidiAppl_ApplClose
  (JNIEnv * env, jobject obj, jint ref) {

	#if defined( __Macintosh__) && defined( __MacOS9__)
		if (MidiGetApplAlarm (ref)) DisposeRoutineDescriptor ((UPPApplAlarmPtr) MidiGetApplAlarm (ref));
	#endif
}


