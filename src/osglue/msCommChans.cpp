/*
  Copyright © Grame 2002

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

#include <stdlib.h>

#ifdef WIN32
	#include "TMsgChan.h"
	typedef TMsgChan	MPChan;
#	define kServerAddr	0
#	define sprintf			wsprintf
#else
	#include <string.h>
	#include "TLocalSocket.h"	
	typedef TLocalSocket MPChan;
#	define kServerAddr	kServerContactName
#endif

#include "msCommDefs.h"
#include "msCommChans.h"
#include "TPipesPair.h"

#define PP(p)	((TPipesPair *)(p))
#define	kMaxPipeIndex	256
enum {
    kContactType = 0x0ace,
    kContactAllocationFailed = -1,
    kPipeCreationFailed = -2,
    kBadContactType = -3
};

//___________________________________________________________________
// meeting point management
//___________________________________________________________________
MeetingPointChan CreateMeetingPoint()
{
    MPChan * mp = new MPChan();
    if (mp) {
        if (mp->Create (kServerContactName))
            return mp;
        delete mp;
    }
    return 0;
}

MeetingPointChan OpenMeetingPoint  ()
{
    MPChan * mp = new MPChan();
    if (mp) {
        if (mp->Open (kServerContactName))
            return mp;
        delete mp;
    }
    return 0;
}

void CloseMeetingPoint (MeetingPointChan mp)
{
    delete (MPChan *)mp;
}

long MPWrite (MeetingPointChan mp, MPMsgPtr msg)
{
    MPChan * lmp = (MPChan *)mp;
#ifdef WIN32
    return lmp->Write (&msg->msg, msg->addr);
#else
    return lmp->Write (msg->addr, &msg->msg, sizeof(msg->msg));
#endif
}

long MPRead (MeetingPointChan mp, MPMsgPtr msg)
{
    MPChan * lmp = (MPChan *)mp;
#ifdef WIN32
    return lmp->Read (&msg->msg,&msg->addr);
#else
    return lmp->Read (&msg->msg, sizeof(msg->msg), msg->addr);
#endif
}

//___________________________________________________________________
// communication channel management
//___________________________________________________________________
static int NewIndexedPipe (TPipe * pipe, char *basename, int limit)
{
    int i; char name[kMaxPipeName];

    for (i=1; i<=limit; i++) {
        snprintf (name, kMaxPipeName, "%s%d", basename, i);
        if (pipe->Create (name)) {
			return i;
		}
    }
    return 0;
}

static int NewPipe (TPipe * pipe, char *basename, int index)
{
    char name[kMaxPipeName];
	snprintf (name, kMaxPipeName, "%s%d", basename, index);
	if (pipe->Create (name))
		return index;
    return 0;
}

static int OpenPipe (TPipe * pipe, char *basename, int index)
{
    char name[kMaxPipeName];
	snprintf (name, kMaxPipeName, "%s%d", basename, index);
	if (pipe->Open (name))
		return index;
    return 0;
}

static CommunicationChan NewCommunicationChannel ()
{
	CommunicationChan cc = (CommunicationChan)malloc(sizeof(struct CChan));
    TPipesPair * pp = new TPipesPair();
    TPipesPair * rtpp = new TPipesPair();

    if (pp && rtpp && cc) {
        cc->cc 		= pp;
		cc->rtcc	= rtpp;
		cc->id 			= 0;
		cc->refcount	= 0;
		return cc;
	}
	if (pp) delete pp;
	if (rtpp) delete rtpp;
    if (cc) free (cc);
    return 0;
}

static long MPSendID (MeetingPointChan mp, int type, int id)
{
	MPMsg msg;
	msg.msg.type = type;
	msg.msg.id = id;
#ifdef WIN32
	msg.addr = kServerAddr;
#else
	strncpy (msg.addr, kServerAddr, sizeof(msg.addr)-1);
#endif
	return MPWrite (mp, &msg);
}

CommunicationChan RequestCommunicationChannel (MeetingPointChan mp)
{
 	CommunicationChan cc = NewCommunicationChannel();

    if (cc) {
        MPMsg msg; long ret; int id;
		TPipesPair * pp = (TPipesPair *)cc->cc;
		TPipesPair * rtpp = (TPipesPair *)cc->rtcc;

        /* creates a first write pipe */
        id = NewIndexedPipe (pp->GetWritePipe(), kClientSndBaseName, kMaxPipeIndex);
        if (!id) goto err;
		else cc->id = id;
        /* creates the real-time write pipe */
		if (!NewPipe (rtpp->GetWritePipe(), kClientRTSndBaseName, id)) goto err;
		else cc->id = id;

        /* send a message to the server with the previously allocated id */
        ret = MPSendID (mp, kContactType, id);
        if (!ret) goto err;

        /* opens the pipe with write permission */
		if (!pp->GetWritePipe()->Open(TPipe::kWritePerm)) goto err;
        /* opens the real-time pipe with write permission */
        if (!rtpp->GetWritePipe()->Open(TPipe::kWritePerm)) goto err;

        /* and read the server reply */
        ret = MPRead (mp, &msg);
        if (!ret) goto err;

        /* checks the server reply */
        switch (msg.msg.type) {
			case kContactType:
				/* and finally opens the read pipes */
				if (!OpenPipe(pp->GetReadPipe(), kClientRcvBaseName, msg.msg.id)) goto err;
				if (!OpenPipe(rtpp->GetReadPipe(), kClientRTRcvBaseName, msg.msg.id)) goto err;
				break;
					
            case kContactAllocationFailed:
			case kPipeCreationFailed:
			case kBadContactType:
                goto err;
            default:
                if (msg.msg.type < 0) goto err;
        }
    }
    return cc;
err:
    CloseCommunicationChannel (cc);
    return 0;
}

CommunicationChan HandleCommunicationChannelRequest (MeetingPointChan mp)
{
	MPMsg msg; CommunicationChan cc = 0;
	
	/* read the meeting point channel */
	long ret = MPRead (mp, &msg);
	if (!ret) return 0;

	/* check for correctness of the message */
	if (msg.msg.type != kContactType) {
		msg.msg.type = kBadContactType;
		MPWrite (mp, &msg);
		return 0;
	}

	cc = NewCommunicationChannel();
    if (cc) {
		TPipesPair * pp = (TPipesPair *)cc->cc;
		TPipesPair * rtpp = (TPipesPair *)cc->rtcc;
		
		cc->id = msg.msg.id;
		/* opens the existing client pipe in read only mode */
 		if (!OpenPipe (pp->GetReadPipe(), kClientSndBaseName, msg.msg.id)) goto err;
		/* opens the existing client real-time pipe in read only mode */
 		if (!OpenPipe (rtpp->GetReadPipe(), kClientRTSndBaseName, msg.msg.id)) goto err;

		/* creates a new pipe */
		if (!NewPipe (pp->GetWritePipe(), kClientRcvBaseName, msg.msg.id)) goto err;
		/* creates a new real-time pipe */
		if (!NewPipe (rtpp->GetWritePipe(), kClientRTRcvBaseName, msg.msg.id)) goto err;

		/* sends the reply back to the client */
		/* the msg type and id actually don't change */
		ret = MPWrite (mp, &msg);
		if (!ret) goto err;

		/* and finally opens the new pipes with write permission */
 		if (!pp->GetWritePipe()->Open (TPipe::kWritePerm)) goto err;
 		if (!rtpp->GetWritePipe()->Open (TPipe::kWritePerm)) goto err;
 		return cc;
	}

err:
	msg.msg.type = kContactAllocationFailed;
	ret = MPWrite (mp, &msg);
    if (cc) CloseCommunicationChannel (cc);
    return 0;
}

void CloseCommunicationChannel (CommunicationChan cc)
{
	if (cc) {
		delete (TPipesPair *)cc->cc;
		delete (TPipesPair *)cc->rtcc;
		free (cc);
	}
}

long CCWrite (CommunicationChan cc, void *buff, long len)
{
	return PP(cc->cc)->Write (buff, len);
}

long CCRead (CommunicationChan cc, void *buff, long len)
{
	return PP(cc->cc)->Read (buff, len);
}

long CCRTWrite (CommunicationChan cc, void *buff, long len)
{
	return PP(cc->rtcc)->Write (buff, len);
}

long CCRTRead (CommunicationChan cc, void *buff, long len)
{
	return PP(cc->rtcc)->Read (buff, len);
}

int		CCInc 		(CommunicationChan cc)		{ return ++cc->refcount; }
int		CCDec 		(CommunicationChan cc)		{ return --cc->refcount; }
int		CCRefCount 	(CommunicationChan cc)		{ return cc->refcount; }
short	CCGetID 	(CommunicationChan cc)		{ return cc->id; }

/*
void CCSetInfos (CommunicationChan cc, void * infos)
{
	PP(cc->cc)->SetInfos (infos);
}

void * CCGetInfos (CommunicationChan cc)
{
	return PP(cc->cc)->GetInfos ();
}
*/
