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
  grame@grame.fr

*/

#ifndef __msAppFun__
#define __msAppFun__

#include "msKernel.h"

/*------------------------------------------------------------------------*/
/* functions declarations                                                 */
void     InitAppls (TClientsPtr g, MSMemoryPtr mem);

MSFunctionType(short)     MSOpen         (MidiName name, TMSGlobalPtr g);
MSFunctionType(void)      MSClose        (short ref, TMSGlobalPtr g);

MSFunctionType(short)     MSCountAppls   (TClientsPtr g);
MSFunctionType(short)     MSGetIndAppl   (short index, TClientsPtr g);
MSFunctionType(short)     MSGetNamedAppl (MidiName name, TClientsPtr g);

MSFunctionType(MidiName)     MSGetName  (short ref, TClientsPtr g);
MSFunctionType(void)         MSSetName  (short ref, MidiName name, TClientsPtr g);
MSFunctionType(FarPtr(void)) MSGetInfo  (short ref, TClientsPtr g);
MSFunctionType(void)         MSSetInfo  (short ref, FarPtr(void) info, TClientsPtr g);

MSFunctionType(MidiFilterPtr) MSGetFilter  (short ref, TClientsPtr g);
MSFunctionType(void)          MSSetFilter  (short ref, MidiFilterPtr filter, TClientsPtr g);

MSFunctionType(RcvAlarmPtr)  MSGetRcvAlarm  (short ref, TClientsPtr g);
MSFunctionType(void)         MSSetRcvAlarm  (short ref, RcvAlarmPtr alarm, TClientsPtr g);
MSFunctionType(ApplAlarmPtr) MSGetApplAlarm (short ref, TClientsPtr g);
MSFunctionType(void)         MSSetApplAlarm (short ref, ApplAlarmPtr alarm, TClientsPtr g);

#endif
