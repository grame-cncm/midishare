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
#include <pwd.h> 
#include <string.h> 
#include <signal.h> 
#include <stdio.h> 
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> 

#include "TUDPSocket.h"

//____________________________________________________________
TUDPSocket::TUDPSocket (short port) : TSocket (port)
{
	fThread = 0;
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
	if (fThread) {
		void *threadRet;
		pthread_cancel (fThread);
		pthread_join (fThread, &threadRet);
		fThread = 0;
	}
	if (Opened()) {
		if (fBind) ;
		close (fRef);
		fRef = 0;
		fBind = false;
	}
}

//_______________________________________________________________________
static void QuitHandler (int sigNum)
{
	pthread_exit (0);	
}

//_______________________________________________________________________
static void * sok_listen (void * ptr)
{
	int old;
	TUDPSocket *sok = (TUDPSocket *)ptr;
	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, &old);
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &old);
	sok->ThreadLoop ();
}

//____________________________________________________________
void TUDPSocket::ThreadLoop ()
{
	IPAddr from; short len;
	while (true) {
		len = sizeof (fPacket);
		SocketStatus err = Listen (&from, &fPacket, &len);
		if (!fClient) continue;
		if (err == noErr) {
			fClient->RcvPacket (&from, &fPacket, len);
		}
		else fClient->SockError (err);
	}
}

//____________________________________________________________
int TUDPSocket::ThreadCreate ()
{
	int  ret = pthread_create(&fThread, NULL, sok_listen, this);
	if (!ret) {
 		struct passwd *ruid;
		struct sched_param param;
		uid_t uid = getuid ();
		param.sched_priority = 99; /* 0 to 99  */
		ruid = getpwnam ("root");
		if (ruid) {
			setuid (ruid->pw_uid); 
   			pthread_setschedparam(fThread, SCHED_RR,  &param); 
 			setuid (uid);
		}
	}
	return ret;	
}

//____________________________________________________________
SocketStatus TUDPSocket::SetOptions (Boolean local)
{
	char tos = IPTOS_LOWDELAY + IPTOS_RELIABILITY;
	int ret = setsockopt (fRef, SOL_IP, IP_TOS, &tos, sizeof(char));
	ret = 1;
	ret = setsockopt (fRef, SOL_SOCKET, SO_RCVLOWAT, &ret, sizeof(int));
	ret = 1;
	ret = setsockopt (fRef, SOL_SOCKET, SO_SNDLOWAT, &ret, sizeof(int));
	if (local) {
		ret = 1;
		ret = setsockopt (fRef, SOL_SOCKET, SO_DONTROUTE, &ret, sizeof(int));
		ret = 1;
		return setsockopt (fRef, SOL_SOCKET, SO_BROADCAST, &ret, sizeof(int));
	}
	return noErr;
}

//____________________________________________________________
SocketStatus TUDPSocket::Open (Boolean local)
{
	int ret; SocketStatus err;
	
	if (Opened()) return noErr;
	ret = socket (AF_INET, SOCK_DGRAM, 0);
	if (ret == -1) return errno; 

	fRef = ret;

	struct sockaddr_in in = { 0 };
	in.sin_family = AF_INET; 		/* host byte order */ 
	in.sin_port = htons(fPort); 		/* short, network byte order */ 
	in.sin_addr.s_addr = INADDR_ANY; 	/* auto-fill with my IP */ 
	bzero(&(in.sin_zero), 8); 		/* zero the rest of the struct */ 
	ret = bind (fRef, (struct sockaddr *)&in, sizeof(struct sockaddr));
	if (ret == -1) return errno;
	fBind = true;
	err = SetOptions (local);
	if (err != noErr) return err;
	return ThreadCreate ();
}

//____________________________________________________________
SocketStatus TUDPSocket::Send (IPAddrPtr to, void *buff, short *len)
{
	int n;
	
	to->sin_port = htons(fPort); 	/* short, network byte order */ 
	bzero(&(to->sin_zero), 8); 	/* zero the rest of the struct */ 
	n = sendto (fRef, buff, *len, 0, (struct sockaddr *)to, sizeof(struct sockaddr));
	*len = n;
	return (n == -1) ? -1 : noErr;
}

//____________________________________________________________
SocketStatus TUDPSocket::Listen (IPAddrPtr from, void *buff, short *len)
{
	socklen_t addr_len; int n;
	
	addr_len = sizeof(struct sockaddr); 
	n = recvfrom(fRef, buff, *len, 0, (struct sockaddr *)from, &addr_len);
	if (n == -1) {
		*len = 0;
		return errno;
	}
	*len = n;
	return noErr;
}
