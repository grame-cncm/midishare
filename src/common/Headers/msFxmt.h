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
