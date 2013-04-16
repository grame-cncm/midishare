/*
  MidiShare Project
  Copyright (C) Grame 2001
  Copyright (C) Mil Productions 2001

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
