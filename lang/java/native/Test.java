

import java.util.ArrayList;
import grame.midishare.Midi;

public class Test {

   public static void main(String[] args) {
		if (Midi.Share()==1) {
			System.out.println("MidiShare Java version " + Midi.GetVersion());
			basictest();
		}
		else
			System.out.println("MidiShare not installed\n");
   }

   public static void basictest() {
		int ref = Midi.Open("test");
		int ref2 = Midi.Open("rcv");
		System.out.println(Midi.GetName(ref) + " application ref: " + ref);
		System.out.println("Current time is: " + Midi.GetTime());
		System.out.println("Free space: " + Midi.FreeSpace());
		Midi.Connect (ref, 0, 1);
		Midi.Connect (ref, ref2, 1);
		System.out.println("Connection state: " + Midi.IsConnected(1,0));

		sendNote (72, ref);
		while (Midi.GetTime() < 500) ;
		long ev = Midi.GetEv (ref2);
		System.out.println("rcv fifo ev: " + ev);
		if (ev != 0) {
			printev (ev);
			Midi.FreeEv (ev);
		}
		
		sendCtrlChge (7, 120, ref);
		while (Midi.GetTime() < 1000) ;		
		ev = Midi.GetEv (ref2);
		System.out.println("rcv fifo ev: " + ev);
		if (ev != 0) {
			printev (ev);
			Midi.FreeEv (ev);
		}

		ev = Midi.GetEv (ref2);
		System.out.println("rcv fifo ev: " + ev);
		if (ev != 0) Midi.FreeEv (ev);
		
		System.out.println("Time to exit: " + Midi.GetTime());
		Midi.Connect (ref, ref2, 0);
		Midi.Close(ref2);		
		Midi.Close(ref);
   }

	static void printev(long e) 
	{
		System.out.println("Event " + e + ":");
		System.out.println("  date   " + Midi.GetDate(e));
		System.out.println("  refnum " + Midi.GetRefnum(e));
		System.out.println("  type   " + Midi.GetType(e));
		System.out.println("  chan   " + Midi.GetChan(e));
		System.out.println("  port   " + Midi.GetPort(e));
		System.out.println("  pitch  " + Midi.GetField(e, 0));
		System.out.println("  vel    " + Midi.GetField(e, 1));
		System.out.println("  dur    " + Midi.GetField(e, 2));
	}

	static void sendNote(int pitch, int ref) 
	{
		long event = Midi.NewEv(Midi.typeNote);  // ask for a new note event	
		if (event != 0) {					// if the allocation was succesfull
			Midi.SetChan(event,1); 			// set the Midi channel
			Midi.SetPort(event,1); 			// set the destination port
			Midi.SetField(event,0,pitch); 	// set the pitch field
			Midi.SetField(event,1,80); 		// set the velocity field
			Midi.SetField(event,2,1000); 	// set the duration field
			Midi.SendIm(ref, event);  		// send the note immediatly
		}
	}

	static void sendCtrlChge(int ctrlnum, int val, int ref) 
	{
		long event = Midi.NewEv(Midi.typeCtrlChange);  // ask for a new ctrl change	
		if (event != 0) {					// if the allocation was succesfull
			Midi.SetChan(event,1); 			// set the Midi channel
			Midi.SetPort(event,1); 			// set the destination port
			Midi.SetField(event,0,ctrlnum); 	// set the control number
			Midi.SetField(event,1,val); 		// set the control value
			Midi.SendIm(ref, event);  		// send the note immediatly
		}
		else System.out.println ("sendCtrlChge: can't allocate a new ev");
	}
}