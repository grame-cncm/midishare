// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
//
// modifications history:
//   [11-12-99] SL - Definition of MidiIsAcceptedType, 
//					 MidiIsAcceptedPort and MidiIsAcceptedChan
// ===========================================================================


#include "MidiFilter.h"

/*--------------------------------------------------------------------------*/

#ifdef __Macintosh__
	
MidiFilterPtr 	MidiNewFilter() { return (MidiFilterPtr)NewPtr(sizeof(TFilter));}
void 		MidiFreeFilter(MidiFilterPtr filter) { DisposePtr((Ptr)filter);}



/*--------------------------------------------------------------------------*/

void MidiAcceptPort (MidiFilterPtr filter, short port, Boolean state)
{
	if (filter) {
		if (state)
			AcceptBit (filter->port, port);
		else
			RejectBit (filter->port, port);
	}
}
/*--------------------------------------------------------------------------*/

void  MidiAcceptChan (MidiFilterPtr filter, short chan, Boolean state)
{
	if (filter) {
		if (state)
			AcceptBit (filter->channel, chan);
		else
			RejectBit (filter->channel, chan);
	}
}
/*--------------------------------------------------------------------------*/

void  MidiAcceptType (MidiFilterPtr filter, short type, Boolean state)
{
	if (filter) {
		if (state)
			AcceptBit (filter->evType, type);
		else
			RejectBit (filter->evType, type);
	}
}

/*--------------------------------------------------------------------------*/

Boolean  MidiIsAcceptedType (MidiFilterPtr filter, short type)
{
	return (filter) ? IsAcceptedBit(filter->evType,type) : false;
}

/*--------------------------------------------------------------------------*/

Boolean  MidiIsAcceptedPort (MidiFilterPtr filter, short port)
{
	return (filter) ? IsAcceptedBit(filter->port,port) : false;
}

/*--------------------------------------------------------------------------*/

Boolean  MidiIsAcceptedChan (MidiFilterPtr filter, short chan)
{
	return (filter) ? IsAcceptedBit(filter->channel,chan): false;
}

#endif