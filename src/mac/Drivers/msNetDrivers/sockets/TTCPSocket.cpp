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



#include "TTCPSocket.h"
#include "TInetAddress.h"
#include "OTUtilities.h"

//____________________________________________________________
// Open Transport callback
//____________________________________________________________
static pascal void TTCPSocketNotifier (void* contextPtr, OTEventCode code, 
									   OTResult result, void* cookie)
{
	TTCPSocket * sok = (TTCPSocket *)contextPtr;
	sok->NotifyProc (code, result, cookie);
}

//____________________________________________________________
// Generic TTCPSocket
//____________________________________________________________
TTCPSocket::TTCPSocket (short port) : TSocket (port)
{
	fTCPClient = 0;
}

//____________________________________________________________
TTCPSocket::TTCPSocket (short port, TTCPListener * client)
	 : TSocket (port)
{
	fTCPClient = client;
}

//____________________________________________________________
TTCPSocket::~TTCPSocket ()
{
	Close ();
}

//____________________________________________________________
SocketStatus TTCPSocket::SetOptions ()
{
	OSStatus err = OTUtilities::SetOption (fRef, XTI_GENERIC, XTI_RCVLOWAT, 1);
	if (err != noErr) return err;
	err = OTUtilities::SetOption (fRef, XTI_GENERIC, XTI_SNDLOWAT, 1);
	return err;
}

//____________________________________________________________
SocketStatus TTCPSocket::Open ()
{
	OSStatus err;
	Close();
	fRef = OTOpenEndpoint(OTCreateConfiguration(kTCPName), 0, nil, &err);
	if (err == noErr)
		return SetOptions ();
	else fRef = 0;
	return err;
}

//____________________________________________________________
void TTCPSocket::Close ()
{
	if (fRef) {
		fRef->SndOrderlyDisconnect ();
		fRef->RemoveNotifier();
		if (fBind) fRef->Unbind();
		OTCloseProvider (fRef);
		fRef = 0;
		fBind = false;
	}
}

//____________________________________________________________
SocketStatus TTCPSocket::Send (void *buff, short *len)
{
	OTResult ret = OTSnd(fRef, buff, *len, 0);
	if (ret < 0) {
		*len = 0;
		return ret;
	}
	*len = ret;
	return noErr;
}

//____________________________________________________________
SocketStatus TTCPSocket::Listen (IPAddrPtr from, void *buff, short *len)
{
	OTFlags flags;
	OTResult ret = OTRcv (fRef, buff, *len, &flags);
	if (ret < 0) {
		*len = 0;
		return ret;
	}
	*len = ret;
	return noErr;
}

//____________________________________________________________
SocketStatus TTCPSocket::Listen (IPAddrPtr from, SocketRef ref, void *buff, short *len)
{
	OTFlags flags;
	OTResult ret = OTRcv (ref, buff, *len, &flags);
	if (ret < 0) {
		*len = 0;
		return ret;
	}
	*len = ret;
	return noErr;
}

//____________________________________________________________
OSStatus TTCPSocket::Disconnect (Boolean orderly)
{
	OSStatus err;
	if (orderly)
		err = fRef->RcvOrderlyDisconnect ();
	else
		err = fRef->RcvDisconnect (nil);
	if (fTCPClient)
		fTCPClient->Disconnect (0);
	return err;
}

//____________________________________________________________
// TTCPServer
//____________________________________________________________
SocketStatus TTCPServer::ServerOpen (short maxClients)
{
	OSStatus err; TBind req;
	struct InetAddress in = { 0 };

	in.fAddressType = AF_INET;
	in.fPort = fPort;
	req.addr.len = sizeof(struct InetAddress);
	req.addr.buf = (unsigned char *) &in;
	
	err = Open ();
	if (err != kOTNoError) return err;
 	err = fRef->InstallNotifier (TTCPSocketNotifier, this);
	if (err != kOTNoError) 	return err;
	err = fRef->SetSynchronous ();
	if (err != kOTNoError) 	return err;
	err = fRef->SetBlocking ();
	if (err != kOTNoError) 	return err;
	
   	fMaxClients = maxClients;
	req.qlen = fMaxClients;
    err = fRef->Bind(&req, 0);
    fBind = (err == noErr);
 	return err;
}

//____________________________________________________________
SocketStatus TTCPServer::SendTo (SocketRef to, void *buff, short *len)
{
	OTResult ret = OTSnd(to, buff, *len, 0);
	if (ret < 0) {
		*len = 0;
		return ret;
	}
	*len = ret;
	return noErr;
}

//____________________________________________________________
SocketStatus TTCPServer::SetOptions ()
{
	OSStatus err = TTCPSocket::SetOptions ();
	if (err != noErr) return err;
	OTUtilities::SetOption (fRef, INET_IP, IP_REUSEADDR, T_YES);
	return noErr;
}

//____________________________________________________________
void TTCPServer::AcceptLoop ()
{
	if (!fTCPClient) return;
	OTResult res = fRef->Look ();
	if (res == T_LISTEN) {
		IPAddr from; SocketRef ref;
		OSStatus err = Accept (&from, &ref);
		if (err != noErr) {
			fTCPClient->SockError (err);
		}
		else {
			fTCPClient->Accept (ref, &from);
		}
	}
}

//____________________________________________________________
OSStatus TTCPServer::Accept (IPAddrPtr addr, SocketRef *ref)
{
	TCall cnct;
	OSStatus err; SocketRef newsok;
	
	cnct.addr.maxlen = sizeof(IPAddr);
	cnct.addr.buf = (UInt8 *)addr;
	cnct.opt.maxlen = cnct.opt.len = 0;
	cnct.opt.buf = 0;
	cnct.udata.maxlen = cnct.udata.len = 0;
	cnct.udata.buf = 0;
	err = fRef->Listen (&cnct);
	if (err != noErr) return err;

	newsok = OTOpenEndpoint (OTCreateConfiguration(kTCPName), 0, nil, &err);
	if (err != noErr) return err;
	
	err = fRef->Accept (newsok, &cnct);
	if (err == noErr) *ref = newsok;
	else OTCloseProvider (newsok);
	return err;
}

//____________________________________________________________
// TTCPClient
//____________________________________________________________
TTCPClient::TTCPClient (SocketRef ref, IPAddrPtr addr, TTCPListener * client)
	: TTCPSocket(0, client)
{
	fRef = ref;
	fMTU = kDefaultMTU;
	fPort = ntohs(addr->fPort);
	fBind = false;
	SetOptions ();
	fRef->InstallNotifier (TTCPSocketNotifier, this);
	fRef->SetAsynchronous ();
	fRef->SetNonBlocking ();
	SetMTU ();
}

//____________________________________________________________
SocketStatus TTCPClient::ClientOpen (IPAddrPtr addr)
{
	OSStatus err; TBind req;
	struct InetAddress in = { 0 };
	in.fAddressType = AF_INET;
	in.fPort = fPort;
	req.addr.len = sizeof(struct InetAddress);
	req.addr.buf = (unsigned char *) &in;
	
	err = Open ();
	if (err != kOTNoError) return err;

	req.qlen = 1;
	req.addr.len = 0;
	err = fRef->Bind(&req, 0);
	fBind = (err == noErr);
	if (err != kOTNoError) 	return err;

	addr->fPort = fPort;
	err = Connect (addr);
	if (err == kOTLookErr) {
		err = fRef->Look();
		if (err == T_DISCONNECT) {
			fRef->RcvDisconnect (nil);
			err = kECONNREFUSEDErr;
		}
	}
	if (err != kOTNoError) 	return err;
	
	SetMTU ();
 	err = fRef->InstallNotifier (TTCPSocketNotifier, this);
	if (err != kOTNoError) 	return err;		
	err = fRef->SetAsynchronous ();
	if (err != kOTNoError) 	return err;
	err = fRef->SetNonBlocking ();
	return err;
}

//____________________________________________________________
void TTCPClient::SetMTU ()
{
	fMTU = kDefaultMTU;
}

//____________________________________________________________
OSStatus TTCPClient::Connect (IPAddrPtr to)
{
	static TCall cnct;
	cnct.addr.len = sizeof(InetAddress);
	cnct.addr.buf = (UInt8 *)to;
	cnct.opt.len = 0;
	cnct.opt.buf = 0;
	cnct.udata.len = 0;
	cnct.udata.buf = 0;
	return OTConnect (fRef, &cnct, 0);
}

//____________________________________________________________________________________
void TTCPSocket::NotifyProc (OTEventCode code, OTResult result, void* cookie)
{
	SocketStatus err; 
	IPAddr from; short len;
	
	switch (code) {
		case T_DISCONNECT: 	Disconnect (false);
			break;
		case T_ORDREL: 		Disconnect (true);
			break;
		case T_CONNECT: 	fRef->RcvConnect (0);
			break;
		case T_EXDATA:
		case T_DATA: 
			do {
				len = kMaxPacketSize;
				err = Listen (&from, (SocketRef)cookie, &fPacket, &len);
				if (err == noErr)
					fTCPClient->RcvPacket (&fPacket, len);
				else if (err != kOTNoDataErr) {
					fTCPClient->SockError (err);
				}
			} while (err == noErr);
			break;
			
		case kOTProviderWillClose:
		case kOTProviderIsClosed:
			fTCPClient->NetDisconnected ();
			break;
		default:
			fTCPClient->RcvEvent (code, result, cookie);
	}
}
