

#include "MidiShareLight.h"
#include "msMemory.h"
#include "msEvents.h"
#include "msSeq.h"


/*******************************************************************************
* 						MidiShare Light reduced API									
*******************************************************************************/
#define kVersion 1.0
#define kEventsChunkSize 4000

//------------------------------------------------------------------------------
MidiLight::MidiLight (unsigned long memsize) : fMemory (0)
{ 
	fMemory = new MSMemory (memsize); 
}

MidiLight::~MidiLight ()
{ 
	delete fMemory; 
}

float MidiLight::GetVersion	(void)				{ return kVersion; }

unsigned long MidiLight::FreeSpace	(void)		{ return fMemory->FreeSpace(); }
unsigned long MidiLight::TotalSpace (void)		{ return fMemory->TotalSpace(); }
unsigned long MidiLight::GrowSpace 	(long n)	{ return fMemory->GrowSpace(n); }

//------------------------------------------------------------------------------
void MidiLight::checkSpace (void)	{ 	if (!fMemory->FreeSpace()) fMemory->GrowSpace (kEventsChunkSize); }

//------------------------------------------------------------------------------
MidiEvPtr MidiLight::NewCell 	(void)
{
	checkSpace();
	return MSNewCellFunction( fMemory->FreeList());
}

void MidiLight::FreeCell (MidiEvPtr e)			{ MSFreeCellFunction (e, fMemory->FreeList()); }

//------------------------------------------------------------------------------
MidiEvPtr MidiLight::NewEv (short typeNum)
{
	checkSpace();
	return MSNewEv(typeNum, fMemory->FreeList());
}

MidiEvPtr MidiLight::CopyEv (MidiEvPtr e)
{
	checkSpace();
	return MSCopyEv (e, fMemory->FreeList());
}

void MidiLight::FreeEv (MidiEvPtr e)		{ MSFreeEv (e, fMemory->FreeList()); }

//------------------------------------------------------------------------------
void MidiLight::SetField 	(MidiEvPtr e, long f, long v)	{ MSSetField(e, f, v); }
long MidiLight::GetField 	(MidiEvPtr e, long f)			{ return MSGetField(e, f); } 
void MidiLight::AddField 	(MidiEvPtr e, long v)
{
	checkSpace();
	return MSAddField (e, v, fMemory->FreeList());
}
long MidiLight::CountFields 	(MidiEvPtr e)					{ return MSCountFields(e); }

//------------------------------------------------------------------------------
MidiSeqPtr MidiLight::NewSeq (void)
{
	checkSpace();
	return MSNewSeq (fMemory->FreeList());
}
void MidiLight::AddSeq 	(MidiSeqPtr s, MidiEvPtr e)			{ MSAddSeq (s, e); }
void MidiLight::FreeSeq (MidiSeqPtr s)						{ MSFreeSeq (s, fMemory->FreeList()); }
void MidiLight::ClearSeq (MidiSeqPtr s)						{ MSClearSeq (s, fMemory->FreeList()); }		
void MidiLight::ApplySeq (MidiSeqPtr s, ApplyProcPtr proc)	{ MSApplySeq (s, proc); } 

