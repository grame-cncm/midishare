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
   [08-09-99] DF - mail box functions finalization

*/

#include "msMail.h"
#include "msSync.h"

/*----------------------------------------------------------------------------*/
/* 				- MidiReadSync - read and clear an address   				  */
/*----------------------------------------------------------------------------*/
MSFunctionType(FarPtr(void)) MSReadSync	(FarPtr(void) FAR * adrMem)
{
	FarPtr(void) content;
	
	do {
		content= *adrMem;
	} while (!MSCompareAndSwap (adrMem, content, 0));
	return content;
}

/*----------------------------------------------------------------------------*/
/* 				- MidiWriteSync - test and write an address				      */
/*----------------------------------------------------------------------------*/
MSFunctionType(FarPtr(void)) MSWriteSync (FarPtr(void) FAR * adrMem, FarPtr(void) val)
{
	FarPtr(void) content;
	
	do {
		content= *adrMem;
	} while (!MSCompareAndSwap (adrMem, content, val));
	return content;
}
