#include "MidiShare.h"

MidiEvPtr  MidiGetFirstEv(MidiSeqPtr s) { return s->first; }
MidiEvPtr  MidiGetLastEv (MidiSeqPtr s)  { return s->last; }
void       MidiSetFirstEv(MidiSeqPtr s, MidiEvPtr e) { s->first = e; }
void       MidiSetLastEv (MidiSeqPtr s, MidiEvPtr e) { s->last = e; }



/*--------------------------------------------------------------------------*/
 void pTocCopy(  char *dest,  unsigned char * src)  /* chaine pascal dans chaine c */
{
	register short i;
	
	i= *src++;
	while( i--)
		*dest++= *src++;
	*dest= 0;
}

/*--------------------------------------------------------------------------*/
 void cTocCopy(  char *dest,  char * src) /* chaine c dans chaine c */
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;	
}

/*--------------------------------------------------------------------------*/
 void cTopCopy(  unsigned char *dest,  char * src) /* chaine c dans chaine p */
{
	register short i = 0;
	
	while (src[i] != 0) {
		dest[i+1] = src[i];
		i++;
	}
	dest[0] = i;	
}

