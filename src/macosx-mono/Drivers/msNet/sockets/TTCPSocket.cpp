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


#include <errno.h> 
#include <fcntl.h> 
#include <pwd.h> 
#include <string.h> 
#include <signal.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h> 

#include "TTCPSocket.h"

typedef void * (*thread_main_ptr) (void * ptr);
extern Boolean doneFlag;

#define IP_MTU		14
//____________________________________________________________
// TCP threads entry points
//____________________________________________________________

static void QuitHandler (int sigNum) { pthread_exit (0); }
static void * sok_accept (void * ptr)
{
	int old;
	TTCPServer *sok = (TTCPServer *)ptr;
	pthread_setcancelstate (PTHREAD_CANCEL_DEFERRED, &old);
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &old);
	sok->AcceptLoop ();
	return 0;
}

static void * sok_listen (void * ptr)
{
	int old;
	TTCPClient *sok = (TTCPClient *)ptr;
	pthread_setcancelstate (PTHREAD_CANCEL_DEFERRED, &old);
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &old);
	sok->ListenLoop ();
	return 0;
}


//____________________________________________________________
// Generic TTCPSocket
//____________________________________________________________
TTCPSocket::TTCPSocket (short port) : TSocket (port)
{
	fThread = 0;
	fTCPClient = 0;
}

//____________________________________________________________
TTCPSocket::TTCPSocket (short port, TTCPListener * client)
	 : TSocket (port)
{
	fThread = 0;
	fTCPClient = client;
}

//____________________________________________________________
TTCPSocket::~TTCPSocket ()
{
	Close ();
}

//____________________________________________________________
SocketStatus TTCPSocket::Open ()
{
	SocketStatus ret;
	Close();
	ret = socket (AF_INET, SOCK_STREAM, 0);
	if (ret == -1) return errno;
	fRef = ret;
	ret = SetOptions ();
	return ret;
}

//____________________________________________________________
SocketStatus TTCPSocket::SetOptions ()
{
	int ret = 1;
	ret = setsockopt (fRef, SOL_SOCKET, SO_RCVLOWAT, &ret, sizeof(int));
	ret = 1;
	ret = setsockopt (fRef, SOL_SOCKET, SO_SNDLOWAT, &ret, sizeof(int));
	return ret;
}

//____________________________________________________________
void TTCPSocket::Close ()
{
	if (fThread) {
		void *threadRet;
		pthread_cancel (fThread);
		usleep (1000);
		pthread_join (fThread, &threadRet);
		fThread = 0;
	}
	if (fRef) {
		if (fBind) ;
		close (fRef);
		fRef = 0;
		fBind = false;
	}
}

//____________________________________________________________
SocketStatus TTCPSocket::Send (void *buff, short *len)
{
	int n = send (fRef, buff, *len, 0);
	*len = n;
	return (n == -1) ? -1 : noErr;
}

//____________________________________________________________
int TTCPSocket::ThreadCreate (TCPSocketMode mode)
{
	thread_main_ptr f = (mode == kTCPServerMode) ? sok_accept : sok_listen;
	return pthread_create(&fThread, NULL, f, this);
}

//____________________________________________________________
// TTCPServer
//____________________________________________________________
SocketStatus TTCPServer::ServerOpen (short maxClients)
{
	int ret = TTCPSocket::Open ();
	if (ret != noErr) return ret;	

	struct sockaddr_in in = { 0 };
	fMaxClients = maxClients;
	
	in.sin_family = AF_INET; 			/* host byte order */ 
	in.sin_port = htons(fPort); 		/* short, network byte order */ 
	in.sin_addr.s_addr = INADDR_ANY; 	/* auto-fill with my IP */ 
	if (bind (fRef, (struct sockaddr *)&in, sizeof(struct sockaddr)) == -1)
		return errno;
	if (listen (fRef, fMaxClients) == -1) return errno;
	return ThreadCreate (kTCPServerMode);
}

//____________________________________________________________
SocketStatus TTCPServer::SetOptions ()
{
	SocketStatus err = TTCPSocket::SetOptions ();
	if (err != noErr) return err;
	int ret = 1;
	return setsockopt (fRef, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(int));
}

//________________________________________________________________________
SocketStatus TTCPServer::SendTo (SocketRef to, void *buff, short *len)
{
	int n = send (to, buff, *len, 0);
	*len = n;
	return (n == -1) ? -1 : noErr;
}

//____________________________________________________________
void TTCPServer::AcceptLoop ()
{
	IPAddr from; socklen_t len;
	fd_set read; int ret; struct timeval t;
	
    while (!doneFlag) {
		t.tv_sec = 0;
		t.tv_usec = 50000;
		FD_ZERO(&read);
		FD_SET(fRef, &read);
		ret = select (fRef+1, &read, 0, 0, &t);
		if (ret > 0) {
			len = sizeof (from);
			int sok = accept (fRef, (struct sockaddr *)&from, &len);
			if (!fTCPClient) continue;
			if (sok == -1)
				fTCPClient->SockError (sok);
			else fTCPClient->Accept (sok, &from);
		}
	}
}

//____________________________________________________________
// TTCPClient
//____________________________________________________________
TTCPClient::TTCPClient (SocketRef ref, IPAddrPtr addr, TTCPListener * client)
	: TTCPSocket(0, client)
{
	fRef = ref;
	fPort = ntohs(addr->sin_port);
	fBind = false;
	ThreadCreate (kTCPClientMode);
	SetMTU ();
}

//____________________________________________________________
SocketStatus TTCPClient::ClientOpen (IPAddrPtr addr)
{
	int ret = TTCPSocket::Open ();
	if (ret != noErr) return ret;	

	addr->sin_port = htons(fPort);
	struct sockaddr * to = (struct sockaddr *)addr;
	ret = connect (fRef, to, sizeof(struct sockaddr));
	if (ret == -1) {
		return errno;
	}
	SetMTU ();
	return ThreadCreate (kTCPClientMode);
}

//____________________________________________________________
void TTCPClient::ListenLoop ()
{
	IPAddr from; short len;
	fd_set read; int ret; struct timeval t;
	
	while (fTCPClient && !doneFlag) {
		t.tv_sec = 0;
		t.tv_usec = 50000;
		FD_ZERO(&read);
		FD_SET(fRef, &read);
		ret = select (fRef+1, &read, 0, 0, &t);
		if (ret > 0) {
			len = sizeof (fPacket);
			SocketStatus err = Listen (&from, &fPacket, &len);
			if (err != noErr) {
				fTCPClient->SockError (err);
				break;
			}
	//		if (!len) break;
			else if (len) fTCPClient->RcvPacket (&fPacket, len);
		}
	}
	fThread = 0;
	if (fTCPClient) 
		fTCPClient->Disconnect (0);
}

//____________________________________________________________
void TTCPClient::SetMTU ()
{
/*	if (fRef) {
		int mtu; socklen_t len = sizeof(mtu);
		if (getsockopt (fRef, SOL_IP, IP_MTU, &mtu, &len) != -1) {
			fMTU = mtu;
			return;
		}
	}
*/
	fMTU = kDefaultMTU;
}

//____________________________________________________________
SocketStatus TTCPSocket::Listen (IPAddrPtr from, void *buff, short *len)
{
	socklen_t addr_len;
	int n = recvfrom(fRef, buff, *len, 0, (struct sockaddr *)from, &addr_len);
	if (n == -1) {
		*len = 0;
		return errno;
	}
	*len = n;
	return noErr;
}
