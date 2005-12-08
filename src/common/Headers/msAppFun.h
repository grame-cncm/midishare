/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/SYSTEM/midishare/common/Headers/msAppFun.h       $
 *     $Date: 2005/12/08 13:38:28 $
 * $Revision: 1.1.1.1.6.1 $
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

*/

#ifndef __msAppFun__
#define __msAppFun__

#include "msKernel.h"

/*------------------------------------------------------------------------*/
/* functions declarations                                                 */
void		InitAppls (TClientsPtr g, MSMemoryPtr mem);

short		MSOpen         (MidiName name, TMSGlobalPtr g);
void		MSClose        (short ref, TMSGlobalPtr g);

short		MSCountAppls   (TClientsPtr g);
short		MSGetIndAppl   (short index, TClientsPtr g);
short		MSGetNamedAppl (MidiName name, TClientsPtr g);

MidiName	MSGetName  (short ref, TClientsPtr g);
void		MSSetName  (short ref, MidiName name, TClientsPtr g);
void*		MSGetInfo  (short ref, TClientsPtr g);
void		MSSetInfo  (short ref, void* info, TClientsPtr g);

MidiFilterPtr	MSGetFilter  (short ref, TClientsPtr g);
void			MSSetFilter  (short ref, MidiFilterPtr filter, TClientsPtr g);

RcvAlarmPtr		MSGetRcvAlarm  (short ref, TClientsPtr g);
void			MSSetRcvAlarm  (short ref, RcvAlarmPtr alarm, TClientsPtr g);
ApplAlarmPtr	MSGetApplAlarm (short ref, TClientsPtr g);
void			MSSetApplAlarm (short ref, ApplAlarmPtr alarm, TClientsPtr g);

#endif
