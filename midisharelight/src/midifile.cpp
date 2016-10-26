/*
  MidiShare Project
  Copyright (C) Grame 2004

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
/* ===========================================================================
 *
 *	MIDIFILE.C
 *
 *	functions to read and write MIDI files
 *
 * modifications history:
 * v113 [April 18, 96] 	DF  procedural access to MidiFile_errno and errno
 *                    	functions MidiFileGetMFErrno and MidiFileGetErrno.
 *                    	error code commented in the read_undef function
 * v114 [April 30, 96] 	DF  ignore META events instead reading undef
 * v115 [May 8, 96] 	DF  tracks marked opened at creation time
 *						(due to corrections for Code Warrior)
 * v116 [Nov 27, 96]   DF  32 bits version

 * v117 [Sept 11,97]   SL conversion fonctions ShortVal and LongVal use now usigned values
 * v118 [Nov 23,98]    SL bug correction in write_tempo function
 * v119 [Sept 23,99]   SL use of MDF_Header_SIZE and MDF_Trk_SIZE to avoid alignement problems for data structures
 * v120 [Jan 1,00]     JJC bug correction in write_SeqNum and MidiFileCloseTrack functions
 * v121 [Dec 4,00]     SL remove the obsolete __MWERKS__ tag
 * v122 [Apr 23,01]    SL add PortPrefix management : works with version 185 or later
 * v123 [Jun 01,01]    SL remove fgetpos and fsetpos use for easier cross platform code management
 * v124 [Mars 26,02]   JJC bug correction in MidiFileOpen function
 * v125 [Jul 16,02]    SL bug correction in write_sysex and read_sysex functions, new functions for typeStream
 * v126 [May 03,04]    JJC bug correction in ReadEv function
 * v127 [Nov 18,08]    SL bug correction in MidiFileWriteEv, private events are just ignored
 * v128 [Oct 17,11]    DF OO impl. + MidiShare Light embedded
 *
 */

#include "midifile.h"

#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <errno.h>
#else
#pragma warning (disable: 4309 4996)
#endif

/*--------------------------------------------------------------------------*/
/* constants																*/
#define MDF_MThd	"MThd"			/* file header					*/
#define MDF_MTrk	"MTrk"			/* track header					*/
#define SRC_VERSION	128				/* source code version 			*/
#define MDF_VERSION 100				/* MIDI File format version 	*/
#define offset_ntrks	10			/* tracks count offset	related */
									/* to the beginning of the file */
#define offset_trkLen	4			/* track length offset related	*/
									/* to the header header			*/

/*--------------------------------------------------------------------------*/
/* functions declaration													*/
static MidiEvPtr mdf_ignoreEv( MIDIFilePtr fd, long len);
static MidiEvPtr read_undef( MIDIFilePtr fd, short status);
static MidiEvPtr read_sysex( MIDIFilePtr fd, short status);
static MidiEvPtr read_stream( MIDIFilePtr fd, short status);
static MidiEvPtr read_2DataEv( MIDIFilePtr fd, short status);
static MidiEvPtr read_1DataEv( MIDIFilePtr fd, short status);
static MidiEvPtr read_0DataEv( MIDIFilePtr fd, short status);
static MidiEvPtr read_text( MIDIFilePtr fd, long len, short type);
static MidiEvPtr read_endTrack( MIDIFilePtr fd, long len, short);
static MidiEvPtr read_tempo( MIDIFilePtr fd, long len, short);
static MidiEvPtr read_keySign( MIDIFilePtr fd, long len, short);
static MidiEvPtr read_timeSign( MIDIFilePtr fd, long len, short);
static MidiEvPtr read_seqNum( MIDIFilePtr fd, long len, short);
static MidiEvPtr read_chanPref( MIDIFilePtr fd, long len, short);
static MidiEvPtr read_smpte( MIDIFilePtr fd, long len, short);
static MidiEvPtr read_portPref( MIDIFilePtr fd, long len, short);
static bool write_note( MIDIFilePtr fd, MidiEvPtr ev, short status);
static bool write_2DataEv( MIDIFilePtr fd, MidiEvPtr ev, short status);
static bool write_1DataEv( MIDIFilePtr fd, MidiEvPtr ev, short status);
static bool write_0DataEv( MIDIFilePtr fd, MidiEvPtr ev, short status);
static bool dont_write( MIDIFilePtr , MidiEvPtr , short);
static bool write_sysex( MIDIFilePtr fd, MidiEvPtr ev, short);
static bool write_stream( MIDIFilePtr fd, MidiEvPtr ev, short);
static bool write_Ctrl14b( MIDIFilePtr fd, MidiEvPtr ev, short);
static bool write_RegP( MIDIFilePtr fd, MidiEvPtr ev, short);
static bool write_NRegP( MIDIFilePtr fd, MidiEvPtr ev, short);

static bool write_SeqNum( MIDIFilePtr fd, MidiEvPtr ev, short);
static bool write_texte( MIDIFilePtr fd, MidiEvPtr ev, short status);
static bool write_chanPref( MIDIFilePtr fd, MidiEvPtr ev, short);
static bool write_endTrack( MIDIFilePtr fd, MidiEvPtr, short);
static bool write_tempo( MIDIFilePtr fd, MidiEvPtr ev, short);
static bool write_smpte( MIDIFilePtr fd, MidiEvPtr ev, short);
static bool write_timeSign( MIDIFilePtr fd, MidiEvPtr, short);
static bool write_keySign( MIDIFilePtr fd, MidiEvPtr, short);
static bool write_portPref( MIDIFilePtr fd, MidiEvPtr ev, short);


/*--------------------------------------------------------------------------*/
/* global variables 														*/

int MidiFile_errno;							/* for errors management		*/


typedef MidiEvPtr (* ReadFunc)    ( MIDIFilePtr fd, short status);
typedef MidiEvPtr (* ReadMetaFunc)( MIDIFilePtr fd, long len, short type);
typedef bool   (* WriteFunc)   ( MIDIFilePtr fd, MidiEvPtr ev, short status);


/*------------------- type/status byte correspondence ---------------------*/
static unsigned char codeTbl[]=
	{ 0x90, 0x90, 0x80, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0,
	  0xf2, 0xf3, 0xf8, 0xfa, 0xfb, 0xfc, 0xf6, 0xfe,
	  0xff, 0xf0, 0xf7
	};

/*------------------- status byte/type correspondence ---------------------*/
static unsigned char typeTbl[]=
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
/* this table is also in charge of the following event types :				*/
/* QFrame, Ctrl14Bits, NonRegParam and RegParam								*/

static unsigned char metaCodeTbl[]=
	{
		0xf1, 0xb0, 0xb0, 0xb0,
		0, 1, 2, 3, 4, 5, 6, 7,
		0x20, 0x2f, 0x51, 0x54,
		0x58, 0x59, 0x7f,0x21
	};

/*------------------- meta event/type correspondence -----------------------*/
static char metaTypeTbl[]=
	{ 1,2,3,4,5,6,7,8,				/*  0-7			*/
	  0,0,0,0,0,0,0,0,				/*  8-15 undefs	*/
	  0,0,0,0,0,0,0,0,				/* 16-23 undefs	*/
	  0,0,0,0,0,0,0,0,				/* 24-31		*/
	  9,16,0,0,0,0,0,0,				/* 32-39 undefs	*/
	  0,0,0,0,0,0,0,10,				/* 40-47		*/
	  0,0,0,0,0,0,0,0,				/* 48-55 undefs	*/
	  0,0,0,0,0,0,0,0,				/* 56-63 undefs	*/
	  0,0,0,0,0,0,0,0,				/* 64-71 undefs	*/
	  0,0,0,0,0,0,0,0,				/* 72-79 undefs	*/
	  0,11,0,0,12,0,0,0,			/* 80-87		*/
	  13,14,0,0,0,0,0,0,			/* 88-95		*/
	  0,0,0,0,0,0,0,0,				/* 96-103  undefs 	*/
	  0,0,0,0,0,0,0,0,				/* 104-111 undefs	*/
	  0,0,0,0,0,0,0,0,				/* 112-119 undefs	*/
	  0,0,0,0,0,0,0,15				/* 120-126 undefs	*/
	 };


/*------------------- functions to read the events ------------------------*/
static ReadFunc ReadEvTbl[]=
	{
			read_2DataEv,		/* $80 : 1 typeKeyOff		*/
			read_2DataEv,		/* $90 : 2 typeKeyOn		*/
			read_2DataEv,		/* $a0 : 3 typeKeyPress		*/
			read_2DataEv,		/* $b0 : 4 typeCtrlChange	*/
			read_1DataEv,		/* $c0 : 5 typeProgChange	*/
			read_1DataEv,		/* $d0 : 6 typeChanPress	*/
			read_2DataEv,		/* $e0 : 7 typePitchWheel	*/

			read_sysex,			/* $f0 :  17 typeSysEx			*/
			read_1DataEv,		/* $f1 : 130 typeQuarterFrame	*/
			read_2DataEv,		/* $f2 :   8 typeSongPos		*/
			read_1DataEv,		/* $f3 :   9 typeSongSel		*/
			read_undef,			/* $f4							*/
			read_undef,			/* $f5							*/
			read_0DataEv,		/* $f6 :  14 typeTune			*/
			read_stream,		/* $f7 :  18 typeStream			*/
			read_0DataEv,		/* $f8 :  10 typeClock			*/
			read_undef,			/* $f9							*/
			read_0DataEv,		/* $fa :  11 typeStart			*/
			read_0DataEv,		/* $fb :  12 typeContinue		*/
			read_0DataEv,		/* $fc :  13 typeStop			*/
			read_undef,			/* $fd							*/
			read_0DataEv,		/* $fe :  15 typeActiveSens		*/
			read_undef,			/* $ff :  16 typeReset			*/
	};

static ReadMetaFunc ReadExtTbl[]=
	{
			(ReadMetaFunc)mdf_ignoreEv,	/* undef type   */
			read_seqNum,		/* 134 typeSeqNum		*/
			read_text,			/* 135 typeText			*/
			read_text,			/* 136 typeCopyright	*/
			read_text,			/* 137 typeSeqName		*/
			read_text,			/* 138 typeInstrName	*/
			read_text,			/* 139 typeLyric		*/
			read_text,			/* 140 typeMarker		*/
			read_text,			/* 141 typeCuePoint		*/
			read_chanPref,		/* 142 typeChanPrefix	*/
			read_endTrack,		/* 143 typeEndTrack		*/
			read_tempo,			/* 144 typeTempo		*/
			read_smpte,			/* 145 typeSMPTEOffset	*/
			read_timeSign,		/* 146 typeTimeSign		*/
			read_keySign,		/* 147 typeKeySign		*/
			read_text,			/* 148 typeSpecific		*/
			read_portPref		/* 149 typePortPrefix  	*/
	};

static WriteFunc WriteEvTbl[]=
	{
			write_note,			/* 0 typeNote			*/
			write_2DataEv,		/* 1 typeKeyOn			*/
			write_2DataEv,		/* 2 typeKeyOff			*/
			write_2DataEv,		/* 3 typeKeyPress		*/
			write_2DataEv,		/* 4 typeCtrlChange		*/
			write_1DataEv,		/* 5 typeProgChange		*/
			write_1DataEv,		/* 6 typeChanPress		*/
			write_2DataEv,		/* 7 typePitchWheel		*/
			write_2DataEv,		/* 8 typeSongPos		*/
			write_1DataEv,		/* 9 typeSongSel		*/
			write_0DataEv,		/* 10 typeClock			*/
			write_0DataEv,		/* 11 typeStart			*/
			write_0DataEv,		/* 12 typeContinue		*/
			write_0DataEv,		/* 13 typeStop			*/
			write_0DataEv,		/* 14 typeTune			*/
			write_0DataEv,		/* 15 typeActiveSens	*/
			dont_write,			/* 16 typeReset			*/
			write_sysex,		/* 17 typeSysEx			*/
			write_stream,		/* 18 typeStream		*/
	};


static WriteFunc WriteExtTbl[]=
	{
			write_1DataEv,		/* 130 typeQuarterFrame	*/
			write_Ctrl14b,		/* 131 typeCtrl14b		*/
			write_NRegP,		/* 132 typeNonRegParam	*/
			write_RegP,			/* 133 typeRegParam		*/

			write_SeqNum,		/* 134 typeSeqNum		*/
			write_texte,		/* 135 typeText			*/
			write_texte,		/* 136 typeCopyright	*/
			write_texte,		/* 137 typeSeqName		*/
			write_texte,		/* 138 typeInstrName	*/
			write_texte,		/* 139 typeLyric		*/
			write_texte,		/* 140 typeMarker		*/
			write_texte,		/* 141 typeCuePoint		*/

			write_chanPref,		/* 142 typeChanPrefix	*/
			write_endTrack,		/* 143 typeEndTrack		*/
			write_tempo,		/* 144 typeTempo		*/
			write_smpte,		/* 145 typeSMPTEOffset	*/
			write_timeSign,		/* 146 typeTimeSign		*/
			write_keySign,		/* 147 typeKeySign		*/
			write_texte,		/* 148 typeSpecific		*/
			write_portPref,		/* 149 typePortPrefix	*/

	};


/*--------------------------------------------------------------------------*/
/* numbers representation dependant	functions										*/
#if defined(WIN32) || defined(linux)
# define __LITTLE_ENDIAN__ 1
#endif

#if __LITTLE_ENDIAN__
static int16 ShortVal(int16 val)
{
	int16 converted= 0;

	converted= (val & 0xff) << 8;
	converted|= (val & 0xff00) >> 8;
	return converted;
}

static int32  LongVal (int32 val)
{
	int32 converted= 0;

	converted=  (int32)ShortVal((int16)(val & 0xffff)) << 16;
	converted|= ShortVal((int16)((val & 0xffff0000)>>16));
	return converted;
}
#else
static int32	LongVal (int32 val)		{ return val;}
static int16	ShortVal(int16 val)		{ return val;}
#endif

/*--------------------------------------------------------------------------*/
/* Reading and writing variable length coded datas							*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static unsigned long ReadVarLen( MIDIFilePtr fd)
{
	FILE *fp;
	unsigned long val;
	short c;

	fp= fd->fd;
	if ((val = (unsigned long) getc(fp)) & 0x80)
	{
		val &= 0x7F;
		do {
			val=  (val<< 7) + ((c= getc(fp)) & 0x7F);
			fd->_cnt--;
		} while (c & 0x80 && !feof( fp));
	}
	fd->_cnt--;
	return val;
}

/*--------------------------------------------------------------------------*/
/* read a variable length and store bytes in the event */
static unsigned long ReadVarLen1( MIDIFilePtr fd, MidiEvPtr str)
{
	FILE *fp;
	unsigned long val;
	short c;

	fp= fd->fd;
	val = (unsigned long) getc(fp);
	fd->fMidi->AddField(str,val);

	if (val & 0x80)
	{
		val &= 0x7F;
		do {
			val=  (val<< 7) + ((c= getc(fp)) & 0x7F);
			fd->fMidi->AddField(str,c);
			fd->_cnt--;
		} while (c & 0x80 && !feof( fp));
	}
	fd->_cnt--;
	return val;
}


/*--------------------------------------------------------------------------*/
static bool WriteVarLen( unsigned long val, FILE *fd)
{
	unsigned long buf;

	buf= val & 0x7F;
	while( val >>= 7)
	{
		buf <<= 8;
		buf |= 0x80;
		buf += (val & 0x7F);
	}
	while( true)
	{
		putc( (int)buf, fd);
		if( buf & 0x80) buf >>= 8;
		else 			break;
	}
	return !ferror( fd);
}


/*--------------------------------------------------------------------------*/
/* Reading and writing the MIDI file header									*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static bool ReadMdfHeader( MIDIFilePtr fd)
{
	MDF_Header h;

	if( fread( &h, MDF_Header_SIZE,1, fd->fd)== 1) {	/* read the header  */
		if( strncmp( h.id, MDF_MThd, 4) || LongVal(h.len)!= 6) {
			MidiFile_errno= MidiFileErrFrmt;/* bad file format		*/
		}
		else {
			fd->format= ShortVal(h.format);			/* file format 			*/
			fd->ntrks= ShortVal(h.ntrks);				/* tracks count 		*/
			fd->time= ShortVal(h.time);				/* time representation	*/
			return true;
		}
	}
	return false;
}

/*--------------------------------------------------------------------------*/
static bool Create_mdfHeader( MIDIFilePtr fd,  short format, short time)
{
	MDF_Header h;
	bool ret;							/* return code 		*/

	strcpy( h.id, MDF_MThd);				/* copy the header		*/
	h.len= LongVal(6);						/* datas length			*/
	h.ntrks= 0;								/* tracks count = 0		*/
	h.format= ShortVal(format);				/* file format			*/
	h.time= ShortVal(time);					/* time representation	*/
	ret= (fwrite( &h, MDF_Header_SIZE, 1, fd->fd)== 1);
	return ret;
}



/*--------------------------------------------------------------------------*/
/* Reading and writing a track header										*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static bool ReadTrkHeader( FILE *fd,  MDF_Trk *h)
{
	bool ret= true;								/* return code 		*/

	if( fread( h,  MDF_Trk_SIZE, 1, fd)== 1)  		/* reading header	*/
	{
		if( strncmp( h->id, MDF_MTrk, 4))
		{
         h->len= LongVal (h->len);
			MidiFile_errno= MidiFileErrFrmt;		/* bad file			*/
			ret= false;
		}
	}
	else ret= false;
	return ret;
}

/*--------------------------------------------------------------------------*/
static bool Create_trkHeader( FILE *fd,  long len)
{
	bool ret= true;								/* return code 		*/
	MDF_Trk h;										/* track header		*/

	strcpy( h.id, MDF_MTrk);						/* copy the header	*/
	h.len= LongVal(len);							/* datas length		*/
	ret= (fwrite( &h,  MDF_Trk_SIZE, 1, fd)== 1);
	return ret;
}


/*--------------------------------------------------------------------------*/
/* Functions to locate within the file										*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
bool MIDIFile::ChooseTrack (short trackIndex)
{
	MDF_Trk h;

	MidiFile_errno= MidiFileNoErr;
	if( trackIndex >= fMidiFile.ntrks)
	{
		MidiFile_errno= MidiFileErrNoTrack;
		return false;
	}
	if( fseek( fMidiFile.fd,  MDF_Header_SIZE, SEEK_SET))	/* pos = piste 0	*/
		return false;
	while( trackIndex--)
		if( !ReadTrkHeader( fMidiFile.fd, &h) || fseek( fMidiFile.fd, LongVal (h.len), SEEK_CUR))
			return false;
	return true;
}


/*--------------------------------------------------------------------------*/
/*																			*/
/* 					       Functions to write events						*/
/*																			*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* 	Writing meta-events														*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static bool write_keySign( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	FILE *fd;

	fd= f->fd;
	putc( META, fd);						/* meta evt	header	*/
	putc( MDF_Ton, fd);						/* meta evt	type	*/
	putc( MDF_TonLen, fd);					/* length			*/
	putc( KSTon(ev), fd);					/* signature		*/
	putc( KSMode(ev), fd);					/* mode: maj/min	*/
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_timeSign( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	FILE *fd;

	fd= f->fd;
	putc( META, fd);						/* meta evt	header	*/
	putc( MDF_Meas, fd);					/* meta evt	type	*/
	putc( MDF_MeasLen, fd);					/* length			*/
	putc( TSNum(ev), fd);					/* numerator		*/
	putc( TSDenom(ev), fd);					/* denominator		*/
	putc( TSClocks(ev), fd);				/* # of MIDI clocks */
	putc( TS32nd(ev), fd);					/* # of 32nd - note */
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_smpte( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	int l;
	FILE *fd;

	fd= f->fd;
	putc( META, fd);						/* meta evt	header	*/
	putc( MDF_Offset, fd);					/* meta evt	type	*/
	putc( MDF_OffsetLen, fd);				/* length			*/
	l= (int)f->fMidi->GetField( ev, 0L);			/* time offset (sec)*/
	putc( l/3600, fd);						/* hours			*/
	l%= 3600;
	putc( l/60, fd);						/* minutes			*/
	putc( l%60, fd);						/* seconds			*/
	l= (int)f->fMidi->GetField( ev, 1L);			/* frame and frac	*/
	putc( l/100, fd);						/* frame			*/
	putc( l%100, fd);						/* fractionnal frame*/
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_tempo( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	unsigned long l;
	unsigned short s;
	FILE *fd;

	fd= f->fd;
 	putc( META, fd);						/* meta evt	header	*/
  	putc( MDF_Tempo, fd);					/* meta evt	type	*/
   	putc( MDF_TempoLen, fd);				/* length			*/
		l= Tempo( ev);
		s= (short)l;
		putc( (int)(l >> 16), fd);
   	putc( s >> 8, fd);
   	putc( s & 0xFF, fd);
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_endTrack( MIDIFilePtr f, MidiEvPtr  unused1, short unused2)
{
	FILE *fd;

	fd= f->fd;
	putc( META, fd);						/* meta evt	header	*/
	putc( MDF_EndTrk, fd);					/* meta evt	type	*/
	putc( MDF_EndTrkLen, fd);				/* length			*/
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_chanPref( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	FILE *fd;

	fd= f->fd;
	putc( META, fd);						/* meta evt	header	*/
	putc( MDF_ChanPref, fd);				/* meta evt	type	*/
	putc( MDF_ChanPrefLen, fd);				/* length			*/
	putc( ChanPrefix(ev), fd);
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_portPref( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	FILE *fd;

	fd= f->fd;
	putc( META, fd);						/* meta evt	header	*/
	putc( MDF_PortPref, fd);				/* meta evt	type	*/
	putc( MDF_PortPrefLen, fd);				/* length			*/
	putc( PortPrefix(ev), fd);
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_texte( MIDIFilePtr f, MidiEvPtr ev, short status)
{
	unsigned long len, i;
	FILE *fd;

	fd= f->fd;
	putc( META, fd);						/* meta evt	header	*/
	putc( status, fd);						/* meta evt type	*/
	len= f->fMidi->CountFields( ev);
	WriteVarLen( len, fd);
	for( i=0; i<len; i++)
		putc( (int)f->fMidi->GetField( ev, i), fd);
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_SeqNum( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	unsigned short s;
	FILE *fd;

	fd= f->fd;
	putc( META, fd);						/* meta evt	header	*/
	putc( MDF_NumSeq, fd);					/* meta evt	type	*/
	putc( MDF_NumSeqLen, fd);				/* length			*/
	s= SeqNum(ev);
	putc( s >> 8, fd);
	putc( s & 0xFF, fd);
	return !ferror( fd);
}


/*--------------------------------------------------------------------------*/
/* 	Writing MIDI events														*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static bool write_param( FILE *fd, short num, short val, short type)
{
	putc( type, fd);						/* NRP number msb			*/
	putc( num >> 7, fd);					/* msb value				*/
	WriteVarLen( 0, fd);					/* next ev, running status 	*/
	putc( type-1, fd);						/* NRP number lsb			*/
	putc( num & 0x7F, fd);					/* lsb value				*/
	WriteVarLen( 0, fd);					/* next ev, running status 	*/
	putc( 6, fd);							/* data entry msb 			*/
	putc( val >> 7, fd);					/* msb						*/
	WriteVarLen( 0, fd);					/* next ev, running status 	*/
	putc( 38, fd);							/* data entry lsb			*/
	putc( val & 0x7F, fd);					/* lsb						*/
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_NRegP( MIDIFilePtr fd, MidiEvPtr ev, short unused1)
{
	short num, val;

	num= (short)fd->fMidi->GetField( ev, 0);					/* control number	*/
	val= (short)fd->fMidi->GetField( ev, 1);					/* 14 bits value	*/
	putc( ControlChg + Canal( ev), fd->fd);		/* status byte		*/
	return write_param( fd->fd, num, val, 99);
}

/*--------------------------------------------------------------------------*/
static bool write_RegP( MIDIFilePtr fd, MidiEvPtr ev, short unused1)
{
	short num, val;

	num= (short)fd->fMidi->GetField( ev, 0);					/* control number	*/
	val= (short)fd->fMidi->GetField( ev, 1);					/* 14 bits value	*/
	putc( ControlChg + Canal( ev), fd->fd);		/* status byte		*/
	return write_param( fd->fd, num, val, 101);
}

/*--------------------------------------------------------------------------*/
static bool write_Ctrl14b( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	short num, val;
	FILE *fd;

	fd= f->fd;
	num= (short)f->fMidi->GetField( ev, 0);			/* control number	*/
	val= (short)f->fMidi->GetField( ev, 1);			/* 14 bits value	*/
	putc( ControlChg + Canal( ev), fd);			/* status byte		*/
	putc( num, fd);								/* control number	*/
	putc( val >> 7, fd);						/* msb				*/
	WriteVarLen( 0, fd);						/* next ev, running status 	*/
	putc( num+32, fd);							/* associated control 	*/
	putc( val & 0x7F, fd);						/* lsb				*/
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_sysex( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	unsigned long count;
	FILE *fd;
	MidiSEXPtr e;

	fd= f->fd;
	count= f->fMidi->CountFields( ev)	;
	putc( 0xF0, fd);					/* sysex header			*/
	WriteVarLen( count+1, fd);			/* message length = bytes to be written + last 0xF7	*/
	e= Link( LinkSE(ev));
	while( count) {
		fwrite( e->data, (count >= 12) ? 12 : (int)count, 1, fd);

		e= Link(e);
		count-= (count >= 12) ? 12 : count;
	}
	putc( 0xF7, fd);					/* sysex end			*/
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_stream( MIDIFilePtr f, MidiEvPtr ev, short unused1)
{
	unsigned long count;
	FILE *fd;
	MidiSEXPtr e;

	fd= f->fd;
	count= f->fMidi->CountFields( ev);
	e= Link( LinkSE(ev));
	while( count) {
		fwrite( e->data, (count >= 12) ? 12 : (int)count, 1, fd);
		e= Link(e);
		count-= (count >= 12) ? 12 : count;
	}
	return !ferror( fd);
}


/*--------------------------------------------------------------------------*/
static bool dont_write( MIDIFilePtr unused1, MidiEvPtr unused2, short unused3)
{
	return true;
}

/*--------------------------------------------------------------------------*/
static bool write_0DataEv( MIDIFilePtr fd, MidiEvPtr unused1, short status)
{
	putc( status, fd->fd);
	return !ferror( fd->fd);
}

/*--------------------------------------------------------------------------*/
static bool write_1DataEv( MIDIFilePtr f, MidiEvPtr ev, short status)
{
	FILE *fd;

	fd= f->fd;
	putc( status, fd);
	putc( Data( ev)[0], fd);
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_2DataEv( MIDIFilePtr f, MidiEvPtr ev, short status)
{
	FILE *fd;

	fd= f->fd;
	putc( status, fd);
	putc( Data( ev)[0], fd);
	putc( Data( ev)[1], fd);
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
static bool write_note( MIDIFilePtr f, MidiEvPtr ev, short status)
{
	FILE *fd = f->fd;
	MidiEvPtr c = f->fMidi->CopyEv (ev);
	if( c ) {
		EvType(c)= typeKeyOff;
		Vel(c) = 0;
		Date(c) = Date( ev) + Dur( ev);
		Dur(c) = 0;
		f->fMidi->AddSeq( f->keyOff, c);
		putc( status, fd);
		putc( Data( ev)[0], fd);
		putc( Data( ev)[1], fd);
	}
	else {
		MidiFile_errno= MidiFileErrEvs;
		return false;
	}
	return !ferror( fd);
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::WriteEv( MIDIFilePtr fd, MidiEvPtr ev)
{
	short type;
	bool ret= true;

	type= EvType( ev);
	if( type < typePrivate)
		ret= (* WriteEvTbl[type])( fd, ev, (type < typeSongPos ? codeTbl[type]+Canal(ev) : codeTbl[type]));
	else if( type >= typeQuarterFrame && type < typeReserved)
	{
		type-= typeQuarterFrame;
		ret= (* WriteExtTbl[type])( fd, ev, metaCodeTbl[type]);
	}
	return ret;
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::FlushKeyOff( MIDIFilePtr fd)
{
	MidiSeqPtr seq;
	MidiEvPtr ev;
	bool ret= true;
	long date;

	seq= fd->keyOff;						/* keyOff sequence ptr	*/
	ev= seq->first;							/* first event			*/
	date= fd->curDate;						/* current date			*/
	while( ev && ret)
	{
		seq->first= Link(ev);				/* first = next			*/
		ret= (WriteVarLen( Date( ev)- date, fd->fd)
			  && WriteEv( fd, ev));			/* write the event		*/
		date= Date( ev);					/* uodate the date		*/
		fd->fMidi->FreeEv( ev);					/* free the event		*/
		ev= seq->first;						/* next event			*/
	}
	if( ret) seq->last= nil;				/* update the sequence	*/
	fd->curDate= date;						/* update current date	*/
	return ret;
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::NewTrack()
{
	MidiFile_errno= MidiFileNoErr;
	if( isTrackOpen() && !CloseTrack())			/* if opened, close the track */
		return false;
	if( fseek( fMidiFile.fd, 0, SEEK_END))			/* locate to the end of the file	*/
		return false;
	if( !Create_trkHeader( fMidiFile.fd, 0))	  	/* write the track header 	*/
		return false;
	fMidiFile.trkHeadOffset = ftell(fMidiFile.fd);	/* get the track location 	*/
	fMidiFile.curDate= 0;							/* current date = 0			*/
	fMidiFile.opened= true;							/* track marked opened		*/
	return true;
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::OpenTrack()
{
	MDF_Trk h;

	MidiFile_errno= MidiFileNoErr;
	if( fMidiFile.mode)
	{
		MidiFile_errno= MidiFileErrNoTrack;
		return false;
	}
	if( isTrackOpen())					/* track still opened		*/
		return true;
	if( ReadTrkHeader( fMidiFile.fd, &h))		/* read the track header	*/
	{
		fMidiFile._cnt= LongVal(h.len); 		/* track length=0 => end	*/
		fMidiFile.curDate= 0;					/* current date = 0			*/
		fMidiFile.opened= true;					/* track marked opened		*/
	}
	else return false;
	return true;
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::CloseTrack()
{
	long offset1, offset2;					/* beg and end track offsets */
	int trkLen;								/* track length				 */
	int16 ntrks;							/* tracks count				 */
	bool ret=true;

	MidiFile_errno= MidiFileNoErr;
	offset1= fMidiFile.trkHeadOffset;
	if( fMidiFile.mode )							/* writing mode					*/
	{
		if( isTrackOpen())							/* track opened					*/
		{
			offset1= fMidiFile.trkHeadOffset;
			ret= FlushKeyOff( &fMidiFile);			/* write the remaining keyOff	*/
			offset2 = ftell(fMidiFile.fd);			/* get the end track location 	*/

			trkLen= LongVal(offset2- offset1);		/* get the track length			*/
			offset1-= offset_trkLen;

			if( fseek( fMidiFile.fd, offset1, SEEK_SET))	/* to track length position 	*/
				return false;
												/* update the track length 		*/
			if( fwrite( &trkLen, 4, 1, fMidiFile.fd)!= 1)
				return false;
												/* to track count position		*/
			if( fseek( fMidiFile.fd, offset_ntrks, SEEK_SET))
				return false;
			fMidiFile.ntrks++;					/* track count				 	*/
												/* update the tracks count 		*/
			ntrks= ShortVal(fMidiFile.ntrks);
			if( fwrite( &ntrks, sizeof(ntrks), 1, fMidiFile.fd)!= 1)
				return false;
		}
	}
	else if( isTrackOpen())					/* reading mode					*/
											/* locate to the beginning of the next track	*/
		ret= ( fseek( fMidiFile.fd, fMidiFile._cnt, SEEK_CUR)== 0);
	fMidiFile.opened= false;							/* track is marked closed		*/
	return ret;
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::WriteEv (MidiEvPtr ev)
{
	MidiSeqPtr seq;
	MidiEvPtr off;
	long date;

	if( fMidiFile.opened)
		MidiFile_errno= MidiFileNoErr;
	else
	{
		MidiFile_errno= MidiFileErrTrackClose;
		return false;
	}
	date= fMidiFile.curDate;						/* current date		*/
	seq= fMidiFile.keyOff;							/* keyOff sequence 	*/
	off= seq->first;								/* next keyOff 		*/
	while( off && (Date(ev) >= Date(off)))			/* key off before current evt	*/
	{
		if( !WriteVarLen( Date(off)- date, fMidiFile.fd) ||	/* write the offset	*/
			!WriteEv( &fMidiFile, off))						/* and the key off	*/
			return false;
		date= Date( off);							/* update current date	*/
		seq->first= Link(off);						/* update the sequence	*/
		fMidiFile.fMidi->FreeEv( off);							/* free the key off		*/
		if( !(off= seq->first))						/* key off = next		*/
			seq->last= nil;
	}

	// 18/11/20008 Private events are just ignored
	if (EvType(ev) >= typePrivate && EvType(ev) < typeQuarterFrame)
		return true;

	if( !WriteVarLen( Date( ev)- date, fMidiFile.fd) ||		/* write the offset	*/
		!WriteEv( &fMidiFile, ev))							/* and the event	*/
		return false;
	fMidiFile.curDate= Date( ev);							/* update current date	*/
	return true;
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::WriteTrack(MidiSeqPtr seq)
{
	MidiEvPtr ev;
	bool ret= true;

	MidiFile_errno= MidiFileNoErr;
	if( !NewTrack())					/* write the track header 	*/
		return false;

	ev= seq->first;
	while( ev && ret)
	{
		ret= WriteEv( ev);				/* write the event		*/
		ev= Link( ev);					/* next event			*/
	}

	if( !CloseTrack())					/* update the track header	*/
		return false;					/* and the file header		*/
	return true;
}


/*--------------------------------------------------------------------------*/
/* Reading a track															*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static bool mdf_GetDate( MIDIFilePtr fd)
{
	long offset;

	offset= ReadVarLen( fd); 			/* get the offset to next event	*/
	if( feof( fd->fd))						/* error control	*/
	{
		MidiFile_errno= MidiFileErrFrmt;
		return false;
	}
	if( ferror( fd->fd))					/* error control	*/
		return false;
	fd->curDate+= offset;			/* add the offset to the current date	*/
	return true;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr mdf_ignoreEv( MIDIFilePtr fd, long len)
{
	fd->_cnt-= len;
	while( len--)
		getc( fd->fd);
	return 0;
}


/*--------------------------------------------------------------------------*/
/* functions to read a meta event											*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_text( MIDIFilePtr fd, long len, short type)
{
	MidiEvPtr ev = fd->fMidi->NewEv( type + 133);

	if(ev) {
		fd->_cnt-= len;
		while( len--)
			fd->fMidi->AddField( ev, (long)getc( fd->fd));
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_endTrack( MIDIFilePtr fd, long len, short unused1)
{
	MidiEvPtr ev=nil;

	if( len!= MDF_EndTrkLen)			/* message length */
		mdf_ignoreEv( fd, len);
	else if( !(ev= fd->fMidi->NewEv( typeEndTrack)))
		MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_tempo( MIDIFilePtr fd, long len, short unused1)
{
	MidiEvPtr ev=nil;
	long tempo;

	if (len != MDF_TempoLen) 			/* message length */
		mdf_ignoreEv( fd, len);
	else if( (ev= fd->fMidi->NewEv( typeTempo)))
	{
		tempo= (long)getc(fd->fd);
		tempo <<= 8;
		tempo|= getc(fd->fd);
		tempo <<= 8;
		tempo|= getc(fd->fd);
		Tempo(ev)= tempo;
		fd->_cnt-= len;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_keySign( MIDIFilePtr fd, long len, short unused1)
{
	MidiEvPtr ev=nil;

	if (len != MDF_TonLen) 				/* message length */
		mdf_ignoreEv( fd, len);
	else if( (ev= fd->fMidi->NewEv( typeKeySign)))
	{
		KSTon( ev)= getc(fd->fd);
		KSMode( ev)= getc(fd->fd);
		fd->_cnt-= len;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_timeSign( MIDIFilePtr fd, long len, short unused1)
{
	MidiEvPtr ev=nil;

	if (len != MDF_MeasLen) 			/* message length */
		mdf_ignoreEv( fd, len);
	else if( (ev= fd->fMidi->NewEv( typeTimeSign)))
	{
		TSNum( ev)= getc(fd->fd);
		TSDenom( ev)= getc(fd->fd);
		TSClocks( ev)= getc(fd->fd);
		TS32nd( ev)= getc(fd->fd);
		fd->_cnt-= len;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_seqNum( MIDIFilePtr fd, long len, short unused1)
{
	MidiEvPtr ev=nil;
	short num;

	if (len != MDF_NumSeqLen) 			/* message length */
		mdf_ignoreEv( fd, len);
	else if( (ev= fd->fMidi->NewEv( typeSeqNum)))
	{
		num= getc(fd->fd);
		num <<= 8;
		num|= getc(fd->fd);
		SeqNum(ev)= num;
		fd->_cnt-= len;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_chanPref( MIDIFilePtr fd, long len, short unused1)
{
	MidiEvPtr ev=nil;

	if (len != MDF_ChanPrefLen) 		/* message length */
		mdf_ignoreEv( fd, len);
	else if( (ev= fd->fMidi->NewEv( typeChanPrefix)))
	{
		ChanPrefix(ev)= getc(fd->fd);
		fd->_cnt-= len;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_portPref( MIDIFilePtr fd, long len, short unused1)
{
	MidiEvPtr ev=nil;

	/*  read only if MidiShare version > 185 to avoid error */
	if (len != MDF_PortPrefLen) 		/* message length */
		mdf_ignoreEv( fd, len);
	else if( (ev= fd->fMidi->NewEv( typePortPrefix))) {
		PortPrefix(ev)= getc(fd->fd);
		fd->_cnt-= len;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_smpte( MIDIFilePtr fd, long len, short unused1)
{
	MidiEvPtr ev=nil;
	long tmp;

	if (len != MDF_OffsetLen) 			/* message length */
		mdf_ignoreEv( fd, len);
	else if( (ev= fd->fMidi->NewEv( typeSMPTEOffset)))
	{
		tmp= getc(fd->fd)* 3600;		/* hours -> sec.		*/
		tmp+= getc(fd->fd)* 60;			/* min.  -> sec.		*/
		tmp+= getc(fd->fd);				/* sec.					*/
		fd->fMidi->SetField( ev, 0L,tmp);
		tmp= getc(fd->fd)* 100;			/* frame count *100 	*/
		tmp+= getc(fd->fd);				/* fractionnal frame	*/
		fd->fMidi->SetField( ev, 1L, tmp);
		fd->_cnt-= len;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr mdf_read_meta( MIDIFilePtr fd)
{
	short type;
	unsigned long len;

	type= getc(fd->fd);						/* read the message	type	*/
	fd->_cnt--;								/* update remain			*/
	len= ReadVarLen(fd);					/* read the message	length 	*/

	if( type & 0x80) type=0;				/* type > 127 => unknown	*/
	else type= metaTypeTbl[type];
	return (*ReadExtTbl[type])( fd, len, type);
}


/*--------------------------------------------------------------------------*/
static MidiEvPtr read_undef( MIDIFilePtr fd,  short len)
{
	//MidiFile_errno= MidiFileErrUnknow;
	return nil;
}


/*--------------------------------------------------------------------------*/
/* functions to read a MIDI event											*/
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
static MidiEvPtr read_sysex( MIDIFilePtr fd, short status)
{
	MidiEvPtr ev1,ev2;
	unsigned long len;
	short c = 0;

	ev1 = fd->fMidi->NewEv(typeSysEx);
	ev2 = fd->fMidi->NewEv(typeStream);

	/* try to parse as if the event is a SysEx or a Stream : choice is made when the last byte is known */

	if( ev1 && ev2)
	{
		fd->fMidi->AddField( ev2, status);			/* store the first byte in the stream event */
		len= ReadVarLen1( fd,ev2);			/* message length bytes are put in the Stream event	*/

		while( len--)
		{
			c= getc( fd->fd);				/* read the datas	*/
			fd->_cnt--;
			fd->fMidi->AddField( ev2, c);			/* and store them to the Stream event */
			if (c != 0xF7)
				fd->fMidi->AddField( ev1, c);		/* and store them to the SysEx event */
		}

		if (c == 0xF7) {					/* Complete SysEx */
			fd->fMidi->FreeEv(ev2);
			return ev1;
		}else {								/* Stream */
	 		fd->fMidi->FreeEv(ev1);
			return ev2;
		}
	}
	else MidiFile_errno= MidiFileErrEvs;
	return nil;
}



/*--------------------------------------------------------------------------*/
static MidiEvPtr read_stream( MIDIFilePtr fd, short status)
{
	unsigned long len;
	short c;

	MidiEvPtr ev = fd->fMidi->NewEv(typeStream);
	if(ev) {
		fd->fMidi->AddField( ev, status);			/* store the first byte in the stream event */
		len= ReadVarLen1( fd, ev);			/* message length bytes are put in the stream event	*/

		while( len--)
		{
			c= getc( fd->fd);				/* read the datas	*/
			fd->_cnt--;
			fd->fMidi->AddField( ev, c);			/* and store them to the Stream event */
		}
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}



/*--------------------------------------------------------------------------*/
static MidiEvPtr read_2DataEv( MIDIFilePtr fd, short status)
{
	MidiEvPtr ev = fd->fMidi->NewEv( typeTbl[status & 0x7F]);

	if( ev )	/* event allocation	*/
	{
		Data( ev)[0]= getc( fd->fd);			/* store the first data */
		Data( ev)[1]= getc( fd->fd);			/* and the 2nd data		*/
		fd->_cnt-= 2;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_1DataEv( MIDIFilePtr fd, short status)
{
	MidiEvPtr ev = fd->fMidi->NewEv( typeTbl[status & 0x7F]);

	if( ev )	/* event allocation	*/
	{
		Data( ev)[0]= getc( fd->fd);			/* store the data	*/
		fd->_cnt--;
	}
	else MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
static MidiEvPtr read_0DataEv( MIDIFilePtr fd, short status)
{
	MidiEvPtr ev = fd->fMidi->NewEv( typeTbl[status & 0x7F]);

	if( !ev)	/* event allocation	*/
		MidiFile_errno= MidiFileErrEvs;
	return ev;
}

/*--------------------------------------------------------------------------*/
MidiEvPtr MIDIFile::ReadEv( MIDIFilePtr fd)
{
	MidiEvPtr ev= nil;
	short c;
	static short status= 0;

	c= getc( fd->fd);
	fd->_cnt--;										/* update remain	*/

	if( c== META)									/* meta evt			*/
	{
		ev= mdf_read_meta( fd);
		c=0;
	}
	else if( c & 0x80)								/* status byte		*/
		status= c;
	else if( status)								/* running status	*/
	{
		ungetc (c, fd->fd);							/* unget the char	*/
		fd->_cnt++;
		c= status;
	}
	else
	{
		MidiFile_errno= MidiFileErrFrmt;
		c= 0;
	}
	if( c)
	{
		if( (ev= (* ReadEvTbl[(c<SysEx) ? (c & 0x7F)/16 : c- SysEx+7])( fd, c)))
			if( c < SysEx)
				Canal(ev)= c%16;
	}
	if( (ferror( fd->fd) || feof( fd->fd)) && ev)
	{
		fd->fMidi->FreeEv( ev);
		ev= 0;
		if( feof( fd->fd))
			MidiFile_errno= MidiFileErrFrmt;
	}
	return ev;
}

/*--------------------------------------------------------------------------*/
MidiEvPtr MIDIFile::ReadEv()
{
	MidiFile_errno= MidiFileNoErr;
	if( isTrackOpen())							/* if the track is opened		*/
	{
		if( fMidiFile._cnt > 0)					/* if there are events to read	*/
		{
			if( mdf_GetDate( &fMidiFile)) {			/* read the next event date		*/
				MidiEvPtr ev = ReadEv( &fMidiFile);	/* read the event				*/
				if( ev ) Date( ev)= fMidiFile.curDate;	/* store its date				*/
				else MidiFile_errno = MidiFileErrEvs;
				return ev;
			}
		}
		else fMidiFile.opened= false;				/* otherwise: close the track	*/
	}
	else MidiFile_errno= MidiFileErrTrackClose;
	return 0;
}

/*--------------------------------------------------------------------------*/
MidiSeqPtr MIDIFile::ReadTrack()
{
	MidiFile_errno= MidiFileNoErr;
	if( OpenTrack()) {							/* open the track			*/
		MidiSeqPtr seq = fMidiFile.fMidi->NewSeq();	/* allocate a sequence 		*/
		if( seq) {
			while( isTrackOpen()) {				/* while there are events	*/
				MidiEvPtr ev = ReadEv();		/* read these events		*/
				if( ev ) {
					if( !seq->first)			/* store them to the seq 	*/
						seq->first= seq->last= ev;
					else {
						Link(seq->last)= ev;
						seq->last= ev;
					}
				}
				else if( isMidiFileError()) {	/* if it's an error			*/
					fMidiFile.fMidi->FreeSeq( seq);			/* free the sequence		*/
					seq= nil;					/* to return error			*/
					break;
				}						/* otherwise it's an ignored event */
			}
			return seq;
		}
		else MidiFile_errno= MidiFileErrEvs;
	}
	return 0;
}



/*--------------------------------------------------------------------------*/
/* Opening and closing a MIDI file											*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
static const char *mdf_getStdMode( short mode)
{
	switch( mode)
	{
		case MidiFileRead:
			return "rb";
		case MidiFileAppend:
			return "rb+";
		default:
			MidiFile_errno= MidiFileErrMode;
			return nil;
	}
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::ErrOpen()
{
	if( fMidiFile.fd) fclose( fMidiFile.fd);	/* file opened: we close it 		*/
	fMidiFile.fd = 0;
	if( fMidiFile.mode && fMidiFile.keyOff)
		fMidiFile.fMidi->FreeSeq( fMidiFile.keyOff);		/* allocated sequence: we free it 	*/
	return false;
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::Init()
{
	fMidiFile.opened= false;
	fMidiFile.curDate= 0;
	if( fMidiFile.mode)
	{
		if( !(fMidiFile.keyOff= fMidiFile.fMidi->NewSeq()))
			return false;
	}
	else fMidiFile.keyOff= nil;
	return true;
}

/*--------------------------------------------------------------------------*/
bool MIDIFile::Open( const char *filename, short mode)
{
	const char *stdMode;			/* standard opening mode	*/
	MidiFile_errno= MidiFileNoErr;
	fMidiFile.fd = 0;
	fMidiFile.mode= 0;								/* standard initialization	*/
	if( (stdMode= mdf_getStdMode( mode)) &&			/* opening mode control		*/
		(fMidiFile.fd= fopen( filename, stdMode)))	/* file opening				*/
	{
		if( ReadMdfHeader( &fMidiFile))				/* read the file header		*/
		{
			if( fMidiFile.format== midifile0 && mode== MidiFileAppend)
			{									/* error :					*/
				MidiFile_errno= MidiFileErrAdd0;/* try to add to a 0 format	*/
				return ErrOpen();				/* file						*/
			} else {
				fMidiFile.mode= (mode== MidiFileAppend ? true : false);
				return Init();
			}
		}
	}
	return ErrOpen();
}


/*--------------------------------------------------------------------------*/
bool MIDIFile::Create( const char *name, short format, short timeDef, short ticks)
{
	short time;								/* time representation 		*/

	MidiFile_errno= MidiFileNoErr;
	fMidiFile.fd = fopen( name, "wb+");		/* open the file	*/
	if( fMidiFile.fd )
	{
		fMidiFile.mode= true;				/* writing mode		*/
		fMidiFile.ntrks= 0;					/* tracks count = 0	*/
		fMidiFile.format= format;			/* file format 		*/
		if( timeDef)						/* smpte time		*/
		{
			time= (timeDef | 0x80) << 8;
			time|= (char) ticks;
		}
		else time= ticks & 0x7FFF;
		if (Init() && Create_mdfHeader( &fMidiFile, format, fMidiFile.time = time))
			return true;
	}
	return false;
}

//--------------------------------------------------------------------------
bool MIDIFile::Close()
{
	bool ret1 = true, ret2 = true;					/* return code		*/

	MidiFile_errno= MidiFileNoErr;
	if( fMidiFile.mode)								/* writing mode		*/
	{
		if( isTrackOpen())							/* track still opened	*/
			ret1= CloseTrack();						/* we close it			*/
		if( fMidiFile.keyOff)
			fMidiFile.fMidi->FreeSeq( fMidiFile.keyOff);			/* free the sequence	*/
	}
	ret2 = fclose( fMidiFile.fd) == 0;				/* close the file		*/
	fMidiFile.fd=0;
	return ret1 && ret2;
}

//--------------------------------------------------------------------------
#ifdef WIN32
MFAPI const MDF_versions * MidiFileGetVersion(void)
#else
const MDF_versions * MFAPI MidiFileGetVersion(void)
#endif
{
	static MDF_versions versions;

	versions.src= SRC_VERSION;
	versions.MidiFile= MDF_VERSION;
	return (MDF_versions *)&versions;
}

//--------------------------------------------------------------------------
MIDIFile::MIDIFile() {
	fMidiFile.format = -1;
	fMidiFile.ntrks = 0;
	fMidiFile.fd = 0;
	fMidiFile.keyOff = 0;
	fMidiFile.curDate = 0;
	fMidiFile.mode = 0;
	fMidiFile.opened = false;
	fMidiFile.fMidi = new MidiLight();
}

MIDIFile::~MIDIFile() {
	if (fMidiFile.fd) fclose (fMidiFile.fd);
	delete fMidiFile.fMidi;
}

//--------------------------------------------------------------------------
int MIDIFile::GetMFErrno (void)		{ return  MidiFile_errno; }
int MIDIFile::GetErrno (void)		{ return errno; }

