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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> 
#include <time.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <linux/version.h> 

#include "TWANControler.h"
#include "LinuxFeedback.h"
#include "TMidiServer.h"

Boolean doneFlag = false;
#define kGroupTime	10
NetConfig gNet = { -1, -1, -1 };
int clientsIndex, clientsCount = 0;
short maxClients = 32;
Boolean serverMode = false, daemonMode = false;
static char *logFile = "/var/log/msInetDriver.log";

char * 	dateString ();
void	log (char *msg);
//_______________________________________________________________________
static void FatalError (const char *msg) 
{
	char buff[512];
	sprintf (buff, "Cannot launch %s\n", msg);
	log (buff);
	exit (1);
}

//_______________________________________________________________________
static void usage (const char *name) 
{
	fprintf (stderr, "usage : %s [options]\n", name);
	fprintf (stderr, "        options: -p <port number> udp socket port number\n");
	fprintf (stderr, "                 -lat <latency>   maximum latency allowed\n");
	fprintf (stderr, "                 -m <max clients> maximum accepted clients count\n");
	fprintf (stderr, "                                  (TCP server mode only)\n");
	fprintf (stderr, "                 -g <group time>  grouping time parameter\n");
	fprintf (stderr, "                 -d               run as a daemon\n");
	fprintf (stderr, "                 -log <log file>  log file in daemon mode\n");
	fprintf (stderr, "                                  default to %s\n", logFile);
	fprintf (stderr, "                 -s [remote list] run in TCP server mode and \n");
	fprintf (stderr, "                    opens a connection for every host in remote list\n");
	exit (1);
}


//_______________________________________________________________________
static void CheckMidiShare () 
{
	if (!MidiShare ()) {
		FatalError ("MidiShare is not installed");
	}
}

//_______________________________________________________________________
static int checkQuit ()
{
	char c = getc(stdin);
	if ((c == 'q') || (c == 'Q'))
		return 1;
	return 0;
}

//_______________________________________________________________________
static void * IdleThread (void * ptr)
{
	int old;
	TInetControler * inet = (TInetControler *)ptr;
	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, &old);
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &old);
	while (!doneFlag) {
		inet->DoIdle();
		usleep(2000);
	}
	return 0;
}

//_______________________________________________________________________
static pthread_t ThreadCreate (TInetControler * inet)
{
	pthread_t thread;
	int  ret = pthread_create(&thread, NULL, IdleThread, inet);
	if (ret) {
		char str[256];
		sprintf (str, "pthread create failed with err code = %d", ret);
		FatalError (str);		
	}
	return thread;
}

//_______________________________________________________________________
char * dateString ()
{
	static char date[256];
	time_t t;
	time(&t);
	strftime (date, 256, "[%D %T]", localtime(&t));
	return date;
}

//_______________________________________________________________________
void log (char *msg)
{
	if (daemonMode) {
		FILE * fd = fopen (logFile, "a");
		if (fd) {
			fprintf (fd, "%s\n", msg);
			fclose (fd);
		}
	}
	else printf ("%s\n", msg);
}

//_______________________________________________________________________
static Boolean daemon_set ()
{
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,2,13) 
	return true;
#else
	if (daemon (1, 0) == -1) {
		char msg[512];
		perror ("Can't run as a daemon");
		sprintf (msg, "Can't run as a daemon : errno %d", errno);
		fprintf (stderr, "%s\n", msg);
		log (msg);
		return false;
	}
	return true;
#endif
}

//_______________________________________________________________________
static void run (pthread_t thread, NetConfig *net)
{
	void *threadRet; int done = false; char msg[512];
	const char * space = "                    ";

 	sprintf (msg, "%s MidiShare Internet driver is running", dateString());
	log (msg);
	if (serverMode) {
		sprintf (msg, "%sTCP server mode on: %d clients allowed", space, (int)maxClients);
		log (msg);
	}
	sprintf (msg, "%sUDP port: %d maximum latency allowed: %d grouping time: %d", 
		space, (int)net->port, (int)net->maxlat, (int)net->groupTime);
	log(msg);
	if (!daemonMode) printf ("type 'q' to quit\n");
	while (!done) {
		if (daemonMode) sleep (10);
		done = daemonMode ? false : checkQuit();
		usleep (10000);
	}
	pthread_cancel (thread);
	pthread_join (thread, &threadRet);
}

//_______________________________________________________________________
static int getclients (int argc, char *argv[], int i)
{
	for (i; i< argc; i++) {
		char * arg = argv[i];
		if (*arg == '-') break;
		else {
			if (!clientsCount) clientsIndex = i;
			clientsCount++;
		}
	}
	return clientsCount;
}

//_______________________________________________________________________
static void getlogfile (int argc, char *argv[], int i)
{
	if (i < argc) {
		char * arg = argv[i];
		if (*arg != '-') {
			logFile = arg;
			return;
		}
	}
	usage (argv[0]);
}

//_______________________________________________________________________
static long getvalue (int argc, char *argv[], int i)
{
	if (i < argc) {
		long v;
		if (sscanf (argv[i], "%ld", &v) == 1)
			return v;
	}
	usage (argv[0]);
	return -1;
}

//_______________________________________________________________________
static int getopts (int argc, char *argv[])
{
	for (int i=1; i< argc; i++) {
		char c, * arg = argv[i];
		if (*arg++ == '-') {
			c = *arg++;
			if ((c!='l') && *arg) usage (argv[0]);
			switch (c) {
				case 's':	serverMode = true;
					i += getclients (argc, argv, i+1);
					break;
				case 'p':	gNet.port = getvalue (argc, argv, ++i);
					break;
				case 'm':	maxClients = getvalue (argc, argv, ++i);
					break;
				case 'l':
					if (!strcmp (arg, "at"))
						gNet.maxlat = getvalue (argc, argv, ++i);
					else if (!strcmp (arg, "og"))
						getlogfile (argc, argv, ++i);
					else usage (argv[0]);
					break;
				case 'g':	gNet.groupTime = getvalue (argc, argv, ++i);
					break;
				case 'd':	daemonMode = true;
					break;
				default:
					usage (argv[0]);
			}
		}
		else usage (argv[0]);
	}
	return 0;
}

//_______________________________________________________________________
static int checkopt (int argc, char *argv[], char opt)
{
	for (int i=1; i< argc; i++) {
		char * arg = argv[i];
		if (*arg++ == '-') {
			while (*arg) {
				if (*arg++ == opt)
					return i;
			}
		}
	}
	return 0;
}

//_______________________________________________________________________
static void checkNetParams (NetConfig *net, Boolean serverMode)
{
	if (net->port == -1) {
		net->port = serverMode ? kDefaultWANPort : kDefaultLANPort;
	}
	if (net->maxlat == -1) {
		net->maxlat = serverMode ? kDefaultWANLatency : kDefaultLANLatency;
	}
	if (net->groupTime == -1) {
		net->groupTime = serverMode ? kDefaultWANGroupTime : kDefaultLANGroupTime;
	}
}

//_______________________________________________________________________
static void connect (char *argv[], TWANControler *wan)
{
	int i = clientsIndex;
	while (clientsCount--) {
		wan->Connect (argv[i++]);
	}
}

//_______________________________________________________________________
static MidiName InetDriverName (MidiName baseName, short port, short defaultPort)
{
	static char name[256];
	if (port == defaultPort)
		sprintf (name, "%s", baseName);
	else
		sprintf (name, "%s:%d", baseName, (int)port);
	return name;
}

//_______________________________________________________________________
static void CheckName (MidiName name)
{
	char msg[512];
	if (MidiGetNamedAppl (name) > 0) {
		sprintf (msg, "Cannot launch %s: %s is already running", name, name);
		log (msg);
		exit(1);
	}
}

//_______________________________________________________________________
int main (int argc, char *argv[])
{
	pthread_t thread;
	CheckMidiShare();
	getopts (argc, argv);
	checkNetParams (&gNet, serverMode);

	if (daemonMode && !daemon_set()) exit (1);
	TMidiServer server (gNet.port);
	TInetControler * inet = 0;
	LinuxFeedback f;
	
	if (serverMode) {
		MidiName name = InetDriverName (strWANName, gNet.port, kDefaultWANPort);
		CheckName (name);
		TWANControler * ctrl = new TWANControler(&server, &gNet, name, maxClients);
		if (!ctrl) return 1;

		ctrl->SetFeedback (&f);
		if (!ctrl->Start()) return 1;

		inet = ctrl;
		thread = ThreadCreate (inet);
		connect (argv, ctrl);
	}
	else {
		MidiName name = InetDriverName (strLANName, gNet.port, kDefaultLANPort);
		CheckName (name);
		inet = new TInetControler(&gNet, name);
		if (!inet) return 1;
		thread = ThreadCreate (inet);
	}
	run (thread, &gNet);
	delete inet;
	return 0;        
}
