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

#include "TClientInit.h"
#include "TPipesPair.h"
#include "TLog.h"

#ifdef WIN32
#	define sprintf			wsprintf
#	define toServer			0
#else
#	define toServer			kServerContactName
#endif

//_____________________________________________________________________
TClientInit::TClientInit (TLog * log)
	: fCommChan(log)
{
	fCommPipes = 0;
	fLog = log;
}

//_____________________________________________________________________
TPipesPair * TClientInit::Start () 
{
	if (fCommPipes) delete fCommPipes;
	

	fCommPipes = new TPipesPair(fLog);
	if (!fCommPipes) {
		if (fLog) fLog->WriteErr ("TClientInit::Start new TPipesPair failed:");
		return 0;
	}
	
	if (fCommChan.Open (kServerContactName)) {
		RemoteAddr addr;
		PipeDesc * pdesc = fCommPipes->WritePipe();
		
		int id = NewWPipe ();
		if (id < 0) goto err;

		if (!fCommChan.Write (id, toServer))
			goto err;

		if (!pdesc->pipe->Open(pdesc->name, TPipe::kWritePerm))
			goto err;

		if (HandleServerReply (fCommChan.Read(From(addr))))
			return PipesDetach ();
	}
err:
	Close();
	return 0;
}

//_____________________________________________________________________
void TClientInit::Close ()
{
	fCommChan.Close();
	if (fCommPipes) delete fCommPipes;
	fCommPipes = 0;
}

//_____________________________________________________________________
int TClientInit::NewWPipe ()
{
	int i; 
	PipeDesc * pdesc = fCommPipes->WritePipe();

	for (i=0; i<kMaxPipeIndex; i++) {
		fCommPipes->SetName (pdesc, kClientSndBaseName, i);
		if (pdesc->pipe->Create (pdesc->name, true))
			return i;
	}
	fCommPipes->SetName (pdesc, kClientSndBaseName, i);
	if (pdesc->pipe->Create (pdesc->name))
		return i;
	return -1;
}

//_____________________________________________________________________
int TClientInit::HandleServerReply (int id) 
{
	PipeDesc * pdesc = fCommPipes->ReadPipe();
	switch (id) {
		case kReadContactFailed:
			return false;
		case kBadContactType:
			if (fLog) fLog->Write ("ServerReply: bad msg type received\n");
			return false;
		default:
			if (id < 0) {
				if (fLog) fLog->Write ("ServerReply: bad id received\n");
				return false;
			}
	}
	fCommPipes->SetName (pdesc, kClientRcvBaseName, id);
	return pdesc->pipe->Open (pdesc->name);
}

//_____________________________________________________________________
TPipesPair * TClientInit::PipesDetach ()	
{
	TPipesPair * com = fCommPipes;
	fCommPipes = 0;
	return com;
}
