/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __IPHeader__
#define __IPHeader__

//__________________________________________________________________________________________
// Internet header structure
//__________________________________________________________________________________________
typedef struct IPHeader {
	unsigned char	v_ihl;		// bit field: version/4 bits and ihl/4 bits
								// version indicates the internet header format (assumes 4)
								// ihl : internet header len (in 32 bits words) should be 5
	unsigned char	stype;		// type of service
	short			length;		// datagram total length (including header and datas)
	short			id;			// id assigned by the sender
	short			flags;		// bit field: includes control flags and fragment offset
	unsigned char	ttl;		// time to live
	unsigned char	protocol;	// next level protocol in data portion
	short			hChecksum;	// header checksum
	long			src;		// source address
	long 			dst;		// destination address
} IPHeader, *IPHeaderPtr;

#define IPVersion(v_ihl) 	(((v_ihl) >> 4) & 0xf)
#define IPIHL(v_ihl) 		((v_ihl) & 0xf)
#define IPFlags(flags) 		(((flags) >> 13) & 0x0007)
#define IPFragment(flags) 	((flags) & 0x1fff)

#endif
