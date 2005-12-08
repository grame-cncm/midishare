/*******************************************************************************
 * C H A M E L E O N    S. D. K.                                               *
 *******************************************************************************
 *  $Archive:: /Chameleon.sdk/SYSTEM/midishare/common/Memory/msFields.c        $
 *     $Date: 2005/12/08 13:38:30 $
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

#include "msFields.h"


MidiEvPtr  MSGetLink (MidiEvPtr e)                  { return Link(e); }
void       MSSetLink (MidiEvPtr e, MidiEvPtr next)  { Link(e) = next; }

DWORD MSGetDate   (MidiEvPtr e)                { return Date(e); }
void  MSSetDate   (MidiEvPtr e, DWORD date)    { Date(e) = date; }
short MSGetRefNum (MidiEvPtr e)                { return RefNum(e); }
void  MSSetRefNum (MidiEvPtr e, short ref)     { RefNum(e) = (BYTE)ref; }
short MSGetType   (MidiEvPtr e)                { return EvType(e); }
void  MSSetType   (MidiEvPtr e, short type)    { EvType(e) = (BYTE)type; }
short MSGetChan   (MidiEvPtr e)                { return Chan(e); }
void  MSSetChan   (MidiEvPtr e, short chan)    { Chan(e) = (BYTE)chan; }
short MSGetPort   (MidiEvPtr e)                { return Port(e); }
void  MSSetPort   (MidiEvPtr e, short port)    { Port(e) = (BYTE)port; }

long  MSGetData0 (MidiEvPtr e)            { return Data(e)[0]; }
long  MSGetData1 (MidiEvPtr e)            { return Data(e)[1]; }
long  MSGetData2 (MidiEvPtr e)            { return Data(e)[2]; }
long  MSGetData3 (MidiEvPtr e)            { return Data(e)[3]; }
void  MSSetData0 (MidiEvPtr e, long v)    { Data(e)[0] = (BYTE)v; }
void  MSSetData1 (MidiEvPtr e, long v)    { Data(e)[1] = (BYTE)v; }
void  MSSetData2 (MidiEvPtr e, long v)    { Data(e)[2] = (BYTE)v; }
void  MSSetData3 (MidiEvPtr e, long v)    { Data(e)[3] = (BYTE)v; }

MidiEvPtr  MSGetFirstEv (MidiSeqPtr seq)               { return seq->first; }
void       MSSetFirstEv (MidiSeqPtr seq, MidiEvPtr e)  { seq->first = e; }
MidiEvPtr  MSGetLastEv  (MidiSeqPtr seq)               { return seq->last; }
void       MSSetLastEv  (MidiSeqPtr seq, MidiEvPtr e)  { seq->last = e; }
