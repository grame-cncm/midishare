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
  research@grame.fr

*/

import java.awt.*;
import java.util.*;
import java.applet.Applet;
import grame.midishare.*;

public class TutorialPart2 extends Applet
{
	TextArea text;
	Button go;
	int testnumber ;
	   
	MidiTutorial appl;
	TutorialTask task;

	 public void init() 
	 {
		testnumber = 0;
		
		Panel bottomPanel = new Panel();
		Panel topPanel = new Panel();
		go = new Button ("Next");

		setLayout(new BorderLayout());
		topPanel.setLayout(new GridLayout(2,1,1,1));

		topPanel.add (new Label ("Tutorial Part2"));

		add("North",topPanel);

		text = new  TextArea(1,1);
		text.appendText( "Welcome to the MidiShare Java tutorial part2! ");
		add("Center",text);

		bottomPanel.setLayout(new GridLayout(1,2,5,5));
		bottomPanel.add (go);
		add("South",bottomPanel);

		text.replaceText( "" , 0 , text.getText().indexOf( "\n" ) + 1 ) ;
		text.appendText( "\n\nYou are now ready to follow the tutorial");
		text.appendText( "\nUse NEXT button to execute the next test\n");
		testnumber = 0;
	}
			
	public void stop () 
	{
	 	if (appl!= null) appl.Close();
	  	appl = null;
	}	 
		 
	void listOfAppl ()
	{
		short i;
		int ref, n = Midi.CountAppls();

		text.appendText("\nList of MidiShare client applications\n");
		text.appendText("-------------------------------------\n");
		for (i = 1; i<= n; i++) {
			ref = Midi.GetIndAppl(i);	// get the refnum form the order number
			text.appendText(String.valueOf(i));
			text.appendText(" : reference number ");
			text.appendText(String.valueOf(ref));
			text.appendText(" name : ");
			text.appendText(Midi.GetName(ref));
			text.appendText("\n");
		}
			text.appendText("-------------------------------------");
	}

	void listDest (int ref1)
	{
		short i;
		int ref2,	n = Midi.CountAppls();

		text.appendText("\nList of the destinations of ");
		text.appendText(String.valueOf(ref1));
		text.appendText("\n");
		text.appendText("-------------------------------------\n");
		for (i = 1; i<= n; i++) {
			
			ref2 = Midi.GetIndAppl(i);	// get the refnum form the order number
				
			if (Midi.IsConnected( ref1, ref2) == 1) {
				text.appendText("--> ");
				text.appendText(Midi.GetName(ref2));
				text.appendText(" refnum = " );
				text.appendText(String.valueOf(ref2));
				text.appendText("\n");
			}
		}
		text.appendText("-------------------------------------");
	}
			
	void listSrc (int ref1)
	{
		short i;
		int ref2, n = Midi.CountAppls();
		
		text.appendText("\nList of the sources of ");
		text.appendText(String.valueOf(ref1));
		text.appendText("\n");
		text.appendText("-------------------------------------\n");
		
		for (i = 1; i<= n; i++) {
			
			ref2 = Midi.GetIndAppl(i);	// get the refnum form the order number
				
			if (Midi.IsConnected( ref2, ref1) == 1) {
				text.appendText("<-- ");
				text.appendText(Midi.GetName(ref2));
				text.appendText(" refnum = " );
				text.appendText(String.valueOf(ref2));
				text.appendText("\n");
			}
		}
		text.appendText("-------------------------------------");
	}
		
	void sendText (int refnum, int type, String str) 
	{
		int event = Midi.NewEv(type);  				// ask for a event
				
		if (event != 0) {					// if the allocation was succesfull
			Midi.SetChan(event,0); 				// set the Midi channel
			Midi.SetPort(event,0); 				// set the destination port
			Midi.SetText(event, str);			// set the text field
			Midi.SendIm(refnum, event);  			// send the event immediatly
		}
	}
	 
	void test1 ()
	{
		try {
			text.appendText( "\n <TEST 1> : Allocate and open a new MidiAppl object");
			appl = new MidiTutorial(this);
	 		appl.Open("TutorialPart2");
	 		text.appendText("\n");
	 		listOfAppl();
	 	}catch (MidiException e) {
	 		System.out.println(e);
	 	}
	 }

	void test2 ()
	{
		text.appendText("\n\n<TEST 2>Configure the Midi filter\n");
		appl.configureFilter();
	}

	void test3 ()
	{
		text.appendText("\n\n<TEST 3> Connect to Midi applications\n");
		Midi.Connect(appl.refnum, 0, 1);
		Midi.Connect(0,appl.refnum,1);
		listSrc(appl.refnum);
		listDest(appl.refnum);
	}
		
	void sendNote (int pitch) 
	{
		int event = Midi.NewEv(Midi.typeNote);  // ask for a new ctrlchange event
			
		if (event != 0) {			// if the allocation was succesfull
			Midi.SetChan(event,0); 		// set the Midi channel
			Midi.SetPort(event,0); 		// set the destination port
			Midi.SetField(event,0,pitch); 	// set the pitch field
			Midi.SetField(event,1,80); 	// set the velocity field
			Midi.SetField(event,2,1000); 	// set the duration field
			Midi.SendIm(appl.refnum, event);// send the note immediatly
		}
	}
		
	void sendCtrlChange (int ctrl, int val) 
	{
		int event = Midi.NewEv(Midi.typeCtrlChange);  // ask for a new note event
			
		if (event != 0) {			// if the allocation was succesfull
			Midi.SetChan(event,0); 		// set the Midi channel
			Midi.SetPort(event,0); 		// set the destination port
			Midi.SetField(event,0,ctrl); 	// set the ctrl number field
			Midi.SetField(event,1,val); 	// set the value field
			Midi.SendIm(appl.refnum, event);// send the note immediatly
		}
	}
		
	void sendProgChange (int pgm) 
	{
		int event = Midi.NewEv(Midi.typeProgChange);  // ask for a new progchange event
			
		if (event != 0) {			// if the allocation was succesfull
			Midi.SetChan(event,0); 		// set the Midi channel
			Midi.SetPort(event,0); 		// set the destination port
			Midi.SetField(event,0,pgm); 	// set the prog number field
			Midi.SendIm(appl.refnum, event);// send the note immediatly
		}
	}

	void test4 ()
	{
		int ref;
		text.appendText("\n\n<TEST 4> Send events\n");
		if ((ref = Midi.GetNamedAppl("msdisplay")) > 0)
			Midi.Connect (appl.refnum,ref, 1);
		text.appendText("Send Notes \n");
		text.appendText("Send CtrlChange \n");
		text.appendText("Send ProgChange \n");
		sendNote(60);
		sendNote(64);
		sendNote(67);
		sendCtrlChange(7,120);
		sendCtrlChange(10,120);
		sendProgChange(2);
	}
		
	void test5 ()
	{		
		int ref1, ref2;
		text.appendText("\n\n<TEST 5>Delay and transpose incoming notes\n");
		ref1 = Midi.GetNamedAppl("mskeyboard");
		ref2 = Midi.GetNamedAppl("msdisplay");
			
		if (ref1 > 0)  Midi.Connect (ref1,appl.refnum, 1);
		if (ref2 > 0)  Midi.Connect (ref1,ref2, 1);
	}

	void test6 ()
	{
		text.appendText("\n\n<TEST 6> Install a Appl alarm \n");
		int  ref = Midi.Open("foo");
		Midi.Connect(ref, 0, 1);
		Midi.SetName(ref, "foo1");
		Midi.Close(ref);
	}

	void test7 ()
	{
		text.appendText("\n\n<TEST 7>Define and schedule a new Midi task object ");
		text.appendText("\n");
		task = new TutorialTask();
		appl.ScheduleTask(task, Midi.GetTime());
	}
		
	void test8 ()
	{
		text.appendText("\n\n<TEST 8>Forget the task");
		task.Forget();
	}
		
	void test9 ()
	{
		text.appendText("\n\n<TEST 8>Close the MidiAppl instance");
		appl.Close();
		appl = null;
		listOfAppl ();
	}
					
	public boolean action(Event e , Object o) 
	{
		if (e.target == go) {
			testnumber++;
			switch (testnumber){
				case 1 : test1(); break;
				case 2 : test2(); break;
				case 3 : test3(); break;
				case 4 : test4(); break;
				case 5 : test5(); break;
				case 6 : test6(); break;
				case 7 : test7(); break;
				case 8 : test8(); break;
				case 9 : test9(); break;
			}
			return true;
		}
		return true;
	}
	 	 
	public static void main(String args[])
	{
		java.util.Hashtable params = new java.util.Hashtable();
		
		// Create the applet.
		TutorialPart2 applet = new TutorialPart2();
	
		// And fire up the applet frame.
		AppletFrame.startApplet(applet, "TutorialPart2", params, 400, 300);
	}
}

class MidiTutorial extends MidiAppl 
{
	TutorialPart2 myUser;
	
	public MidiTutorial (TutorialPart2 user)
	{
		myUser = user;
	}
	
	public void configureFilter () 
 	{
		int i;
		
		for (i = 0 ; i<256; i++) {
			Midi.AcceptPort(filter, i, 1);
			Midi.AcceptType(filter, i, 1);
		}
			
		for (i = 0 ; i<16; i++) {
			Midi.AcceptChan(filter, i, 1);
		}
			
		Midi.AcceptType(filter, Midi.typeActiveSens, 0);
		Midi.AcceptType(filter, Midi.typeSysEx, 0);
	}
	
	public void ReceiveAlarm (int event) 
	{
		switch (Midi.GetType (event)) {
		
			case Midi.typeNote:    // for Key event
			case Midi.typeKeyOn:
			case Midi.typeKeyOff:
				Midi.SetField(event, 0, Midi.GetField(event,0) + 12);// change the pitch field
				Midi.SetDate(event, Midi.GetDate(event) + 1000);     // change the date field
				Midi.Send(refnum, event);							 // re-send the event
				break;
				
			default:
				Midi.FreeEv(event);		// otherwise dispose the event
				break;
		}
	}
		
	public void ApplAlarm (int code) 
	{
		int param = code & 0x0000FFFF ;
		int ref = (code & 0xFFFF0000) >> 16 ;

		if (myUser.testnumber == 6) {
		    switch (param){
		      case Midi.OpenAppl :
		      	myUser.text.appendText(" A new application has opened \n");
		        break ;
		      case Midi.CloseAppl :
		        myUser.text.appendText(" An application has closed \n");
		        break ;
		      case Midi.ChgConnect :
		        myUser.text.appendText(" A connection has changed \n");
		        break ;
		      case Midi.ChgName :
		        myUser.text.appendText(" A name has changed \n");
		        break ;
		    }
		}
	}
}

class TutorialTask extends MidiTask {

	public void Execute (MidiAppl obj, int date)
	{
		MidiTutorial appl = (MidiTutorial) obj;
		
		appl.myUser.text.appendText("Execution of the task at date: ");
		appl.myUser.text.appendText(String.valueOf(date));
		appl.myUser.text.appendText("\n");
		appl.myUser.sendText(appl.refnum, Midi.typeText, "Hello from tutorial");
		appl.ScheduleTask(this, date + 3000);
	}
} 
