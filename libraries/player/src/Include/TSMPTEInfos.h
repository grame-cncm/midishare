// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-2003, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


// ===========================================================================
//	TSMPTEInfos.h			    
// ===========================================================================
//
//	Infos for the kSMPTESync mode
// 


#ifndef __TSMPTEInfos__
#define __TSMPTEInfos__

 
//----------------------
// Class TSMPTEInfos 
//----------------------

class TSMPTEInfos {

	private:

		ULONG 		fSMPTEoffset;
		TSyncInfo   fSyncinfo;
	 
	public:
	
		TSMPTEInfos() {fSMPTEoffset = 0;}
		~TSMPTEInfos() {}
	
		void SetSMPTEOffset(SmpteLocPtr smptepos) 
		{ 
			#ifdef __Macintosh__
			fSMPTEoffset = MidiSmpte2Time(smptepos);
			#endif
		}
		
		Boolean IsSMPTELocked()
		{
			MidiGetSyncInfo(&fSyncinfo);
			return fSyncinfo.syncLocked;
		}
		
		ULONG GetSMPTEOffset() {return fSMPTEoffset;}
		
		
};

typedef TSMPTEInfos FAR * TSMPTEInfosPtr;

#endif
