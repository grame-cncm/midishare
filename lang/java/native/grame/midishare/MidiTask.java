/*
  MidiShare Project
  Copyright (C) Grame 1999

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


/*****************************************************************************/
/* MidiTask class : the base MidiTask class, must be derived to implement real tasks
/* the behaviour of the task must be coded in the Execute method,the Forget method 
/* allows to forget a task.
/*****************************************************************************/

package grame.midishare;
import grame.midishare.MidiAppl;

/**
This class is the basic class of Midi tasks. 
Internally, a task can be in <B> kIdle </B> state or <B>kRunning </B> state:
<BR>
- A task in  <B> kIdle </B> state can be scheduled using the <B> ScheduleTask </B>
  method of a MidiAppl instance. It's internal state becomes <B>kRunning </B>.
  A running task can not be scheduled anymore. The <B> ScheduleTask </B> will return false
  if called on a <B>kRunning </B> task.
<BR>
- A task in <B> kRunning </B> state can be forgeted using it's <B> Forget </B>
  method. It's state  becomes <B> kIdle </B> and the task can be scheduled again.
<BR>
- After it's execution (the task <B> Execute </B> method will be called by a MidiAppl instance),
  the task state becomes <B> kIdle </B> and the task can be scheduled again.
<BR>
Real tasks must be defined by deriving the MidiTask class and overloading the Execute method.
*/

public  class MidiTask {

	private long taskptr = 0;
	
	final boolean isIdle() 		{return (taskptr == 0);}
	final boolean isRunning() 	{return (taskptr != 0);}
		
	void setRunning(int taskEv){this.taskptr = taskptr; }
	
	/**
    * Constructor.
    */

	public MidiTask () {}

	/**
	This method is called when the task must be executed.
	It must contain the code associated to the task.
	*@param appl is the task owner, that is the MidiAppl object which has scheduled the task.
	*@param date is the task scheduling date.
	*/
	
	public void Execute (MidiAppl appl , long date){}
 
	/**
	Forgets a previously scheduled task. 
	The internal task state becomes <B> kIdle </B> and the task can be scheduled again.
	*/
	
	public void  Forget() 
	{
		Midi.ForgetTask(taskptr);
		taskptr = 0;
	}
	
}
