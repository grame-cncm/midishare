/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

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
