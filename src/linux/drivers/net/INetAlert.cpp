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


#include <stdio.h>
#include <string.h>

#include "INetAlert.h"

extern Boolean doneFlag;
void	log (char *msg);

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
	sprintf (msg, "%s: %s, %s %s\n", what, obj, reason, infos);
	log (msg);
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
