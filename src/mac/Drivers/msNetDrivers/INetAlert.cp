
#include <TextUtils.h>
#include <Notification.h>

#include "HostTools.h"
#include "INetAlert.h"

extern Boolean doneFlag;

//_________________________________________________________________________________
StringPtr HostTools::PStrCat (Str255 s1, Str255 s2, Str255 dst)
{
	unsigned char * ptr = &dst[1];
	short i, n1 = *s1++, n2 = *s2++;
	if ((n1 + n2) > 255) n2 = 255 - n1;
	
	for (i=0; i<n1; i++)
		*ptr++ = *s1++;
	for (i=0; i<n2; i++)
		*ptr++ = *s2++;
	dst[0] = n1 + n2;
	return dst;
}

//_________________________________________________________________________________
unsigned char * INetAlert::MakeOSErrString (long err)
{
	static Str255 dst = "\perr. code #";
	Str255 numStr; 
	dst[0] = 11;
	NumToString(err, numStr);
	for (int i=1, j=12; i<=numStr[0]; i++, j++)
		dst[j] = numStr[i];
	dst[0] += numStr[0];
	return dst;
}

//_________________________________________________________________________________
unsigned char * INetAlert::GetErrorText (long err)
{
	switch (err) {
		case memFullErr:
			return "\pnot enough memory for the requested action";
		case opWrErr:
			return "\pfile already open with with write permission";
		case fBsyErr:
			return "\pfile is busy";
		case dupFNErr:
			return "\pduplicate filename";
		case permErr:
			return "\ppermissions error";
		case wrPermErr:
			return "\pwrite permissions error";
		case dirNFErr:
			return "\pdirectory not found";
		case dirFulErr:
			return "\pdirectory full";
		case dskFulErr:
			return "\pdisk full";
		case fnOpnErr:
			return "\pfile not open";
		case eofErr:
			return "\pend of file";
		case posErr:
			return "\ptried to position to before start of file";
		case mFulErr:
			return "\pmemory full (open) or file won't fit (load)";
		case tmfoErr:
			return "\ptoo many files open";
		case fnfErr:
			return "\pfile not found";
		case wPrErr:
			return "\pdiskette is write protected";
		case fLckdErr:
			return "\pfile is locked";
		case noHardwareErr:
			return "\pno hardware (sound manager)";
	}
	return "\p";
}

//_________________________________________________________________________________
void INetAlert::Report (const Str255 what, const Str255 obj, const Str255 reason, long err)
{
	unsigned const char *ptr, *errStr;

	errStr = err ? MakeOSErrString (err) : "\p";
	ptr = reason[0] ? reason : GetErrorText (err);
	Report (what, obj, ptr, errStr);
}

//_________________________________________________________________________________
void INetAlert::FatalError (const Str255 what, const Str255 obj, const Str255 reason, long err)
{
	unsigned const char *ptr, *errStr;

	errStr = err ? MakeOSErrString (err) : "\p";
	ptr = reason[0] ? reason : GetErrorText (err);
	FatalError (what, obj, ptr, errStr);
}

#ifdef __BackgroundOnly__

//_________________________________________________________________________________
static void fatalProc (NMRecPtr nmReqPtr)
{
	Boolean * flag = (Boolean *)nmReqPtr->nmRefCon;
	*flag = true;
}
static void contProc (NMRecPtr nmReqPtr)
{
}

//_________________________________________________________________________________
void INetAlert::Report (const Str255 what, const Str255 obj, const Str255 reason, 
					 const Str255 infos, void *proc)
{
	static NMRec notify;
	static Str255 str;
	OSErr err;

	notify.qType = nmType;
	notify.nmMark = 0;
	notify.nmIcon = 0;
	notify.nmSound = (Handle)-1;
	HostTools::PStrCat ((unsigned char *)what, "\p: ", str);
	HostTools::PStrCat (str, (unsigned char *)obj, str);
	HostTools::PStrCat (str, "\p: ", str);
	HostTools::PStrCat (str, (unsigned char *)reason, str);
	HostTools::PStrCat (str, "\p ", str);
	HostTools::PStrCat (str, (unsigned char *)infos, str);
	notify.nmStr = str;
	notify.nmResp = NewNMProc(proc ? proc : contProc);
	notify.nmRefCon = (long)&doneFlag;

	err = NMInstall(&notify);
	if (err != noErr) ExitToShell ();
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, const ErrString infos)
{
	Report (what, obj, reason, infos, fatalProc);
}

#else
//_________________________________________________________________________________
void INetAlert::Report (const Str255 what, const Str255 obj, const Str255 reason, 
					 const Str255 infos, void *proc)
{
	ResetAlertStage ();
	ParamText (what, obj, reason, infos);
	Alert (fAlertID, nil);
}

//_________________________________________________________________________________
void INetAlert::FatalError (const ErrString what, const ErrString obj, 
						const ErrString reason, const ErrString infos)
{
	Report (what, obj, reason, infos);
	doneFlag = true;
}

#endif
