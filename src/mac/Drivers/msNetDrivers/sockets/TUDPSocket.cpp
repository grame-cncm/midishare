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


#include "TUDPSocket.h"
#include "OTUtilities.h"

//____________________________________________________________
TUDPSocket::TUDPSocket (short port) : TSocket (port)
{
	fClient = 0;
}

//____________________________________________________________
TUDPSocket::~TUDPSocket ()
{
	Close ();
}

//____________________________________________________________
void TUDPSocket::Close ()
{
	if (Opened()) {
		fRef->SetSynchronous ();
		fRef->RemoveNotifier();
		if (fBind) fRef->Unbind();
		OTCloseProvider (fRef);
		fRef = 0;
		fBind = false;
	}
}

//____________________________________________________________
static pascal void TUDPSocketNotifier (void* contextPtr, OTEventCode code, 
									   OTResult result, void* cookie)
{
	TUDPSocket * sok = (TUDPSocket *)contextPtr;
	sok->NotifyProc (code, result, cookie);
}

//____________________________________________________________
SocketStatus TUDPSocket::SetOptions (Boolean local)
{
	UInt32 value = 0; OSStatus err;

	if (local) {
		err = OTUtilities::SetOption (fRef, INET_IP, IP_BROADCAST, T_YES);
		err = OTUtilities::SetOption (fRef, INET_IP, IP_DONTROUTE, T_YES);
	}
	else {
		err = OTUtilities::SetOption (fRef, INET_IP, IP_DONTROUTE, T_NO);
	}
	err = OTUtilities::SetOption (fRef, INET_IP, IP_TOS, SET_TOS(T_NETCONTROL, T_LDELAY + T_HIREL));
	err = OTUtilities::SetOption (fRef, XTI_GENERIC, XTI_RCVLOWAT, 1);
	if (err != noErr) return err;
	err = OTUtilities::SetOption (fRef, XTI_GENERIC, XTI_SNDLOWAT, 1);
	return err;
}

//____________________________________________________________
SocketStatus TUDPSocket::Open (Boolean local)
{
	OSStatus err; TBind req;
	struct InetAddress in = { 0 };
	
	if (Opened()) return noErr;
	fRef = OTOpenEndpoint(OTCreateConfiguration(kUDPName), 0, nil, &err);
	if (fRef && (err == noErr)) {
		err = SetOptions (local);
		if (err != kOTNoError) 	return err;

		err = fRef->SetAsynchronous ();
		if (err != kOTNoError) 	return err;
	
		err = fRef->InstallNotifier (TUDPSocketNotifier, this);
		if (err != kOTNoError) 	return err;
		
		in.fAddressType = AF_INET;
		in.fPort = fPort;
		req.addr.len = sizeof(struct InetAddress);
		req.addr.buf = (unsigned char *) &in;
		req.qlen = 1;										// don't care for udp
   		err = fRef->Bind(&req, 0);
    	fBind = (err == noErr);
	}
	return err;
}

//____________________________________________________________
SocketStatus TUDPSocket::Send (IPAddrPtr to, void *buff, short *len)
{
	TUnitData udata;
	
	to->fPort = fPort;
	udata.addr.buf 	= (UInt8 *)to;
	udata.addr.len 	= sizeof(IPAddr);
	udata.opt.buf 	= nil;
	udata.opt.len 	= 0;
	udata.udata.buf = (unsigned char *)buff;
	udata.udata.len = *len;

	SocketStatus err = fRef->SndUData (&udata);
	if (err == kOTLookErr) {
		err = fRef->Look();
		if (err == T_UDERR) {
			err = fRef->RcvUDErr (0);
		}
	}
	if (err != noErr) {
		err = fRef->RcvUDErr (0);
	}
	return err;
}

//____________________________________________________________
SocketStatus TUDPSocket::Listen (IPAddrPtr from, void *buff, short *len)
{
	OTFlags flags = 0; TUnitData data;
	OSStatus err;
	
	data.addr.buf 		= (UInt8 *)from;
	data.addr.maxlen 	= sizeof(IPAddr);
	data.opt.buf 		= nil;
	data.opt.maxlen 	= 0;
	data.udata.buf 	= (unsigned char *)buff;
	data.udata.maxlen 	= *len;
	err = OTRcvUData(fRef, &data, &flags);
	*len = (err == noErr) ? data.udata.len : 0;
	return err;
}

//____________________________________________________________________________________
void TUDPSocket::NotifyProc (OTEventCode code, OTResult result, void* cookie)
{
	SocketStatus err; IPAddr from;
	short len;

	if (code == T_DATA) {
			do {
				len = kMaxPacketSize;
				err = Listen (&from, &fPacket, &len);
				if (fClient) {
					if (err == noErr)
						fClient->RcvPacket (&from, &fPacket, len);
					else if (err != kOTNoDataErr) {
						fClient->SockError (err);
					}
				}
			} while (err == noErr);
	}
	else if (fClient) fClient->RcvEvent (code, result, cookie);
}
