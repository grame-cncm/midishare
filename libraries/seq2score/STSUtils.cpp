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


/*---------------------------------*/
/* Utilities for Sequence To Score */
/*---------------------------------*/


/* #include <vcl\vcl.h>  Uncomment for CBuilder compilation*/
/* #pragma hdrstop       Uncomment for CBuilder compilation*/

#include "StsUtils.h"
#ifdef __UseMSThunking__
  #include "ThkFunc.h"
#endif
/*---------------------------------------------------*/
void ShiftDates (MidiSeqPtr s, unsigned long offset)
{
   MidiEvPtr e;
   unsigned long d;

   if(s){
     if((e = FirstEv(s)) != 0){
       d = Date(e);
       if (offset > d) offset = d;
       while(e){
         d = Date(e) - offset;
       #ifdef __UseMSThunking__
         MidiSetDate(e, d);
       #else
         Date(e) = d;
       #endif
         e = Link(e);
       }
     }
   }
}
/*---------------------------------------------------*/
void TicksToMsecs (MidiSeqPtr s, int clicks)
{

 MidiEvPtr e;
 float tickTime = 0.0, msec = 0.0, diffTime = 0.0, msecTime = 0.0;

 if(s){
   e = FirstEv(s);
   while(e){
     diffTime = ((float)Date(e) - tickTime) * msec;
     if(EvType(e) == typeTempo){
       tickTime = (float)Date(e);
     #ifdef __UseMSThunking__
       MidiSetDate(e, diffTime + msecTime);
     #else
       Date(e) = difftime + msecTime;
     #endif
       msecTime = (float)Date(e);
       msec = ((float) MidiGetField(e, 0) / 1000.0) / (float)clicks;
     }
     else{
      #ifdef __UseMSThunking__
        MidiSetDate(e, diffTime + msecTime);
      #else
        Date(e) = diffTime + msecTime;
      #endif
     }
     e = Link(e);
   }
 }
}
/*---------------------------------------------------*/
/* MatchKeyOff : change (KeyOn, KeyOff)              */
/* to Note events  in a sequence                     */
/*---------------------------------------------------*/
void MatchKeyOff (MidiSeqPtr s)
{
  THashTbl h;
  MidiEvPtr prev, e1, e2;
  int p;

  if(s){
    InitHashTbl(&h);
    e1 = FirstEv(s);
    prev = FirstEv(s);
    while(e1){
      if((EvType(e1) == typeKeyOn) && (MidiGetField(e1, 1) > 0)){
        InsertHashTbl(&h, e1);
        prev = e1;
        e1 = Link(e1);
      }
      else if ((EvType(e1) == typeKeyOff) ||
              ((EvType(e1) == typeKeyOn) && (MidiGetField(e1, 1) == 0))){
        p = SearchHashTbl(&h, e1);
        if(p >= 0){
           e2 = h.entry[p];
           h.entry[p] = 0;
           h.space++;
           #ifdef __UseMSThunking__
             MidiSetType(e2, typeNote);
           #else
             EvType(e2) = typeNote;
           #endif
           MidiSetField(e2, 2, Date(e1) - Date(e2));
           #ifdef __UseMSThunking__
             MidiSetLink(prev, MidiGetLink(e1));
           #else
             Link(prev) = Link(e1);
           #endif
           MidiFreeEv(e1);
           e1 = Link(prev);
        }
        else{
           prev = e1;
           e1 = Link(e1);
        }
      }
      else{
           prev = e1;
           e1 = Link(e1);
      }
    }
  }
}
/*---------------------------------------------------*/
void InitHashTbl (THashTblPtr h)
{
    int i;
    h->space = HSIZE;
    for (i = 0; i < HSIZE; i++) h->entry[i] = 0;
}

/*---------------------------------------------------*/
unsigned int HashKey (MidiEvPtr e)
{
 unsigned int ret, f0, ch;
 #ifdef __UseMSThunking__
   f0 = MidiGetField(e, 0);
   ch = Chan(e);
   ret = ((f0 << 4) | ch) & HMASK;
 #else
   ret = ((Pitch(e) << 4) | Chan(e)) & HMASK;
 #endif
  return ret;
}
/*---------------------------------------------------*/
int InsertHashTbl (THashTblPtr h, MidiEvPtr e)
{
   unsigned int k;

   if(h->space > 0){
     h->space--;
     k = HashKey(e);
     while(h->entry[k]){k--; k &= HMASK;}
       h->entry[k] = e;
       return k;
   }
   else return -1;
}
/*---------------------------------------------------*/
int SearchHashTbl (THashTblPtr h, MidiEvPtr e1)
{
    int k, i;
    MidiEvPtr e2;

    k = HashKey(e1);
    for(i = k; i >= 0; i--){
      e2 = h->entry[i];
      if(e2 &&
        (MidiGetField(e1, 0) == MidiGetField(e2, 0)) &&
        (Chan(e1) == Chan(e2)) &&
        (Port(e1) == Port(e2)))
        return i;
      }
      for(i = HMASK; i > k; i--){
        e2 = h->entry[i];
        if(e2 &&
           (MidiGetField(e1, 0) == MidiGetField(e2, 0)) &&
           (Chan(e1) == Chan(e2)) &&
           (Port(e1) == Port(e2)))
           return i;
        }
        return -1;
}
/*---------------------------------------------------*/
