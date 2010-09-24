/*

  Copyright © Grame 1999

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
