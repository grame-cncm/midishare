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

 
 #ifndef __MidiShareFilter__
 #define __MidiShareFilter__

#include "MidiShare.h"
#include "FilterUtils.h"

typedef unsigned char uchar;

//_______________________________________________________________________________
class MidiShareFilter
{
	public:
				 MidiShareFilter ();
		virtual ~MidiShareFilter ();
		
		MidiFilterPtr	GetMidiFilter (void);
		Boolean		IsAcceptedEv  (MidiEvPtr e);
		
		void	AcceptAll	();
		void	RejectAll	();

		void	AcceptPort	(uchar port);
		void	RejectPort	(uchar port);
		void	InvertPort	(uchar port);
		Boolean	IsAcceptedPort	(uchar port);
		
		void	AcceptChan	(uchar chan);
		void	RejectChan	(uchar chan);
		void	InvertChan	(uchar chan);
		Boolean	IsAcceptedChan	(uchar chan);
		
		void	AcceptType	(uchar type);
		void	RejectType	(uchar type);
		void	InvertType	(uchar type);
		Boolean	IsAcceptedType	(uchar type);
		
	protected:
#if macintosh
		TFilter	fFilter;
#else
		MidiFilterPtr	fFilter;
#endif
};

//_______________________________________________________________________________
#if macintosh
inline MidiFilterPtr MidiShareFilter::GetMidiFilter()	{ return &fFilter; }

inline void	MidiShareFilter::AcceptPort	(uchar port)	{ AcceptBit(fFilter.port, port); }
inline void	MidiShareFilter::RejectPort	(uchar port)	{ RejectBit(fFilter.port, port); }
inline void	MidiShareFilter::InvertPort	(uchar port)	{ InvertBit(fFilter.port, port); }
inline Boolean	MidiShareFilter::IsAcceptedPort	(uchar port)
													{ return IsAcceptedBit(fFilter.port, port); }
inline void	MidiShareFilter::AcceptChan	(uchar chan)	{ AcceptBit(fFilter.channel, chan); }
inline void	MidiShareFilter::RejectChan	(uchar chan)	{ RejectBit(fFilter.channel, chan); }
inline void	MidiShareFilter::InvertChan	(uchar chan)	{ InvertBit(fFilter.channel, chan); }
inline Boolean	MidiShareFilter::IsAcceptedChan	(uchar chan)
													{ return IsAcceptedBit(fFilter.channel, chan); }
inline void	MidiShareFilter::AcceptType	(uchar type)	{ AcceptBit(fFilter.evType, type); }
inline void	MidiShareFilter::RejectType	(uchar type)	{ RejectBit(fFilter.evType, type); }
inline void	MidiShareFilter::InvertType	(uchar type)	{ InvertBit(fFilter.evType, type); }
inline Boolean	MidiShareFilter::IsAcceptedType	(uchar type)
													{ return IsAcceptedBit(fFilter.evType, type); }

#else

inline MidiFilterPtr MidiShareFilter::GetMidiFilter()	{ return fFilter; }

inline void	MidiShareFilter::AcceptPort	(uchar port)	{ MidiAcceptPort(fFilter, port, true); }
inline void	MidiShareFilter::RejectPort	(uchar port)	{ MidiAcceptPort(fFilter, port, false); }
inline void	MidiShareFilter::InvertPort	(uchar port)	
								{ MidiAcceptPort(fFilter, port, !IsAcceptedPort(port)); }
inline Boolean	MidiShareFilter::IsAcceptedPort	(uchar port)
								{ return MidiIsAcceptedPort(fFilter, port); }

inline void	MidiShareFilter::AcceptChan	(uchar chan)	{ MidiAcceptChan(fFilter, chan, true); }
inline void	MidiShareFilter::RejectChan	(uchar chan)	{ MidiAcceptChan(fFilter, chan, false); }
inline void	MidiShareFilter::InvertChan	(uchar chan)	
								{ MidiAcceptChan(fFilter, chan, !IsAcceptedChan(chan)); }
inline Boolean	MidiShareFilter::IsAcceptedChan	(uchar chan)
								{ return MidiIsAcceptedChan(fFilter, chan); }

inline void	MidiShareFilter::AcceptType	(uchar type)	{ MidiAcceptType(fFilter, type, true); }
inline void	MidiShareFilter::RejectType	(uchar type)	{ MidiAcceptType(fFilter, type, false); }
inline void	MidiShareFilter::InvertType	(uchar type)	
								{ MidiAcceptType(fFilter, type, !IsAcceptedType(type)); }
inline Boolean	MidiShareFilter::IsAcceptedType	(uchar type)
								{ return MidiIsAcceptedType(fFilter, type); }
#endif

#endif
