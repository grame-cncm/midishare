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


#ifndef __MacFeedback__
#define __MacFeedback__

#include "FeedbackProvider.h"

//_______________________________________
typedef struct Throughput {
	short		level;
	short		overflow;
	PicHandle	pic;
	Rect		rect;
} Throughput;

typedef struct WANList * WANListPtr;
typedef struct WANList {
	WANListPtr		next;
	TMidiRemote * 	remote;
	WindowPtr		win;
	Str255			name;
	unsigned long 	missed;
	short 			latOver;
	short			latency;
	long			nextUpdate;
	Throughput		in;
	Throughput		out;
} WANList;

//_________________________________________________________________________________
class MacFeedback : public FeedbackProvider
{
	public:
		//------------------------------------------ constructeur / destructeur
				 MacFeedback (WindowPtr * win, MenuHandle remote);
		virtual ~MacFeedback (void) {}		

		//------------------------------------------ methodes
		void 	Lookup 		(strPtr name);
		void 	Connecting 	(char * name);
		void 	CheckLatency (char * name);
		void 	CloseReport (TMidiRemote * remote, CnxReportPtr report);
		void 	Complete 	(TMidiRemote * remote);
		void 	Remove 		(TMidiRemote * remote);
		void 	CnxRefused	(strPtr host, short reason) {};
		void 	Failed 		(short reason)	{ Clear (); }
		void 	Clear 		();

		void 	CompleteDebug 	(Str255 name);

		void 	DoIdle 		();
		void 	Infos 		(short index);
		void 	CloseInfos 	(long Wrefcon);
		void 	Disconnect 	(WANListPtr r);
	
	private:
		enum { kFeedbackItem = 2, kRemoteWinID = 131 };
		
		unsigned char *	MakeText (Str255 dst, Str255 t1, Str255 t2);
		unsigned char *	MakeText (Str255 dst, Str255 t1, char * t2);
		void	SetText 	 (Str255 text);
		void	SetText 	 (DialogPtr dlog, short item, Str255 text);
		void	SetValue 	 (DialogPtr dlog, short item, long value);

		void 		Throughput (Rect *r, short lev, short overflow);
		void 		Throughput (WANListPtr r);
		void 		DoIdle (WANListPtr r);
		void		Remove (WANListPtr item, WANListPtr prev);
		WANListPtr	Find (TMidiRemote * item, WANListPtr *prev, short * index);
		WANListPtr	Find (short index, WANListPtr *prev);
		void	SaveLocation (DialogPtr dlog);
		void	Enlarge  (WindowPtr win);

		WindowPtr * fWinH;
		MenuHandle	fRemoteMenu;
		Rect		fSavedRect;
		Boolean 	fSaved, fEnlarged;
		WANListPtr	fRemoteList;
};

#endif