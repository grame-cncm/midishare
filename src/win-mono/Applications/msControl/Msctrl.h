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

#ifndef __msCtrl__
#define __msCtrl__

#define VERSIONINFO_1	1
#define Version	235

// ------------------------------------------------------------------
// DECLARATION DES CONSTANTES
// ------------------------------------------------------------------
// indice de chaque objet
#define IND_PORT	0
#define IND_CHAN	1
#define IND_PROG	2
#define IND_VOL		3
#define IND_MOD		4
#define IND_PAN		5
#define IND_PITCH	6
#define IND_VEL		7

// indentifiants
#define IDD_TX_PORT	100
#define IDD_TX_CHAN	101
#define IDD_TX_PROG	102
#define IDD_TX_VOL	103
#define IDD_TX_MOD	104
#define IDD_TX_PAN	105
#define IDD_TX_PITCH	106
#define IDD_TX_VEL	107

#define IDD_SB_PORT	200
#define IDD_SB_CHAN	201
#define IDD_SB_PROG	202
#define IDD_SB_VOL	203
#define IDD_SB_MOD	204
#define IDD_SB_PAN	205
#define IDD_SB_PITCH	206
#define IDD_SB_VEL	207

// echelles
#define MIN_PORT	0
#define MAX_PORT	15
#define DEF_PORT	0

#define MIN_CHAN	0
#define MAX_CHAN	15
#define DEF_CHAN	0

#define MIN_PROG	0
#define MAX_PROG	127
#define DEF_PROG	0

#define MIN_VOL		0
#define MAX_VOL		127
#define DEF_VOL		0

#define MIN_MOD		0
#define MAX_MOD		127
#define DEF_MOD		0

#define MIN_PAN		0
#define MAX_PAN		127
#define DEF_PAN		0

#define MIN_PITCH	0
#define MAX_PITCH	127
#define DEF_PITCH	0

#define MIN_VEL		0
#define MAX_VEL		127
#define DEF_VEL		0

//#define DEF_DUREE	80

#define IDD_QUITTER	2
#define AboutButton	800

#define MAXLEN		80

// ------------------------------------------------------------------
// DECLARATION DES MACROS
// ------------------------------------------------------------------
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

// ------------------------------------------------------------------
// DECLARATION DES TYPES
// ------------------------------------------------------------------
typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned long	ulong;

typedef char TSTR[MAXLEN];

// ------------------------------------------------------------------
// DECLARATION DES PROTOTYPES
// ------------------------------------------------------------------

#ifdef WIN32
#define CALLBACKAPI		CALLBACK
#else
#define CALLBACKAPI		FAR PASCAL _export
#endif

BOOL CALLBACKAPI UniversalCtrlDlgProc(HWND,UINT,WPARAM,LPARAM);

#endif /* __WMSCTRL__ */
