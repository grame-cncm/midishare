/*

  Copyright © Grame 2001

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

#include <stdio.h> 
#include <windows.h>
#include <winsock.h> 

#include "TUDPSocket.h"

//____________________________________________________________
TUDPSocket::TUDPSocket (short port) : TSocket (port)
{
	fThread = 0;
	fThreadID = 0;
	fRunFlag = false;;
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
		fRunFlag = false;
		WaitForSingleObject (fThread, 50);
		CloseHandle (fThread);
		fThread = 0;
		fThreadID = 0;
	}
	if (Opened()) {
//		if (fBind) ;
		shutdown (fRef, 0);
		closesocket (fRef);
		fRef = 0;
		fBind = false;
	}
}

//_______________________________________________________________________
static DWORD WINAPI sok_listen (LPVOID ptr)
{
	TUDPSocket *sok = (TUDPSocket *)ptr;
	sok->ThreadLoop ();
	return 0;
}

//____________________________________________________________
void TUDPSocket::ThreadLoop ()
{
	IPAddr from; short len;
	while (fRunFlag) {
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
DWORD TUDPSocket::ThreadCreate ()
{
	fRunFlag = true;
	fThread = CreateThread(NULL, 0, sok_listen, this, 0, &fThreadID);
	if (fThread) {
		SetThreadPriority (fThread, THREAD_PRIORITY_HIGHEST);
	}
	else return GetLastError();
	return noErr;
}

//____________________________________________________________
SocketStatus TUDPSocket::SetOptions (Boolean local)
{
	char ret;
/*	char tos = IPTOS_LOWDELAY + IPTOS_RELIABILITY;
	ret = setsockopt (fRef, SOL_IP, IP_TOS, &tos, sizeof(char));
	ret = 1;
	ret = setsockopt (fRef, SOL_SOCKET, SO_RCVLOWAT, &ret, sizeof(int));
	ret = 1;
	ret = setsockopt (fRef, SOL_SOCKET, SO_SNDLOWAT, &ret, sizeof(int));
*/
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
	SOCKET sok; SocketStatus err; int ret;
	
	if (Opened()) return noErr;
	sok = socket (AF_INET, SOCK_DGRAM, 0);
	if (sok == INVALID_SOCKET) {
		return WSAGetLastError (); 
	}
	fRef = sok;

	struct sockaddr_in in = { 0 };
	in.sin_family = AF_INET; 		/* host byte order */ 
	in.sin_port = htons(fPort); 		/* short, network byte order */ 
	in.sin_addr.s_addr = INADDR_ANY; 	/* auto-fill with my IP */ 
	memset(&(in.sin_zero), 0, 8); 		/* zero the rest of the struct */ 
	ret = bind (fRef, (struct sockaddr *)&in, sizeof(struct sockaddr));
	if (ret == SOCKET_ERROR) return WSAGetLastError ();
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
	memset(&(to->sin_zero), 0, 8); 	/* zero the rest of the struct */ 
	n = sendto (fRef, (const char *)buff, *len, 0, (struct sockaddr *)to, sizeof(struct sockaddr));
	*len = n;
	return (n == SOCKET_ERROR) ? WSAGetLastError () : noErr;
}

//____________________________________________________________
SocketStatus TUDPSocket::Listen (IPAddrPtr from, void *buff, short *len)
{
	int addr_len; int n;
	
	addr_len = sizeof(struct sockaddr); 
	n = recvfrom(fRef, (char *)buff, *len, 0, (struct sockaddr *)from, &addr_len);
	if (n == SOCKET_ERROR) {
		*len = 0;
		return WSAGetLastError ();
	}
	*len = n;
	return noErr;
}
