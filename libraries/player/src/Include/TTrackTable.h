/*

  Copyright © Grame 1996-2006

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


// ===========================================================================
//	TTrackTable.h			    
// ===========================================================================

#ifndef __TTrackTable__
#define __TTrackTable__

#include "PlayerDefs.h"
#include "MidiShare.h"
#include "TPlayerConstants.h"

//-------------------
// Class TTrackTable 
//-------------------
/*! 
	\brief Tracks management.
*/

class TTrackTable {

	private:	
	
		struct TTrack {

			friend class TTrackTable;
		        
			short fParam[kMaxParam];   // Parameter table for one track
			
		 	TTrack() {Init();}	
		 	virtual ~TTrack() {}
		 		
		 	void Init() 
		 	{
		 		fParam[kSolo] = kSoloOff; 
		 		fParam[kMute] = kMuteOff;
		 	}
		 	
		};
		
		TTrack 	fTracktable[kMaxTrack];   	// Track table
		long 	fSolo;			    	// Number of solo tracks
		
		Boolean	IsValid (short tracknum , short param)  
		{ 
			return ((tracknum >= 0) && (tracknum < kMaxTrack) && (param >= 0) && (param < kMaxParam));
		}
		
		Boolean	IsNewValue(short tracknum, short param, short value) {return (value != fTracktable[tracknum].fParam[param]);}
		
	public:
 	
 		TTrackTable():fSolo(0) {}
 		virtual ~TTrackTable() {}
 	
		void	SetParam (short tracknum, short param, short value);
		short	GetParam (short tracknum, short param);
	
		MidiEvPtr 	IsPlayable (MidiEvPtr e);
		void 		Clear();
		
		Boolean IsOnTrack(short tracknum) 
		{ 
			return ((fSolo > 0) &&  (fTracktable[tracknum].fParam[kSolo] == kSoloOn))
                                |((fSolo == 0) && (fTracktable[tracknum].fParam[kMute] == kMuteOff));
		}
				
		Boolean IsOffTrack(short tracknum) {return (!IsOnTrack(tracknum));}
};

typedef  TTrackTable FAR * TTrackTablePtr;

#endif
