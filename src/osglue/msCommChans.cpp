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
    return lmp->Write (msg->msg, msg->addr);
#else
    return lmp->Write (msg->addr, &msg->msg, sizeof(msg->msg));
#endif
}

long MPRead (MeetingPointChan mp, MPMsgPtr msg)
{
    MPChan * lmp = (MPChan *)mp;
#ifdef WIN32
    return lmp->Read (msg->msg,&msg->addr);
#else
    return lmp->Read (&msg->msg, sizeof(msg->msg), msg->addr);
#endif
}

//___________________________________________________________________
// communication channel management
//___________________________________________________________________
static int NewIndexedPipe (TPipesPair * pp, char *basename, int limit)
{
    int i;
    PipeDesc * pdesc = pp->GetWritePipe();

    for (i=1; i<=limit; i++) {
        pp->SetName (pdesc, basename, i);
        if (pdesc->pipe->Create (pdesc->name))
            return i;
    }
    return 0;
}

CommunicationChan RequestCommunicationChannel (MeetingPointChan mp)
{
    /* creates a pair of pipes */
    TPipesPair * pp = new TPipesPair();
    if (pp) {
        MPMsg msg; long ret;
        PipeDesc * wpdesc = pp->GetWritePipe();
        PipeDesc * rpdesc = pp->GetReadPipe();

        /* creates a first pipe (will be the write pipe) */
        msg.msg.id = NewIndexedPipe (pp, kClientSndBaseName, kMaxPipeIndex);
        if (!msg.msg.id) goto err;

        /* send a message to the server with the previously allocated id */
        msg.msg.type = kContactType;
#ifdef WIN32
        msg.addr = kServerAddr;
#else
        strncpy (msg.addr, kServerAddr, sizeof(msg.addr)-1);
#endif

        ret = MPWrite (mp, &msg);
        if (!ret) goto err;

        /* opens the pipe with write permission */
        if (!wpdesc->pipe->Open(wpdesc->name, TPipe::kWritePerm))
            goto err;

        /* and read the server reply */
        ret = MPRead (mp, &msg);
        if (!ret) goto err;

        /* checks the server reply */
        switch (msg.msg.type) {
			case kContactType:
				/* and finally opens the read pipe */
				pp->SetName (rpdesc, kClientRcvBaseName, msg.msg.id);
				if (!rpdesc->pipe->Open (rpdesc->name)) goto err;
				break;
					
            case kContactAllocationFailed:
			case kPipeCreationFailed:
			case kBadContactType:
                goto err;
            default:
                if (msg.msg.type < 0) goto err;
        }
    }
    return pp;
err:
    CloseCommunicationChannel (pp);
    return 0;
}

CommunicationChan HandleCommunicationChannelRequest (MeetingPointChan mp)
{
	MPMsg msg; TPipesPair * pp = 0;
	
	/* read the meeting point channel */
	long ret = MPRead (mp, &msg);
	if (!ret) return 0;

	/* check for correctness of the message */
	if (msg.msg.type != kContactType) {
		msg.msg.type = kBadContactType;
		MPWrite (mp, &msg);
		return 0;
	}
	else {
		/* creates a new pair of pipes */
		pp = new TPipesPair();
		if (pp) {
			/* opens the existing client pipe in read only mode */
			PipeDesc * pdesc = pp->GetReadPipe();
			pp->SetName (pdesc, kClientSndBaseName, msg.msg.id);
			if (!pdesc->pipe->Open (pdesc->name)) goto err;

			/* creates a new pipe */
			pdesc = pp->GetWritePipe();
			pp->SetName (pdesc, kClientRcvBaseName, msg.msg.id);
			if (!pdesc->pipe->Create (pdesc->name)) goto err;

			/* sends the reply back to the client */
			/* the msg type and id actually don't change */
			ret = MPWrite (mp, &msg);
			if (!ret) goto err;

			/* and finally opens the new pipe with write permission */
			if (!pdesc->pipe->Open (pdesc->name, TPipe::kWritePerm))
				goto err;
			return pp;
		}
	}

err:
	msg.msg.type = kContactAllocationFailed;
	ret = MPWrite (mp, &msg);
    CloseCommunicationChannel (pp);
    return 0;
}

void CloseCommunicationChannel (CommunicationChan cc)
{
	if (cc) delete (TPipesPair *)cc;
}

long CCWrite (CommunicationChan cc, void *buff, long len)
{
	return PP(cc)->Write (buff, len);
}

long CCRead (CommunicationChan cc, void *buff, long len)
{
	return PP(cc)->Read (buff, len);
}

short CCGetID (CommunicationChan cc)
{
	return PP(cc)->GetID ();
}

/*
void CCSetInfos (CommunicationChan cc, void * infos)
{
	PP(cc)->SetInfos (infos);
}

void * CCGetInfos (CommunicationChan cc)
{
	return PP(cc)->GetInfos ();
}
*/
