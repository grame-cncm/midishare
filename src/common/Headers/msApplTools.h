/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/SYSTEM/midishare/common/Headers/msApplTools.h    $
 *     $Date: 2005/12/08 13:38:28 $
 * $Revision: 1.2.6.1 $
 *-----------------------------------------------------------------------------*
 * This file is part of the Chameleon Software Development Kit                 *
 *                                                                             *
 * Copyright (C) 2001 soundart                                                 *
 * www.soundart-hot.com                                                        *
 * codemaster@soundart-hot.com                                                 *
 ******************************************************************************/

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
  research@grame.fr

*/


#ifndef __msApplTools__
#define __msApplTools__

#include "msAppls.h"
#include "msDefs.h"
#include "msDriver.h"
#include "msKernel.h"

#define NewAppl(size)		(TApplPtr)AllocateMemory(kernelSharedMemory, size)
#define FreeAppl(appl)		DisposeMemory(appl)

void makeClient (TClientsPtr g, TApplPtr ap, short ref, MidiName n, short folder);
void closeClient (short ref, TMSGlobalPtr g);
void makeDriver (TClientsPtr g, TApplPtr appl, short ref, TDriverInfos * infos, TDriverOperation *op);
void setName 	(MidiName dst, MidiName name);


#endif
