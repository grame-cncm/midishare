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

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "INetAlert.h"
#include "NetTools.h"
#include "misc.h"

extern Boolean doneFlag;

#ifdef WANDriver
static char * ErrFile = "msWANDriver.log";
#else
static char * ErrFile = "msLANDriver.log";
#endif

//_______________________________________________________________________
char * dateString ()
{
	static char date[256];
	time_t t;
	time(&t);
	strftime (date, 256, "[%D %T]", localtime(&t));
	return date;
}

//_______________________________________________________________________
void logmsg (char *msg)
{
	FILE * fd = fopen (MakeMSFullName(ErrFile), "a");
	if (fd) {
		fprintf (fd, "%s\n", msg);
		fclose (fd);
	}
}

//_________________________________________________________________________________
void INetAlert::Report (const ErrString what, const ErrString obj, 
	const ErrString reason, long err)
{
	Report (what, obj, reason, err ? strerror (err) : "");
}

//_________________________________________________________________________________
void INetAlert::Report (const ErrString what, const ErrString obj, 
	const ErrString reason, const ErrString infos, void *proc)
{
	char msg[512];
	sprintf (msg, "%s: %s: %s %s %s\n", dateString(), 
			what, obj, reason, infos);
	logmsg (msg);
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, long err)
{
	Report (what, obj, reason, err);
	doneFlag = true;
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, const ErrString infos)
{
	Report (what, obj, reason, infos);
	doneFlag = true;
}
