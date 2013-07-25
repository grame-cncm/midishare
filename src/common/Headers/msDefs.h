/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
  
  modifications history:
   [08-09-99] DF - improving TMidiST and TMidiEv 

*/

#ifndef __msDefs__
#define __msDefs__

#include "MidiShare.h"

typedef const char* MidiName;

/******************************************************************************
*                               FILTER MACROS
*------------------------------------------------------------------------------
* somes macros to set and reset filter's bits.
*******************************************************************************/

#ifdef __cplusplus
    inline void AcceptBit( char *a, Byte n)   { (a)[(n)>>3] |=  (1<<((n)&7)); }
    inline void RejectBit( char *a, Byte n)   { (a)[(n)>>3] &= ~(1<<((n)&7)); }
    inline void InvertBit( char *a, Byte n)   { (a)[(n)>>3] ^=  (1<<((n)&7)); }

    inline Boolean IsAcceptedBit( char *a, Byte n) { return (a)[(n)>>3] & (1<<((n)&7)); }

#else

    #define AcceptBit(a,n)      ( ((char*) (a))[(n)>>3] |=   (1<<((n)&7)) )
    #define RejectBit(a,n)      ( ((char*) (a))[(n)>>3] &=  ~(1<<((n)&7)) )
    #define InvertBit(a,n)      ( ((char*) (a))[(n)>>3] ^=   (1<<((n)&7)) )

    #define IsAcceptedBit(a,n)  ( ((char*) (a))[(n)>>3]  &   (1<<((n)&7)) )
#endif

#endif
