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

import  grame.midishare.*;

/**
A class which contains functions to manipulate MidiShare sequences. More functions
will be available in future release.  
*/

final public class MidiSequence {


	/*--------------------------------------------------------------------------*/
	/**
	Copy a MidiShare sequence.
	*@param src is a pointer to the sequence to be copied.
	*@return The result is a pointer to the new sequence.
	*/

	public static int Copy(int seq) {
	
		int copy = Midi.NewSeq();
		int cur = Midi.GetFirstEv(seq);
		
		if (copy != 0) {
	
			while (cur !=0) {
				Midi.AddSeq(copy, Midi.CopyEv(cur));
				cur= Midi.GetLink(cur);
			}
		}
		return copy;
	}
	

/*--------------------------------------------------------------------------*/
/*    					fonctions de mixage des sequences					*/
/*--------------------------------------------------------------------------*/

static int Add(int e1, int e2)
{
	int  next;
	
	while((next= Midi.GetLink(e1)) != 0)				/* tant qu'une séquence n'est finie */
	{
	
		if( Midi.GetDate(next) <= Midi.GetDate(e2)){	/* date inférieure dans la même seq */
			e1= next;					/* rien à faire : on continue		*/
		}
		else							/* sinon							*/
		{
			Midi.SetLink(e1,e2);		/* on linke avec l'autre séquence	*/
			e1= e2;						/* et on les inverse				*/
			e2= next;
		}
	}
	
	Midi.SetLink(e1, e2);
	if(Midi.GetLink(e1) != 0)			/* linke avec la fin de l'autre séquence 	*/
		while(Midi.GetLink(e2) != 0){
			e2= Midi.GetLink(e2);
		}

	return e2;							/* et renvoie le dernier evt de la séquence */
}

/*--------------------------------------------------------------------------*/
/**
Mix two MidiShare sequences and keep the time ordering of their events.
*@param src is a pointer to the sequence to be mixed in the dest sequence.
*@param dest is a pointer to the destination.
*/

public static void Mix( int src, int dest)
{
	int firstSrc, firstDest;
	
	if((dest != 0) && (src != 0))							/* dest et src existent		*/
	{
		firstSrc= Midi.GetFirstEv(src);
		
		if(firstSrc != 0)              /* src non vide				*/
		{
			firstDest = Midi.GetFirstEv(dest);
			
			if(firstDest == 0)			/* si destination est vide	*/
			{
				Midi.SetFirstEv(dest, firstSrc);        		/* recopie du premier et	*/
				Midi.SetLastEv(dest, Midi.GetLastEv(src));    	/* dernier evt de src		*/

			}
			else if( Midi.GetDate(firstSrc) < Midi.GetDate(firstDest))
													/* 1ier evt source précède	*/
			{										/* le 1ier evt destination	*/
				Midi.SetFirstEv(dest, firstSrc);    /* range dans destination	*/
				Midi.SetLastEv(dest, Add( firstSrc, firstDest));	/* et chainage	*/
			}
			else Midi.SetLastEv(dest, Add( firstDest, firstSrc));	/* et chainage	*/
		}
	}
}


/*--------------------------------------------------------------------------*/
/* 		supprime les fins de pistes qui sont au milieu de la séquence		*/
/*--------------------------------------------------------------------------*/

/**
Delete endTrack events which are in the middle of a sequence.
*@param seq is a pointer to the source sequence.
*/

 public static void DelEndTrack( int seq)
{
	int ev, prev, tmp;
	
	prev= 0;
	ev= Midi.GetFirstEv(seq);
	while( ev != 0)
	{
		if((Midi.GetType(ev)== Midi.typeEndTrack) && (Midi.GetLink(ev) != 0))	/* evt fin de piste		*/
		{												/* n'est pas le dernier */
			if(prev != 0){								/* n'est pas le premier	*/
				Midi.SetLink(prev, Midi.GetLink(ev)); 	/* mod. le chainage		*/
			}else{
				Midi.SetFirstEv(seq, Midi.GetLink(ev)); /* sinon				*/
														/* suivant= premier		*/
			}
			tmp= Midi.GetLink(ev); 	/* sauve le suivant		*/
			Midi.FreeEv(ev);		/* libère l'evt			*/
			ev= tmp;				/* evt courant=suivant	*/
		}
		else 
		{
			prev= ev;
			ev= Midi.GetLink(ev); 
		}
	}
}

/**
Convert a sequence where the dates are in ticks into a sequence where the dates 
are in millisecond by using the tempo informations included in the sequence.
This method is used to convert a sequence read from a MIDIfile with a "SMPTE" time
definition.
*@param seq is a pointer to the source sequence.
*@param fps is the frame_per_second value.
*@param tpf is the ticks_per_frame.
*@see grame.midishare.midifile.MidiFileInfos
*/

public static void TrsfSmpte(int s, int fps, int tpf)
	{
		int	e;
		int	tps;
		
		tps = fps * tpf;
		e = Midi.GetFirstEv(s);
		
		while (e != 0) {
			Midi.SetDate( e, Midi.GetDate(e) * 1000 / tps) ;
			e = Midi.GetLink(e);
		}
	}


	/**
	Convert a sequence where the dates are in ticks into a sequence where the dates 
	are in millisecond by using the tempo informations included in the sequence.
	This method is used to convert a sequence read from a MIDIfile with a 
	"TicksPerQuarterNote" time definition.
	*@param seq is a pointer to the source sequence.
	*@param tick_per_quarter is read in the MIDIdile.
	*@see grame.midishare.midifile.MidiFileInfos
	*/


	public static void TrsfTempo(int seq, double tick_per_quarter) 
	{
		double	tt;	// durée d'un ticks (micro sec)
		double	t1;	// temps du dernier changement de tempo (micro sec)
		double	t2;	// temps actuel (micro sec)
		int	d1;		// date du dernier changement de tempo (ticks)
		
		int	e;
		
		t1 = 0.0;
		tt = 500000.0 / tick_per_quarter;	
		d1 = 0;
		e = Midi.GetFirstEv(seq);
		
		while (e != 0) {
			t2 = t1 + tt * (Midi.GetDate(e) - d1);
			if (Midi.GetType(e) == Midi.typeTempo) {
				tt = Midi.GetField(e, 0) / tick_per_quarter;
				d1 = Midi.GetDate(e);
				t1 = t2;
			}
			Midi.SetDate(e,(int) (t2 / 1000.0)) ;
			e = Midi.GetLink(e);
		}
	}
	
	/**
	Removes an event from a MidiShare sequence.
	*@param seq is a pointer to the sequence.
	*@param e is the event to be removed.
	*@return The result is true if the event has been successfully removed false otherwise.
	*/
	
	public static boolean RemoveEv( int seq, int e)
	{
		int cur = Midi.GetFirstEv(seq);
		int prev = 0;
		
		while(cur != 0)
		{
			if( cur == e) {			
					
				if (prev == 0)									
					Midi.SetFirstEv(seq, Midi.GetLink(cur));			
				else											
					Midi.SetLink(prev, Midi.GetLink(cur));
					
				if (Midi.GetLink(cur) == 0)	Midi.SetLastEv(seq, prev);     
				
				return true;	
					
			}else {
				prev = cur;
				cur = Midi.GetLink(cur);
			}
		}
		return false;
	}
	
	
	/**
	Convert KeyOn/KeyOff pairs in Note events. This method changes the source sequence. KeyOn/KeyOff pairs with a duration
	bigger than 0x7FFF are not transformed (the duration field of a Note event is only 2 bytes long).
	*@param seq is a pointer to the source sequence.
	*/

	public static void MatchKey(int seq) { new MidiMatchKey().MatchKey(seq);}


}


/**
This class is used internally. 
*/

final class MidiMatchKey {

	MidiHashtable  eventTable;
	
	public MidiMatchKey () { eventTable = new MidiHashtable();}
		
		public void MatchKey(int seq) {
			int type;
			int keyon;
			int prevEv,curEv,lastEv;
			prevEv = curEv =  Midi.GetFirstEv(seq);
			lastEv = Midi.GetLastEv(seq);
			
			while (curEv != 0) {
				type = Midi.GetType(curEv);
			
				if ((type == Midi.typeKeyOn) && (Midi.GetField(curEv,1) > 0)) {
					eventTable.put(curEv);
					prevEv = curEv;
					curEv = Midi.GetLink(curEv);
				}else if ((type == Midi.typeKeyOff) 
							|| ((type == Midi.typeKeyOn) && (Midi.GetField(curEv,1) == 0))) {
					
					if ((keyon = eventTable.remove(curEv)) != 0) {
					
						if ((Midi.GetDate(curEv) - Midi.GetDate(keyon)) < 0x7FFF) {
							Midi.SetType(keyon, Midi.typeNote);
							Midi.SetField(keyon,2, Midi.GetDate(curEv) - Midi.GetDate(keyon));
							Midi.SetLink(prevEv, Midi.GetLink(curEv));  // liberation keyOff
							if (curEv == lastEv) {
								Midi.SetLastEv(seq,prevEv); // si on libere le dernier ev
							}
							//pas de gestion du premier ev: un keyOff ne peut pas etre le premier ev
							Midi.FreeEv(curEv);	
							curEv = Midi.GetLink(prevEv);
						}
						
					}else {
						prevEv = curEv;
						curEv = Midi.GetLink(curEv);
					}
				}else{
					prevEv = curEv;
					curEv = Midi.GetLink(curEv);
				}
			}
		}
	
		
	
}


/**
This class is used internally. 
*/


final class MidiHashtable {
	
	static final int HMASK = 511;
	static final int HSIZE = 512;
	int table[];
	int hspace;
	
	public MidiHashtable () { 
		table = new int[HSIZE]; 
		hspace = HSIZE;
	 }

	int HashKey (int e) { return ((Midi.GetField(e,0) << 4) | Midi.GetChan(e)) & HMASK;}
	
	int put (int e) {
       	int k;
        
        if (hspace > 0) {
           hspace--;
           k = HashKey(e);
           while (table[k] != 0) { k--; k &= HMASK; }
           table[k] = e; 
           return k;
        }else{
           return 0;
        }
	}
	
	int remove (int e1) {
        int  k, i, e2;
         
        k = HashKey(e1);
        
        for (i = k; i >= 0; i--) {
              if (((e2 = table[i]) != 0)
                    && (Midi.GetField(e1,0) == Midi.GetField(e2,0))
                    && (Midi.GetChan(e1) == Midi.GetChan(e2))
                    && (Midi.GetPort(e1) == Midi.GetPort(e2))){
                    
                    int res = table[i];
                    table[i] = 0; 
                   	hspace++;
                    return res;
              }
        }
        
        for (i = HMASK; i > k; i--) {
              if (((e2 = table[i]) != 0)
                   && (Midi.GetField(e1,0) == Midi.GetField(e2,0))
                   && (Midi.GetChan(e1) == Midi.GetChan(e2))
                   && (Midi.GetPort(e1) == Midi.GetPort(e2))){
                    
                    int res = table[i];
                    table[i] = 0; 
                   	hspace++;
                    return res;
              }
        }
        
        return 0;
	}
	
}

