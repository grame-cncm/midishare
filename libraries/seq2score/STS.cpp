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

/* #include <vcl\vcl.h>  Uncomment for CBuilder compilation*/
/* #pragma hdrstop       Uncomment for CBuilder compilation*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "STS.h"

#ifdef __UseMSThunking__
  #include "ThkFunc.h"
#endif

/*--------------------------------------------
        Default custom routines

They simply map input values to default 
tables or return input with no modification.
Pointers to these functions are automatically
created by the STS_OpenSession routine. You 
may want to redirect them to your functions. 
----------------------------------------------*/

static float PitchToHertz(ScorePtr sp, long p);      /* Pitch handling*/
static float MapAmplitude(ScorePtr sp, long a);      /* Velocity handling*/
static void  HandlePrivate(ScorePtr sp, char* str,   /* Private events handling*/
                                        long e);

/*-------------------------------------------
      Internal functions
---------------------------------------------*/
static int  STS_AppendToScoreFile(char* f,           /* Merge custom file to output*/
                                  FILE* out);
static long STS_Build14BitValue (long e);            /* Build value from LSB and MSB*/
static void STS_ConvertAmpli(ScorePtr sp,            /* Velocity conversion*/
                             char* str,
                             long amp);
static void STS_ConvertPitch(ScorePtr sp,            /* Pitch conversion*/
                             char* str,
                             long pitch);
static void STS_OrderParams(ScorePtr sp,             /* p4 and p5 order*/
                            char* dest,
                            char* str,
                            char* str2);

static float Cps[]=                                  /* Default hertz table*/
   {8.176, 8.662, 9.177, 9.723, 10.301, 10.913,
    11.562, 12.250, 12.978, 13.750, 14.568, 15.534,
    16.352, 17.324, 18.354, 19.455, 20.602, 21.827,
    23.125, 24.500, 25.957, 27.500, 29.135, 30.868,
    32.703, 34.648, 36.708, 38.891, 41.203, 43.654,
    46.249, 48.999, 51.913, 55.000, 58.270, 61.735,
    65.406, 69.296, 73.416, 77.782, 82.407, 87.307,
    92.499, 97.999, 103.826, 110.000, 116.541, 123.471,
    130.813, 138.591, 146.832, 155.563, 164.814, 174.614,
    184.997, 195.998, 207.652, 220.000, 233.082, 246.942,
    261.626, 277.183, 293.665, 311.127, 329.628, 349.228,
    369.994, 391.995, 415.305, 440.000, 466.164, 493.883,
    523.251, 554.365, 587.330, 622.254, 659.255, 698.456,
    739.989, 783.991, 830.609, 880.000, 932.328, 987.767,
    1046.502, 1108.731, 1174.659, 1244.508, 1318.510, 1396.913,
    1479.978, 1567.982, 1661.219, 1760.000, 1864.655, 1975.533,
    2093.005, 2217.461, 2349.318, 2489.016, 2637.020, 2793.826,
    2959.955, 3135.963, 3322.438, 3520.000, 3729.310, 3951.066,
    4186.009, 4434.922, 4698.636, 4978.032, 5274.041, 5587.652,
    5919.911, 6271.927, 6644.875, 7040.000, 7458.620, 7902.133,
    8372.018, 8869.844, 9397.273, 9956.063, 10548.08, 11175.30,
    11839.82, 12543.85
    };
/*---------------------------------------------------*/
/*---------------------------------------------------*/
/*---------------------------------------------------*/
float MapAmplitude(ScorePtr sp, long a)
{
  float ret;
  switch(sp->aAction){
    case 0:
     ret = sp->atable[a];
     break;
  }
  return ret;
}
/*---------------------------------------------------*/
float PitchToHertz(ScorePtr sp, long p)
{
  float ret;

  switch(sp->pAction){
    case 0:
      ret = sp->ptable[p];
      break;
  }
  return ret;
}
/*---------------------------------------------------*/
void HandlePrivate(ScorePtr sp, char* str, long e)
{
 switch(sp->cAction){
  case 0:  /* Simply write midi values*/
    long f0 = MidiGetField(e, 0);
    long f1 = MidiGetField(e, 1);
    long f2 = MidiGetField(e, 2);
    long f3 = MidiGetField(e, 3);
    sprintf(str, "%03u \t %03u \t %03u \t %03u \t", f0, f1, f2, f3);
    break;
 }
}
/*---------------------------------------------------*/
/*---------------------------------------------------*/
/*---------------------------------------------------*/
long STS_Build14BitValue (long e)
{
   long field0 = MidiGetField(e, 0);
   long field1 = MidiGetField(e, 1);
   long val = (field1 << 7) | field0;
   return val;
}
/*---------------------------------------------------*/
int STS_AppendToScoreFile(char* f, FILE* out)
{
  FILE * in;
  int ret;
  char c;
  if((in = fopen(f, "rt")) != NULL){
    while (!feof(in)){
     if((c = fgetc(in)) != EOF)fputc(c, out);
    }
    fclose(in);
    ret = 1;
  }
  else ret = 0;
  return ret;
}
/*---------------------------------------------------*/
void STS_CloseSession(ScorePtr sp)
{
   if(sp != NULL) free(sp);
}
/*---------------------------------------------------*/
void STS_ConvertAmpli(ScorePtr sp, char* str, long amp)
{
  float p5;

   if(sp->ac == acAuto){
     p5 = ((float)sp->maxamp / 128.) * (float) amp;
     sprintf(str, "%.3f \t", p5);
   }
   if(sp->ac == acFunc) sprintf(str, "%.3f \t", sp->AFunc(sp, amp));
   if(sp->ac == acMidi) sprintf(str, "%03u \t", amp);
}
/*---------------------------------------------------*/
void STS_ConvertPitch(ScorePtr sp, char* str, long pitch)
{
 long p4, p4N;
    if(sp->pc == pcPch){
      p4 = pitch / 12;
      p4 += 3;
      p4N = pitch % 12;
      sprintf(str, "%02u.%02u \t", p4, p4N);
    }
    if(sp->pc == pcFunc){
      sprintf(str, "%.3f \t", sp->PFunc(sp, pitch));
    }
    if(sp->pc == pcMidi){
     sprintf(str, "%03u \t", pitch);
    }
}
/*---------------------------------------------------*/
int STS_LoadCustomTable(ScorePtr sp, char* f,  STSTabletype tt, short l, short r)
{
  
  /* Every line of the text file is an entry in the table.
     Comments (or empty lines) can be created using the ";" sign.
     To avoid the need to build text files with 128 lines every
     time a table is required, an offset can be specified both
     for start and end. So, for instance, a text file with only 12
     lines can be used to map pitches between C3 and C4 if a
     start offset of 60 is specified.
     WARNING: this function doesn't check for atof overflows*/
  
  FILE * in;
  int ret, i = l, ch = ';';
  unsigned int cnt;
  char str[1024];
  char* pd;

  if((in = fopen(f, "rt")) != NULL){
    while (!feof(in)){
     fgets(str, 1024, in);
     if((i >= l && i <= r) && strcspn(str, ";") > 0){
       if((pd = strchr(str, ch)) != NULL){
         cnt = pd - str + 1;
         strncpy(str, str, cnt);
         if(cnt < strlen(str)) strcat(str, "\0");
       }
       switch(tt){
         case ttPitch: sp->ptable[i] = atof(str); break;
         case ttAmpli: sp->atable[i] = atof(str); break;
       }
     }
     i++;
    }
    fclose(in);
    ret = 1;
  }
  else ret = 0;
  return ret;
}
/*---------------------------------------------------*/
ScorePtr STS_OpenSession(char* scorefile)
{
  int i;
  ScorePtr sp = NULL;
  if((sp = (ScorePtr) calloc(1, sizeof(TScore))) != 0){
     sp->scorefile = scorefile;
     sp->addinfile = "NULL";
     STS_SetEventsFilter(sp, mtPitched);
     sp->pc = pcPch;
     sp->PFunc = PitchToHertz;
     sp->maxamp = 5000;
     for(i = 0; i < 128; i++) sp->atable[i] = (sp->maxamp / 128.) * (float) i;
     for(i = 0; i < 128; i++) sp->ptable[i] = Cps[i];
     sp->ac = acAuto;
     sp->AFunc = MapAmplitude;
     sp->CFunc = HandlePrivate;
     sp->defdur = 1000.;
     sp->ab = abTrack;
     sp->fo = foMshare;
     sp->writesection = false;
     sp->padout = 0.;
     sp->shiftref = false;
     sp->trkchn[0] = false;
     for(i = 1; i < 256; i++) sp->trkchn[i] = true;
   }
   return sp;
}
/*---------------------------------------------------*/
void STS_OrderParams(ScorePtr sp, char* dest, char* str, char* str2)
{
  if(sp->fo == foMshare){
    strcat(dest, str);
    strcat(dest, str2);
  }
  else{
    strcat(dest, str2);
    strcat(dest, str);
  }
}
/*---------------------------------------------------*/
void STS_SetEventsFilter(ScorePtr sp, STSMidiType mt)
{
     int i;

  for(i = typeNote; i < typeReserved; i++) sp->events[i] = false;
    switch(mt){
     case mtAll:
       sp->events[typeNote] = true;
       for(i = typeKeyPress; i <= typePitchWheel; i++)    sp->events[i] = true;
       for(i = typeStream + 1; i <= typeLastPrivate; i++) sp->events[i] = true;
       for(i = typeCtrl14b; i <= typeRegParam; i++)       sp->events[i] = true;
       break;
     case mtPitched:
       sp->events[typeNote] = true;
       sp->events[typeKeyPress] = true;
       break;
     case mtUnpitched:
       for(i = typeCtrlChange; i <= typePitchWheel; i++)   sp->events[i] = true;
       for(i = typeStream + 1; i <= typeLastPrivate1; i++) sp->events[i] = true;
       for(i = typeCtrl14b;    i <= typeRegParam; i++)     sp->events[i] = true;
       break;
     case mtPrivate:
       for(i = typeStream + 1; i <= typeLastPrivate; i++) sp->events[i] = true;
       break;
     case mtCtrls:   sp->events[typeCtrlChange] = true; break;
     case mtPChange: sp->events[typeProgChange] = true; break;
     case mtChPress: sp->events[typeChanPress] =  true; break;
     case mtBend:    sp->events[typePitchWheel] = true; break;
   }
}
/*---------------------------------------------------*/
void STS_WriteScore (long s, ScorePtr sp)
{
 MidiEvPtr e = 0;
 float p2, p3;
 long param;
 int ref, chan, type;
 FILE *f;
 char scoreline[1024];
 char str[25];
 char str2[25];

if((f = fopen(sp->scorefile, "wt")) != NULL){
 if(sp->addinfile != "NULL") STS_AppendToScoreFile(sp->addinfile, f);
 e = FirstEv(s);
 if(e){
   while(e){
     type = EvType(e);

     if(!sp->events[type]){e = Link(e); continue;}
     if(sp->ab == abTrack){
       ref = RefNum(e);
       if(sp->shiftref) ref += 1;
       if(!sp->trkchn[ref]){e = Link(e); continue;}
       else sprintf(scoreline, "i%u \t", ref);
     }
     if(sp->ab == abChan){
       chan = Chan(e);
       if(!sp->trkchn[chan]){e = Link(e); continue;}
       else sprintf(scoreline, "i%u \t", chan + 1);
     }
     p2 = (float)Date(e) / 1000.;
     sprintf(str, "%.3f \t", p2); strcat(scoreline, str);
     switch(type){
       case typeNote:
         p3 = (float)MidiGetField(e, 2) / 1000.;
         sprintf(str, "%.3f \t", p3); strcat(scoreline, str);
         STS_ConvertPitch(sp, str,  MidiGetField(e, 0));
         STS_ConvertAmpli(sp, str2, MidiGetField(e, 1));
         STS_OrderParams(sp, scoreline, str, str2);
         break;
       case typeKeyPress:
         sprintf(str, "%.3f \t", sp->defdur / 1000.); strcat(scoreline, str);
         STS_ConvertPitch(sp, str,  MidiGetField(e, 0));
         STS_ConvertAmpli(sp, str2, MidiGetField(e, 1));
         STS_OrderParams(sp, scoreline, str, str2);
         break;
       case typeCtrlChange: 
       case typeCtrl14b: 
       case typeNonRegParam: 
       case typeRegParam:
         sprintf(str, "%.3f \t", sp->defdur / 1000.); strcat(scoreline, str);
         sprintf(str, "%03u \t", MidiGetField(e, 0)); strcat(scoreline, str);
         sprintf(str, "%03u \t", MidiGetField(e, 1)); strcat(scoreline, str);
         break;
       case typeProgChange: 
       case typeChanPress:
         sprintf(str, "%.3f \t", sp->defdur / 1000.); strcat(scoreline, str);
         sprintf(str, "%03u \t", MidiGetField(e, 0)); strcat(scoreline, str);
         break;
       case typePitchWheel:
         sprintf(str, "%.3f \t", sp->defdur / 1000.); strcat(scoreline, str);
         param = STS_Build14BitValue((long)e) - 8192;
         sprintf(str, "%03li \t", param); strcat(scoreline, str);
         break;
       default:
         if(type >= typePrivate && type <= typeLastPrivate){
           sprintf(str, "%.3f \t", sp->defdur / 1000.); strcat(scoreline, str);
           sp->CFunc(sp, str, (long)e);
           strcat(scoreline, str);
         }
         break;
      }
      strcat(scoreline, "\n"); fputs(scoreline, f);
      e = Link(e);
    }
  }
  if(sp->writesection){
     if(sp->padout > 0.){
       sprintf(str, "f0 %.3f\n", sp->padout / 1000.);
       fputs(str, f);
     }
     fputs("s", f);
  }
  else fputs("e", f);
  fclose(f);
 }
}
/*---------------------------------------------------*/


