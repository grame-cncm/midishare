/*
  Copyright © Grame 2002,2003

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

#ifndef __msCommChans__
#define __msCommChans__


#ifdef WIN32
# include <windows.h>
typedef DWORD RemoteAddr;
typedef struct{
    UINT	type;
    WPARAM	id;
} ContactMsg, * ContactMsgPtr;
#else
typedef char RemoteAddr[256];
typedef struct{
    int		type;
    int		id;
} ContactMsg, * ContactMsgPtr;
#endif

typedef struct {
    RemoteAddr 	addr;
    ContactMsg	msg;
} MPMsg, * MPMsgPtr;

typedef void * MeetingPointChan;
typedef struct CChan {
	void * 	cc;
	void * 	rtcc;
	int  	refcount;
	int		id;
} * CommunicationChan;

#ifdef __cplusplus
extern "C" {
#endif

/* meeting point management */
/* warning: on windows, this CreateMeetingPoint function MUST be called by the reader thread */
MeetingPointChan	CreateMeetingPoint();
MeetingPointChan	OpenMeetingPoint  ();
void 				CloseMeetingPoint (MeetingPointChan mp);

long	MPWrite 	(MeetingPointChan mp, MPMsgPtr msg);
long	MPRead 		(MeetingPointChan mp, MPMsgPtr msg);

/* communication channel management */
CommunicationChan 	RequestCommunicationChannel (MeetingPointChan mp);
CommunicationChan 	HandleCommunicationChannelRequest (MeetingPointChan mp);
void 				CloseCommunicationChannel   (CommunicationChan cc);

long	CCWrite 	(CommunicationChan cc, void *buff, long len);
long	CCRead 		(CommunicationChan cc, void *buff, long len);

long	CCRTWrite 	(CommunicationChan cc, void *buff, long len);
long	CCRTRead 	(CommunicationChan cc, void *buff, long len);
int	CCRTReadHandler (CommunicationChan cc);

int		CCInc 		(CommunicationChan cc);
int		CCDec 		(CommunicationChan cc);
int		CCRefCount 	(CommunicationChan cc);
short	CCGetID 	(CommunicationChan cc);

void	CCSetInfos 	(CommunicationChan cc, void * infos);
void *	CCGetInfos 	(CommunicationChan cc);

#ifdef __cplusplus
}
#endif

#endif
