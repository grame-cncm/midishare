
#include <Carbon/Carbon.h>
#include <stdio.h>

#include "HostTools.h"
#include "INetAlert.h"

extern Boolean doneFlag;

//_________________________________________________________________________________
static unsigned char * c2pstr (const char *cstr, unsigned char *pstr)
{
	*pstr++ = strlen (cstr);
	strcpy ((char *)pstr, cstr);
	return --pstr; 
}

//_________________________________________________________________________________
void INetAlert::Report (const ErrString what, const ErrString obj, const ErrString reason, long err)
{
	ErrString buff;
	sprintf (buff, "%ld", err);
	Report (what, obj, reason, buff);
//	fprintf (stderr, "%s %s %s err:%ld\n", what, obj, reason, err);
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, const ErrString reason, long err)
{
	char buf[50];
	sprintf (buf, "(error %ld)", err);
	FatalError (what, obj, reason, buf);
}


//_________________________________________________________________________________
void INetAlert::Report (const ErrString what, const ErrString obj, const ErrString reason, 
					 const ErrString infos, void *proc)
{
	Str255 pwhat, pobj, preason, pinfos;
	ResetAlertStage ();
	ParamText (c2pstr(what,pwhat), c2pstr(obj,pobj), c2pstr(reason,preason), c2pstr(infos,pinfos));
	Alert (fAlertID, nil);
//	fprintf (stderr, "%s %s %s %s\n", what, obj, reason, infos);
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, const ErrString infos)
{
	Report (what, obj, reason, infos);
	doneFlag = true;
}
