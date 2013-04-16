/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
  modifications history:
*/


#ifndef __MSFRCV__
#define __MSFRCV__


#include "msDefs.h"
#include "msKernel.h"

/*------------------------------------------------------------------------*/
/* Etat des fifos MIDI */

#define kLineUndef	0		/* fifo non encore initialisé		*/
#define kLineOff	2		/* fifo initialisé mais non actif	*/
#define kLineOn		3		/* fifo initialisé et actif			*/


typedef struct RFifo * RFifoPtr;
typedef void   (*RcvMethodPtr)  (RFifoPtr f, char c);

/*__________________________________________________________________________*/
/*                 structure d'accès rapide aux champs d'un événement            */

typedef union TMidiFastEv
{
    MidiEvPtr          std;
    struct {
        MidiEvPtr     link;        /* link to next event           */
        unsigned long date;        /* event date (in ms)           */
        union {                    /* common datas                 */
            Byte      byteC[4];
            short     shortC[2];
            long      longC;
        }             common;
        union {                    /* info depending of event type */
            Byte      byteF[4];
            short     shortF[2];
            long      longF;
        }             specific;
    }*           fast;
    long *       tab;
} TMidiFastEv;

#define Common(e)        (e).fast->common
#define Specific(e)      (e).fast->specific


/*------------------------------------------------------------------------*/
/* Fifo de Reception MIDI */

typedef struct RFifo{
	 short        refNum;    /* RefNum de l'appli      	       */
	 short        reserved1; /* Reserve pour alignement            */
	 TMSGlobalPtr global;    /* Pointeur sur la variable globale   */
	 short        state;     /* Etat du fifo : actif ou pas        */
	 char         basePort;  /* offset port                        */
	 char         reserved;  /* Reserve                            */
	 RcvMethodPtr parse;     /* Etat de l'analyse (adr. routine)   */
	 MidiEvPtr    ptrCur;    /* Pointeur SysEx en cours            */
	 ulong        errCount;  /* Nombre d'erreurs en reception      */
	 ulong        date;      /* Date de l'evt sur 24 bits de pFort */
	 union {
		struct {
			 uchar  type;      /* Type de l'evenement                */
			 uchar  refNum;    /* numero application de destination  */
			 uchar  port;      /* Port de sortie                     */
			 uchar  canal;     /* Canal Midi de destination          */
		}std;
		long       fast;
	}common;
	 union {
		  char     data[4];   /* Les informations recues            */
		  long     fast;
	 }infos;
} RFifo;


/*___________________________________*/

void RFInit( RFifoPtr f,TMSGlobalPtr global, short ref);
void RFInitMthTbl();
void InitTypeTbl();
void RFPutByte( register RFifoPtr f, Byte code);
void SwitchRcvQFrameMethod (Boolean syncOn);

extern  RcvMethodPtr  	RcvTbl[];
extern  Byte  		TypeTbl[];

#endif /* __MSFRCV__ */
