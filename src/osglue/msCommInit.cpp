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

#include "msCommInit.h"
#include "TClientInit.h"
#include "TServerInit.h"
#include "TLog.h"
#include "TPipesPair.h"
#include "TThreads.h"


#ifdef WIN32
#	define sleep(n)		Sleep(n)
#else
#	include <unistd.h>
#	define sleep(n)		usleep(n*1000)
#endif

#define PP(p)	((TPipesPair *)(p))

typedef struct {
	TThreads * thread;
	TServerInit * init;
	NewClientProcPtr proc;
} ServerComm, *ServerCommPtr;

ServerComm gSCom = { 0 };
extern TLog gLog;

//___________________________________________________________________
// client communication initialization
//___________________________________________________________________
CInitHandler CreateClientInit ()
{
	return new TClientInit ();
}

PipesPair StartClientInit (CInitHandler cih)
{
	TClientInit * c = (TClientInit *)cih;
	return c ? c->Start () : 0;
}

void EndClientInit (CInitHandler cih)
{
	TClientInit * c = (TClientInit *)cih;
	if (c) delete c;
}

//___________________________________________________________________
// server communication initialization
//___________________________________________________________________
static ThreadAPI(mainServerCom, ptr)
{
	ServerCommPtr com = (ServerCommPtr)ptr;
	TServerInit * init = com->init;

	if (!init->Start()) {
		delete com->init;
		com->init = 0;
		return 0;
	}

	while (1) {
		RemoteAddr addr;
		TPipesPair * pipes;
		
		int ret = init->Read (From(addr));
		if (ret >= 0) {
			pipes = init->HandleClientRequest (ret);
			if (pipes) {
				PipeDesc * pdesc = pipes->WritePipe();
				init->Write (ret, addr);
				if (!pdesc->pipe->Open (pdesc->name, TPipe::kWritePerm))
					delete pipes;
				else {
					com->proc(pipes);
				}					
			}			
		}
		else if (ret == kBadContactType)
			gLog.Write ("mainServerCom: bad msg type received\n");
	}
	return 0;
}

int ServerMainComInit (NewClientProcPtr proc)
{
	if (!proc) return false;
	gSCom.proc = proc;
	gSCom.init = new TServerInit (&gLog);
	if (!gSCom.init) goto err;
	gSCom.thread = new TThreads (&gLog);
	if (!gSCom.thread) goto err;
	if (!gSCom.thread->Create (mainServerCom, &gSCom)) goto err;
	sleep (100);				// wait for thread initialization
	if (!gSCom.init) goto err;	// init unable to start
	return true;
	
err:
	ServerMainComStop (true);
	return false;
}

void ServerMainComStop (int stopThread) {
	if (gSCom.init) delete gSCom.init;
	gSCom.init = 0;
	if (gSCom.thread && stopThread) {
		delete gSCom.thread;
	}
	gSCom.thread = 0;
}

//___________________________________________________________________
// pipes management
//___________________________________________________________________
void ClosePipesPair (PipesPair p)
{
	if (p) delete (TPipesPair *)p;
}

long PPWrite (PipesPair p, void *buff, long len)
{
	return PP(p)->Write (buff, len);
}

long PPRead (PipesPair p, void *buff, long len)
{
	return PP(p)->Read (buff, len);
}

short PPID (PipesPair p)
{
	return PP(p)->GetID ();
}

void PPSetInfos (PipesPair p, void * infos)
{
	PP(p)->SetInfos (infos);
}

void * PPGetInfos (PipesPair p)
{
	return PP(p)->GetInfos ();
}
