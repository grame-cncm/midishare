/*

  Copyright � Grame 2001
  Copyright � Mil Productions 2001

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

#include "HostTools.h"

//________________________________________________________________________
strPtr HostTools::HostName (char *host)
{
#if macintosh
	static unsigned char s[256]; 
	unsigned char n=0, *ptr = &s[1];

	while (*host && (n < 255)){
		*ptr++ = *host++;
		n++;
	}
	s[0] = n;
	return s;
#else 
	return host;
#endif
}

//________________________________________________________________________
char * HostTools::HostName ()
{
#if macintosh
	StringHandle h;
	unsigned char *s; short resFile;

	resFile= CurResFile();
	UseResFile( nil);
	h= GetString (kMachineNameID);
	s=  h ? *h : "\punknown";
	s[s[0]+1] = 0;
	UseResFile( resFile);
	return (char *)&s[1];
#else 
	static char s[kMaxName];
	int ret = gethostname(s, kMaxName); 
	return (ret == -1) ? (char *)"unknown" : s;
#endif
}
