/*

  Copyright © Grame 2000

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


#ifndef __msMtpDriverAppl__
#define __msMtpDriverAppl__

#ifdef __MTP_Printer__
#define MtpDriverName	"\pMTP Driver Printer"
#define MtpPortName		"\pMTP Printer Port "
#define BusyMsg			"\pPrinter port is in use"
#define StateFile		"\pmsMtp Driver State (Printer)"	
enum{ StateCreator = 'MsMp', StateType = 'stMp' };	

#else

#define MtpDriverName	"\pMTP Driver Modem"
#define MtpPortName		"\pMTP Modem Port "
#define BusyMsg			"\pModem port is in use"
#define StateFile		"\pmsMTP Driver State (Modem)"	
enum { StateCreator = 'MsMm', StateType = 'stMm' };	
#endif

#define kNoCurrentPort	-1

extern Boolean	doneFlag;

Boolean CheckSCC ();
Boolean CheckSCCFree ();
Boolean SetUpMidi ();
void 	Start ();
void 	CloseMidi ();
void 	DoIdle ();

#endif