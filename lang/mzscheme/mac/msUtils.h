#include "MidiShare.h"

MidiEvPtr  MidiGetFirstEv(MidiSeqPtr s) { return s->first; }
MidiEvPtr  MidiGetLastEv (MidiSeqPtr s)  { return s->last; }
void       MidiSetFirstEv(MidiSeqPtr s, MidiEvPtr e) { s->first = e; }
void       MidiSetLastEv (MidiSeqPtr s, MidiEvPtr e) { s->last = e; }

MidiEvPtr  MidiGetLink (MidiEvPtr e) { return Link(e); }
void	 MidiSetLink	(MidiEvPtr e, MidiEvPtr next) { Link(e) = next; }
long	 MidiGetDate	(MidiEvPtr e)	{ return Date(e); }
void	 MidiSetDate	(MidiEvPtr e, long d) { Date(e) = d; }
short	 MidiGetRefNum	(MidiEvPtr e)	{ return RefNum(e); }
void	 MidiSetRefNum	(MidiEvPtr e, short r) { RefNum(e) = (Byte)r; }
short	 MidiGetType	(MidiEvPtr e)	{ return EvType(e); }
void	 MidiSetType	(MidiEvPtr e, short t) { EvType(e) = (Byte)t; }
short	 MidiGetChan	(MidiEvPtr e)	{ return Chan(e); }
void	 MidiSetChan	(MidiEvPtr e, short c) { Chan(e) = (Byte)c; }
short	 MidiGetPort	(MidiEvPtr e)	{ return Port(e); }
void	 MidiSetPort	(MidiEvPtr e, short p) { Port(e) = (Byte)p; }

long		 MidiGetData0	(MidiEvPtr e) { return Data(e)[0]; }
long		 MidiGetData1	(MidiEvPtr e) { return Data(e)[1]; }
long		 MidiGetData2	(MidiEvPtr e) { return Data(e)[2]; }
long		 MidiGetData3	(MidiEvPtr e) { return Data(e)[3]; }
void		 MidiSetData0	(MidiEvPtr e, long v) { Data(e)[0]=(Byte)v; }
void		 MidiSetData1	(MidiEvPtr e, long v) { Data(e)[1]=(Byte)v; }
void		 MidiSetData2	(MidiEvPtr e, long v) { Data(e)[2]=(Byte)v; }
void		 MidiSetData3	(MidiEvPtr e, long v) { Data(e)[3]=(Byte)v; }


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

