

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
		System.out.println(Midi.GetName(ref) + " application ref: " + ref);
		System.out.println("Current time is: " + Midi.GetTime());
		while (Midi.GetTime() < 2000) ;
		System.out.println("Time to exit: " + Midi.GetTime());
		Midi.Close(ref);		
   }

}