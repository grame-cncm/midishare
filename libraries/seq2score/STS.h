/*
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
*/


/*--------------------------------------------------------*/
/*    MidiShare sequence to CSound score file converter   */
/*--------------------------------------------------------*/
/*                      Version 1.0                       */
/*--------------------------------------------------------*/
/*                  c ALFIO FAZIO 1999                    */
/*--------------------------------------------------------*/

#ifndef __stsH__
#define __stsH__

#include "MidiShare.h"

/*-----------------------------
      Conversion settings
-------------------------------*/

#define typeLastPrivate  typeProcess-1    /* last private event type      */

enum STSFieldOrder{foMshare = 0,  /* p4, p5 order*/
                   foInverse};

enum STSAssignBy{abTrack = 0,     /* Instruments assignement*/
                 abChan};

enum STSPitchConvert{pcPch = 0,   /* Midi pitch conversion*/
                     pcFunc,
                     pcMidi};

enum STSAmpliConvert{acAuto = 0,  /* Midi velocity conversion*/
                     acFunc,
                     acMidi};

enum STSMidiType{mtAll = 0,       /* all supported types*/
                 mtPitched,       /* all supported types with pitch field*/
                 mtUnpitched,     /* all supported types without pitch field*/
                 mtPrivate,       /* code number >= 19 <= 127*/
                 mtCtrls,         /* only controllers*/
                 mtPChange,       /* only program-change*/
                 mtChPress,       /* only channel-pressure*/
                 mtBend};         /* only pitch-bend*/

enum STSTabletype{ttPitch = 0,    /* Type of table to read from file*/
                  ttAmpli};

typedef struct TScore FAR *ScorePtr;  /* Score object pointer*/

/*---------------------------------
    Custom routines pointers
-----------------------------------*/

typedef float (*PitchFunPtr) (ScorePtr sp, long pitch);
typedef float (*AmpliFunPtr) (ScorePtr sp, long ampli);
typedef void  (*CustomFunPtr) (ScorePtr sp, char* str, long e);

/*---------------------------------
    Score object structure
----------------------------------*/

typedef struct TScore
{
   unsigned char *scorefile;   /* Output CSound sco file*/
   unsigned char *addinfile;   /* Additional file (functions and so on)*/
   STSPitchConvert pc;         /* Midi pitch conversion settings*/
   PitchFunPtr PFunc;          /* Custom Midi pitch conversion routine */
   short pAction;              /* Identify action in PFunc*/
   STSAmpliConvert ac;         /* Midi velocity conversion settings*/
   AmpliFunPtr AFunc;          /* Custom Midi velocity conversion routine*/
   short aAction;              /* Identify action in AFunc*/
   int maxamp;                 /* Amplitude value for acAuto: (maxamp / 128) * vel*/
   CustomFunPtr CFunc;         /* Custom routine for MShare Privates events*/
   short cAction;              /* Identify action in CFunc*/
   STSAssignBy ab;             /* Tracks/chans to CSound instruments assignement*/
   STSFieldOrder fo;           /* p4 and p5 parameters order*/
   float defdur;               /* Default duration for non-notes events (msecs)*/
   bool writesection;          /* Write data as CSound score or section*/
   bool shiftref;              /* Shift tracks by one (useful for format0 Midifiles)*/
   float padout;               /* Padding out (f0 statement) value for sections (msecs)*/
   float ptable[128];          /* Custom or default pitch-table for pitch conversion*/
   float atable[128];          /* Custom amp-table for velocity conversion*/
   bool events [149];          /* Events from source sequence to consider*/
   bool trkchn [256];          /* Tracks/Channels from source sequence to consider*/
}TScore;

typedef ScorePtr ScorePtr;

/*---------------------------------
    public functions
-----------------------------------*/

void STS_CloseSession(ScorePtr sp);             /* Dealloc score object memory */
int STS_LoadCustomTable(ScorePtr sp, char* f,   /* Load table from user defined file*/
                        STSTabletype tt,
                        short l, short r);
ScorePtr STS_OpenSession(char* scorefile);      /* Alloc memory and sets defaults */
void STS_SetEventsFilter(ScorePtr sp,           /* Define events to consider*/
                         STSMidiType mt);
void STS_WriteScore (long s, ScorePtr sp);      /* Main file-output routine*/

#endif