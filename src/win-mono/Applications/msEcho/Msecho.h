/*

  Copyright © Grame 2001

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
  
  modifications history:

*/

#ifndef __msEcho__
#define __msEcho__

#define Version	235

// ------------------------------------------------------------------
// CONSTANTS
// ------------------------------------------------------------------

#define IDD_TX_DELAY	100
#define IDD_TX_VEL	101
#define IDD_TX_PITCH	102
#define IDD_TX_CHAN	103

#define IDD_SB_DELAY	200
#define IDD_SB_VEL	201
#define IDD_SB_PITCH	202
#define IDD_SB_CHAN	203

#define AboutButton	800
#define IDD_QUITTER	2


#define MAXLEN		80

#define kMinDelay	1
#define kMaxDelay	2000
#define kDefaultDelay	1000

#define kMinVel	-127
#define kMaxVel	127
#define kDefaultVel	-10

#define kMinPitch	-127
#define kMaxPitch	127
#define kDefaultPitch	0

#define kMinChan	0
#define kMaxChan	16
#define kDefaultChan	0

// ------------------------------------------------------------------
// TYPES
// ------------------------------------------------------------------
typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned long	ulong;

typedef char TSTR[MAXLEN];

// ------------------------------------------------------------------
// PROTOTYPES
// ------------------------------------------------------------------
#ifdef WIN32
#define CALLBACKAPI	CALLBACK
#else
#define CALLBACKAPI FAR PASCAL _export
#endif

BOOL CALLBACKAPI msEchoDlgProc(HWND,UINT,UINT,LONG);

#endif
