/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/SYSTEM/midishare/common/Headers/msFrcv.h         $
 *     $Date: 2005/12/08 13:38:28 $
 * $Revision: 1.1.6.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

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


typedef struct RFifo     * RFifoPtr;
typedef void   (*RcvMethodPtr)  (RFifoPtr f, char c);

/*__________________________________________________________________________*/
/*                 structure d'accès rapide aux champs d'un événement            */

typedef union TMidiFastEv
{
    MidiEvPtr          std;
    struct {
        MidiEvPtr     link;        /* link to next event           */
        DWORD         date;        /* event date (in ms)           */
        union {                    /* common datas                 */
            BYTE      byteC[4];
            short     shortC[2];
            long      longC;
        }             common;
        union {                    /* info depending of event type */
            BYTE      byteF[4];
            short     shortF[2];
            long      longF;
        }             specific;
    } *			      fast;
    long *			  tab;
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
	 DWORD        errCount;  /* Nombre d'erreurs en reception      */
	 DWORD        date;      /* Date de l'evt sur 24 bits de pFort */
	 union {
		struct {
			 BYTE  type;      /* Type de l'evenement                */
			 BYTE  refNum;    /* numero application de destination  */
			 BYTE  port;      /* Port de sortie                     */
			 BYTE  canal;     /* Canal Midi de destination          */
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
void RFPutByte( register RFifoPtr f, BYTE code);
void SwitchRcvQFrameMethod (BOOL syncOn);

extern  RcvMethodPtr  	RcvTbl[];
extern  BYTE  		TypeTbl[];

#endif /* __MSFRCV__ */
