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


/*****************************************************************************/
/* MidiAppl class : provides simpler API for MidiShare application use
/*
/* A MidiAppl object can define :
/*  -  ReceiveAlarm and ApplAlarm methods
/*  -  Schedule MidiTask (look at the MidiTask file)
/* 
/*
/* 12/10/97  Add the "final" keyword, new TaskTable class, MidiThread made final
/* 09/04/98  Change for task management with kIdle and kRunning state
/*           (to avoid scheduling an already scheduled task)
/*****************************************************************************/

package grame.midishare;

import grame.midishare.MidiTask;
import java.util.*;


/**
This class allows a simpler manipulation of MidiShare applications in Java. 
A MidiAppl instance is associated to a MidiShare application, allocates
a thread which is in charge of the management of incoming Midi events, 
application context alarms and tasks.  
*/

public class MidiAppl {

		static final int typeTask 	= 19;
		static final int typeAlarm 	= 20;
		
		static final int kPollingMode 	= 0; // Calling mode : polling
		static final int kNativeMode 	= 1; // Calling mode : direct call of the Java code from the native side


		MidiApplImpl appl = null;	
		int mode;
		
		/**
 		* The MidiShare application reference number associated to the object.
 		*/
	
		public int refnum = -1;
		
		/**
 		* The Midi filter associated to the object.
 		*/

		public int filter = 0;
		
		private native final int  	ApplOpen(int ref,int mode);
		private native final void  	ApplClose(int ref);
		
		
		/**
		The <b>ReceiveAlarm</b> method is automatically called 
		when Midi events are received. The
	 	default method frees all events. 
	 	This method can be redefined in derived classes. 
	 
		*@param event is a pointer to the incoming Midi event. This event must
	 	be used by the method: either re-sended, or desallocated or kept in a data structure.
	 	*/
	
	
		public void ReceiveAlarm(int event){ Midi.FreeEv(event);}
	
		/**
		The <b>ApplAlarm</b> method is automatically 
		called when a MidiShare context switch occurs.
 		The default method does nothing. This method can be redefined in derived classes.   
 
 		*@param code is the context modification code : 0xRRRRMMMM where RRRR is the reference
      	number of the involved application and MMMM the 
      	*@see grame.midishare.Midi  	 
 		*/

		public void ApplAlarm(int code) {}
			
			
		/**
		Schedule a MidiTask object to be executed at a given date.
		Task objects are instances of the MidiTask (or derived) class. 
	
		*@param task is the task object to be scheduled.
		*@param date is the task execution date.
		*@return The result is a boolean to indicated if the task was properly 
		scheduled.
		*@see grame.midishare.MidiTask  	 
		*/
		
		public final boolean ScheduleTask(MidiTask task, int date)
		{
			if (task.isIdle()) {
				return (ScheduleTask(task,date,refnum,mode) != 0);
			}else{
      			return false;
      		}
		}
		
		/**
 		* Schedule a native task.
		*/
		private native int ScheduleTask (MidiTask task, int date, int ref, int mode);
		
		
		/**
 		* Constructor.
		*/
		public MidiAppl() {}
		
		/* WARNING : this method must not :
			- be declared private since MidiNativeAppl could not overwrite it
			- be declared protected otherwise a derived class could overwrite it by error.
		*/
		
		void Init123456789()  // Find an internal name
		{
			mode = kPollingMode;
			appl = new MidiApplPolling();
		}
		
		/**
		The <b>Open</B> method applied on an previously allocated MidiAppl instance, opens the
		corresponding MidiShare application, allocates a new Midi filter, and starts a thread 
		to manage incoming Midi events, application context alarms and tasks.
	       
		*@param name is the name of the MidiShare application.
		*@exception  grame.midishare.MidiException When the MidiShare 
		application cannot be opened or if the Midi filter cannot be allocated.  	 
		*/

		public void Open(String name) throws  MidiException
		{
			int i;
			
			if (Midi.Share() == 0) throw new MidiException ("MidiShare not installed");
			
			if (refnum == -1) {
			
				if ((refnum = Midi.Open(name)) < 0) {
					System.out.println (name);
					System.out.println (refnum);
					throw new MidiException ("Open error");
				}

	    		if ((filter = Midi.NewFilter()) == 0) throw new MidiException ("Filter allocation error");
	    		
	    		// Initialise calling mode
	    		Init123456789(); 
	    		
	    		if (ApplOpen(refnum,mode) == 0) throw new MidiException ("Allocation error");

	     		for (i = 0 ; i<256; i++) {
	                Midi.AcceptPort(filter, i, 1);
	                Midi.AcceptType(filter, i, 1);
	            }
	                    
	            for (i = 0 ; i<16; i++) { Midi.AcceptChan(filter, i, 1); }
	            
	   			Midi.SetFilter(refnum, filter);
	   			
	   			appl.Open(this);
		  	}
		}
		
		
		/**
		The <b>Close</b> method applied on an previously allocated MidiAppl instance, closes the
		corresponding MidiShare application, desallocates the Midi filter, and stops the thread. 
 		*/

		public void Close()
		{
			if (filter != 0){
				Midi.SetFilter(refnum, 0);
				Midi.FreeFilter(filter);
				filter = 0;
			}
				
			if (refnum > 0){
				if (appl != null) appl.Close();
				ApplClose(refnum);
				Midi.Close(refnum);
				refnum = -1; 
			}
		}
		
		
		/**
 		* Midi event loop called by a polling thread or directly from the native side.
		*/
		
		void MidiEventLoop() 
		{
			int ev,n;
			
			// Execute pending tasks
			for (n = Midi.CountDTasks(refnum); n > 0; n--) Midi.Exec1DTask(refnum);
		
			// Handle incoming events
			while ((ev = Midi.GetEv(refnum)) != 0) {
				switch (Midi.GetType(ev))
				{
					case MidiAppl.typeAlarm:
						ApplAlarm(Midi.GetField(ev,0));
					  	Midi.FreeEv(ev);
					  	break;
                 
					default:
					  	ReceiveAlarm(ev);
					  	break;
				}
			}
		}
}


/**
 Internal use
*/
class MidiApplImpl {

		void Open(MidiAppl appl){}
		void Close(){}
}


/**
 Internal use : polling class
*/
final class MidiApplPolling extends MidiApplImpl {

		Thread thread ;
			
		void Open(MidiAppl appl)
		{
			thread = new MidiPollingThread(appl);
			thread.start();
		}
		
		void Close()
		{
			try {
			 	thread.interrupt();
	    	 	thread.join();
	    	}catch (InterruptedException e) {}
		}
							
}

/**
 Internal use : polling thread
*/
final class MidiPollingThread extends Thread {
		private MidiAppl appl;

		MidiPollingThread (MidiAppl appl){
		  	super ("MidiThread");
		  	this.appl = appl;
		  	setPriority(MAX_PRIORITY);
		}

		public void run() {
			while (true) {
				try{
					appl.MidiEventLoop();
					sleep(50);
				}catch (InterruptedException e) {
					return;
				}
			}
		}
 }
 
 
