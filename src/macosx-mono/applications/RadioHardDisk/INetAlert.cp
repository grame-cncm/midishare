
#include <Carbon/Carbon.h>
#include <stdio.h>

#include "HostTools.h"
#include "INetAlert.h"

extern Boolean doneFlag;

//_________________________________________________________________________________
void INetAlert::Report (const ErrString what, const ErrString obj, const ErrString reason, long err)
{
	fprintf (stderr, "%s %s %s err:%ld\n", what, obj, reason, err);
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, const ErrString reason, long err)
{
	char buf[50];
	sprintf (buf, "%ld", err);
	FatalError (what, obj, reason, buf);
}


//_________________________________________________________________________________
void INetAlert::Report (const ErrString what, const ErrString obj, const ErrString reason, 
					 const ErrString infos, void *proc)
{
	fprintf (stderr, "%s %s %s %s\n", what, obj, reason, infos);
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, const ErrString infos)
{
	Report (what, obj, reason, infos);
	doneFlag = true;
}
