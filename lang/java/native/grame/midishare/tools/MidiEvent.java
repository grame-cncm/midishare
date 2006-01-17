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

package grame.midishare.tools;

import grame.midishare.Midi;
import java.util.*;
import java.lang.*;

/**
A class which contains functions to manipulate MidiShare events. 
*/

public final class MidiEvent {

	protected int event = 0;

	final private static String[] eventType = new String[ 256 ];
	final private static int[] dataLength = new int[ 256 ]  ;
	final private static  int TEXT = -1 ;
	final private static  Hashtable typeTable = new Hashtable(256);
	
	public MidiEvent(int event) { this.event = event;}
	
	static {
	
		typeTable.put("Note",new Integer(0));
		typeTable.put("KeyOn",new Integer(1));
		typeTable.put("KeyOff",new Integer(2));
		typeTable.put("KeyPress",new Integer(3));
		typeTable.put("CtrlCh",new Integer(4));
		typeTable.put("ProgCh",new Integer(5));
		typeTable.put("ChanPress",new Integer(6));
		typeTable.put("PitchWheel",new Integer(7));
		typeTable.put("SongPos",new Integer(8));
		typeTable.put("SongSel",new Integer(9));
		typeTable.put("Clock",new Integer(10));
		typeTable.put("Start",new Integer(11));
		typeTable.put("Continue",new Integer(12));
		typeTable.put("Stop",new Integer(13));
		typeTable.put("Tune",new Integer(14));
		typeTable.put("ActiveSens",new Integer(15));
		typeTable.put("Reset",new Integer(16));
		typeTable.put("SysEx",new Integer(17));
		typeTable.put("Stream",new Integer(18));
		
	 	 for ( int i = 19 ; i < 128 ; i++ ) {
	     	typeTable.put("Private",new Integer(i));
	     }
	     
	    typeTable.put("Process",new Integer(128)) ;
	    typeTable.put("DProcess",new Integer(129)) ;
	    typeTable.put("QFrame",new Integer(130) );
	    typeTable.put("Ctrl14b",new Integer(131)) ;
	    typeTable.put("NonRegParam",new Integer(132)) ;
	    typeTable.put("RegParam",new Integer(133)) ;
	    typeTable.put("SeqNum",new Integer(134) );
	    typeTable.put("Text",new Integer(135) );
	    typeTable.put("Copyright",new Integer(136) );
	    typeTable.put("SeqName",new Integer(137)) ;
	    typeTable.put("InstrName",new Integer(138) );
	    typeTable.put("Lyric",new Integer(139)) ;
	    typeTable.put("Marker",new Integer(140) );
	    typeTable.put("CuePoint",new Integer(141)) ;
	    typeTable.put("ChanPrefix",new Integer(142)) ;
	    typeTable.put("EndTrack",new Integer(143)) ;
	    typeTable.put("Tempo",new Integer(144)) ;
	    typeTable.put("SMPTEOffset",new Integer(145)) ;
	    typeTable.put("TimeSign",new Integer(146) );
	    typeTable.put("KeySign",new Integer(147)) ;
	    typeTable.put("Specific",new Integer(48) );	
		
		for ( int i = 149 ; i < 255 ; i++ ) {
	     typeTable.put("Reserved" , new Integer(i));
	    }
	   
	    eventType[ 0 ] = "Note" ;
	    eventType[ 1 ] = "KeyOn" ;
	    eventType[ 2 ] = "KeyOff" ;
	    eventType[ 3 ] = "KeyPress" ;
	    eventType[ 4 ] = "CtrlCh" ;
	    eventType[ 5 ] = "ProgCh" ;
	    eventType[ 6 ] = "ChanPress" ;
	    eventType[ 7 ] = "PitchWheel" ;
	    eventType[ 8 ] = "SongPos" ;
	    eventType[ 9 ] = "SongSel" ;
	    eventType[ 10 ] = "Clock" ;
	    eventType[ 11 ] = "Start" ;
	    eventType[ 12 ] = "Continue" ;
	    eventType[ 13 ] = "Stop" ;
	    eventType[ 14 ] = "Tune" ;
	    eventType[ 15 ] = "ActiveSens" ;
	    eventType[ 16 ] = "Reset" ;
	    eventType[ 17 ] = "SysEx" ;
	    eventType[ 18 ] = "Stream" ;
	    
	    for ( int i = 19 ; i < 128 ; i++ )
	      eventType[ i ] = "Private" ;
	      
	    eventType[ 128 ] = "Process" ;
	    eventType[ 129 ] = "DProcess" ;
	    eventType[ 130 ] = "QFrame" ;
	    eventType[ 131 ] = "Ctrl14b" ;
	    eventType[ 132 ] = "NonRegParam" ;
	    eventType[ 133 ] = "RegParam" ;
	    eventType[ 134 ] = "SeqNum" ;
	    eventType[ 135 ] = "Text" ;
	    eventType[ 136 ] = "Copyright" ;
	    eventType[ 137 ] = "SeqName" ;
	    eventType[ 138 ] = "InstrName" ;
	    eventType[ 139 ] = "Lyric" ;
	    eventType[ 140 ] = "Marker" ;
	    eventType[ 141 ] = "CuePoint" ;
	    eventType[ 142 ] = "ChanPrefix" ;
	    eventType[ 143 ] = "EndTrack" ;
	    eventType[ 144 ] = "Tempo" ;
	    eventType[ 145 ] = "SMPTEOffset" ;
	    eventType[ 146 ] = "TimeSign" ;
	    eventType[ 147 ] = "KeySign" ;
	    eventType[ 148 ] = "Specific" ;
			
	    for ( int i = 149 ; i < 255 ; i++ )
	      eventType[ i ] = "Reserved" ;
	      
	    dataLength[ 0 ] = 3 ;
	    dataLength[ 1 ] = 2 ;
	    dataLength[ 2 ] = 2 ;
	    dataLength[ 3 ] = 2 ;
	    dataLength[ 4 ] = 2 ;
	    dataLength[ 5 ] = 1 ;
	    dataLength[ 6 ] = 1 ;
	    dataLength[ 7 ] = 2 ;
	    dataLength[ 8 ] = 2 ;
	    dataLength[ 9 ] = 1 ;
	    dataLength[ 10 ] = 0 ;
	    dataLength[ 11 ] = 0 ;
	    dataLength[ 12 ] = 0 ;
	    dataLength[ 13 ] = 0 ;
	    dataLength[ 14 ] = 0 ;
	    dataLength[ 15 ] = 0 ;
	    dataLength[ 16 ] = 0 ;
	    dataLength[ 17 ] = 3 ;
	    dataLength[ 18 ] = 3 ;
	    
	    for ( int i = 19 ; i < 255 ; i++ )
	      dataLength[ i ] = 0 ;
	    
	    dataLength[ 130 ] = 2 ;
	    dataLength[ 131] = 2 ;
	    dataLength[ 132] = 2 ;
	    dataLength[ 133] = 2 ;
	    dataLength[ 134] = 1 ;

	    for ( int i = 135 ; i < 142 ; i++ )
	      dataLength[ i ] = TEXT ;
	    
	    dataLength[ 142 ] = 1 ;  
	    dataLength[ 144 ] = 1 ;
	    dataLength[ 145 ] = 4 ;
	    dataLength[ 146 ] = 4 ;
	    dataLength[ 147 ] = 2 ;
	    dataLength[ 148 ] = 3 ;
  }
  
  	/**
	Parse a string representation of a date into its millisecond value. The string format is
	"00:00:00:000".

	*@param s is the string representation of the date.
	*@return The result is the date in millisecond.
	*/

	public static final int stringToDate (String s) {
		StringTokenizer  st = new StringTokenizer (s, ":");
		int hrs = Integer.parseInt(st.nextToken());
		int min = Integer.parseInt(st.nextToken());
		int sec = Integer.parseInt(st.nextToken());
		int ms = Integer.parseInt(st.nextToken());
		return hrs*3600000 + min *60000+sec*1000+ms;
	}
	
	/**
	 Create a  string representation of a date given in millisecond.

	*@param date is the date in millisecond.
	*@return The result is the string representation of the date.
	*/

	public static final String dateToString (int date)
	{
		StringBuffer sb = new StringBuffer();
		
		int d1 = date/3600000;
		
		if (d1< 100)
      	sb.append( '0') ;
    	if (d1 < 10)
      	sb.append( '0' ) ;
    	sb.append(d1) ;
    	sb.append( ':' ) ;
    
    	d1 = (date % 3600000) / 60000;
    
    	if (d1< 10) sb.append( '0' ) ;
    	sb.append(d1) ;
    	sb.append( ':'  ) ;
    
    	d1 = (date % 60000) / 1000;
    
    	if (d1 < 10) sb.append( '0' ) ;
    	sb.append(d1 ) ;
    	sb.append( ':'  ) ;
    
    	d1 =(date % 1000);
    
    	if (d1 < 100) sb.append( '0') ;
    	if (d1 < 10) sb.append( '0' ) ;
    	sb.append(d1 ) ;
    	
    	return new String(sb);
	}
			
	public final String toString() {return event != 0 ? toString(event) : "";}
	
	/**
	Creates a string representation of the event argument. The date, common
	fields (type, channel,port) and  specific fields depending of the event type
	are converted to a string.

	*@param event is the event to be converted.
	*@return The result is string representation of the event.
	*/
		
	public  static final String toString(int event) {
	    StringBuffer sb = new StringBuffer();
	     
	  	sb.append(dateToString(Midi.GetDate(event)));
	    
	    while (sb.length() < 16) sb.append(' ');
	     
	    sb.append(Midi.GetPort(event) );
	    sb.append('/') ;
	    sb.append(Midi.GetChan(event)+1) ;
	    
	   	while (sb.length() < 21) sb.append(' ');
	    
	    int type = Midi.GetType(event);
	    sb.append(eventType[type]) ;
	   
	    while (sb.length() < 34) sb.append(' ');
	     
	    if ( dataLength[type] == TEXT) {
	     	String str = Midi.GetText(event);
	     	sb.append(str.substring(0, Math.min(str.length(), 10))) ;
	    } else {
	        for (int i = 0 ; i < dataLength[type] ; i++) {
	        	while (sb.length() < (34 +  i * 4)) sb.append(' ');
	        	 sb.append(Midi.GetField(event,i)) ;
			  }
	    }
	  	return new String(sb);
	}


 	/**
	Parse a string representation of an event into this event. The string format is
	"00:00:00:000".

	*@param s is the string representation of the event to be parsed.
	*@return The result is a newly constructed event which fields are initialized 
	to the value extracted from the string argument. 
	*/
	
public static final int valueOf(String s) {
	StringTokenizer  st = new StringTokenizer (s);
	int i = 0;
       
    int date = stringToDate(st.nextToken());
    
    StringTokenizer  st1 = new StringTokenizer (st.nextToken(),"/");
 	int port = Integer.parseInt(st1.nextToken());
 	int chan = Integer.parseInt(st1.nextToken());
 	
    int type = ((Integer)typeTable.get(st.nextToken())).intValue();
     
    int event = Midi.NewEv(type); 
     
    if (event != 0) {
     	Midi.SetDate(event, date);
     	Midi.SetPort(event, port);
     	Midi.SetChan(event, chan-1);
  	
  		if (dataLength[type] == TEXT) {
  			Midi.SetText(event, st.nextToken());
  		} else {
  			while (st.hasMoreTokens()) {
  				Midi.SetField(event , i, Integer.parseInt(st.nextToken()));
  				i++;
  			}
  		}
     }
     return event;
  }
  
   /**
  
   Creates a Note event and returns a pointer to the event or 0 if there is no
   more memory space. 
       
   */
  
  public static int Note (int pitch, int vel, int dur,int chan,int port) {
  		int note = Midi.NewEv(Midi.typeNote);
  		
  		if (note != 0) {
  			Midi.SetField(note,0,pitch);
  			Midi.SetField(note,1,vel);
  			Midi.SetField(note,2,dur);
  			Midi.SetChan(note,chan);
  			Midi.SetPort(note,port);
   		}
		return note;
  }
  
   /**
  
   Creates a KeyOn event and returns a pointer to the event or 0 if there is no
   more memory space. 
       
   */
  
  public static int KeyOn (int pitch, int vel, int chan,int port) {
  		int key = Midi.NewEv(Midi.typeKeyOn);
  		
  		if (key != 0) {
  			Midi.SetField(key,0,pitch);
  			Midi.SetField(key,1,vel);
  			Midi.SetChan(key,chan);
  			Midi.SetPort(key,port);
   		}
		return key;
  }
  
   /**
  
   Creates a KeyOff event and returns a pointer to the event or 0 if there is no
   more memory space. 
       
   */
  
  public static int KeyOff (int pitch, int vel, int chan,int port) {
  		int key = Midi.NewEv(Midi.typeKeyOff);
  		
  		if (key != 0) {
  			Midi.SetField(key,0,pitch);
  			Midi.SetField(key,1,vel);
  			Midi.SetChan(key,chan);
  			Midi.SetPort(key,port);
   		}
		return key;
  }
  
   /**
  
   Creates a KeyPress event and returns a pointer to the event or 0 if there is no
   more memory space. 
       
   */
  
  public static int KeyPress (int ctrl, int val, int chan,int port) {
  		int key = Midi.NewEv(Midi.typeKeyPress);
  		
  		if (key != 0) {
  			Midi.SetField(key,0,ctrl);
  			Midi.SetField(key,1,val);
  			Midi.SetChan(key,chan);
  			Midi.SetPort(key,port);
   		}
		return key;
  }
  
   /**
  
   Creates a CtrlChange event and returns a pointer to the event or 0 if there is no
   more memory space. 
       
   */
  
	public static int CtrlChange (int ctrl, int val, int chan,int port) {
  		int key = Midi.NewEv(Midi.typeCtrlChange);
  		
  		if (key != 0) {
  			Midi.SetField(key,0,ctrl);
  			Midi.SetField(key,1,val);
  			Midi.SetChan(key,chan);
  			Midi.SetPort(key,port);
   		}
		return key;
  }
  
  /**
  
  Creates a ProgChange event and returns a pointer to the event or 0 if there is no
  more memory space. 
       
  */
  
  public static int ProgChange (int pgm, int chan,int port) {
  		int key = Midi.NewEv(Midi.typeProgChange);
  		
  		if (key != 0) {
  			Midi.SetField(key,0,pgm);
  			Midi.SetChan(key,chan);
  			Midi.SetPort(key,port);
   		}
		return key;
  }
  
   /**
  
   Creates a ChanPress event and returns a pointer to the event or 0 if there is no
   more memory space. 
       
   */
   
   public static int ChanPress (int ch, int chan,int port) {
  		int key = Midi.NewEv(Midi.typeChanPress);
  		
  		if (key != 0) {
  			Midi.SetField(key,0,ch);
  			Midi.SetChan(key,chan);
  			Midi.SetPort(key,port);
   		}
		return key;
  }
  
 /**
  
   Creates a PitchWheel event with a parameter between -8192 and 8191 and returns a pointer 
   to the event or 0 if there is no more memory space. 
       
   */

    public static int PitchWheel(int wheel, int chan, int port){
           int offset = 8192;
           int min = -8192;
           int max = 8191;
           int e =  Midi.NewEv(Midi.typePitchWheel);

           wheel = (wheel>max) ? max : (wheel<min) ? min : wheel;

           if ( e != 0)    /* Check not NIL */ 
           {
               Midi.SetChan(e,chan);       
               Midi.SetPort(e,port);
               Midi.SetField(e,0,(wheel+offset) & 0x7F);    	/* LS-7bits Field */
               Midi.SetField(e,1,(wheel+offset)>>7 & 0x7F);     /* MS-7bits Field */
           }
           return e;
	}
	
	/**
	Creates a SongPos event with a location in Midi clocks. The location is
    internally divided by 6. Return a pointer to the event or 0 if there is no
    more memory space. 
	*/
	
	public static int SongPos( int pos, int port)
       {
           int e = Midi.NewEv( Midi.typeSongPos);

           pos = pos / 6;

           if ( e != 0)    /* Allocate a new event. Check not NIL */ 
           {
               Midi.SetPort(e,port);
               Midi.SetField(e,0,pos & 0x7F);   	/* LS-7bits Field */
               Midi.SetField(e,1,pos>>7 & 0x7F);    /* MS-7bits Field */
           }
           return e;
       }

	/**
  
    Creates a SongSel event and returns a pointer to the event or 0 if there is no
    more memory space. 
       
   	*/
   
	public static int SongSel (int song, int port)
       {
           int e = Midi.NewEv( Midi.typeSongSel);

          if ( e != 0)   /* Allocate a new event. Check not NIL */ 
           {
              Midi.SetPort(e,port);
              Midi.SetField(e,0,song);    /* Field particular to SongSel */
           }
           return e;
       }
              
    /**
  
       Creates a Start event and returns a pointer to the event or 0 if there is no
       more memory space. 
       
   	*/
          
       public static int Start (int port)
       {
           int e = Midi.NewEv( Midi.typeStart );

           if ( e != 0)/* Allocate a new event. Check not NIL */ 
           {
                Midi.SetPort(e,port);
           }
           return e;
       }
       
       /**
  
       Creates a Continue event and returns a pointer to the event or 0 if there is no
       more memory space. 
       
		*/
       
       public  static int Continue (int port)
       {
           int e = Midi.NewEv( Midi.typeContinue );

           if ( e != 0)/* Allocate a new event. Check not NIL */ 
           {
                Midi.SetPort(e,port);
           }
           return e;
       }
       
        /**
  
       Creates a Stop event and returns a pointer to the event or 0 if there is no
       more memory space. 
       
   		*/
       
		public  static int Stop (int port)
		{
           int e = Midi.NewEv( Midi.typeStop );

           if ( e != 0)/* Allocate a new event. Check not NIL */ 
           {
                Midi.SetPort(e,port);
           }
           return e;
       }
       
        /**
  
       Creates a Tune event and returns a pointer to the event or 0 if there is no
       more memory space. 
       
   		*/
 
		public static int Tune (int port)
		{
           int e = Midi.NewEv( Midi.typeTune);

           if ( e != 0)/* Allocate a new event. Check not NIL */ 
           {
                Midi.SetPort(e,port);
           }
           return e;
       }
       
        /**
  
       Creates a ActiveSens event and returns a pointer to the event or 0 if there is no
       more memory space. 
       
   		*/
		public  static int ActiveSens (int port)
		{
           int e = Midi.NewEv( Midi.typeActiveSens);

           if ( e != 0)/* Allocate a new event. Check not NIL */ 
           {
                Midi.SetPort(e,port);
           }
           return e;
       }

  	 /**
  
       Creates a Reset event and returns a pointer to the event or 0 if there is no
       more memory space. 
       
   		*/
       public static int Reset (int port)
       {
           int e = Midi.NewEv( Midi.typeReset);

           if ( e != 0)/* Allocate a new event. Check not NIL */ 
           {
                Midi.SetPort(e,port);
           }
           return e;
       }
       
       /**
       Creates a SysEx event from an array of int and returns a pointer to the
       event or 0 if there is no more memory space. 
		*/
     
		public static int SysEx (int[] p, int chan, int port)
		{
           int e = Midi.NewEv( Midi.typeSysEx );
           int c,len = p.length;

           if ( e != 0 )    /* Allocate a new event. Check not NIL */ 
           {
               Midi.SetChan(e,chan);       
               Midi.SetPort(e,port);
               for ( c = 0; c < len ; c++) { Midi.AddField(e,p[c]);}
               if (Midi.CountFields(e) < len ) /* if event smaller than len then*/
               {
                   Midi.FreeEv(e);        /* we run out of memory, free it */
                   e = 0;            	  /*     and return 0 */
               }
           }
           return e;
		}

	  /**
       Creates a Stream event from an array of int and returns a pointer to the
       event or 0 if there is no more memory space. 
		*/
    
       public static int Stream (int[] p, int chan, int port)
       {
           int e = Midi.NewEv( Midi.typeStream );
           int c,len = p.length;

           if ( e != 0 )    /* Allocate a new event. Check not NIL */ 
           {
               Midi.SetChan(e,chan);       
               Midi.SetPort(e,port);
               for ( c = 0; c < len ; c++) { Midi.AddField(e,p[c]);}
               if (Midi.CountFields(e) < len ) /* if event smaller than len then*/
               {
                   Midi.FreeEv(e);        /* we run out of memory, free it */
                   e = 0;            	  /*     and return 0 */
               }
           }
           return e;
       }

		/**
	
       Creates a QuarterFrame event and returns a pointer to the event or NIL if
       there is no more memory space. 
		*/

       public static int QuarterFrame( int type, int nibble, int port)
       {
           int e = Midi.NewEv( Midi.typeQFrame );

           if ( e != 0 )/* Allocate a new event. Check not NIL */ 
           {
               Midi.SetPort(e,port);
               Midi.SetField(e,0,type);    /* Fields particular to QuarterFrame  */
               Midi.SetField(e,1,nibble);
           }
           return e;
       }


		/**
       Creates a CtrlChange event. Return a pointer to the event or 0 if there is
       no more memory space. 
		*/

       public static int CtrlChange14b(int ctrl, int val, int chan, int port)
       {
           int e = Midi.NewEv( Midi.typeCtrl14b );

           if ( e != 0 )/* Allocate a new event. Check not NIL */ 
           {
              Midi.SetChan(e,chan);
  			  Midi.SetPort(e,port);
              Midi.SetField(e,0,ctrl);    /* Fields particular to CtrlChange  */
              Midi.SetField(e,1,val);
           }
           return e;
       }

		/**
       Creates a Non Registered Parameter event and returns a pointer to the event
       or 0 if there is no more memory space. 
		*/

       public static int NonRegParam(int param, int val, int chan, int port)
       {
           int e = Midi.NewEv( Midi.typeNonRegParam );

           if ( e != 0 )/* Allocate a new event. Check not NIL */ 
           {
               Midi.SetChan(e,chan);
  			   Midi.SetPort(e,port);
               Midi.SetField(e,0,param);    /* Fields particular to NonRegParam  */
               Midi.SetField(e,1,val);
           }
           return e;
       }

		/**
       Creates a  Registered Parameter event and returns a pointer to the event
       or 0 if there is no more memory space. 
		*/

		public  static int RegParam(int param, int val, int chan, int port)
		{
           int e = Midi.NewEv( Midi.typeRegParam );

           if ( e != 0 )/* Allocate a new event. Check not NIL */ 
           {
               Midi.SetChan(e,chan);
  			   Midi.SetPort(e,port);
               Midi.SetField(e,0,param);    /* Fields particular to RegParam  */
               Midi.SetField(e,1,val);
           }
           return e;
		}

		/**
		Creates a Sequence Number event and returns a pointer to the event or 0
       if there is no more memory space. 
		*/
	
		public  static int  SeqNum(int num, int port)
		{
           int e = Midi.NewEv( Midi.typeSeqNum );

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
              Midi.SetPort(e,port);
              Midi.SetField(e,0,num);    /* the sequence number field */
           }
           return e;
		}

		/**
       Creates a Text event from a character string and returns a pointer to the
       event or 0 if there is not enough memory space. 
		*/

		public static int Text (String str, int chan, int port)
		{
           int e = Midi.NewEv( Midi.typeText);

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
           	   Midi.SetChan(e,chan);
               Midi.SetPort(e,port);
               Midi.SetText(e,str);
           }
           return e;
		}


		/**
       Creates a Copyright event from a character string and returns a pointer to the
       event or 0 if there is not enough memory space. 
		*/


		public  static int Copyright (String str, int chan, int port)
		{
           int e = Midi.NewEv( Midi.typeCopyright);

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
           	   Midi.SetChan(e,chan);
               Midi.SetPort(e,port);
               Midi.SetText(e,str);
           }
           return e;
		}
       
		/**
       Creates a SeqName event from a character string and returns a pointer to the
       event or 0 if there is not enough memory space. 
		*/

		public static int SeqName (String str, int chan, int port)
		{
           int e = Midi.NewEv( Midi.typeSeqName);

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
           	   Midi.SetChan(e,chan);
               Midi.SetPort(e,port);
               Midi.SetText(e,str);
           }
           return e;
		}

		/**
       Creates a InstrName event from a character string and returns a pointer to the
       event or 0 if there is not enough memory space. 
		*/

		public static int InstrName (String str, int chan, int port)
		{
           int e = Midi.NewEv( Midi.typeInstrName);

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
           	   Midi.SetChan(e,chan);
               Midi.SetPort(e,port);
               Midi.SetText(e,str);
           }
           return e;
		}

		/**
       Creates a Lyric event from a character string and returns a pointer to the
       event or 0 if there is not enough memory space. 
		*/

       public static int Lyric (String str, int chan, int port)
       {
           int e = Midi.NewEv( Midi.typeLyric);

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
           	   Midi.SetChan(e,chan);
               Midi.SetPort(e,port);
               Midi.SetText(e,str);
           }
           return e;
       }

		/**
       Creates a Marker event from a character string and returns a pointer to the
       event or 0 if there is not enough memory space. 
		*/

       public static int Marker (String str, int chan, int port)
       {
           int e = Midi.NewEv( Midi.typeMarker);

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
           	   Midi.SetChan(e,chan);
               Midi.SetPort(e,port);
               Midi.SetText(e,str);
           }
           return e;
       }

		/**
       Creates a CuePoint event from a character string and returns a pointer to the
       event or 0 if there is not enough memory space. 
		*/

       public static int CuePoint (String str, int chan, int port)
       {
           int e = Midi.NewEv( Midi.typeCuePoint);

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
           	   Midi.SetChan(e,chan);
               Midi.SetPort(e,port);
               Midi.SetText(e,str);
           }
           return e;
       }
       
      /**
       Creates a ChanPrefix event. Return a pointer to the event or 0 if
       there is not enough memory space. 
		*/

		public  static int ChanPrefix (int prefix)
		{
           int e = Midi.NewEv( Midi.typeChanPrefix);

           if ( e != 0 )/* Allocate a new event. Check not NIL */
           {
               Midi.SetField( e, 0, prefix);
           }
           return e;
       }

		/**
       Creates a EndTrack event. Return a pointer to the event or 0 if
       there is not enough memory space. 
		*/

		public  static int EndTrack ()
		{
    	   return Midi.NewEv( Midi.typeEndTrack);
		}

		/**
       Creates a typeTempo event from a floating point tempo value in
       quarter-notes per minutes. Returns a pointer to the event or 0 if there is
       not enough memory space. 
		*/

		public static int Tempo(float tempo)
		{
           int e =  Midi.NewEv(Midi.typeTempo);     

           if ( e != 0)    /* Allocate a new event. Check not NIL */ 
           {
               Midi.SetField(e, 0, (int)(60000000.0 / tempo));    
           }
           return e;
		}


		/**
		Creates a SMPTE Offset event and returns a pointer to the event or NIL if
       there is no more memory space. 
		*/

		public  static int  SMPTEOffset(int format, int hr, int mn, int sec, int frames, int subframes)
		{
           int e =  Midi.NewEv(Midi.typeSMPTEOffset);   

           if (e != 0)    /* Allocate a new event. Check not NIL */ 
           {
               Midi.SetField(e, 0, format);
               Midi.SetField(e, 1, hr);
               Midi.SetField(e, 2, mn);
               Midi.SetField(e, 3, sec);
               Midi.SetField(e, 4, frames);
               Midi.SetField(e, 5, subframes);
           }
           return e;
		}
	
		/**
       Creates a Time Signature event and returns a pointer to the event or 0 if
       there is no more memory space. 
       */

		public static int   TimeSign (int date, int num, int denom, int click, int quarterDef)
		{
           int e =  Midi.NewEv(Midi.typeTimeSign);   

           if ( e != 0)    /* Allocate a new event. Check not NIL */ 
           {
               Midi.SetField(e, 0, num);
               Midi.SetField(e, 1, denom);
               Midi.SetField(e, 2, click);
               Midi.SetField(e, 3, quarterDef);
           }
           return e;
		}
       
       	/**
       Creates a Key Signature event and returns a pointer to the event or NIL if
       there is no more memory space. 
		*/

		public static int KeySign (int sharpflats, int minor)
		{
          int e =  Midi.NewEv(Midi.typeKeySign);  

           if ( e != 0)    /* Allocate a new event. Check not NIL */ 
           {
               Midi.SetField(e, 0, sharpflats);
               Midi.SetField(e, 1, minor);
           }
           return e;
		}

		/**
		Creates a typeSpecific event from an array of bytes. Returns a pointer to the
       event or 0 if there is no more memory space. 
		*/

		public  static int   Specific (byte[] p)
		{
           int e =  Midi.NewEv( Midi.typeSpecific );
           int    c,len = p.length;

           if ( e != 0 )    /* Allocate a new event. Check not NIL */ 
           {
               for (c = 0; c <len;c++) Midi.AddField(e,p[c]);
               if (Midi.CountFields(e) < len ) /* if event smaller than len then*/
               {
                   Midi.FreeEv(e);   /*     we run out of memory, free it */
                   e = 0;            /*     and return nil */
               }
           }
           return e;
		}

}