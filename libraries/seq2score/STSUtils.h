/*
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
*/


#ifndef _StsUtilsH_
#define _StsUtilsH_

#include "MidiShare.h"

#define HSIZE 512
#define HMASK 511

/*--------------------------------------------------------------------------*/

typedef struct THashTbl *THashTblPtr;
typedef struct THashTbl
{
  int space;
  MidiEvPtr entry [HSIZE];
} THashTbl;

void ShiftDates (MidiSeqPtr s, unsigned long offset);
void TicksToMsecs (MidiSeqPtr s, int clicks);
void MatchKeyOff (MidiSeqPtr s);
void InitHashTbl (THashTblPtr h);
unsigned int HashKey (MidiEvPtr e);
int InsertHashTbl (THashTblPtr h, MidiEvPtr e);
int SearchHashTbl (THashTblPtr h, MidiEvPtr e1);

#endif