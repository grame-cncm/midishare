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
import java.io.InputStream;

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

	public void			setStream(String key, InputStream stream) {}
	public InputStream	getStream(String key)	{ return null; }
	public Iterator		getStreamKeys()			{ return null; }
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
