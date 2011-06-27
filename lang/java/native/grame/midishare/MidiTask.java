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
