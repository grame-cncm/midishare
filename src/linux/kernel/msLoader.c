/*

  Copyright © Grame 1999-2005

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU General Public License
  along with this source file; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr
  
  [19-02-01] SL - CallQuitAction removed, use of pthread_cancel in the library
  [22-06-01] SL - The device close function calls mskCloseAll to close all
   		  remaining application associated with a file descriptor
  [06-01-05] YO - Kernel 2.6 module, dynamic major, sysfs support, module_get/put
*/







/*===================================================================================
						
							INCLUDES AND DECLARATIONS

====================================================================================*/





#ifdef MODULE
# ifdef MODVERSIONS
# include <config/modversions.h>
# endif
#define EXPORT_SYMTAB
#include <linux/module.h>
#endif

#include <linux/kernel.h>
#include <linux/fs.h>		
#include <linux/device.h>	

#include "msAppFun.h"
#include "msLoader.h" 
#include "msInit.h"
#include "msConf.h"

/* Module description */

MODULE_AUTHOR("Grame, http://www.grame.fr");
MODULE_DESCRIPTION ("Realtime MIDI module with IAC capabilities");
MODULE_LICENSE("GPL");
MODULE_SUPPORTED_DEVICE("midishare");



/* Global variables */

#define MIDISHARE_CONF "/etc/MidiShare.conf"
#define DEFAULT_MEM  30000

typedef int (*KernelMth) (unsigned long userptr, struct file* f);

static struct class_simple*	gMidiShareClass;			/* for sysfs support*/
static int					gMidiShareMajor;			/* dynamically assigned major number */
static msConf 				gConf;						/* default configuration	*/


static KernelMth 			gKernelMthTable[kMaxMth];





void MidiShareSpecialInit(unsigned long defaultSpace);  /* A REVOIR */






/*===================================================================================
						
							MIDISHARE METHOD TABLE
		
				dispatch table to every midishare function

====================================================================================*/


int MidiReset (unsigned long userptr, struct file * f) 
{
	int n = MSCountAppls(Clients(gMem));
	
	while (n){
	 	MSClose( MSGetIndAppl(n--,Clients(gMem)),gMem);
	}
	
	return 0;
}


static int mskNullMth(unsigned long userptr,struct file * f){ return 0; }


static void initMthTable() {

	int i;
	for (i = 0; i< kMaxMth; i++) { gKernelMthTable[i] = mskNullMth; }
	
	gKernelMthTable[kMidiGetVersion] = mskGetVersion;
	
	gKernelMthTable[kMidiCountAppls] = mskCountAppls;
	gKernelMthTable[kMidiGetIndAppl] = mskGetIndAppl;
	gKernelMthTable[kMidiGetNamedAppl] = mskGetNamedAppl;
	
	gKernelMthTable[kMidiGetSyncInfo] = mskGetSyncInfo;
	gKernelMthTable[kMidiGetExtTime] = mskGetExtTime;
	gKernelMthTable[kMidiInt2ExtTime] = mskInt2ExtTime;
	gKernelMthTable[kMidiExt2IntTime] = mskExt2IntTime;
	gKernelMthTable[kMidiTime2Smpte] = mskTime2Smpte;
	gKernelMthTable[kMidiSmpte2Time] = mskSmpte2Time;
	
	gKernelMthTable[kMidiGetTimeAddr] = mskGetTimeAddr;
	
	gKernelMthTable[kMidiOpen] = mskOpen;
	gKernelMthTable[kMidiClose] = mskClose;
	
	gKernelMthTable[kMidiGetName] = mskGetName;
	gKernelMthTable[kMidiSetName] = mskSetName;
	gKernelMthTable[kMidiGetInfo] = mskGetInfo;
	gKernelMthTable[kMidiSetInfo] = mskSetInfo;
	gKernelMthTable[kMidiGetFilter] = mskGetFilter;
	gKernelMthTable[kMidiSetFilter] = mskSetFilter;
	
	/*
	gKernelMthTable[kMidiGetRcvAlarm] = mskGetRcvAlarm;
	*/
	
	gKernelMthTable[kMidiSetRcvAlarm] = mskSetRcvAlarm;
	
	/*
	gKernelMthTable[kMidiGetApplAlarm] = mskGetApplAlarm;
	*/
	
	gKernelMthTable[kMidiSetApplAlarm] = mskSetApplAlarm;
	
	gKernelMthTable[kMidiConnect] = mskConnect;
	gKernelMthTable[kMidiIsConnected] = mskIsConnected;
	
	gKernelMthTable[kMidiGetPortState] = mskGetPortState;
	gKernelMthTable[kMidiSetPortState] = mskSetPortState;
	
	gKernelMthTable[kMidiGetTime] = mskGetTime;
	
	gKernelMthTable[kMidiSendIm] = mskSendIm;
	gKernelMthTable[kMidiSend] = mskSend;
	gKernelMthTable[kMidiSendAt] = mskSendAt;
	
	gKernelMthTable[kMidiCountEvs] = mskCountEvs;
	gKernelMthTable[kMidiGetEv] = mskGetEv;
	gKernelMthTable[kMidiAvailEv] = mskAvailEv;
	gKernelMthTable[kMidiFlushEvs ] = mskFlushEvs;
	
	gKernelMthTable[kMidiReadSync] = mskReadSync;
	gKernelMthTable[kMidiWriteSync ] = mskWriteSync;
	
	gKernelMthTable[kMidiCall] = mskCall;
	gKernelMthTable[kMidiTask] = mskTask;
	gKernelMthTable[kMidiDTask] = mskDTask;
	gKernelMthTable[kMidiForgetTask] = mskForgetTask;
	
	gKernelMthTable[kMidiCountDTasks ] = mskCountDTasks;
	gKernelMthTable[kMidiFlushDTasks ] = mskFlushDTasks;
	
	/*
	gKernelMthTable[kMidiExec1DTask ] = mskExec1DTask;
	*/
	
	gKernelMthTable[kMidiGetCommand ] = mskGetCommand;
	gKernelMthTable[kMidiGetDTask ] = mskGetDTask;
	
	gKernelMthTable[kMidiNewFilter ] = mskNewFilter;
	gKernelMthTable[kMidiFreeFilter ] = mskFreeFilter;
	
	gKernelMthTable[kMidiAcceptChan ] = mskAcceptChan;
	gKernelMthTable[kMidiAcceptPort ] = mskAcceptPort;
	gKernelMthTable[kMidiAcceptType ] = mskAcceptType;
	
	gKernelMthTable[kMidiIsAcceptedChan ] = mskIsAcceptedChan;
	gKernelMthTable[kMidiIsAcceptedPort ] = mskIsAcceptedPort;
	gKernelMthTable[kMidiIsAcceptedType ] = mskIsAcceptedType;
	
	gKernelMthTable[kMidiReset ] = MidiReset;
	
	/* release 1.80 additionnal methods */
	gKernelMthTable[kMidiRegisterDriver ] = mskRegisterDriver;
	gKernelMthTable[kMidiUnregisterDriver ] = mskUnregisterDriver;
	gKernelMthTable[kMidiCountDrivers ] = mskCountDrivers;
	gKernelMthTable[kMidiGetIndDriver ] = mskGetIndDriver;
	gKernelMthTable[kMidiGetDriverInfos ] = mskGetDriverInfos;
	gKernelMthTable[kMidiAddSlot ] = mskAddSlot;
	gKernelMthTable[kMidiGetIndSlot ] = mskGetIndSlot;
	gKernelMthTable[kMidiRemoveSlot ] = mskRemoveSlot;
	gKernelMthTable[kMidiGetSlotInfos ] = mskGetSlotInfos;
	gKernelMthTable[kMidiConnectSlot ] = mskConnectSlot;
	gKernelMthTable[kMidiIsSlotConnected ] = mskIsSlotConnected;

}




/*===================================================================================
						
							FILE EMULATION OPERATIONS
		
		ioctl is used to access midishare functions. write and read are not used

====================================================================================*/


static int myopen(struct inode *inode, struct file * f)
{
	try_module_get(THIS_MODULE);
	return 0; 		
}



static int myclose(struct inode *inode, struct file * f)
{
	mskCloseAll(f);
	module_put(THIS_MODULE);
	return 0;
}



static ssize_t myread(struct file * f, char * s, size_t n, loff_t * i) 
{ 
	return -EINVAL; 
}



static ssize_t mywrite(struct file * f, const char * s, size_t n, loff_t * i)
{ 
	return -EINVAL; 
}



static int myioctl(struct inode *inode, struct file *f, unsigned int msg, unsigned long param)
{ 
	return (* gKernelMthTable[msg]) (param,f); 
}



struct file_operations myops = {
	read : myread,
	write : mywrite,
	ioctl : myioctl,
	open : myopen,
	release : myclose,
};


/*===================================================================================
						
								MODULE INIT AND CLEANUP 

====================================================================================*/

#if 0

int init_module()
{
	/* initialisations */
	msConf conf;

	/* register a chrdev with a dynamically allocated major number */
	gMidiShareMajor = register_chrdev(0, kMidiShareName, &myops);
	if (gMidiShareMajor < 0) {
		printk("Error registering chrdev when opening MidiShare module");
		return gMidiShareMajor; 
	}

	/* sysfs support (is it correct ?)*/
	gMidiShareClass = class_simple_create(THIS_MODULE, kMidiShareName);
	class_simple_device_add(gMidiShareClass, MKDEV(gMidiShareMajor, 0), NULL, kMidiShareName);
	devfs_mk_cdev(MKDEV(gMidiShareMajor, 0), S_IFCHR | S_IRUGO | S_IWUGO, kMidiShareName);

	
	initMthTable();
	read_conf (MIDISHARE_CONF, &conf);
	MidiShareSpecialInit ((conf.kmem > 0) ? conf.kmem : DEFAULT_MEM);
	
	return 0;
}


void cleanup_module()
{
	if (unregister_chrdev(gMidiShareMajor, kMidiShareName) < 0) printk("Error when closing MidiShare module");
	
	/* YO : added sysfs support (is it correct ?)*/
	devfs_remove(kMidiShareName);
	class_simple_device_remove(MKDEV(gMidiShareMajor, 0));
	class_simple_destroy(gMidiShareClass);
	/*----*/

}

#else


/* nouvelle version simplifiée */


int init_module()
{
	/* register character device and get a dynamically allocated major number */
	gMidiShareMajor = register_chrdev(0, kMidiShareName, &myops);
	if (gMidiShareMajor < 0) {
		printk("Error %d registering chrdev when opening MidiShare module", gMidiShareMajor);
		return gMidiShareMajor; 
	}

	/* add sysfs support with class and device*/
	gMidiShareClass = class_simple_create(THIS_MODULE, kMidiShareName);
	class_simple_device_add(gMidiShareClass, MKDEV(gMidiShareMajor, 0), NULL, kMidiShareName);
	
	/* init the midishare methode table and configuration*/
	initMthTable();
	read_conf (MIDISHARE_CONF, &gConf);
	MidiShareSpecialInit ((gConf.kmem > 0) ? gConf.kmem : DEFAULT_MEM);
	
	return 0;
}


void cleanup_module()
{
	
	/* destroy sysfs device and class*/
	class_simple_device_remove(MKDEV(gMidiShareMajor, 0));
	class_simple_destroy(gMidiShareClass);
	
	/* unregister character device */
	if (unregister_chrdev(gMidiShareMajor, kMidiShareName) < 0) printk("Error closing MidiShare module");

}

#endif


#include "msExports.h"

