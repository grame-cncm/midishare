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

#define __USE_UNIX98

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "TMutex.h"

//_____________________________________________________________________
TMutex::TMutex ()
{
	Init ();
}

//_____________________________________________________________________
void TMutex::Init ()
{
	pthread_mutex_init (&fMutex, 0);
}

//_____________________________________________________________________
int TMutex::Destroy ()
{
    if (pthread_mutex_destroy (&fMutex)) {
        return false;
    }
    return true;
}

//_____________________________________________________________________
int TMutex::Lock ()
{
	if (pthread_mutex_lock (&fMutex)) {
		return false;
	}
	return true;
}

//_____________________________________________________________________
int TMutex::Unlock ()
{
	if (pthread_mutex_unlock (&fMutex)) {
		return false;
	}
	return true;
}
