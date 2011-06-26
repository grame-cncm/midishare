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
/* MidiFileStream class : reading and writing MidiFiles
/*****************************************************************************/
/*
/*
/* 14/05/97 : When saving, use of a SeekOutputStream object which allows to
/* write anywhere in the file
/* 
/* 20/06/97 : Correction of a bug in GetSeqRef, the previous event returned by GetTrackName 
/* was not used correctly
/* 
/* 1/12/97  : New functions
/* 			- Save (OutputStream outstream, int seq,  MidiFileInfos info) 
/* 			- Load (InputStream inputstream, int seq,  MidiFileInfos info)
/*			- Open (InputStream)
/* 			New prototype for Create : the file or url parameter has been removed.
/* 			
/* 13/02/98 : Correction of a bug with format 1 in FlushKeyOff
/* 10/02/99 : Correction of a bug in writeEv
/* 16/07/02 : Correction of a bug in write_sysex and read_sysex functions, new functions for typeStream
/*****************************************************************************/
  
 
 package grame.midishare.midifile;
 
 import java.io.*;
 import java.net.*;
 
 import  grame.midishare.*;
 import  grame.midishare.tools.*;
 
 /**
 A class to load a MIDIfile an convert it in a MidiShare sequence or to save a MidiShare
 sequence in a MIDIfile. The most important methods are Load and Save. 
 */

 
 public class MidiFileStream {
 
   /*------------------- type/status byte correspondence ---------------------*/
 
 	 final static short codeTbl[]=
        { 0x90, 0x90, 0x80, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0,
          0xf2, 0xf3, 0xf8, 0xfa, 0xfb, 0xfc, 0xf6, 0xfe,
          0xff, 0xf0, 0xf7
        };
        
 	/*------------------- status byte/type correspondence ---------------------*/
 
 	 final static short typeTbl[]=
        { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
          1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
          3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
          4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
          5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
          6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
          7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
          17,130,8,9,0,0,14,18,10,0,11,12,13,0,15,16
         };
 
 	/*------------------- type/meta event correspondence -----------------------*/
 	/* this table is also in charge of the following event types :              */
 	/* QFrame, Ctrl14Bits, NonRegParam and RegParam                             */
 
 	final  static short metaCodeTbl[]=
        { 0xf1, 0xb0, 0xb0, 0xb0,
          0, 1, 2, 3, 4, 5, 6, 7,
          0x20, 0x2f, 0x51, 0x54,
          0x58, 0x59, 0x7f,0x21
        };
 
 	/*------------------- meta event/type correspondence -----------------------*/
 	final  static byte metaTypeTbl[]=
        { 1,2,3,4,5,6,7,8,                              /*  0-7                 */
          0,0,0,0,0,0,0,0,                              /*  8-15 undefs */
          0,0,0,0,0,0,0,0,                              /* 16-23 undefs */
          0,0,0,0,0,0,0,0,                              /* 24-31                */
          9,16,0,0,0,0,0,0,                             /* 32-39 undefs */
          0,0,0,0,0,0,0,10,                             /* 40-47                */
          0,0,0,0,0,0,0,0,                              /* 48-55 undefs */
          0,0,0,0,0,0,0,0,                              /* 56-63 undefs */
          0,0,0,0,0,0,0,0,                              /* 64-71 undefs */
          0,0,0,0,0,0,0,0,                              /* 72-79 undefs */
          0,11,0,0,12,0,0,0,                    		/* 80-87                */
          13,14,0,0,0,0,0,0,                    		/* 88-95                */
          0,0,0,0,0,0,0,0,                              /* 96-103  undefs       */
          0,0,0,0,0,0,0,0,                              /* 104-111 undefs       */
          0,0,0,0,0,0,0,0,                              /* 112-119 undefs       */
          0,0,0,0,0,0,0,15                              /* 120-126 undefs       */
         };
 
 	final static MfEvent ReadEvTbl [] = 
 	{
 	    new Data2Ev(),           /* $80 : 1 typeKeyOff           */
        new Data2Ev(),           /* $90 : 2 typeKeyOn            */
        new Data2Ev(),           /* $a0 : 3 typeKeyPress         */
        new Data2Ev(),           /* $b0 : 4 typeCtrlChange       */
        new Data1Ev(),           /* $c0 : 5 typeProgChange       */
        new Data1Ev(),           /* $d0 : 6 typeChanPress        */
        new Data2Ev(),           /* $e0 : 7 typePitchWheel       */
 
        new sysEx(),             /* $f0 :  17 typeSysEx         */
        new Data1Ev(),           /* $f1 : 130 typeQuarterFrame  */      
        new Data2Ev(),           /* $f2 :   8 typeSongPos       */
        new Data1Ev(),           /* $f3 :   9 typeSongSel       */
        new undef(),             /* $f4                         */
        new undef(),             /* $f5                         */
        new Data0Ev(),           /* $f6 :  14 typeTune          */
        new stream(),            /* $f7 :  18 typeStream        */
        new Data0Ev(),           /* $f8 :  10 typeClock         */
        new undef(),             /* $f9                         */
        new Data0Ev(),           /* $fa :  11 typeStart         */
        new Data0Ev(),           /* $fb :  12 typeContinue      */
        new Data0Ev(),           /* $fc :  13 typeStop          */
        new undef(),             /* $fd                         */
        new Data0Ev(),           /* $fe :  15 typeActiveSens    */
        new undef(),             /* $ff :  16 typeReset         */
 	};
 	
 	 final static MfEvent ReadExtTbl[]=
     {
        new ignoreEv(),     	/* undef type   				*/
        new seqNum(),           /* 134 typeSeqNum               */
        new text(),             /* 135 typeText                 */
        new text(),             /* 136 typeCopyright    		*/
        new text(),             /* 137 typeSeqName              */
        new text(),             /* 138 typeInstrName    		*/
        new text(),             /* 139 typeLyric                */
        new text(),             /* 140 typeMarker               */
        new text(),             /* 141 typeCuePoint             */
        new chanPref(),         /* 142 typeChanPrefix           */
        new endTrack(),         /* 143 typeEndTrack             */
        new tempo(),            /* 144 typeTempo                */
        new smpte(),            /* 145 typeSMPTEOffset          */
        new timeSign(),         /* 146 typeTimeSign             */
        new keySign(),          /* 147 typeKeySign              */
        new text(),             /* 148 typeSpecific             */
        new portPref()          /* 149 typePortPref             */
     };
     
     
     	final MfEvent WriteEvTbl[]=
 		{
 			new Note(),			/* 0 typeNote			*/
 			new Data2Ev(),		/* 1 typeKeyOn			*/
 			new Data2Ev(),		/* 2 typeKeyOff			*/
 			new Data2Ev(),		/* 3 typeKeyPress		*/
 			new Data2Ev(),		/* 4 typeCtrlChange		*/
 			new Data1Ev(),		/* 5 typeProgChange		*/
 			new Data1Ev(),		/* 6 typeChanPress		*/
 			new Data2Ev(),		/* 7 typePitchWheel		*/
 			new Data2Ev(),		/* 8 typeSongPos		*/
 			new Data1Ev(),		/* 9 typeSongSel		*/
 			new Data0Ev(),		/* 10 typeClock			*/
 			new Data0Ev(),		/* 11 typeStart			*/
 			new Data0Ev(),		/* 12 typeContinue		*/
 			new Data0Ev(),		/* 13 typeStop			*/
 			new Data0Ev(),		/* 14 typeTune			*/
 			new Data0Ev(),		/* 15 typeActiveSens	*/
 			new dont_write(),	/* 16 typeReset			*/
 			new sysEx(),		/* 17 typeSysEx			*/
 			new stream(),		/* 18 typeStream		*/
 		};
 
 
 		final MfEvent WriteExtTbl[]=
 		{
 			new Data1Ev(),		/* 130 typeQuarterFrame	*/	
 			new Ctrl14b(),		/* 131 typeCtrl14b		*/
 			new NRegP(),		/* 132 typeNonRegParam	*/
 			new RegP(),			/* 133 typeRegParam		*/
 
 			new seqNum(),		/* 134 typeSeqNum		*/
 			new text(),			/* 135 typeText			*/
 			new text(),			/* 136 typeCopyright	*/
 			new text(),			/* 137 typeSeqName		*/
 			new text(),			/* 138 typeInstrName	*/
 			new text(),			/* 139 typeLyric		*/
 			new text(),			/* 140 typeMarker		*/
 			new text(),			/* 141 typeCuePoint		*/
 
 			new chanPref(),		/* 142 typeChanPrefix	*/
 			new endTrack(),		/* 143 typeEndTrack		*/
 			new tempo(),		/* 144 typeTempo		*/
 			new smpte(),		/* 145 typeSMPTEOffset	*/
 			new timeSign(),		/* 146 typeTimeSign		*/
 			new keySign(),		/* 147 typeKeySign		*/
 			new text(),			/* 148 typeSpecific		*/
 			new portPref()          /* 149 typePortPref             */
 		};
 
 
 	/* meta events types  */
                                                                                                  
 	 static final short META               =  0xFF;           	/* meta event header code                      */
 
 	 static final byte MDF_NumSeq          =  0;               	/* sequence number                             */
 	 static final byte MDF_Texte           =  1;               	/* texte message                               */
 	 static final byte MDF_Copyright   	   =  2;             	/* copyright message                           */
 	 static final byte MDF_SeqName         =  3;               	/* sequence or track name                      */
 	 static final byte MDF_InstrName   	   =  4;             	/* instrument name                             */
 	 static final byte MDF_Lyric           =  5;               	/* lyrics                                                       */
 	 static final byte MDF_Mark            =  6;               	/* marker                                                       */
 	 static final byte MDF_CuePt           =  7;                /* cue point                                   */
 	 static final byte MDF_ChanPref    	   =  0x20;          	/* Midi Channel Prefix                         */
 	 static final byte MDF_EndTrk          =  0x2F;            	/* end track                                   */
 	 static final byte MDF_Tempo           =  0x51;            	/* tempo change                                */
 	 static final byte MDF_Offset          =  0x54;            	/* smpte offset                                */
 	 static final byte MDF_Meas            =  0x58;            	/* time signature                              */
 	 static final byte MDF_Ton             =  0x59;            	/* key signature                               */
 	 static final byte MDF_Extra           =  0x7F;            	/* sequencer specific meta event               */
 	 static final byte MDF_PortPref    	   =  0x21;          	/* Midi Port Prefix                        	   */
 	
 	/* length of the differents meta events */
 
 	 static final short MDF_NumSeqLen   	= 2;
 	 static final short MDF_ChanPrefLen 	= 1;
 	 static final short MDF_PortPrefLen 	= 1;
 	 static final short MDF_EndTrkLen  		= 0;
 	 static final short MDF_TempoLen    	= 3;
 	 static final short MDF_OffsetLen   	= 5;
 	 static final short MDF_MeasLen     	= 4;
 	 static final short MDF_TonLen      	= 2;
 	
 	 static final String MDF_MThd      		= "MThd";
 	 static final String MDF_MTrk     		= "MTrk";
 	
 	 static final String Player =  "PLAYER ref:";
 	 static final short maxTrack =  256;
 	
 	 int trackListe[];	
  
 	// static final short NoErr           = 0;     /* everything is ok                             */
 	// static final short ErrFrmt         = -1;    /* internal datas format error                  */
 	// static final short ErrEvs          = -2;    /* MidiShare memory exhausted                   */
 	// static final short ErrNoTrack      = -3;    /* the track doesn't exists                     */
 	// static final short ErrAdd0         = -4;    /* try to add to a format 0     Midi file       */
 	// static final short ErrMode         = -5;    /* uncorrect file opening mode                  */
 	// static final short ErrUnknow       = -6;    /* unknown event type                           */
 	// static final short ErrReset        = -7;    /* try to write a reset event                   */
 	// static final short ErrTrackClose   = -8;    /* the track is closed                          */
 
 	 short   format;              /* file format                                    */
  	 short   ntrks = 0;           /* number of tracks                               */
  	 short   time;                /* time representation                  			*/
   								  /* for Midi time: tick count per quarter note           */
                                  /* for smpte time: b. 15  = 1                           */
                                  /*                 b.8-14 = frame count per sec         */
                                  /*                 b.0-7  = tick count per frame        */
                                      
                                      
  	DataInputStream   input;                  /* standard file descriptor  */
  	DataOutputStream   output;                /* standard file  */
  	SeekOutputStream  seekoutput;
  	
  	
  	int 	countpos;                       /* la position du nombre de pistes dans le stream */
  	int 	lenpos;                         /* la position de la longueur d'une piste dans le stream */
  	
  	 
 	int  	trkHeadOffset;                  /* track header offset                  */
  	int    	_cnt;                           /* count for end track detection        */
 	long    keyOff;                         /* keyOff coming from typeNote events   */
  	long    curDate;                        /* current date                         */
  	boolean opened;                     	/* flag for opened track                */
  
  	private  short status = 0;
  
  	final short offset_ntrks	= 10;				/* tracks count offset	related */
 													/* to the beginning of the file */
 													
  	final short offset_trkLen	= 4;				/* track length offset related	*/
 													/* to the header header			*/
 
 
  	final boolean  isTrackOpen(){return  opened;}
  	final boolean  IsTempoMap(int t){return (t == Midi.typeCopyright) 
  										|| (t== Midi.typeMarker) 
  										|| ((t >=Midi.typeTempo) && (t<=Midi.typeKeySign)); }
 
 	final short smpte()  {return  (short)(time & 0x8000); }
 	final short frame_par_sec() {return (short)((time & 0x8000) >> 8); }
 	final short ticks_par_frame()  {return  (short)(time & 0xFF); }
 	final short ticks_par_quarterNote() {return  time ;}
 	
 	
 	
 	
 	 public MidiFileStream () {
 		trackListe = new int [MidiFileStream.maxTrack];
 	}
 	
 	
 	/*--------------------------------------------------------------------------*/
	/*																			*/
	/*   fonctions qui positionnent le numéro de référence des événement en		*/
	/*   fonction du numéro de piste ou des infos contenues dans l'événement	*/
	/*   SeqName de la piste.													*/
	/*																			*/
	/*--------------------------------------------------------------------------*/

  /*------------------------------------------------------------------*/
   // Open
   /*------------------------------------------------------------------*/
 	/**
 	Open an existing MIDI file. 
 	*@param file is the file to be opened.
 	*@exception Exception If the file can not be opened. 
  	*/
  	
   final public void Open (File file) throws Exception
   {
      	input = new DataInputStream( new BufferedInputStream (new FileInputStream(file)));
     	MdfHeader mdfheader =  new MdfHeader();
 	 	mdfheader.read(input);
 	 	 		
 	 	format = mdfheader.format;
 	 	ntrks = mdfheader.ntrks;
 	 	time = mdfheader.time;
   }
  
  /*--------------------------------------------------------------------------*/

   final public void Open (InputStream inputstream) throws Exception
   {
      	input = new DataInputStream( new BufferedInputStream (inputstream));
     	MdfHeader mdfheader =  new MdfHeader();
 
     	mdfheader.read(input);
 	 	 		
 	 	format = mdfheader.format;
 	 	ntrks = mdfheader.ntrks;
 	 	time = mdfheader.time;
   }
  
   /*--------------------------------------------------------------------------*/
	/**
 	Open an existing MIDI file. 
 	*@param url is the url to be opened.
 	*@exception Exception If the file can not be opened. 
 	*/

  final public void Open (URL url) throws Exception
  {
    input = new DataInputStream(new BufferedInputStream(url.openStream()));
 	MdfHeader mdfheader =  new MdfHeader();
 
    InitRead();
 	mdfheader.read(input);
 	 	 		
	format = mdfheader.format;
	ntrks = mdfheader.ntrks;
	time = mdfheader.time;
  }
 
 
   /*--------------------------------------------------------------------------*/

	/**
 	Create a MIDIFile format file. The function parameters are as follow:
    *@param format	is the  MIDIFile format of the file, it can takes the following values:
	<BR>
	- midifile0 : format 0 (one track)
	<BR>
	- midifile1 : format 1 (several tracks, to read according
				       	      to the tempo map contained in the track 0)
	<BR>
	- midifile2 : format 2 (several independant patterns one per track, every track contains its own tempo map)
	

    *@param timeDef	is the time representation, it can takes the following values:
    <br>
    - TicksPerQuarterNote 	: MIDI measured time.
    <br>
    - Smpte24            	: smpte time 24 frame/sec.
    <br>
    - Smpte25             	: smpte time 25 frame/sec.
    <br>
    - Smpte29             	: smpte time 29 frame/sec.
    <br>
    - Smpte30             	: smpte time 30 frame/sec.
    *@param  ticks    	: for MIDI time: represents the ticks count per quarter note.
	for smpte time: represents the ticks count per frame.
			  		
	*@exception Exception If the file can not be created. 
	*/

   final public  void Create (short format, short ntrks, short timeDef, short ticks) throws Exception{
 
   		int time;
      	
      	seekoutput = new SeekOutputStream();
	 	output = new DataOutputStream  (seekoutput);
      	
     	MdfHeader mdfheader =  new MdfHeader();
     	
     	this.ntrks= 0;
     	this.format = format;
     	
     	if( timeDef == MidiFileInfos.TicksPerQuarterNote) {	
     		time= ((short)ticks & 0x7FFF);
     	}else{
			time= (timeDef | 0x80) << 8;
			time|=  ticks;
		}
		
 		InitWrite();
 	 	countpos = mdfheader.write(output,ntrks,format, (short)time); // sauvegarde de la position 
 	  
  }
  
  /*--------------------------------------------------------------------------*/
  /**
  Close the file previously opened with Open or Create. 
  If a track is still opened, the function closes it with a call to CloseTrack.
  *@exception Exception If the file can not be closed. 
  */
  
  /*--------------------------------------------------------------------------*/
 	final public void Close() throws Exception{
		/* writing mode		*/
	
		if(isTrackOpen())	{					/* track still opened	*/
			CloseTrack();						/* we close it			*/
		}
		if(keyOff != 0){
			Midi.FreeSeq(keyOff);				/* free the sequence	*/
		}
		output.close();
	}

  /*--------------------------------------------------------------------------*/
  final void InitRead() throws Exception{
	opened= false;
	curDate= 0;
	keyOff= 0;
  }

 /*--------------------------------------------------------------------------*/
  final void InitWrite() throws Exception {
	opened= false;
	curDate= 0;
	
	keyOff= Midi.NewSeq();
	if (keyOff == 0) throw new MidiException ("No more MidiShare event");
  }

 
   /*------------------------------------------------------------------*/
   // ReadEv
   /*------------------------------------------------------------------*/
 	/**
 	ReadEv returns the next event within the current track. The track must be opened using 
	OpenTrack before reading an event. When you reach the end of the current track, it is 
	automaticaly closed  and the function returns 0.
	*@exception Exception If a  MidiShare error occurs.
 	*/
 	
   final public long ReadEv()throws Exception {
      long ev = 0;
 
      if (isTrackOpen()) {
       	if (_cnt > 0) {
  			mdf_GetDate();
  			ev = ReadEvAux();
  	    	if (ev != 0)  Midi.SetDate(ev,curDate);
        }else{
          	opened = false;      /* otherwise: close the track   */
      	}
      }else throw new IOException  ("Error track closed");
      return ev;
  }
 
   /*------------------------------------------------------------------*/
   // ReadTrack
   /*------------------------------------------------------------------*/
 	/**
 	The function reads the current track from the file and returns the result in a MidiShare 
 	sequence.  ReadTrack automaticaly opens and closes the track to read.
	*@Exception Exception If the function returns nil when the track is still opened.
 	*/
 	
  final public long ReadTrack ()throws Exception {
  	
  	 long ev,seq = 0;
 
      OpenTrack();
      
      if ((seq = Midi.NewSeq()) != 0) {
      
      	try {
			while(isTrackOpen()) {                  
				
			if ((ev = ReadEv()) != 0){				 							
				if(Midi.GetFirstEv(seq)==0) {         
					Midi.SetFirstEv(seq,ev);
					Midi.SetLastEv(seq,ev);	
				}else {
					Midi.SetLink(Midi.GetLastEv(seq),ev);
					Midi.SetLastEv(seq,ev);
				}
			}else {
				//System.out.println ("Event ignore ");
				}	
			}
	        return seq;
	         
      	}catch (MidiException e){ 
      		Midi.FreeSeq(seq);         		
         	throw e;
      	}catch  (Exception  e) {
      		throw e;
        }                            
      	
      }else throw new MidiException ("No more MidiShare event");
      
  }
  	
  
   /*------------------------------------------------------------------*/
   // OpenTrack
   /*------------------------------------------------------------------*/
 	/**
 	Open the track if the file is opened for reading.The purpose of this 
	function consists essentially in data initialization to facilitate the track handling.
	*@exception Exception If the track is already opened.
 	*/
 	
   final public void OpenTrack () throws Exception {
   		TrkHeader trkheader = new TrkHeader();
 
      	if (isTrackOpen()) throw new IOException ("Track already opened");
   	 	trkheader.read(input);
 	  	_cnt = trkheader.len;
      	curDate = 0;
      	trkHeadOffset = 0;
      	opened= true;
    }
 
   /*------------------------------------------------------------------*/
   // ReadEv
   /*------------------------------------------------------------------*/
 
     final long ReadEvAux() throws Exception {
        long ev = 0;
        short c;
 
 		try {
        	input.mark(0xffff);
        	c = (short)input.read();
        	_cnt--;                                 /* update remain        */
        	if( c == META){
        		ev= mdf_read_meta();       		
            	c = status = 0;
        	}else if((c & 0x80) != 0)              	/* status byte          */
        		status  = c;
        	else if (status  != 0){                 /* running status       */
        		input.reset();
        		_cnt++;
          		c = status ;
        	}else {
        		c = 0;
          		throw new IOException ("Midi File error format");
          	}
        	if(c != 0){
        		if ((ev = ReadEvTbl[(c < Midi.SysEx) ? (c & 0x7F)/16 : c - Midi.SysEx+7].read(this, c)) != 0){
                if( c < Midi.SysEx) Midi.SetChan(ev,c%16); 
            }else {}
        	}
        	return ev;
        }catch (Exception e) {
        		if (ev != 0) Midi.FreeEv(ev);
           	throw e;
        }
 	}
 	
 	
 	/*------------------------------------------------------------------*/
   	// read META
   	/*------------------------------------------------------------------*/
 
    final long mdf_read_meta() throws Exception{
        short type;
        int len;
 				
 	    type= (short)input.read();      /* read the message     type    */
        _cnt--;                     	/* update remain               	*/
        len = ReadVarLen();          	/* read the message     length  */
 
        if((type & 0x80)!= 0){ 
        	type = 0;          			/* type > 127 => unknown        */
        }else{ 
        	type = MidiFileStream.metaTypeTbl[type];
        }	
        return MidiFileStream.ReadExtTbl[type].read(this, len, type);
   	}
 
 
 	/*------------------------------------------------------------------*/
   	// read  var length
   	/*------------------------------------------------------------------*/
 
 	final int ReadVarLen() throws Exception{
      	int val;
      	short c;
 
        if (((val = input.read()) & 0x80) != 0){
            val &= 0x7F;
            do {
                val = (val<< 7) + ((c =  (short)input.read()) & 0x7F);
                _cnt--;
           } while ((c & 0x80) != 0);
        }
        _cnt--;       
        return val;
 	}
 	
 	/*------------------------------------------------------------------*/
   	// read  var length
   	/*------------------------------------------------------------------*/
 
 	final int ReadVarLen1(long ev) throws Exception{
      	int val;
      	short c;
      	
      	val = input.read();
      	Midi.AddField(ev,val);
 
        if ((val & 0x80) != 0){
            val &= 0x7F;
            do {
                val = (val<< 7) + ((c =  (short)input.read()) & 0x7F);
                Midi.AddField(ev,c);
                _cnt--;
           } while ((c & 0x80) != 0);
        }
        _cnt--;       
        return val;
 	}
 
 
 /*--------------------------------------------------------------------------*/
  final void WriteVarLen(long val) throws Exception{
 	
 	long buf,val1;
 	
 	if (val < 0) throw new IllegalArgumentException ("WriteVarLen : arg < 0 !!");
 	val1 = (long) val;
  	buf= val1 & 0x7F;
 	while( (val1 >>= 7) != 0)
 	{
 		buf <<= 8;
 		buf |= 0x80;
 		buf += (val1 & 0x7F);
 	}
 	while(true)
 	{
 		output.write((int)buf); 
 		if( (buf & 0x80)!= 0) {
 			buf >>= 8;
 		}else 	
 			break;
 	}
 }
 
 
 /*--------------------------------------------------------------------------*/
  final static int GetVarLen(long val){
 	long buf;
 	int res = 0;
 
 	buf= val & 0x7F;
 	while( (val >>= 7) != 0)
 	{
 		buf <<= 8;
 		buf |= 0x80;
 		buf += (val & 0x7F);
 	}
 	while(true)
 	{
 		res++; 
 		if( (buf & 0x80)!= 0) buf >>= 8;
 		else 	break;
 	}
 	
 	return res;
 }
 
 
 /*--------------------------------------------------------------------------*/
 final void write_param (short num, short val, short type) throws Exception{
 
 		output.write( type);					/* NRP number msb			*/
 		output.write( num >> 7);				/* msb value				*/
 		WriteVarLen( 0);						/* next ev, running status 	*/
 		output.write( type-1);					/* NRP number lsb			*/
 		output.write( num & 0x7F);				/* lsb value				*/
 		WriteVarLen( 0);						/* next ev, running status 	*/
 		output.write( 6);						/* data entry msb 			*/
 		output.write( val >> 7);				/* msb						*/
 		WriteVarLen( 0);						/* next ev, running status 	*/
 		output.write( 38);						/* data entry lsb			*/
 		output.write( val & 0x7F);				/* lsb						*/
 
 	}
 
 /*--------------------------------------------------------------------------*/
 final  void WriteEvAux(long ev) throws Exception
 {
 	int type;
 	
 	type= Midi.GetType(ev);
 
 	if( type < Midi.typePrivate)
 		WriteEvTbl[type].write(this,ev,(short)(type < Midi.typeSongPos ? codeTbl[type]+Midi.GetChan(ev) : codeTbl[type]));
 	else if( type >= Midi.typeQFrame && type < Midi.typeReserved){
 		type-= Midi.typeQFrame;
 		WriteExtTbl[type].write(this,ev,metaCodeTbl[type]);
 	}
 	
 }
 
 /*--------------------------------------------------------------------------*/
  final void FlushKeyOff() throws Exception 
 {
 	long seq,ev,date;
 	
 	seq= keyOff;						/* keyOff sequence ptr	*/
 	ev= Midi.GetFirstEv(seq);			/* first event			*/
 	date= curDate;						/* current date			*/
 	
 	while(ev != 0)
 	{
 		Midi.SetFirstEv(seq, Midi.GetLink(ev));	/* first = next			*/
 		WriteVarLen(Midi.GetDate(ev) - date); 
 		WriteEvAux(ev);							/* write the event		*/
 		date= Midi.GetDate(ev);					/* update the date		*/
 		Midi.FreeEv(ev);						/* free the event		*/
 		ev= Midi.GetFirstEv(seq);				/* next event			*/
 	}
 	Midi.SetLastEv(seq, 0);  			/* update the sequence	*/
 	curDate= date;						/* update current date	*/
 }
 
 
 /*--------------------------------------------------------------------------*/
 	/**
 	NewTrack adds a new track header at the end of the file and open the corresponding track. 
 	You can use  this function only if the file is opened for writing. 
	 A previously opened track will first been closed.
 	*@exception Exception 
  	*/

 final  public void NewTrack() throws Exception  {
 	TrkHeader trkheader = new TrkHeader();
 	
 	if(isTrackOpen()) CloseTrack();			/* eventualy close the track */
 	lenpos = trkheader.write(output);  		/* sauvegarde de la position */
 	curDate= 0;								/* current date = 0			*/
 	opened= true;							/* track marked opened		*/
 }
 
 
 /*--------------------------------------------------------------------------*/
 	/**
 	Close a track previously opened with OpenTrack or created with NewTrack.
 		<br> 
	- If the file is opened for reading, CloseTrack locate the file pointer at the beginning of the next 
	track.
 		<br> 
	- If the file is opened for writing, CloseTrack flush the KeyOff sequence (coming from typeNote 
	events), update the track header and the file header.
	The function does nothing  if the track is still closed.

 	*@exception Exception If the track is already closed.
  	*/

 final public void CloseTrack() throws Exception {
 	
 	if(isTrackOpen()) {				/* track opened					*/
 		FlushKeyOff();				/* write the remaining keyOff	*/
 		/* update track length */
 		int length = output.size() - lenpos - 4; /* the coded number size must be substracted*/
 		seekoutput.writeInt(length, lenpos);
 		
 		/* update track number */
 		ntrks++;
 		seekoutput.writeShort(ntrks, countpos);
 		
 		opened= false;				/* track is marked closed		*/
 		
 	}else
 		throw new IOException ("Track already closed");
 }
 
  /**
	WriteEv writes the event  to the current track. The track must be previously opened 
	using the NewTrack function.
	*@param event is the pointer to the event to be written
 	*@exception Exception If the track is closed.
 	*/
 	
 /*--------------------------------------------------------------------------*/
final  public void WriteEv(long event) throws Exception{
 	long seq,off,date;
 	
 	if(!isTrackOpen()) throw new IOException ("Track  closed");
 		
 	date= curDate;									// current date		
 	seq= keyOff;									// keyOff sequence 	
 	off= Midi.GetFirstEv(seq);						// next keyOff 	
 	
 	while( (off != 0) && (Midi.GetDate(event) >= Midi.GetDate(off)))	// key off before current evt	
 	{
 		WriteVarLen(Midi.GetDate(off) - date);   	// write the offset	
 		WriteEvAux(off);							// and the key off		
 		
 		date = Midi.GetDate(off);					// update current date	
 		Midi.SetFirstEv(seq, Midi.GetLink(off));	// update the sequence	
 		Midi.FreeEv( off);							// free the key off		
 		if((off = Midi.GetFirstEv(seq)) == 0)		// key off = next		
 			Midi.SetLastEv(seq, 0);
 	}
 	
  	WriteVarLen(Midi.GetDate(event) - date);   	// write the offset	
 	WriteEvAux(event);							// and the event	
 		
 	curDate = Midi.GetDate(event);				// update current date	
  }
 
  
 
 /*--------------------------------------------------------------------------*/
 	/**
 	Writes in order all the events of the sequence seq to the file. WriteTrack automatically 
	create and close the written track. 
	*@exception Exception If a IO error or a MidiShare error occurs.
 	*/
 	
 final public void  WriteTrack(long seq) throws Exception  {
 	long ev;
 	
 	NewTrack();	  				/* write the track header 	*/
 	ev = Midi.GetFirstEv(seq);
 	
  	while(ev!= 0)
 	{
 	 	WriteEv( ev);	
 		ev= Midi.GetLink(ev);			
 	}
  		
 	CloseTrack();		
 }
 
  final public void  WriteTrack1(long seq) throws Exception  {
 	long ev;
 	int port = 0;
 	long lastWrite = 0;
 	
 	NewTrack();	  				/* write the track header 	*/
 	ev = Midi.GetFirstEv(seq);
 	
  	while(ev!= 0)
 	{
 		/* Ecrit un changement de port */
		if (Midi.GetPort(ev) != port) {
			port = Midi.GetPort(ev);
			WritePortPrefix(lastWrite,port);
		}
		
 	 	WriteEv( ev);	
 	 	lastWrite= ev;
 		ev= Midi.GetLink(ev);			
 	}
  		
 	CloseTrack();		
 }
 

 
 /*--------------------------------------------------------------------------*/
 final  void mdf_GetDate() throws Exception{
 	int offset;
 	
 	offset= ReadVarLen(); 			/* get the offset to next event	*/
 	curDate+= offset;				/* add the offset to the current date	*/
 }
 
 /*--------------------------------------------------------------------------*/
 
 
 /*--------------------------------------------------------------------------*/
  /**
	Load a MIDIfile and convert it in a MidiShare sequence. For a multi-tracks MIDIfile,
	the tracks are mixed in a single MidiShare sequence but are distinguished by the 
	reference number of their events.
	*@param url is the location of the MIDIFile.
	*@param seq is a pointer to the MidiShare sequence to be filled with the MIDIFile.
	*@param info is a MidiFileInfos object  to be filled with various informations read in the MIDIFile.
 	*@exception Exception If the MIDIfile cannot be read of if a MidiShare error occurs. 	
 	*/
 	

 final public void Load (URL url,long seq, MidiFileInfos info ) throws Exception {
	Open(url);
 	
 	if (seq == 0) throw new MidiException ("No more MidiShare event");
 	
 	Midi.SetFirstEv(seq, 0);
 	Midi.SetLastEv(seq, 0);     /* sequence init */
 	
 	try {
 		ReadTracks(seq);				/* read tracks				*/
 		MidiSequence.DelEndTrack( seq);	/* remove EndTrack events	*/
 		info.format= format;
		ReturnTimeInfos(info);
		SetLoadDates(info, seq);
		info.tracks = ntrks;    
 		input.close();    				/* closes the file   		*/
	}catch (IOException e) {
		Midi.ClearSeq( seq);	
		throw e;
	}
}

 
 /*--------------------------------------------------------------------------*/
 /**
	Load a MIDIfile and convert it in a MidiShare sequence. For a multi-tracks MIDIfile,
	the tracks are mixed in a single MidiShare sequence but are distinguished by the 
	reference number of their events.
	*@param mf is the location of the MIDIFile.
	*@param seq is a pointer to the MidiShare sequence to be filled with the MIDIFile.
	*@param info is a MidiFileInfos object  to be filled with various informations read in the MIDIFile.
 	*@exception Exception If the MIDIfile cannot be read of if a MidiShare error occurs. 	
 	*/

 final public void Load (File mf, long seq, MidiFileInfos info) throws Exception {
 	int i,tmp,tmp1;
 	
 	Open(mf);
   	if (seq == 0) throw new MidiException ("No more MidiShare event");
 	
 	Midi.SetFirstEv(seq, 0);
 	Midi.SetLastEv(seq, 0);       /* sequence init */
 	
  	try {
 		ReadTracks(seq);				/* read tracks				*/
 		MidiSequence.DelEndTrack( seq);	/* remove EndTrack events	*/
 		info.format= format;
		ReturnTimeInfos(info);
		SetLoadDates(info, seq);
		info.tracks = ntrks;    
 		input.close();    				/* closes the file   		*/
	}catch (IOException e) {
		Midi.ClearSeq( seq);	
		throw e;
	}
 }
 
  /*--------------------------------------------------------------------------*/
 /**
	Load a MIDIfile and convert it in a MidiShare sequence. For a multi-tracks MIDIfile,
	the tracks are mixed in a single MidiShare sequence but are distinguished by the 
	reference number of their events.
	*@param inputstream is the location of the MIDIFile. The stream will be closed by the function.
	*@param seq is a pointer to the MidiShare sequence to be filled with the MIDIFile.
	*@param info is a MidiFileInfos object  to be filled with various informations read in the MIDIFile.
 	*@exception Exception If the MIDIfile cannot be read of if a MidiShare error occurs. 	
 	*/

 final public void Load (InputStream  inputstream, long seq, MidiFileInfos info) throws Exception {
 	int i,tmp,tmp1;
 	
 	Open(inputstream);
 	if (seq == 0) throw new MidiException ("No more MidiShare event");
 	Midi.SetFirstEv(seq, 0);
 	Midi.SetLastEv(seq, 0);      /* sequence init */
 	
 	try {
 		ReadTracks(seq);				/* read tracks				*/
 		MidiSequence.DelEndTrack( seq);	/* remove EndTrack events	*/
 		info.format= format;
		ReturnTimeInfos(info);
		SetLoadDates(info, seq);
		info.tracks = ntrks;    
 		input.close();    				/* closes the file   		*/
	}catch (IOException e) {
		Midi.ClearSeq( seq);	
		throw e;
	}
 }
 
 
 /*--------------------------------------------------------------------------*/
  final void ReturnTimeInfos(MidiFileInfos info){
	int t;
	
	if((time & 0x8000) != 0)				/* smpte time 	*/
	{
		t= time & 0x7fff;
		info.timedef= t >> 8;				/* timedef		*/
		info.clicks= t & 0xff;				/* clicks		*/
	}
	else									/*  midi time	*/
	{
		info.timedef = 0;
		info.clicks = time;
	}
}


final  void SaveAux (long seq,  MidiFileInfos info) throws Exception {
 	
	if( info.format < 0 || info.format > 2) throw new IOException ("MidiFileFormat error");
	InitTrackListe();					/* init track list to nil	*/
	
 	Create ((short)info.format,  (short)info.tracks, (short)info.timedef,  (short)info.clicks);
  
  	if(info.format != 0){				/* format 1 or 2			*/
		AnalyseSeq(seq);				/* sequence analysis 	 	*/
		WriteTracks(seq);				/* write tracks				*/
	}else {
		WriteTrack1(seq);				/* format 0					*/
	}
	
  }
 
 /*--------------------------------------------------------------------------*/
 /**
	Save a MidiShare sequence as a MIDIfile.
	Each track in a multi-tracks sequence must be distinguished by the 
	reference number of their events.
	*@param file is the location of the MIDIFile.
	*@param seq is a pointer to the MidiShare sequence to be saved.
	*@param info is a MidiFileInfos object and contains informations about the sequence.
	*@exception Exception If the MIDIfile cannot be saved of if a MidiShare error occurs. 	
 	*/

 final public void Save (File file, long seq,  MidiFileInfos info) throws Exception {
 	
 	SaveAux(seq,info);
	
	// copy the memory stream into the file
	
	BufferedOutputStream   tmp = new BufferedOutputStream (new FileOutputStream(file) , 1000);
	seekoutput.writeTo(tmp);
	tmp.close();
 	
	Close();			
  }
  
   /*--------------------------------------------------------------------------*/
 	/**
	Save a MidiShare sequence as a MIDIfile. 
	Each track in a multi-tracks sequence must be distinguished by the 
	reference number of their events.
	*@param url is the location of the MIDIFile.
	*@param seq is a pointer to the MidiShare sequence to be saved.
	*@param info is a MidiFileInfos object and contains informations about the sequence.
	*@exception Exception If the MIDIfile cannot be saved of if a MidiShare error occurs. 	
 	*/

 final public void Save (URL url, long seq,  MidiFileInfos info) throws Exception {
 	
	SaveAux(seq,info);
	
	// copy the memory stream into the file
	
	URLConnection connection  = url.openConnection();
	BufferedOutputStream   tmp = new BufferedOutputStream(connection.getOutputStream() , 1000);
	seekoutput.writeTo(tmp);
	tmp.close();
	Close();			
  
  }


/*--------------------------------------------------------------------------*/
 /**
	Save a MidiShare sequence as a MIDIfile.
	Each track in a multi-tracks sequence must be distinguished by the 
	reference number of their events.
	*@param outstream is the location of the MIDIFile. The stream will be closed by the function.
	*@param seq is a pointer to the MidiShare sequence to be saved.
	*@param info is a MidiFileInfos object and contains informations about the sequence.
	*@exception Exception If the MIDIfile cannot be saved of if a MidiShare error occurs. 	
 	*/


 final public void Save (OutputStream outstream, long seq,  MidiFileInfos info) throws Exception {
 	
 	SaveAux(seq,info);
	
	// copy the memory stream into the file
	
	BufferedOutputStream   tmp = new BufferedOutputStream (outstream , 1000);
	seekoutput.writeTo(tmp);
	tmp.close();
	Close();			
   }

  
  /*--------------------------------------------------------------------------*/
  final void SetLoadDates(  MidiFileInfos info, long s){
	long e;
	
	if( info.timedef != 0 )  				/* SMPTE time*/
	{
		if ((e = Midi.NewEv(Midi.typeTempo)) != 0) {   // add a tempo event (conversion in Midi format)
			Midi.SetField(e,0, info.timedef * info.clicks * 2000);
			Midi.SetDate(e,0);
			Midi.AddSeq (s, e);
			info.clicks = 500;
			info.timedef = 0;  // midi format 
			}
		}
	}

  
/*--------------------------------------------------------------------------*/
final void WriteTracks(long seq) throws Exception {
	short i=0, numPiste= 0;
	
	if(format == MidiFileInfos.midifile1) {
		NewTrack();
		WriteTempoMap(seq); 
		CloseTrack();
		numPiste++;
		i++;
	}
	for (i = 0; i< MidiFileStream.maxTrack; i++){
	
		if( trackListe[i] > 0) {
			NewTrack();
			if(format== MidiFileInfos.midifile1){
				WriteTrackFormat1(seq, i, numPiste);
			}else{
				WriteTrackFormat2( seq, i, numPiste);
			}
			numPiste++;
			CloseTrack();
		}
	}
}

/*--------------------------------------------------------------------------*/
 final  void AnalyseSeq(long seq){
	long ev; int type;

	ev = Midi.GetFirstEv(seq);
	
	if( format == MidiFileInfos.midifile1){			/* it's a format 1	*/
		while (ev != 0)
		{
			type= Midi.GetType(ev);
			
			if( !IsTempoMap( type))		/* does not belong to the tempo map	*/
				trackListe[ Midi.GetRefnum(ev)]= 1;
			
			if (ev == Midi.GetLink (ev)){
				ev = 0;
			}else{
				ev = Midi.GetLink(ev);
			}
		}
	}else{
		while(ev != 0){
			trackListe[Midi.GetRefnum(ev)] = 1;
			ev = Midi.GetLink(ev);
		}
	}
}


/*--------------------------------------------------------------------------*/
  final void WriteTempoMap(long seq) throws Exception {
	int type; long ev, lastWrite= 0;
	
	ev = Midi.GetFirstEv(seq);
	
	while(ev != 0)
	{
		type= Midi.GetType(ev);
		if( IsTempoMap(type))				/* it's a tempo map	event 	*/
		{
			WriteEv(ev);					/* write it					*/
			lastWrite = ev;
		}
		else if( Midi.GetRefnum(ev) == 0)		/* otherwise, if it's refnum is  0	*/
		{
			WriteEv(ev);						/*  write it						*/
			lastWrite= ev;
		}
		ev= Midi.GetLink(ev);
	}
	
	if ((lastWrite == 0) || (Midi.GetType(lastWrite) != Midi.typeEndTrack)) 
		WriteEndTrack(lastWrite);
}
  
  /*--------------------------------------------------------------------------*/
   final void WriteTrackFormat1 (long seq, short ref, short numPiste) throws Exception {
	
	boolean firstName = true;
	int type; long ev, lastWrite= 0;
	int port = 0;

	ev = Midi.GetFirstEv(seq);
	
	while(ev != 0)
	{
		if(Midi.GetRefnum(ev) == ref)
		{
			/* Ecrit un changement de port */
			if (Midi.GetPort(ev) != port) {
				port = Midi.GetPort(ev);
				WritePortPrefix(lastWrite,port);
			}
		
			type = Midi.GetType(ev);
			if( !IsTempoMap(type))		/* does not belong to the tempo map	*/
			{
				if( firstName && Midi.GetType(ev) == Midi.typeSeqName)
				{
					WriteSeqName(ev, ref, numPiste);
					firstName = false;
				}
				else {
					WriteEv( ev);
				}
				lastWrite = ev;
			}
		}
		ev= Midi.GetLink(ev);
	}
	
	if ((lastWrite == 0) || (Midi.GetType(lastWrite) != Midi.typeEndTrack)){
		WriteEndTrack(lastWrite);
	}
}
	
	
/*--------------------------------------------------------------------------*/
   final void WriteTrackFormat2 (long seq, short ref, short numPiste) throws Exception {

	boolean firstName = true;
	int type; long ev, lastWrite= 0;
	int port = 0;

	ev = Midi.GetFirstEv(seq);
	
	while(ev != 0)
	{
		if(Midi.GetRefnum(ev) == ref)
		{
			/* Ecrit un changement de port */
			if (Midi.GetPort(ev) != port) {
				port = Midi.GetPort(ev);
				WritePortPrefix(lastWrite,port);
			}
		
			if( firstName && Midi.GetType(ev) == Midi.typeSeqName)
				{
					WriteSeqName(ev, ref, numPiste);
					firstName = false;
				}
			else {
				WriteEv( ev);
			}
			lastWrite = ev;
		}
		ev= Midi.GetLink(ev);
	}
	
	if ((lastWrite == 0) || (Midi.GetType(lastWrite) != Midi.typeEndTrack)) 
		WriteEndTrack(lastWrite);
}

/*--------------------------------------------------------------------------*/
final void WriteSeqName( long ev, short ref, short numPiste) throws Exception {
	int name;
		
	if( ref == numPiste){					/* refNum equal to the track num 	*/
	
		WriteEv(ev);						/* write the event 					*/
	
	}else if((name = Midi.CopyEv(ev)) != 0) {	/* otherwise copy it			*/
	
		Midi.SetText(name, Midi.GetText(name) + " " + MidiFileStream.Player + ref);
		WriteEv(name);						/* write the event				*/
		Midi.FreeEv( name);					/* and free it					*/
	}else
		throw new IOException ("No more MidiShare events");
}


/*--------------------------------------------------------------------------*/
final void  WriteEndTrack(long prev) throws Exception {
	long ev, seq;
	
	if(( ev = Midi.NewEv(Midi.typeEndTrack)) != 0){		/* allocates an EndTrack event	*/
		Midi.SetLink(ev,0);
		if( ((seq = keyOff) != 0) && (Midi.GetFirstEv(seq) != 0))	/* still keyOff to be written */
		{
			Midi.SetDate(ev , Midi.GetDate(Midi.GetLastEv(seq)));
			Midi.SetLink(Midi.GetLastEv(seq), ev);	/* EndTrack put at the end of the sequence 	*/
			Midi.SetLastEv(seq,ev); 				
			
		}else{
			if(prev != 0)					/* if an event before	*/
			{
				Midi.SetDate(ev,Midi.GetDate(prev));		/* set it's date					*/
				if (Midi.GetType(prev)== Midi.typeNote)		/* if the preceding event is a Note */
													/* add the Nore duration	*/
					Midi.SetDate(ev, Midi.GetDate(ev) + Midi.GetField(prev,2));
			}
			else Midi.SetDate(ev, 0);
			WriteEv(ev);					/* write the event		*/
			Midi.FreeEv(ev);				/* free it				*/
		}
	}else
		throw new IOException ("No more MidiShare events");
}

/*--------------------------------------------------------------------------*/
final void WritePortPrefix( long prev, int port) throws Exception {
	long ev;
	long seq;
	
	if (Midi.GetVersion() < 185) {
		return;  /* Teste la version de MidiShare */ 
	}else if( (ev= Midi.NewEv( Midi.typePortPrefix))!= 0){	/* alloue un evt port prefix	*/
		Midi.SetLink(ev,0);
		Midi.SetField(ev,0,port);
		
		if( prev!=0)									/* s'il y a un événement avant	*/
			Midi.SetDate(ev,Midi.GetDate(prev));		/* positionne sa date			*/
		else
			Midi.SetDate(ev,0);
			
		WriteEv(ev);	/* écrit l'événement			*/
		Midi.FreeEv( ev);				/* et le libère					*/
	}else
		throw new IOException ("No more MidiShare events");
}

/*--------------------------------------------------------------------------*/
 final void InitTrackListe(){
	int i;

	for (i= 0; i< MidiFileStream.maxTrack; i++){
		trackListe[i] = 0;
	}
}


/*--------------------------------------------------------------------------*/
/*				read  all tracks of a MidiFile								*/
/*--------------------------------------------------------------------------*/

 final void UseTrack (long seq, long dest , int i)
	{
		SetSeqRef( seq, GetSeqRef(seq, i));		/* restore the refnum			*/
		SetSeqPort(seq);						/* restore the port				*/
		MidiSequence.Mix( seq, dest);			/* mixe  with the dest sequence	*/
												
		Midi.SetFirstEv(seq, 0);
		Midi.SetLastEv(seq, 0);
		Midi.FreeSeq(seq);						/* free the sequence			*/
	}

/*--------------------------------------------------------------------------*/

final void TryToReadTrack ( long dest, int i)  throws Exception {
	long seq;

	seq = ReadTrack();
	UseTrack (seq, dest, i);
	
}		

/*--------------------------------------------------------------------------*/
 final void ReadTracks( long dest)  throws Exception {
	int i;
	
	for (i=0; i< ntrks; i++)
	{
		TryToReadTrack (dest,i);
	}
}

/*--------------------------------------------------------------------------*/	
	final  void SetSeqRef(long seq, int refNum)
	{
	  long ev;
	
		ev = Midi.GetFirstEv(seq);
		while( ev != 0)
		{
			Midi.SetRefnum(ev,refNum);
			ev = Midi.GetLink(ev);
		}
	}

/*--------------------------------------------------------------------------*/
/* 		restore l'information de port										*/
/*--------------------------------------------------------------------------*/
final void SetSeqPort( long seq)
{
	long  ev, prev, tmp;
	int port = 0;
	
	prev= 0;
	ev= Midi.GetFirstEv(seq);
	while( ev != 0)
	{
		if( (Midi.GetType(ev) == Midi.typePortPrefix) && (Midi.GetLink(ev) != 0))	/* evt fin de piste		*/
		{															/* n'est pas le dernier */
			port = Midi.GetField(ev,0);
			if( prev!=0)								/* n'est pas le premier	*/
				Midi.SetLink(prev,Midi.GetLink(ev));	/* mod. le chainage		*/
			else										/* sinon				*/
				Midi.SetFirstEv(seq,Midi.GetLink(ev)) ; /* suivant= premier		*/
			tmp= Midi.GetLink(ev);						/* sauve le suivant		*/
			Midi.FreeEv(ev);							/* libère l'evt			*/
			ev= tmp;									/* evt courant=suivant	*/
		}
		else 
		{	Midi.SetPort (ev,port);  					/* attribue le port courant */
			prev = ev;
			ev = Midi.GetLink(ev);
		}
	}
}

/*--------------------------------------------------------------------------*/
 final int GetBeginKey( String buff)
	{
		int i,l = buff.length();
		int len = MidiFileStream.Player.length();
		
		for (i = 0; i < l ; i++) {
		
			if (buff.regionMatches(i, MidiFileStream.Player,0, len))
				break;
		}
		return i;
	}

/*--------------------------------------------------------------------------*/
  final long RestoreSeqName( String buff)
{
	long ev = 0;
	
	if((ev= Midi.NewEv(Midi.typeSeqName)) != 0){		/* new event		*/
		Midi.SetText(ev, buff.substring(0, GetBeginKey(buff)));
	} 
	return ev;
}

/*--------------------------------------------------------------------------*/
/* returns the event preceding the first event of type SeqName or TrackName of the sequence */
 final long GetTrackName( long seq)
{
	long ev, prev = 0;
	
	ev = Midi.GetFirstEv(seq);
	while(ev != 0)
	{
		if( Midi.GetType(ev) == Midi.typeSeqName){
			break;
		}
		prev= ev;
		ev= Midi.GetLink(ev);
	}
	if (prev == 0) {
		return ev; 		// si the event if the first returns it
	}else if (ev != 0){
		return prev; 	// otherwise returs the preceding
	}else
		return ev;
}

/*--------------------------------------------------------------------------*/
/* returns the refNum of a SeqName event put in a buffer */
  final int GetEvRef( String buff, int keyLen, int numPiste)	
{
	int index;
	String strRef;
	
	index = GetBeginKey(buff) ;
	
	if (index == buff.length()) { // key not found
		return numPiste;
	}else {
		return Integer.parseInt(buff.substring(index + keyLen)); /* conversion in decimal	*/
	}
}

/*--------------------------------------------------------------------------*/
/*  returns le refNum of a sequence (by default: track number)				*/
  final int GetSeqRef( long seq, int numPiste)
{
	long ev, ori; int refNum;
	long prec= 0;
	int i, n, l;
	String buff;
	
	refNum= numPiste;
	if((ev= GetTrackName(seq)) != 0) { // returns the preceding event or the first  
	
		if (Midi.GetType(ev) != Midi.typeSeqName) { // GetTrackName returned the preceding
			prec = ev;
			ev = Midi.GetLink(ev);
		}else {   // GetTrackName returned the event itsef, the prec is null
			prec = 0;   
		}
	
		n= Midi.CountFields(ev);				/* event field number	*/
		l= MidiFileStream.Player.length();		/* longueur + que minimale	*/
		if( n < 511)
		{
			buff = Midi.GetText(ev);            /* read the event content	*/
			
			if (buff.regionMatches(0,MidiFileStream.Player,0,l) && (prec == 0)) /* evt added		*/
			{
				Midi.SetFirstEv(seq, Midi.GetLink(ev)); /* removed the event from the sequence 		*/
				if (Midi.GetFirstEv(seq) == 0)
					Midi.SetLastEv(seq,0);
				Midi.FreeEv(ev);					/* and frees it				*/
				
			}
			else if((ori= RestoreSeqName(buff)) != 0)		/* modified event	*/
			{
				Midi.SetDate(ori, Midi.GetDate(ev));
				Midi.SetLink(ori, Midi.GetLink(ev));  /* restore the oroginal	*/
				if( prec != 0){ 
					Midi.SetLink(prec,ori); 		/* and replaces it in		*/
				}else{
					Midi.SetFirstEv(seq,ori); 	/* the sequence					*/
				}
				Midi.FreeEv( ev);				/* frees the modified event		*/
			}
			refNum = GetEvRef( buff, l, numPiste);
		}
	}
	return refNum;
	}
 
 }
 
 /*--------------------------------------------------------------------------*/
 /* datas structures                                                                                                                     */
 /*--------------------------------------------------------------------------*/
 
 /*--------------------------------------------------------------------------*/
 /* Midifile header                                                                                                                   */
 /*--------------------------------------------------------------------------*/
 
 	final class MdfHeader{
 	 	 byte id[];       /* header id       */
    	 int   len;       /* datas length    */
 		 short format; 
 		 short ntrks; 
 		 short time;
 
 
 	MdfHeader(){ id = new byte[4];}
 
 
    final void read (DataInputStream input) throws IOException{
 
 		id = new byte[4];
    	input.readFully(id);
    		
     	String str = new String(id);  
    		
    	if (!(str.equals(MidiFileStream.MDF_MThd)))
    		throw new IOException ("MidiFile format error");
    	
        len = input.readInt();
        
        if (len != 6) 
        	throw new IOException ("MidiFile format error");
         
        format =  input.readShort();
        ntrks =  input.readShort();
        time =  input.readShort();
 
 	   }
 	   
 	   final  int write (DataOutputStream output, short tracknum, short format, short time) throws Exception{
 	    	int countpos;
 	    	output.writeBytes(MidiFileStream.MDF_MThd);
 	    	output.writeInt(6);
 	    	output.writeShort(format); 
 	    	countpos = output.size();  
 	    	output.writeShort(tracknum); // number of tracks
 	     	output.writeShort(time);
 	    	return countpos;
 	    }
 
 }
 
 /*--------------------------------------------------------------------------*/
 /* track header structure                                                                                                                  */
 /*--------------------------------------------------------------------------*/
 
final class TrkHeader{
 	 byte id[];       /* header id       */
     int   len;       /* datas length    */
 
 
      TrkHeader(){ id = new byte[4];}
 
       final void read (DataInputStream input) throws IOException{
      
      	input.readFully(id);
      	String str = new String(id); 
      	
    		
    	if (!(str.equals(MidiFileStream.MDF_MTrk)))
    		throw new IOException ("MidiFile format error");
 
       	len = input.readInt();
       		
		if (Midi.FreeSpace() < (len / 2)) {
			Midi.GrowSpace(len/2);
		}
   
      }
      
      
      final int write (DataOutputStream output) throws IOException{
 	    	int lenpos;
 	    	output.writeBytes(MidiFileStream.MDF_MTrk);
 	    	lenpos = output.size();    // save the position
 	    	output.writeInt(0);        // write a default value
 	    	return lenpos;
 	    }
 
 
 }
 
 /*--------------------------------------------------------------------------*/
 /* class MfEvent                                                            */
 /*--------------------------------------------------------------------------*/
 
 class MfEvent {
 
 	  long  read	(MidiFileStream  mfile, int len, short status)throws Exception{return 0;}
 	  long  read	(MidiFileStream  mfile, short status)throws Exception{return 0;}
  	  void	write	(MidiFileStream  mfile, long event, short status)throws Exception{}
  }
 
 /*--------------------------------------------------------------------------*/
 /* read the standard events                                          		 */
 /*--------------------------------------------------------------------------*/

 /*--------------------------------------------------------------------------*/
 /* class sysex                                                              */
 /*--------------------------------------------------------------------------*/
 
 final class  sysEx extends MfEvent {
 
 	final long read (MidiFileStream  mfile, short status)throws Exception{
    	long ev1,ev2;
    	int len;
    	int c = 0;
    	
    	ev1 = Midi.NewEv(Midi.typeSysEx);			
		ev2 = Midi.NewEv(Midi.typeStream);

    	if ((ev1 != 0) && (ev2 != 0)){
    		Midi.AddField( ev2, status);			/* store the first byte in the stream event */
	    	len = mfile.ReadVarLen1(ev2);           /* message length bytes are put in the Stream event	*/
	    	
          	while(len-- > 0){
                c = mfile.input.read();         /* read the datas       */
                mfile._cnt--;
                Midi.AddField(ev2, c);          /* and store them to the Stream event */
                if (c != 0xF7)
					Midi.AddField( ev1, c);		/* and store them to the SysEx event */
          	}
          	
          	if (c == 0xF7) {					/* Complete SysEx */
				Midi.FreeEv(ev2);
				return ev1;
			}else {								/* Stream */
		 		Midi.FreeEv(ev1);
				return ev2;
			}	
			
	    } else throw new MidiException ("No more MidiShare event");
  	 }
 	 
    final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
   		
   		int i,count;
 	
		count= Midi.CountFields( ev);
		mfile.output.write( 0xF0);						/* sysex header			*/
		mfile.WriteVarLen( count+1);					/* message length = bytes to be written + last 0xF7	*/
		
		for (i = 0; i< count; i++) {
			mfile.output.write(Midi.GetField(ev,i));		
		}
		
		mfile.output.write( 0xF7);						/* sysex end			*/
 	}
 
  }
 
 /*--------------------------------------------------------------------------*/
 /* class stream                                                              */
 /*--------------------------------------------------------------------------*/
 
 final class stream extends MfEvent {
 
 	 final long read (MidiFileStream  mfile, short status) throws Exception{
    	long ev;
    	int len;
    	int c;
    
    	if ((ev = Midi.NewEv(Midi.typeStream)) != 0){
    		Midi.AddField(ev, status);                  /* store the first byte in the stream event */
          	len = mfile.ReadVarLen1(ev);                /* message length       */
          	while(len-- > 0){
                c = mfile.input.read();                 /* read the datas       */
                mfile._cnt--;
                Midi.AddField(ev, c);                   /* and store them to the event */
          }
        } else throw new MidiException ("No more MidiShare event");
      	return ev;
 	 }
 	 
    final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
   		
   		int i,count;
 	
 		count= Midi.CountFields( ev);
 		for (i = 0; i< count; i++) {
 			mfile.output.write(Midi.GetField(ev,i));		
 		}
 	}
 
  }

 
 /*--------------------------------------------------------------------------*/
 /* class Note                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class  Note extends MfEvent {
 
    final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
   
   		int c;
 	
 			if((c = Midi.CopyEv( ev)) != 0) {
 				
 				Midi.SetType(c,Midi.typeKeyOff);
 				Midi.SetField(c,1,0);
 				Midi.SetDate(c,Midi.GetDate(ev) + Midi.GetField(ev,2));
 				Midi.SetField(c,2,0);
 				Midi.AddSeq(mfile.keyOff,c);
 				mfile.output.write(status);
 				mfile.output.write( Midi.GetData0( ev));
 				mfile.output.write( Midi.GetData1( ev));   	
 				
 			}else	{
 				throw new MidiException ("No more MidiShare event");
 			}
 		}
 		
 }
 
 /*--------------------------------------------------------------------------*/
 final class  dont_write extends MfEvent {
 
    final void write(MidiFileStream  mfile, int ev, short status)throws Exception{}
 }
 
 
 /*--------------------------------------------------------------------------*/
 /* class NRegP                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class  NRegP extends MfEvent {
 
    final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
   		short num, val;
 
 			num= (short)Midi.GetField( ev, 0);							/* control number	*/
 			val= (short)Midi.GetField( ev, 1);							/* 14 bits value	*/
 			mfile.output.write(Midi.ControlChg + Midi.GetChan(ev));		/* status byte		*/
 			mfile.write_param(num, val, (short)99);
 	 }		
   }
 
 /*--------------------------------------------------------------------------*/
 /* class RegP                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class  RegP extends MfEvent {
 
    final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
   
   		short num, val;
 
 			num= (short)Midi.GetField( ev, 0);							/* control number	*/
 			val= (short)Midi.GetField( ev, 1);							/* 14 bits value	*/
 			mfile.output.write( Midi.ControlChg + Midi.GetChan(ev));	/* status byte		*/
 			mfile.write_param(num, val, (short)101);
 	 }		
 
 }
 
 
 /*--------------------------------------------------------------------------*/
 /* class Ctrl14b                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class  Ctrl14b extends MfEvent {
 
    final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
   		short num, val;
 	
 			num= (short)Midi.GetField( ev, 0);						/* control number	*/
	 		val= (short)Midi.GetField( ev, 1);						/* 14 bits value	*/
	 		mfile.output.write(Midi.ControlChg + Midi.GetChan(ev));	/* status byte		*/
	 		mfile.output.write( num);								/* control number	*/
	 		mfile.output.write( val >> 7);							/* msb				*/
	 		mfile.WriteVarLen(0);									/* next ev, running status 	*/
	 		mfile.output.write( num+32);							/* associated control 	*/
	 		mfile.output.write( val & 0x7F);						/* lsb				*/
	 }	
 	
 }
 
 
 /*--------------------------------------------------------------------------*/
 /* class Data2Ev                                                                                   
 /*--------------------------------------------------------------------------*/
 
final  class  Data2Ev extends MfEvent {
 
 	 final long read (MidiFileStream  mfile, short status) throws Exception {
       long ev = Midi.NewEv(MidiFileStream.typeTbl[status & 0x7F]);
 
     	if (ev != 0) {
            Midi.SetData0(ev, mfile.input.read());
 	        Midi.SetData1(ev, mfile.input.read());
 	        mfile._cnt -=2;
 	     }else throw new MidiException ("No more MidiShare event");		 
 	    return ev;
 	 }
 	 
   final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
   
  		mfile.output.write(status);
 		mfile.output.write( Midi.GetData0( ev));
 		mfile.output.write( Midi.GetData1( ev));  
   }
  }
 
 /*--------------------------------------------------------------------------*/
 /* class Data1Ev                                                                                   
 /*--------------------------------------------------------------------------*/
 
final  class  Data1Ev extends MfEvent {
 
 	final  long read (MidiFileStream  mfile, short status) throws Exception {
    	long ev = Midi.NewEv(MidiFileStream.typeTbl[status & 0x7F]);
 			
     	if (ev != 0) {
 	        Midi.SetData0(ev, mfile.input.read());
 	        mfile._cnt--;
 	     }else
 	     	throw new MidiException ("No more MidiShare event");		 
 		 return ev;
 	 }
 	 
    final void write(MidiFileStream  mfile, int ev, short status)throws Exception{
   		mfile.output.write(status);
 		mfile.output.write( Midi.GetData0( ev));
   }
   
  }
 
 /*--------------------------------------------------------------------------*/
 /* class Data0Ev                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class  Data0Ev extends MfEvent {
 
 	final long read (MidiFileStream  mfile, short status)throws Exception{
    	long ev = Midi.NewEv(MidiFileStream.typeTbl[status & 0x7F]);
    
    	if (ev == 0) throw new MidiException ("No more MidiShare event");		 
 		return ev;
 	}
 	 
    final void write(MidiFileStream  mfile, long ev, short status)  throws Exception{
   	 	mfile.output.write(status);
   	}
  }
 
 /*--------------------------------------------------------------------------*/
 /* lesture des META evenement                                                                                                          */
 /*--------------------------------------------------------------------------*/
 
 /*--------------------------------------------------------------------------*/
 /* class text                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class text extends MfEvent{
 
 	final long read (MidiFileStream  mfile, int len, short type) throws Exception
 	{
        long ev = 0;
        
        if((ev = Midi.NewEv( type + 133)) != 0)
        {
          	mfile._cnt-= len;
          	while(len-- > 0) Midi.AddField(ev, (int)mfile.input.read());
        }
        else 
        	throw new MidiException ("No more MidiShare event");
        return ev;
 	}
 	
 	final  void write(MidiFileStream  mfile, long ev, short status) throws Exception{
 		int len,i;
 		
 		mfile.output.write(MidiFileStream.META);						/* meta evt	header	*/
 		mfile.output.write( status);									/* meta evt type	*/
 		len= Midi.CountFields( ev);
 		mfile.WriteVarLen(len);
 		for(i=0; i<len; i++){
 			mfile.output.write((int)Midi.GetField( ev, i));
 		}
 	
 	}
 	
 }
 
 /*--------------------------------------------------------------------------*/
 /* class endTrack                                                                                   
 /*--------------------------------------------------------------------------*/
 
 
 final class endTrack extends MfEvent{
 
 	 final long read(MidiFileStream  mfile, int len, short unused1) throws Exception
 	{
        long ev = 0;
        
        if( len != MidiFileStream.MDF_EndTrkLen){                        /* message length */
         	return	mfile.ReadExtTbl[0].read(mfile, len,(short)0);
        }else if((ev = Midi.NewEv(Midi.typeEndTrack)) == 0){
            throw new MidiException ("No more MidiShare event");
        }
        
         return ev;
 	}
 	 final void write(MidiFileStream  mfile, long ev, short status) throws Exception{
 	
 		mfile.output.write( MidiFileStream.META);			/* meta evt	header	*/
 		mfile.output.write( MidiFileStream.MDF_EndTrk);		/* meta evt	type	*/
 		mfile.output.write( MidiFileStream.MDF_EndTrkLen);	/* length			*/
 	}
  }
 
 /*--------------------------------------------------------------------------*/
 /* class tempo                                                                                   
 /*--------------------------------------------------------------------------*/
 
 
 final class tempo extends MfEvent{
 
 	 final long read (MidiFileStream  mfile, int len, short unused1)throws Exception
 	{
        long ev = 0;
        int tempo;
        
        if (len != MidiFileStream.MDF_TempoLen){                        /* message length */
            
          return  mfile.ReadExtTbl[0].read(mfile, len,(short)0);
            
        }else if((ev= Midi.NewEv(Midi.typeTempo)) != 0)
        {
            tempo= (int)mfile.input.read();
            tempo <<= 8;
            tempo|= mfile.input.read();
            tempo <<= 8;
            tempo|= mfile.input.read();
            Midi.SetField(ev,0,tempo);
            mfile._cnt-= len;
        }
        else throw new MidiException ("No more MidiShare event");
        	
         return ev;
 	}
 	
 	 final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
 		int l;
 		short s;
 	
 		mfile.output.write( MidiFileStream.META);				/* meta evt	header	*/
  		mfile.output.write( MidiFileStream.MDF_Tempo);			/* meta evt	type	*/
   		mfile.output.write( MidiFileStream.MDF_TempoLen);		/* length			*/
 		l= Midi.GetField(ev,0);
 		s= (short)l;
 		mfile.output.write( (int)(l >> 16));
   		mfile.output.write( s >> 8);
   		mfile.output.write( s & 0xFF);
 	}
 }
 
 /*--------------------------------------------------------------------------*/
 /* class keySign                                                                                   
 /*--------------------------------------------------------------------------*/
 
final  class keySign extends MfEvent{
 
 	final  long read( MidiFileStream mfile, int len, short unused1)throws Exception
 	{
        long ev = 0;
 
        if (len != MidiFileStream.MDF_TonLen)                          /* message length */
          return mfile.ReadExtTbl[0].read(mfile, len,(short)0);
 
        else if( (ev = Midi.NewEv( Midi.typeKeySign)) != 0)
        {
        	Midi.SetField(ev,0, mfile.input.read());
        	Midi.SetField(ev,1, mfile.input.read());
          mfile._cnt-= len;
        }
        else throw new MidiException ("No more MidiShare event");
        
          return ev;
     }
     
      final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
     
 		mfile.output.write(MidiFileStream.META);			/* meta evt	header*/
 		mfile.output.write(MidiFileStream.MDF_Ton);			/* meta evt	type	*/
 		mfile.output.write(MidiFileStream.MDF_TonLen);		/* length		*/
 		mfile.output.write(Midi.GetField(ev,0));			/* signature		*/
 		mfile.output.write(Midi.GetField(ev,1));			/* mode: maj/min	*/
 	}
  }
 
 /*--------------------------------------------------------------------------*/
 /* class timeSign                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class timeSign extends MfEvent{
 
 	final  long read( MidiFileStream mfile, int len, short unused1) throws Exception
 	{
        long ev = 0;
 
        if (len != MidiFileStream.MDF_MeasLen)                         /* message length */
           return  mfile.ReadExtTbl[0].read(mfile, len,(short)0);
        else if( (ev= Midi.NewEv( Midi.typeTimeSign)) != 0)        {
        	Midi.SetField(ev,0, mfile.input.read());
        	Midi.SetField(ev,1, mfile.input.read());
        	Midi.SetField(ev,2, mfile.input.read());
        	Midi.SetField(ev,3, mfile.input.read());
         mfile._cnt-= len;
        }
        else throw new MidiException ("No more MidiShare event");
        	
         return ev;
     }
     
      final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
     
     		mfile.output.write( MidiFileStream.META);						/* meta evt	header	*/
 			mfile.output.write( MidiFileStream.MDF_Meas);					/* meta evt	type	*/
 			mfile.output.write( MidiFileStream.MDF_MeasLen);				/* length			*/
 			mfile.output.write( Midi.GetField(ev,0));						/* numerator		*/
 			mfile.output.write( Midi.GetField(ev,1));						/* denominator		*/
 			mfile.output.write( Midi.GetField(ev,2));						/* # of MIDI clocks */
 			mfile.output.write( Midi.GetField(ev,3));						/* # of 32nd - note */
     
     }
  }
 
 /*--------------------------------------------------------------------------*/
 /* class seqNum                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class seqNum extends MfEvent{
 
 	 final long read( MidiFileStream mfile, int len, short unused1)throws Exception
 	{
        long ev = 0;
        int num;
 
        if (len != MidiFileStream.MDF_NumSeqLen)                       /* message length */
           return  mfile.ReadExtTbl[0].read(mfile, len,(short)0);
 
        else if( (ev= Midi.NewEv( Midi.typeSeqNum)) != 0)
        {
            num= mfile.input.read();
            num <<= 8;
            num|= mfile.input.read();
            Midi.SetField(ev, 0,num);
            mfile._cnt-= len;
        }
        else throw new MidiException ("No more MidiShare event");
        
        return ev;
    }
     
     final  void write(MidiFileStream  mfile, long ev, short status)throws Exception{
     
     	short s;
 			
 		mfile.output.write( MidiFileStream.META);				/* meta evt	header	*/
 		mfile.output.write( MidiFileStream.MDF_NumSeq);			/* meta evt	type	*/
 		mfile.output.write( MidiFileStream.MDF_NumSeqLen);		/* length			*/
 		s= (short)Midi.GetField(ev,0);
 		mfile.output.write( s >> 8);
 		mfile.output.write( s & 0xF);
     
     }
   }
 
 /*--------------------------------------------------------------------------*/
 /* class chanPref                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class chanPref extends MfEvent{
 
 	final  long read( MidiFileStream mfile, int len, short unused1)throws Exception
 	{
        long ev = 0;
 
        if (len != MidiFileStream.MDF_ChanPrefLen)             /* message length */
            return mfile.ReadExtTbl[0].read(mfile, len,(short)0);
            
        else if( (ev= Midi.NewEv(Midi.typeChanPrefix)) != 0)
        {
            Midi.SetField(ev,0, mfile.input.read());
            mfile._cnt-= len;
        }
        else 
        	throw new MidiException ("No more MidiShare event");	
        	
        return ev;
    }
     
     final  void write(MidiFileStream  mfile, long ev, short status)throws Exception{
     
     	mfile.output.write( MidiFileStream.META);			/* meta evt	header	*/
 		mfile.output.write( MidiFileStream.MDF_ChanPref);	/* meta evt	type	*/
 		mfile.output.write( MidiFileStream.MDF_ChanPrefLen);/* length			*/
 		mfile.output.write( Midi.GetField(ev,0));
 
     }
 }
 
 /*--------------------------------------------------------------------------*/
 /* class portPref                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class portPref extends MfEvent{
 
 	final  long read( MidiFileStream mfile, int len, short unused1)throws Exception
 	{
        long ev = 0;
 
        if ((Midi.GetVersion() < 185) || (len != MidiFileStream.MDF_PortPrefLen))             /* message length */
            return mfile.ReadExtTbl[0].read(mfile, len,(short)0);
            
        else if( (ev= Midi.NewEv(Midi.typePortPrefix)) != 0)
        {
            Midi.SetField(ev,0, mfile.input.read());
            mfile._cnt-= len;
        }
        else 
        	throw new MidiException ("No more MidiShare event");	
        	
        return ev;
    }
     
     final  void write(MidiFileStream  mfile, long ev, short status)throws Exception{
     
     	mfile.output.write( MidiFileStream.META);			/* meta evt	header	*/
 		mfile.output.write( MidiFileStream.MDF_PortPref);	/* meta evt	type	*/
 		mfile.output.write( MidiFileStream.MDF_PortPrefLen);/* length			*/
 		mfile.output.write( Midi.GetField(ev,0));
 
     }
 }
 
 /*--------------------------------------------------------------------------*/
 /* class smpte                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class smpte extends MfEvent{
 
 	final  long read( MidiFileStream mfile, int len, short unused1)throws Exception
 	{
        long ev = 0;
        int tmp;
 
        if (len != MidiFileStream.MDF_OffsetLen)                       /* message length */
        
            return mfile.ReadExtTbl[0].read(mfile, len,(short)0);
 
        else if( (ev= Midi.NewEv(Midi.typeSMPTEOffset)) != 0)
        {
        	tmp =  mfile.input.read()*3600;        		/* hours -> sec.       */
        	tmp = tmp + mfile.input.read()*30;			/* min.  -> sec.       */
          	tmp+= mfile.input.read();          			/* sec.                */
          	Midi.AddField( ev,tmp);
          	tmp=  mfile.input.read()* 100;              /* frame count *100    */
          	tmp+= mfile.input.read();                   /* fractionnal frame   */
          	Midi.AddField( ev, tmp);
         	 mfile._cnt-= len;
        }
        else 
        	throw new MidiException ("No more MidiShare event");
        	
        return ev;
    }
     
     final void write(MidiFileStream  mfile, long ev, short status)throws Exception{
     
     		int l;
 
     		mfile.output.write(MidiFileStream.META);				/* meta evt	header	*/
 			mfile.output.write(MidiFileStream.MDF_Offset);			/* meta evt	type	*/
 			mfile.output.write(MidiFileStream.MDF_OffsetLen);		/* length			*/
 			l= (int)Midi.GetField(ev, 0);							/* time offset (sec)*/
 			mfile.output.write(l/3600);								/* hours			*/
 			l%= 3600;
 			mfile.output.write(l/60);								/* minutes			*/
 			mfile.output.write(l%60);								/* seconds			*/
 			l= (int)Midi.GetField( ev, 1);							/* frame and frac	*/
 			mfile.output.write(l/100);								/* frame			*/
 			mfile.output.write(l%100);								/* fractionnal frame*/
     
     }
 }
 
 
 /*--------------------------------------------------------------------------*/
 /* class undef                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class  undef extends MfEvent {
 
 	 final long read (MidiFileStream  mfile, short status)throws Exception{
    	throw new IOException ("Midifile error unknow");
 
 	 }
 	 
    final void write(MidiFileStream  input, int event, short status)throws Exception{}
  }
 
 
 /*--------------------------------------------------------------------------*/
 /* class ignoreEv                                                                                   
 /*--------------------------------------------------------------------------*/
 
 final class  ignoreEv extends MfEvent {
 
 	 final long read (MidiFileStream  mfile, int len,  short status)throws Exception{
 	  	mfile._cnt-= len;
 		while( len-->0){ mfile.input.read(); }
 		return 0;
 	 }
  }
 
