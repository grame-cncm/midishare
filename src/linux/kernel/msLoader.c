/*

  Copyright © Grame 1999

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
  grame@rd.grame.fr

*/

/*
 * --------------------------------------------------------------------------
 *	Includes for kernel code
 * --------------------------------------------------------------------------
*/


#include <linux/kernel.h>

#ifdef MODULE
# ifdef MODVERSIONS
# include <linux/modversions.h>
# endif
#include <linux/module.h>
#endif

#include <linux/fs.h>		/* for devices	*/
#include <asm/uaccess.h>	/* for put_user	*/
#include <linux/sched.h>	/* for current 	*/
#include <linux/tty.h>		/* for tty_struct */

#include "msAppFun.h"
#include "msLoader.h" 
#include "msInit.h"

/* Global variables */

typedef int (*KernelMth) (unsigned long userptr);

static KernelMth KernelMthTable[kMaxMth];

void MidiShareSpecialInit(unsigned long defaultSpace);  /* A REVOIR */

/*__________________________________________________________________________________*/
/* -- display functions */

void prnt (char* s)
{
	struct tty_struct * dst  = current->tty;
	int	n = 0; 

	while (s[n]) n++;	/* longueur de s */
	if (dst != NULL) {
		(* dst->driver.write)(dst, 0, s, n);
		(* dst->driver.write)(dst, 0, "\015\012", 2);
	}
}

/*__________________________________________________________________________________*/

char* append (char dst[], char src[])
{
	int i = 0, j = 0;
	while (dst[i]) i++;
	while (src[j]) { dst[i++] = src[j++]; }
	dst[i] = 0;
	return dst;	
}

/*__________________________________________________________________________________*/

char* itoa (long n, char s[])
{
	int ii, jj, c, i=0, signe=n;
	if (signe < 0) n = -n;
	
	/* -- production de la chaine à l'envers */
	do {
		s[i++] = n%10 + '0';
	} while ((n /= 10) > 0);
	if (signe < 0) s[i++] = '-';
	s[i]=0;
	
	/* -- inversion de la chaine */
	for (ii=0, jj=i-1; ii<jj; ii++, jj--) {
		c = s[ii];
		s[ii] = s[jj];
		s[jj] = c;
	}
	return s;
}

/*__________________________________________________________________________________*/

int MidiReset (unsigned long userptr) 
{
	int n = MSCountAppls(Clients(gMem));
	
	while (n){
	 	MOD_DEC_USE_COUNT;
	 	MSClose( MSGetIndAppl(n--,Clients(gMem)),gMem);
	}
	
	return 0;
}

/*__________________________________________________________________________________*/

static int myopen(struct inode *inode, struct file * f)
{
	MOD_INC_USE_COUNT;
	return 0; 		/* 0 = OK, tout va bien */
}

/*__________________________________________________________________________________*/

static int myclose(struct inode *inode, struct file * f)
{
	MOD_DEC_USE_COUNT;
	return 0;
}

/*__________________________________________________________________________________*/

static ssize_t myread(struct file * f, char * s, size_t n, loff_t * i)
{
	return 0;
}

/*__________________________________________________________________________________*/

static ssize_t mywrite(struct file * f, const char * s, size_t n, loff_t * i)
{
	return -EINVAL;
}


/*__________________________________________________________________________________*/
/* kernel implementation */

static int mskNullMth(unsigned long userptr)
{
	return 0;
}


/*__________________________________________________________________________________*/
/* Init the ioctl method table */ 

static void initMthTable() {

	int i;
	for (i = 0; i< kMaxMth; i++) { KernelMthTable[i] = mskNullMth; }
	
	KernelMthTable[kMidiGetVersion] = mskGetVersion;
	
	KernelMthTable[kMidiCountAppls] = mskCountAppls;
	KernelMthTable[kMidiGetIndAppl] = mskGetIndAppl;
	KernelMthTable[kMidiGetNamedAppl] = mskGetNamedAppl;
	
	KernelMthTable[kMidiGetSyncInfo] = mskGetSyncInfo;
	KernelMthTable[kMidiGetExtTime] = mskGetExtTime;
	KernelMthTable[kMidiInt2ExtTime] = mskInt2ExtTime;
	KernelMthTable[kMidiExt2IntTime] = mskExt2IntTime;
	KernelMthTable[kMidiTime2Smpte] = mskTime2Smpte;
	KernelMthTable[kMidiSmpte2Time] = mskSmpte2Time;
	
	KernelMthTable[kMidiGetTimeAddr] = mskGetTimeAddr;
	
	KernelMthTable[kMidiOpen] = mskOpen;
	KernelMthTable[kMidiClose] = mskClose;
	
	KernelMthTable[kMidiGetName] = mskGetName;
	KernelMthTable[kMidiSetName] = mskSetName;
	KernelMthTable[kMidiGetInfo] = mskGetInfo;
	KernelMthTable[kMidiSetInfo] = mskSetInfo;
	KernelMthTable[kMidiGetFilter] = mskGetFilter;
	KernelMthTable[kMidiSetFilter] = mskSetFilter;
	
	/*
	KernelMthTable[kMidiGetRcvAlarm] = mskGetRcvAlarm;
	*/
	
	KernelMthTable[kMidiSetRcvAlarm] = mskSetRcvAlarm;
	
	/*
	KernelMthTable[kMidiGetApplAlarm] = mskGetApplAlarm;
	*/
	
	KernelMthTable[kMidiSetApplAlarm] = mskSetApplAlarm;
	
	KernelMthTable[kMidiConnect] = mskConnect;
	KernelMthTable[kMidiIsConnected] = mskIsConnected;
	
	KernelMthTable[kMidiGetPortState] = mskGetPortState;
	KernelMthTable[kMidiSetPortState] = mskSetPortState;
	
	KernelMthTable[kMidiGetTime] = mskGetTime;
	
	KernelMthTable[kMidiSendIm] = mskSendIm;
	KernelMthTable[kMidiSend] = mskSend;
	KernelMthTable[kMidiSendAt] = mskSendAt;
	
	KernelMthTable[kMidiCountEvs] = mskCountEvs;
	KernelMthTable[kMidiGetEv] = mskGetEv;
	KernelMthTable[kMidiAvailEv] = mskAvailEv;
	KernelMthTable[kMidiFlushEvs ] = mskFlushEvs;
	
	KernelMthTable[kMidiReadSync] = mskReadSync;
	KernelMthTable[kMidiWriteSync ] = mskWriteSync;
	
	KernelMthTable[kMidiCall] = mskCall;
	KernelMthTable[kMidiTask] = mskTask;
	KernelMthTable[kMidiDTask] = mskDTask;
	KernelMthTable[kMidiForgetTask] = mskForgetTask;
	
	KernelMthTable[kMidiCountDTasks ] = mskCountDTasks;
	KernelMthTable[kMidiFlushDTasks ] = mskFlushDTasks;
	
	/*
	KernelMthTable[kMidiExec1DTask ] = mskExec1DTask;
	*/
	
	KernelMthTable[kMidiGetCommand ] = mskGetCommand;
	KernelMthTable[kMidiGetDTask ] = mskGetDTask;
	
	KernelMthTable[kMidiNewFilter ] = mskNewFilter;
	KernelMthTable[kMidiFreeFilter ] = mskFreeFilter;
	
	KernelMthTable[kMidiAcceptChan ] = mskAcceptChan;
	KernelMthTable[kMidiAcceptPort ] = mskAcceptPort;
	KernelMthTable[kMidiAcceptType ] = mskAcceptType;
	
	KernelMthTable[kMidiIsAcceptedChan ] = mskIsAcceptedChan;
	KernelMthTable[kMidiIsAcceptedPort ] = mskIsAcceptedPort;
	KernelMthTable[kMidiIsAcceptedType ] = mskIsAcceptedType;
		
	KernelMthTable[kMidiQuit ] = mskQuitAction;
	
	KernelMthTable[kMidiReset ] = MidiReset;
	
}


/*__________________________________________________________________________________*/
/* ioctl dispatch */ 

static int myioctl(struct inode *inode, struct file *f, unsigned int msg, unsigned long param)
{ 
  return (* KernelMthTable[msg]) (param); 
}


struct file_operations myops = {
	NULL,  		/* seek 	*/
	myread,		/* read 	*/
	mywrite,	/* write	*/
	NULL,   	/* readdir 	*/
  	NULL,   	/* select 	*/
  	myioctl,   	/* ioctl	*/
  	NULL,   	/* mmap 	*/
	myopen,		/* open		*/
	NULL,		/* flush 	*/
	myclose		/* close	*/
};

/*__________________________________________________________________________*/

Boolean IsMidiShareRunning (TMSGlobalPtr g){ return (g->local!=0); }

/*__________________________________________________________________________________*/

int init_module()
{
	/* initialisations */
	
	int r = register_chrdev(kMidiShareMajor, kMidiShareName, &myops);
	if (r < 0) {
		prnt("Error when opening MidiShare module");
		return r; 
	}
	
	initMthTable();
	MidiShareSpecialInit (30000);
	
	return 0;
}

/*__________________________________________________________________________________*/

void cleanup_module()
{
	if (IsMidiShareRunning(gMem)) MidiShareSleep(gMem);
	if (unregister_chrdev(kMidiShareMajor, kMidiShareName) < 0) prnt("Error when closing MidiShare module");
}


