/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

  modifications history:
   [08-09-99] DF - mail box functions finalization

*/

#include "msMail.h"
#include "msSync.h"

/*----------------------------------------------------------------------------*/
/* 				- MidiReadSync - read and clear an address   				  */
/*----------------------------------------------------------------------------*/
MSFunctionType(void*) MSReadSync	(void** adrMem)
{
	void* content;
	
	do {
		content= *adrMem;
	} while (!MSCompareAndSwap (adrMem, content, 0));
	return content;
}

/*----------------------------------------------------------------------------*/
/* 				- MidiWriteSync - test and write an address				      */
/*----------------------------------------------------------------------------*/
MSFunctionType(void*) MSWriteSync (void** adrMem, void* val)
{
	void* content;
	
	do {
		content= *adrMem;
	} while (!MSCompareAndSwap (adrMem, content, val));
	return content;
}
