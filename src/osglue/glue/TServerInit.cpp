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
#ifndef WIN32
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#endif

#include "TPipesPair.h"
#include "TServerInit.h"
#include "msCommDefs.h"
#include "TLog.h"

//_____________________________________________________________________
TServerInit::TServerInit (TLog * log)
	: fCommChan(log)
{
	fLog = log;
}

//_____________________________________________________________________
int TServerInit::Start () 
{
	Cleanup ();
	return fCommChan.Create (kServerContactName);
}

//_____________________________________________________________________
void TServerInit::Close ()
{
	fCommChan.Close();
}

//_____________________________________________________________________
TPipesPair * TServerInit::PipesInit (int clientID)
{
	PipeDesc * pdesc;
	TPipesPair * com = new TPipesPair (fLog);
	if (!com) {
		if (fLog) fLog->WriteErr ("PipesInit: new TPipesPair failed:");
		return 0;
	}

	pdesc = com->ReadPipe();
	com->SetName (pdesc, kClientSndBaseName, clientID);
	if (!pdesc->pipe->Open (pdesc->name)) goto err;

	pdesc = com->WritePipe();
	com->SetName (pdesc, kClientRcvBaseName, clientID);
	if (!pdesc->pipe->Create (pdesc->name)) goto err;
	return com;
err:
	delete com;
	return 0;
}

//_____________________________________________________________________
TPipesPair * TServerInit::HandleClientRequest (int id) 
{
	switch (id) {
		case kReadContactFailed:
			return 0;
		case kBadContactType:
			if (fLog) fLog->Write ("ServerReply: bad msg type received\n");
			return 0;
		default:
			if (id < 0) {
				if (fLog) fLog->Write ("ServerReply: bad id received\n");
				return 0;
			}
	}
	return PipesInit (id);
}

//_____________________________________________________________________
void TServerInit::Cleanup ()
{
#ifndef WIN32
	TPipesPair com; struct stat s;
	PipeDesc * pdesc;
	
	for (int i=0; i<kMaxPipeIndex; i++) {
		pdesc = com.ReadPipe();
		com.SetName (pdesc, kClientSndBaseName, i);
		if (!stat(pdesc->name, &s)) unlink (pdesc->name);

		pdesc = com.WritePipe();
		com.SetName (pdesc, kClientRcvBaseName, i);
		if (!stat(pdesc->name, &s)) unlink (pdesc->name);		
	}
#endif
}
