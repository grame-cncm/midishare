// ===========================================================================
// Copyright (c) Grame, Computer Music Research Laboratory 2001, 
// distribution as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


import java.awt.*;
import java.util.*;
import java.applet.Applet;
import java.awt.event.*;
import java.applet.*;

import grame.midishare.Midi;


public class TutorialPart1 extends Applet 
 {
     	TextArea text;
	Button start, go;
	int testnumber ;
	int ourRefNum  = -1;

	 public void init() 
	 {
		testnumber = 0;
		ourRefNum = -1;
	
		Panel bottomPanel = new Panel();
		Panel topPanel = new Panel();
		go = new Button ("Next");

		setLayout(new BorderLayout());
		topPanel.setLayout(new GridLayout(2,1,1,1));

		topPanel.add (new Label ("Tutorial Part1"));

		add("North",topPanel);

		text = new  TextArea(1,1);
		text.appendText( "Welcome to the MidiShare Java tutorial ! ");
    		add("Center",text);

		bottomPanel.setLayout(new GridLayout(1,2,5,5));
		bottomPanel.add (go);
		add("South",bottomPanel);
		 
		
		text.replaceText( "" , 0 , text.getText().indexOf( "\n" ) + 1 ) ;
		text.appendText( "\n\nYou are now ready to follow the tutorial");
		text.appendText( "\nUse NEXT button to execute the next test\n");
		testnumber = 0;
	}
	
	public void stop() 
	{
	 	if (ourRefNum > 0) Midi.Close(ourRefNum);
	 	ourRefNum = -1;
	}
	
	void test1 ()
	{
		text.appendText("\n <TEST 1> : Check if MidiShare is installed");
		text.appendText("\n");
		text.appendText("MidiShare installed : ");
		text.appendText(String.valueOf(Midi.Share()));
		text.appendText("\n");
		text.appendText("1 means that MidiShare is installed\n");
		text.appendText("0 means that MidiShare is not installed");
	}

	void test2 ()
	{
		text.appendText("\n\n<TEST 2> Get MidiShare version\n");
		text.appendText("MidiShare version :  ");
		text.appendText (String.valueOf(Midi.GetVersion()));
	}
	
	void test3 ()
	{
		text.appendText("\n\n<TEST 3> Count MidiShare client applications\n");
		text.appendText("Count of MidiShare client applications : ");
		text.appendText( String.valueOf(Midi.CountAppls()));
	}
	
	void test4 ()
	{
		text.appendText("\n\n<TEST 4> Register a client application\n");
		ourRefNum = Midi.Open("Tutorial");
		if (ourRefNum > 0){
			text.appendText("MidiOpen succeed. Our reference number is : ");
			text.appendText( String.valueOf(ourRefNum));
		}else{
			text.appendText("MidiOpen fail. The error code is : ");
			text.appendText( String.valueOf(ourRefNum));
		}
	}
	
	void test5 ()
	{
		text.appendText("\n\n<TEST 5> Count again MidiShare client applications \n");
		text.appendText("Count of MidiShare client applications : ");
	    	text.appendText(String.valueOf(Midi.CountAppls()));
	}
	
	void listOfAppl ()
	{
		short i;
		int ref,	n = Midi.CountAppls();
	
		text.appendText("List of MidiShare client applications\n");
		text.appendText("-------------------------------------\n");
		for (i = 1; i<= n; i++) {
			ref = Midi.GetIndAppl(i);			// get the refnum from the order number
			text.appendText(String.valueOf(i));
			text.appendText(" : reference number ");
			text.appendText(String.valueOf(ref));
			text.appendText(" name : ");
			text.appendText(Midi.GetName(ref));
			text.appendText("\n");
		}
		text.appendText("-------------------------------------");
	}
	
	
	void test6 ()
	{
		text.appendText("\n\n<TEST 6> List every MidiShare client applications\n");
		listOfAppl();
	}
	
	
	void test7 ()
	{
		text.appendText("\n\n<TEST 7> search the reference number of: ");
		text.appendText("Tutorial");
		text.appendText("\n");
		text.appendText("Reference number of : ");
		text.appendText("Tutorial");
		text.appendText(" : ");
		text.appendText(String.valueOf (Midi.GetNamedAppl("Tutorial")));
	}
	
	void test8 ()
	{
		text.appendText("\n\n<TEST 8> search the reference number of 'XYZ'\n");
		text.appendText("Reference number of  XYZ: ");
		text.appendText(String.valueOf (Midi.GetNamedAppl("XYZ")));
		text.appendText("\n");
		text.appendText("A negative result means that the application was not found.");
	}
	
	void test9 ()
	{
		text.appendText("\n\n<TEST 9> Change the name of client application Tutorial to JavaTutorial\n");
		Midi.SetName (Midi.GetNamedAppl ("Tutorial"), "JavaTutorial");
		listOfAppl();
	}
	
	void test10 ()
	{
		text.appendText("\n\n<TEST 10> Connect JavaTutorial to MidiShare\n");
		Midi.Connect (ourRefNum, 0, 1);
		
	}
	
	void test11 ()
	{
		text.appendText("\n\n<TEST 11> Test if JavaTutorial is connected to MidiShare\n");
		text.appendText("Connection to MidiShare : ");
		text.appendText(String.valueOf(Midi.IsConnected (ourRefNum, 0)));
		text.appendText("\n");
		text.appendText("Connection from MidiShare : ");
		text.appendText(String.valueOf(Midi.IsConnected (0,ourRefNum)));
	}
	
	void listDest (int ref1)
	{
		short i;
		int ref2,	n = Midi.CountAppls();
	
		text.appendText("List of the destinations of ");
		text.appendText(String.valueOf(ref1));
		text.appendText("\n");
		text.appendText("-------------------------------------\n");
		for (i = 1; i<= n; i++) {
		
			ref2 = Midi.GetIndAppl(i);	// get the refnum from the order number
			
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
	
	void test12 ()
	{
		text.appendText("\n\n<TEST 12> List the destinations of an application\n");
		listDest (ourRefNum);
	}
	
	
	void listSrc (int ref1)
	{
		short i;
		int ref2,	n = Midi.CountAppls();
	
		text.appendText("List of the sources of ");
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
		
	
	void test13 ()
	{
		text.appendText("\n\n<TEST 13> List the sources of an application\n");
		listSrc (0);
	}
	
	
	void sendNote (int pitch) 
	{
		int event = Midi.NewEv(Midi.typeNote);  // ask for a new note event
		
		if (event != 0) {				// if the allocation was succesfull
			Midi.SetChan(event,0); 			// set the Midi channel
			Midi.SetPort(event,0); 			// set the destination port
			Midi.SetField(event,0,pitch); 		// set the pitch field
			Midi.SetField(event,1,80); 		// set the velocity field
			Midi.SetField(event,2,1000); 		// set the duration field
			Midi.SendIm(ourRefNum, event);  	// send the note immediatly
		}
	}
	
	void test14 ()
	{	int ref;
		text.appendText("\n\n<TEST 14>Send a note with pitch, velocity and duration\n");
		if ((ref = Midi.GetNamedAppl("msdisplay")) > 0)
			Midi.Connect (ourRefNum,ref, 1);
		sendNote (72); 
	}
	
	void sendMultipleNotes (int n, int pitch, int delay) 
	{
		int event = Midi.NewEv(Midi.typeNote);  	// ask for a new note event
		int date = Midi.GetTime();              	// remember the current time
		
		if (event != 0) {				// if the allocation was succesfull
			Midi.SetChan(event,0); 			// set the Midi channel
			Midi.SetPort(event,0); 			// set the destination port
			Midi.SetField(event,0,pitch); 		// set the pitch field
			Midi.SetField(event,1,80); 		// set the velocity field
			Midi.SetField(event,2,(delay - 1)); 	// set the duration field
			
			for (int i = 0; i< n ; i++) {			// ; loop for the requested number of events
				Midi.SendAt(ourRefNum, Midi.CopyEv(event), (date + (i * delay))); // send a copy of the original note
			}
			Midi.FreeEv (event);  // dispose the original note
		}
	}
	
	void test15 ()
	{
		text.appendText("\n\n<TEST 15>Send multiple notes\n");
		sendMultipleNotes (10, 72, 1000); 
	}
	
	void sendLyric (String str) 
	{
		int event = Midi.NewEv(Midi.typeLyric); 	// ask for a new lyric event
			
		if (event != 0) {				// if the allocation was succesfull
			Midi.SetChan(event,0); 			// set the Midi channel
			Midi.SetPort(event,0); 			// set the destination port
			Midi.SetText(event, str);		// set the text field
			Midi.SendIm(ourRefNum, event);  	// send the event immediatly
		}
	}
	
	void test16 ()
	{
		text.appendText("\n\n<TEST 16>Send 'hello' lyric\n");
		sendLyric("Hello"); 
	}
	
	
	void sendText (int type, String str) 
	{
		int event = Midi.NewEv(type);  			// ask for a event
			
		if (event != 0) {				// if the allocation was succesfull
			Midi.SetChan(event,0); 			// set the Midi channel
			Midi.SetPort(event,0); 			// set the destination port
			Midi.SetText(event, str);		// set the text field
			Midi.SendIm(ourRefNum, event);  	// send the event immediatly
		}
	}
	
	void test17 ()
	{
		text.appendText("\n\n<TEST 17>Send text events\n");
		sendText (Midi.typeText, "Hello");
		sendText (Midi.typeCopyright, "Mozart");
		sendText (Midi.typeSeqName, "Concerto");
		sendText (Midi.typeInstrName ,"Piano");
		sendText (Midi.typeLyric, "Hiiiiiii");
		sendText (Midi.typeMarker, "mark 1");
		sendText (Midi.typeCuePoint, "Reverb here") ;
	}
	
	void test18 ()
	{
		text.appendText("\n\n<TEST 18>Close the JavaTutorial application\n");
		Midi.Close (ourRefNum);
		listOfAppl ();
		//System.Runtime.exit();
	}
	
	
	public boolean action( Event e , Object o) 
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
				case 10 : test10(); break;
				case 11 : test11(); break;
				case 12 : test12(); break;
				case 13 : test13(); break;
				case 14 : test14(); break;
				case 15 : test15(); break;
				case 16 : test16(); break;
				case 17 : test17(); break;
				case 18 : test18(); break;
				}
			return true;
		  }
		
	 	return true;
	}
	
	public static void main(String args[])
	{
		java.util.Hashtable params = new java.util.Hashtable();
		
		// Create the applet.
		TutorialPart1 applet = new TutorialPart1();
	
		// And fire up the applet frame.
		AppletFrame.startApplet(applet, "TutorialPart1", params, 400, 300);
	}

}


// Utility classes

 class AppletFrame extends Frame
	implements WindowListener
{
	//private static final String extraTitle = " - Applet Window";
	private static final String extraTitle = "";
	
	static Applet applet1;
	
	public AppletFrame() {}

	// Support the old calling convention.
	
	public  void startApplet(String className, String title, String[] args) {
	
		try {
			// create an instance of your applet class
			applet1 = (Applet) Class.forName(className).newInstance();
			
		} catch (ClassNotFoundException e) {
			return;
		} catch (InstantiationException e) {
			return;
		} catch (IllegalAccessException e) {
			return;
		}
	
		startApplet(applet1,title,new Hashtable(),200,200);
	}
	
	// This is the real entry point.
	// First, we have the name of the applet class,
	// Second, the title for the window,
	// Next, the Hashtable that contains the
	// parameter -> parameter value mapping.
	// This simulates the <param ...> tag.
	// Finally the width and height of the applet.
	
	public static void startApplet(Applet applet, String title,Hashtable params,int width,int height) {

		applet1 = applet;
		// setup so as getParameter, etc, will work
		OurAppletContext newAppletContext 
			= new OurAppletContext(applet.getToolkit());
		OurAppletStub newAppletStub 
			= new OurAppletStub(newAppletContext,params);
		applet.setStub(newAppletStub);

		// create new application frame window
		AppletFrame f = new AppletFrame(title + extraTitle);
	
		// add applet to frame window
		f.add("Center", applet);

		// add a quit menu item
		MenuBar menubar = new MenuBar();
		Menu file = new Menu("File", true);
		MenuItem item = new MenuItem("Quit");
		menubar.add(file);
		file.add(item);
		f.setMenuBar(menubar);
		item.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// At this point, we simply leave.
				java.lang.Runtime.getRuntime().exit(0);
			} 
		});

		
		// resize frame window to fit applet
		f.pack();
		f.setSize(width,height);
		applet.setSize(width,height);

		// initialize the applet
		applet.init();
		applet.start();

		// show the window
		f.show();

		f.repaint();
	
	}  // end startApplet()


	// constructor needed to pass window title to class Frame
	public AppletFrame(String name) {
		// call java.awt.Frame(String) constructor
		super(name);
		addWindowListener(this);
	}

	public void windowActivated(WindowEvent e) {}
	public void windowClosed(WindowEvent e) {}
	public void windowClosing(WindowEvent e) {
	
	
		// At this point, we simply leave.
		//java.lang.Runtime.getRuntime().exit(0);
		applet1.stop();
		
	}
	public void windowDeactivated(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowOpened(WindowEvent e) {}
}   // end class AppletFrame



class OurAppletContext implements AppletContext {
	private Toolkit ourToolkit;

	OurAppletContext(Toolkit toolkit) {
		ourToolkit = toolkit;
	}

	public Image getImage(java.net.URL url) {
		return ourToolkit.getImage(url);
	}

	// The rest are blank/void for this
	// implementation of our applet viewer.
	
	public AudioClip getAudioClip(java.net.URL url) { 
		// Think of silent movies...
		return new OurAudioClip(url);
	}

	public Applet getApplet(String name) { return null; }
	public Enumeration getApplets() { return null; }
	public void showDocument(java.net.URL url) {}
	public void showDocument(java.net.URL url,String target) {}
	public void showStatus(String status) {}
}


class OurAppletStub implements AppletStub {
	private Hashtable paramTable;
	private AppletContext appletContext;
	private java.net.URL codeBase;
	private java.net.URL documentBase;

	OurAppletStub(AppletContext appContext,Hashtable params) {
		appletContext = appContext;
		paramTable = params;
	}

	public boolean isActive() { return true; }

	public java.net.URL getDocumentBase() { 
		if (documentBase == null) {
			try {
				java.io.File file = new java.io.File("");
				documentBase = new java.net.URL("file",
						"",  // no host 
						file.getAbsolutePath().toString());
			}
			catch (java.net.MalformedURLException e) {
			}
		}
		return documentBase;
	}

	public java.net.URL getCodeBase() { 
		if (codeBase == null) {
			try {
				java.io.File file = new java.io.File("");
				codeBase = new java.net.URL("file",
						"",  // no host 
						file.getAbsolutePath().toString());
			}
			catch (java.net.MalformedURLException e) {
			}
		}
		return codeBase;
	}

	public String getParameter(String str) { 
		return (String) paramTable.get(str.toUpperCase());
	}

	public AppletContext getAppletContext() { 
		return appletContext; 
	}
	public void appletResize(int x,int y) {
		// since the browser would ignore this,
		// we choose to ignore it as well.
	}
}

// Dummy sound class.
class OurAudioClip implements AudioClip {
	public OurAudioClip(java.net.URL url) {}
	public void loop() {}
	public void play() {}
	public void stop() {}
}
