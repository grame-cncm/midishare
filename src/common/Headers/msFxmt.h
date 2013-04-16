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


#ifndef __MSFXMT__
#define __MSFXMT__

#include "msDefs.h"
#include "msKernel.h"

#define kLineUndef	0	/* fifo non encore initialisé		*/
#define kLineOff	2	/* fifo initialisé mais non actif	*/
#define kLineOn		3	/* fifo initialisé et actif		*/

/*------------------------------------------------------------------------*/
/* Les codes de Status MIDI : définis dans MidiShare.h */
#define WAIT		0xFF

#define NRPLSB		98		/* non registr. param number (lsb)	*/
#define NRPMSB		99		/* non registr. param number (msb)	*/

#define RPLSB		100		/* registred param number (lsb)		*/
#define RPMSB		101		/* registred param number (msb)		*/

#define DENTRYMSB	6		/* data entry value (msb)		*/
#define DENTRYLSB	38		/* data entry value (lsb)		*/


typedef struct XFifo  * XFifoPtr;

typedef void	(*XFAlarm)(XFifoPtr f);
typedef void	(*XFContinuation)(XFifoPtr f);
typedef void    (*LinearizeMethodPtr)(MidiEvPtr e, XFifoPtr f);


typedef struct XFifo{

	short        	refNum;    	/* Application refNum      	   	*/
	short        	reserved1; 	/* Reserved for alignement         	*/
	TMSGlobalPtr   	global;       	/* Pointeur sur la variable globale  	*/
    	short          	state;        	/* Etat du fifo : actif ou pas       	*/
    	short          	reserved2;    	/* Reserved for alignement          	*/
   	XFContinuation 	cont;         	/* continuationde l'émission         	*/
    	MidiSEXPtr     	nextCell;     	/* Pointeur partie en cours SysEx    	*/
    	unsigned long  	lastXmtDate;  	/* dernière date d'émission effective	*/
    	uchar          	runStat;      	/* Code du status courant            	*/
    	uchar          	port;         	/* Port courant                      	*/
    	uchar          	reserved3;    	/* Reserved for alignement           	*/
    	int            	count;
    	Byte           	data[16];
  
} XFifo;

/*___________________________________*/

void XFInit( XFifoPtr f, TMSGlobalPtr g, short refnum);
void XFInitMthTbl ();

void 	XFPutEvent( register XFifoPtr f, register MidiEvPtr e);
Boolean	XFGetByte( register XFifoPtr f, Byte *code, Byte *port);
int	XFCountByte( register XFifoPtr f);

extern LinearizeMethodPtr LinearizeTbl[];


#endif /* __MSFXMT__ */
