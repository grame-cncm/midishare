/*

  Copyright © Grame 2000

  This program is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
  more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr


*/
 
#ifdef __Windows__
#	include <stdio.h>
#	include <MidiShare.h>
#	define CNAME
#	define CTASKS
	typedef char Boolean;
#endif

#ifdef __Macintosh__
#	include <stdio.h>
#	include "MidiShare.h"
#	define PASCALNAME
#	define PASCALTASKS
#	define unused1
#	define unused2
#	define unused3
#	define unused4
#	define TestMidiCall
#	define flush        fflush(stdout)
#	define print        printf
#endif

#ifdef __Linux__
#ifdef MODULE
# 	ifdef MODVERSIONS
# 		include <linux/modversions.h>
# 	endif
#	include <linux/module.h>
#	include <linux/kernel.h>
#	include "MidiShareKernel.h"
#	define flush
#	define print(args...)	printk(## args)
inline Boolean MidiShare() { return true; }
#else
#	include <stdio.h>
#	include "MidiShare.h"
#	define flush           fflush(stdout)
#	define print(args...)	fprintf(stdout, ## args)
#endif
#	define CNAME
#	define CTASKS
#	define nil 0
#endif

#include "TDrivers.h"

#define true	1
#define false	0

char *OK = "OK", *Erreur= "ERREUR";
short refNum= nil;

#ifdef PASCALNAME
MidiName ApplName   = "\pDrivers";
MidiName NewName    = "\pNewName";
MidiName MSName 	= "\pMidiShare";
DriverName TestName   = "\pTest Driver";
TDriverInfos 	 gDrvInfo1 = { "\pTest Driver1", 100, 0 };
TDriverInfos 	 gDrvInfo2 = { "\pTest Driver2", 100, 0 };
#endif

#ifdef CNAME
MidiName ApplName   = "Drivers";
MidiName NewName    = "NewName";
MidiName MSName  	= "MidiShare";
DriverName TestName   = "Test Driver";
TDriverInfos 	 gDrvInfo1 = { "Test Driver1", 100, 0 };
TDriverInfos 	 gDrvInfo2 = { "Test Driver2", 100, 0 };
#endif

static void wakeup (short refnum);
static void sleep  (short refnum);
static void silentwakeup (short refnum);
static void silentsleep  (short refnum);
static Boolean	slotInfo (SlotRefNum slot, TSlotInfos * infos);

TDriverOperation gDrvOp   	 = { wakeup, sleep, slotInfo };
TDriverOperation gSilentOp   = { silentwakeup, silentsleep, slotInfo };
MidiEvPtr		 gReceived   = 0;

/*____________________________________________________________________*/
#ifdef PASCALTASKS
static pascal void RcvAlarm( short refnum)
#endif
#ifdef CTASKS
void RcvAlarm( short refnum)
#endif
{
	MidiEvPtr e;
	while (e= MidiGetEv(refnum)) {
		RefNum(e) = refnum;
		Link(e) = gReceived;
		gReceived = e;
	}
}

/*____________________________________________________________________*/
static void wakeup (short refnum) 
	{ print ("       wakeup called for ref %d\n", (int)refnum); }
static void silentwakeup (short refnum) { 
	MidiConnect (refnum, 127, true);
	MidiConnect (127, refnum, true);
}

static void sleep (short refnum)
		{ print ("       sleep called for ref %d\n", (int)refnum); }
static void silentsleep (short unused1) { }

#define MaxSlot	4
static Boolean	slotInfo (SlotRefNum slot, TSlotInfos * infos) 
{	
	short i = 0;
	if (Slot(slot) > MaxSlot) return false;
#ifdef PASCALNAME
	infos->name[i++] = 1;
#endif
	infos->name[i++] = '0' + slot;
	infos->name[i] = 0;
	infos->direction = (slot & 1) ? MidiInputSlot : MidiOutputSlot;
	return true;
}

/*____________________________________________________________________*/
int Environment()
{
	int version, count, n, ref;
	
	print ("\nGlobal MidiShare environment :\n");
	print ("    MidiGetVersion : ");flush;
	version= MidiGetVersion();
	print ("%d.%d\n", version/100, version%100);
	if (version < 180) {
		print ("Drivers management not implemented\n");flush;
		return 0;
	}

	print ("    MidiCountAppls : ");flush;
	n= MidiCountAppls();
	print ("%d  %s\n", n, OK);

	print ("    MidiCountDrivers : ");flush;
	count= MidiCountDrivers();
	print ("%d  %s\n", count, OK);

	if (!n) {
		print ("    WakeUp MidiShare...\n");flush;
		ref = MidiOpen (TestName);
		if (ref > 0) {
			print ("    MidiCountDrivers : ");flush;
			n= MidiCountDrivers();
			print ("%d  %s\n", n, OK);
			if (n != 1) {
				print ("Warning : incorrect drivers count\n");
			}
			MidiClose (ref);
		}
		else print ("MidiOpen failed\n");
	}
	
	print ("    MidiRegisterDriver : ");flush;
	ref= MidiRegisterDriver(&gDrvInfo1, &gDrvOp);
	print ("%d  %s\n", ref, OK);
	if (ref > 0) {
		n= MidiCountDrivers();
		if (n != count + 1)
			print ("Warning : incorrect drivers count (%d)\n", n);

		print ("    MidiUnregisterDriver : ");flush;
		MidiUnregisterDriver(ref);
		print ("%s\n", OK);
		n= MidiCountDrivers();
		if (n != count)
			print ("Warning : incorrect drivers count (%d)\n", n);
	}
	else print ("MidiRegisterDriver failed\n");
	return 1;
}

/*____________________________________________________________________*/
void OpenClose()
{
	int i, n, count;
	short r[256];
	
	count = MidiCountDrivers();
	print ("\nMidiRegisterDriver and MidiUnregisterDriver :\n");flush;
	for( i=0; i<256; i++)
	{
		if( (r[i]= MidiRegisterDriver(&gDrvInfo1, &gSilentOp)) < 0)
			break;
	}
	print ("    max number of opened drivers : %d\n", i);
	n= MidiCountDrivers();
	print ("    number returned by MidiShare : %d\n", n);
	for( ; i>=0; i--)
		MidiUnregisterDriver( r[i]);
	n= MidiCountDrivers();
	if( n!= count)
		print ("Warning : incorrect drivers count (%d)\n", n);
}

/*____________________________________________________________________*/
void WakeupSleep()
{
	int r1=0, r2=0, r3=0, r4;
	
	print ("\nWakeup and Sleep operations :\n");flush;
	if (MidiCountAppls ()) {
		print ("Please, close all MidiShare client applications\n");
		print ("to check the wakeup and sleep operations.\n");
	}
	else {
		r1 = MidiRegisterDriver(&gDrvInfo1, &gDrvOp);
		r2 = MidiRegisterDriver(&gDrvInfo2, &gDrvOp);
		print ("    Drivers registered [%d , %d]\n", r1, r2);flush;
		print ("    WakeUp MidiShare...\n");flush;
		r4 = MidiOpen (ApplName);
		if (r4 < 0) 
			print ("MidiOpen failed\n");
		else {
			print ("    more driver...\n"); flush;
			r3 = MidiRegisterDriver(&gDrvInfo1, &gDrvOp);
			print ("    unregister %d\n", r2); flush;
			MidiUnregisterDriver (r2);
			r2 = 0;
			print ("    Sleep...\n");flush;
			MidiClose (r4);
			MidiUnregisterDriver (r3);
			r3 = 0;
			print ("    WakeUp MidiShare...\n");flush;
			MidiOpen (ApplName);
			MidiClose (r4);
		}
	}
	MidiUnregisterDriver (r1);
	MidiUnregisterDriver (r2);
	MidiUnregisterDriver (r3);
}

/*____________________________________________________________________*/
void Infos()
{
	int r1=0, r2=0, r3, ref, n, i;
	MidiName name; char *s;
	TDriverInfos infos;
	
	print ("\nDrivers infos :\n");flush;
	r1 = MidiRegisterDriver(&gDrvInfo1, &gSilentOp);
	r2 = MidiRegisterDriver(&gDrvInfo2, &gSilentOp);
	print ("    Drivers registered [%d , %d]\n", r1, r2);flush;
	r3 = MidiOpen (ApplName);
	if (r3 < 0) 
		print ("MidiOpen failed\n");
	else {
		n = MidiCountDrivers ();
		if (n != 3)
			print ("Warning : incorrect drivers count (%d)\n", n);
		
		print ("    MidiGetIndDriver : \n");flush;
		for (i = 1; i<=n; i++) {
			ref = MidiGetIndDriver(i);
			name = MidiGetName (ref);
#ifdef PASCALNAME
			name[name[0]+1] = 0;
			s = (char *)&name[1];
#else
			s = name;
#endif
			print ("      %d : \"%s\" (%d)\n", i, s, ref);
		}
		
		print ("    MidiGetDriverInfos : \n");flush;
		for (i = 1; i<=n; i++) {
			ref = MidiGetIndDriver(i);
			if (MidiGetDriverInfos (ref, &infos)) {
#ifdef PASCALNAME
				infos.name[infos.name[0]+1] = 0;
				s = (char *)&infos.name[1];
#else
				s = infos.name;
#endif
				print ("      %d : \"%s\" v%d - %d slots\n", i, s, infos.version, infos.slots);
			}
			else print ("Warning : MidiGetDriverInfos returned false for %d\n", ref);
		}
		if (MidiGetDriverInfos (r3, &infos)) {
			print ("Warning : MidiGetDriverInfos returned true for application %d\n", r3);
		}
		MidiClose (r3);
	}
	MidiUnregisterDriver (r1);
	MidiUnregisterDriver (r2);
}

/*____________________________________________________________________*/
void Connections()
{
	int r1=0, r2=0, r3;
	
	print ("\nConnections :\n");flush;
	r1 = MidiRegisterDriver(&gDrvInfo1, &gSilentOp);
	r2 = MidiRegisterDriver(&gDrvInfo2, &gSilentOp);
	print ("    MidiConnect drivers : ");flush;
	MidiConnect (r1, r2, true);
	print ("%s\n", OK);
	if (MidiIsConnected (r1, r2))
		print ("Warning : MidiIsConnected returned true (MidiShare sleeping)\n");
	print ("    WakeUp MidiShare...\n");flush;
	r3 = MidiOpen (ApplName);
	if (r3 < 0) 
		print ("MidiOpen failed\n");
	else {
		print ("    MidiConnect drivers : ");flush;
		MidiConnect (r1, r2, true);
		print ("%s\n", OK);
		if (!MidiIsConnected (r1, r2))
			print ("Warning : MidiIsConnected returned false\n");

		print ("    MidiConnect drivers to appl : ");flush;
		MidiConnect (r1, r3, true);
		print ("%s\n", OK);
		if (MidiIsConnected (r1, r3))
			print ("Warning : MidiIsConnected returned true\n");

		print ("    MidiConnect appl to driver : ");flush;
		MidiConnect (r3, r1, true);
		print ("%s\n", OK);
		if (MidiIsConnected (r3, r1))
			print ("Warning : MidiIsConnected returned true\n");
		MidiClose (r3);
	}
	MidiUnregisterDriver (r1);
	MidiUnregisterDriver (r2);
}

/*____________________________________________________________________*/
void Slots()
{
	int r1=0, i; SlotRefNum sref1, sref2, sr; Boolean ret;
	TDriverInfos infos; TSlotInfos islot; char *s;
	
	print ("\nSlots management :\n");flush;
	r1 = MidiRegisterDriver(&gDrvInfo1, &gSilentOp);
	print ("    MidiAddSlot : ");flush;
	sref1 = MidiAddSlot (r1);
	sref2 = MidiAddSlot (r1);
	print ("(%lx , %lx) %s\n", sref1, sref2, OK);
	if (MidiGetDriverInfos (r1, &infos)) {
		if (infos.slots != 2)
			print ("Warning : MidiGetDriverInfos returned %d slots (instead 2)\n", (int)infos.slots);
	}
	else print ("Warning : MidiGetDriverInfos returned false\n");

	print ("    MidiGetIndSlot : ");flush;
	sr = MidiGetIndSlot (r1, 1);
	print ("%lx %s\n", sr, OK);

	print ("    MidiGetSlotInfos : \n");flush;
	for (i=1; i<=2; i++) {
		sr = MidiGetIndSlot (r1, i);
		if (sr > 0) {
			if (MidiGetSlotInfos (sr, &islot)) {
#ifdef PASCALNAME
				s = (char *)&islot.name[1];
#else
				s = islot.name;
#endif
				print ("      %d : \"%s\" direction: %s slot\n", i, s, 
						(islot.direction == MidiInputSlot) ? "input" : "output");				
			}
			else print ("Warning : MidiGetSlotInfos returned false for %lx\n", sr);
		}
		else print ("Warning : MidiGetIndSlot invalid slot refnum (%lx)\n", sr);
	}
	sr = MidiGetIndSlot (r1, 50);
	if (sr > 0) {
		print ("Warning : MidiGetIndSlot returned refnum %lx for slot 50\n", sr);
	}

	print ("    MidiConnectSlot : ");flush;
	MidiConnectSlot (0, sref1, true);
	print ("%s\n", OK);

	print ("    MidiIsSlotConnected : ");flush;
	ret = MidiIsSlotConnected (0, sref1);
	print ("%s\n", OK);
	if (!ret) print ("Warning : MidiIsSlotConnected return false\n");
	if (MidiGetSlotInfos (sref1, &islot)) {
		if (!IsAcceptedBit(islot.cnx, 0))
			print ("Warning : MidiGetSlotInfos slot not connected\n");
	}
	else print ("Warning : MidiGetSlotInfos returned false for %lx\n", sref1);

	print ("    disconnect...\n");flush;
	MidiConnectSlot (0, sref1, false);
	if (MidiIsSlotConnected (0, sref1))
		print ("Warning : unable to disconnect the slot\n");
	if (MidiGetSlotInfos (sref1, &islot)) {
		if (IsAcceptedBit(islot.cnx, 0))
			print ("Warning : MidiGetSlotInfos slot is connected\n");
	}

	print ("    MidiRemoveSlot : ");flush;
	MidiRemoveSlot (sref1);
	print ("%s\n", OK);
	if (MidiGetDriverInfos (r1, &infos)) {
		if (infos.slots != 1)
			print ("Warning : MidiGetDriverInfos returned %d slots (instead 1)\n", (int)infos.slots);
	}
	else print ("Warning : MidiGetDriverInfos returned false\n");
	if (MidiGetSlotInfos (sref1, &islot)) {
		print ("Warning : MidiGetSlotInfos returned infos for removed slot\n");				
	}
	sr = MidiGetIndSlot (r1, 1);
	if (sr != sref2) print ("Warning : remaining slot is %lx\n", sr);
	MidiUnregisterDriver (r1);
}

/*____________________________________________________________________*/
static void printReceived()
{
	MidiEvPtr next, e = gReceived;
	gReceived = 0;
	while (e) {
		next = Link(e);
		print ("         event type %d received by %d on port %d\n", 
					(int)EvType(e), (int)RefNum(e), (int)Port(e));
		MidiFreeEv(e);
		e = next;
	}
}

/*____________________________________________________________________*/
static void countReceived()
{
	MidiEvPtr e = gReceived;
	long n = 0;
	while (e) {
		n++;
		e = Link(e);
	}
	print ("received events count is %ld\n", n);
	print ("free space is %ld\n", MidiFreeSpace());
}

/*____________________________________________________________________*/
static void send (short port, short ref, Boolean shouldReceive)
{
	long t;
	MidiEvPtr e = MidiNewEv (typeNote);
	if (e) {
		Port(e) = port;
		MidiSendIm (ref, e);
		t= MidiGetTime ();
		while ((MidiGetTime() -t) < 10)
			;
		if (shouldReceive) {
			if (gReceived) printReceived ();
			else print ("Warning : no event received\n");
		} else {
			if (gReceived) {
				print ("Warning : events received\n");
				printReceived ();
			}
		}
	}
	else print ("Cannot allocate a new MidiShare event\n");
}

/*____________________________________________________________________*/
void SendingAndReceiving()
{
	int r1=0, r2=0, r3; SlotRefNum sref1, sref2; long free, n;
	
	print ("\nSending and receiving :\n");flush;
	r1 = MidiRegisterDriver(&gDrvInfo1, &gSilentOp);
	r2 = MidiRegisterDriver(&gDrvInfo2, &gSilentOp);
	r3 = MidiOpen (ApplName);
	if (r3 < 0) print ("MidiOpen failed\n");
	else {
		if (!MidiIsConnected (r1, 127) || !MidiIsConnected (127, r1))
			print ("Warning : driver not connected\n");
		else {
			MidiConnect (r3, 0, true);
			MidiConnect (0, r3, true);
			MidiSetRcvAlarm (r1, RcvAlarm);
			MidiSetRcvAlarm (r3, RcvAlarm);
			sref1 = MidiAddSlot (r1);
			sref2 = MidiAddSlot (r1);
			MidiConnectSlot (0, sref2, true);
			MidiConnectSlot (2, sref1, true);
			print ("    Port 0 connected to %lx\n", sref2);
			print ("    Port 2 connected to %lx\n", sref1);

			free = MidiFreeSpace();
			print ("    Client to driver :\n");			
			print ("       send to port 0 :\n");
			send (0, r3, true);
			print ("       send to port 1 :\n");
			send (1, r3, false);
			print ("       send to port 2 :\n");
			send (2, r3, true);

			print ("    Driver to client :\n");
			print ("       send from slot %d:\n", (int)(sref1 &0xffff));
			send (sref1 &0xffff, r1, true);
			print ("       send from slot %d:\n", (int)(sref2 &0xffff));
			send (sref2 &0xffff, r1, true);
			MidiConnectSlot (0, sref2, false);
			print ("       send from slot %d (disconnected)\n", (int)(sref2 &0xffff));
			send (sref2 &0xffff, r1, false);
			print ("       send from slot %d (don't exist)\n", 50);
			send (50, r1, false);
		}
		n = MidiFreeSpace();
		if (n!=free) 
			print ("Warning : incoherent free space (%ld events loosed)\n", free-n);
		MidiClose (r3);
	}
	MidiUnregisterDriver (r1);
	MidiUnregisterDriver (r2);
}

/*____________________________________________________________________*/
main()
{
	print ("\nMidiShare drivers test.\n");
	print ("================================\n");
	print ("Warning : no client application should be running\n");
	print ("          while testing drivers\n");
	
	if( MidiShare()) {
		if (Environment()) {
			flush;
			OpenClose(); flush;
			WakeupSleep(); flush;
			Infos(); flush;
			Connections(); flush;
			Slots(); flush;
			SendingAndReceiving(); flush;
		}
	}
	else print ("MidiShare is not installed !\n");
	print ("\nEnd of drivers test.\n");
	return 0;
}
