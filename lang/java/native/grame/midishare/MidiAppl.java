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
import com.apple.mrj.jdirect.CompletionRoutineNotifier;


/**
This class allows a simpler manipulation of MidiShare applications in Java. 
A MidiAppl instance is associated to a MidiShare application, allocates
a thread which is in charge of the management of incoming Midi events, 
application context alarms and tasks.  
*/


public class MidiAppl{

		static final int typeTask 	= 19;
		static final int typeAlarm 	= 20;
		
		private  Thread thread = null;
		final TaskTable tasktable = new TaskTable();
		
		/**
 		* The MidiShare application reference number associated to the object.
 		*/
	
		public int refnum = -1;
		
		/**
 		* The Midi filter associated to the object.
 		*/

		public int filter = 0;
		
		private native final int  	ApplOpen(int ref);
		private native final void  	ApplClose(int ref);
		
	 	native final int  	GetCommand(int ref);
	
		
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
			
		synchronized final void ExecuteTask(int date, int i)
		{
			MidiTask task = tasktable.removeTask(i);
			if (task != null) {
				task.setIdle();
				task.Execute(this,date);
			}
		}
		
		
		/**
		Schedule a MidiTask object to be executed at a given date.
		Task objects are instances of the MidiTask (or derived) class. 
	
		*@param task is the task object to be scheduled.
		*@param date is the task execution date.
		*@return The result is a boolean to indicated if the task was properly 
		scheduled.
		*@see grame.midishare.MidiTask  	 
		*/

		public synchronized final boolean ScheduleTask(MidiTask task, int date)
		{
			if (task.isIdle()) {
		
				int ev = Midi.NewEv(typeTask);
				int line = tasktable.insertTask(task);
			
				if ((ev != 0) && (line > 0)) {
					Midi.SetField(ev,0,line);
					Midi.SendAt(refnum, ev, date);
					task.setRunning(this,line);
					return true;
				}else {
					if (ev != 0) Midi.FreeEv(ev);
      				return false;
      			}
      		}else{
      			return false;
      		}
		}
		
		
		/**
 		* Constructor.
		*/
		public MidiAppl(){}
	
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
				if (ApplOpen(refnum) == 0) throw new MidiException ("Allocation error");

	     		for (i = 0 ; i<256; i++) {
	                Midi.AcceptPort(filter, i, 1);
	                Midi.AcceptType(filter, i, 1);
	            }
	                    
	            for (i = 0 ; i<16; i++) { Midi.AcceptChan(filter, i, 1); }
	   			Midi.SetFilter(refnum, filter);
				
				/*
				try {
					System.out.println(" TRY MidiThread2");
					thread = new MidiThread2(this); // Thread avec MidiGetCommand (Linux)
					thread.start();
				}catch (Exception e)  {
					System.out.println(e);
				*/
					try {
						thread = new MidiThread1(this); // Thread with aynchronous notification from interrupt (Macintosh)
						thread.start();
					}catch(Throwable ex) {
						thread = new MidiThread(this);
						thread.start();
					}
				//}
				
	    	}
		}
		
		/**
		The <b>Close</b> method applied on an previously allocated MidiAppl instance, closes the
		corresponding MidiShare application, desallocates the Midi filter, and stops the thread. 
 		*/

		public void Close()
		{
			try {
			 	thread.interrupt();
        	 	thread.join();
        	}catch (InterruptedException e) {}
		
			if (filter != 0){
				Midi.SetFilter(refnum, 0);
				Midi.FreeFilter(filter);
				filter = 0;
			}
				
			if (refnum > 0){
				ApplClose(refnum);
				Midi.Close(refnum);
				refnum = -1; 
			}
		}
			
	}




final class MidiThread extends  Thread{
		private MidiAppl appl;

		MidiThread (MidiAppl appl){
		  super ("MidiThread");
		  this.appl = appl;
		  setPriority(MAX_PRIORITY);
		}

		public void run() {
			int ev;

			while (true) {
				while ((ev = Midi.GetEv(appl.refnum)) != 0) {
					switch (Midi.GetType(ev)){

						case MidiAppl.typeTask:
							appl.ExecuteTask(Midi.GetDate(ev), Midi.GetField(ev,0));
							Midi.FreeEv(ev);
							break;

						case MidiAppl.typeAlarm:
							appl.ApplAlarm(Midi.GetField(ev,0));
						  	Midi.FreeEv(ev);
						  	break;
                     
						default:
						  appl.ReceiveAlarm(ev);
						  break;
						}
				}
				try{
					sleep(50);
				}catch (InterruptedException e) {
					System.err.println("Thread QUIT");
					return;
				}
			}
		}
 }
 
 /*
 
 final class MidiThread extends  Thread{
		private MidiAppl appl;
		private int refnum;
	
		MidiThread (MidiAppl appl){
		  super ("MidiThread");
		  this.appl = appl;
		  refnum = appl.refnum;
		  setPriority(MAX_PRIORITY);
		}

		public native void  threadsuspend();
	
		public void run() {
			int ev;

			while (true) {
				System.out.println("OOOOOOOOOOO");
				while(Midi.AvailEv(refnum) == 0 ) {Thread.yield();} ?? marche pas sur MAC
			
				System.out.println("EVENT");
				while ((ev = Midi.GetEv(refnum)) != 0) {
					switch (Midi.GetType(ev)){

						case MidiAppl.typeTask:
							appl.ExecuteTask(Midi.GetDate(ev), Midi.GetField(ev,0));
							Midi.FreeEv(ev);
							break;

						case MidiAppl.typeAlarm:
							appl.ApplAlarm(Midi.GetField(ev,0));
						  	Midi.FreeEv(ev);
						  	break;
                     
						default:
						  appl.ReceiveAlarm(ev);
						  break;
						}
				}
				
				try{
					sleep(50);
				}catch (Exception e) {
					System.out.println("Error: MidiTread stopped");
					System.out.println(e);
				}
				
			}
		}
 }
 */
 
 final class MidiThread1 extends  Thread{
		private MidiAppl appl;
		private IOCompletionUPP ioCompletionUPP;
		
		
		MidiThread1 (MidiAppl appl){
		  super ("MidiThread");
		  this.appl = appl;
		  setPriority(MAX_PRIORITY);
		  // Set up notification routine
		  ioCompletionUPP = new IOCompletionUPP(this);
		  Midi.SetInfo(appl.refnum, ioCompletionUPP.getUPP());
		}

		public synchronized void  run() {
			int ev;

			while (true) {
				try {
					wait();
					while ((ev = Midi.GetEv(appl.refnum)) != 0) {
						switch (Midi.GetType(ev)){

							case MidiAppl.typeTask:
								appl.ExecuteTask(Midi.GetDate(ev), Midi.GetField(ev,0));
								Midi.FreeEv(ev);
								break;

							case MidiAppl.typeAlarm:
								appl.ApplAlarm(Midi.GetField(ev,0));
							  	Midi.FreeEv(ev);
							  	break;
	                     
							default:
							  appl.ReceiveAlarm(ev);
							  break;
							}
					}
					
				} catch (InterruptedException e) {
					//System.err.println("Thread QUIT");
					return;
				}
			}
			
		}
 }
 
 
 
 /* Thread for Linux */
 
  final class MidiThread2 extends  Thread{
		private MidiAppl appl;
		private int refnum;	
		
		MidiThread2 (MidiAppl appl){
		  super ("MidiThread");
		  this.appl = appl;
		  refnum = appl.refnum;
		  setPriority(MAX_PRIORITY);
 		}

		public synchronized void  run() {
			int ev;
		
			while (true) {
			
				System.out.println("RUN ");
				
				while ((ev = appl.GetCommand(refnum)) != 0) {
				
					System.out.println("GetCommand "+ Midi.GetType(ev));
					
					switch (Midi.GetType(ev)){

						case 150:
							appl.ApplAlarm(Midi.GetField(ev,0));
						  	Midi.FreeEv(ev);
						  	break;
                     
						default:
						  while  ((ev = Midi.GetEv(refnum)) != 0) {
						  
						  	switch (Midi.GetType(ev)){
						  
						  		case MidiAppl.typeTask:
									appl.ExecuteTask(Midi.GetDate(ev), Midi.GetField(ev,0));
									Midi.FreeEv(ev);
									break;

						  		default:
						  			appl.ReceiveAlarm(ev);
						  			break;
						  	}
						  }
						  break;
					}
				}
			}
		}
 }
 
 
 
 
 /**
 * An IOCompletionUPP is a CompletionRoutineNotifier suitable for use with async
 * I/O calls, such as PBReadAsync and PBWriteAsync. Typically it is constructed
 * with a reference to an <code>IOParam</code> object, which uses the completion routine
 * from a synchronized method and calls wait.
 */
final class IOCompletionUPP extends CompletionRoutineNotifier {
	private static final int uppIOCompletionProcInfo = 0;

	public IOCompletionUPP(Object monitor) {
		super(uppIOCompletionProcInfo, monitor);
	}
	
	public final int getUPP() {
		return getPointer();
	}
}



/**
Internal class used to manage tasks
*/

final class TaskTable  {
		static final int LENGTH = 256;
		final MidiTask 	tasktable[]  = new MidiTask[LENGTH]; 
		final int 		indicetable[] =  new int[LENGTH];


		TaskTable(){
			for (int i =0; i<LENGTH-1 ;i++) { indicetable[i]= i+1;}
			indicetable[LENGTH-1]= -1;
		}
			
		final int insertTask(MidiTask task) {
			int line = indicetable[0];
		
			if (line > 0) {
				indicetable[0] =  indicetable[line];
      			tasktable[line] = task;
			}
      		return line;
		}
		
		final MidiTask removeTask(int line) {
			indicetable[line] = indicetable[0];
			indicetable[0]= line;
			MidiTask res = tasktable[line];
			tasktable[line] = null;
			return res;
		}
		
		final void clearTask(int line) { tasktable[line] = null;}
		
 }