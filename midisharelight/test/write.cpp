
#include <iostream>

#include "midifile.h"

using namespace std;



//--------------------------------------------------------------------------
static MidiSeqPtr makeTMap(MidiLight* midi, long tempo)
{
	MidiSeqPtr seq = midi->NewSeq();
	MidiEvPtr e = midi->NewEv(typeTempo);
	Date(e) = 0;
	Tempo(e) = tempo;
	midi->AddSeq(seq, e);

	e = midi->NewEv(typeEndTrack);
	Date(e) = 1000;
	midi->AddSeq(seq, e);
	return seq;
}

//--------------------------------------------------------------------------
static MidiSeqPtr makeSeq(MidiLight* midi, int pitch, short dur, int offset)
{
	MidiSeqPtr seq = midi->NewSeq();
	long date = offset;
	for (int i=0; i<10; i++) {
		MidiEvPtr e = midi->NewEv(typeNote);
		Date(e) = date;
		Dur(e) = dur/2;
		Vel(e) = 100;
		Pitch(e) = pitch;
		date += dur;
		midi->AddSeq(seq, e);
	}
	MidiEvPtr e = midi->NewEv(typeEndTrack);
	Date(e) = date;
	midi->AddSeq(seq, e);
	return seq;
}

//--------------------------------------------------------------------------
static void WriteMIDIFile(const char *name)
{
	MIDIFile file;

	/* we first create a new MIDI file using a format 1	*/
	if( file.Create( name, midifile1, TicksPerQuarterNote, 960))
	{
		/* for the file consistency, the first track 	*/
		/* to write is the tempo map					*/				
		MidiSeqPtr seq = makeTMap (file.midi(), 1000000);
		file.WriteTrack( seq);
		file.midi()->FreeSeq (seq);

		seq = makeSeq(file.midi(), 60, 1000, 0);
		file.WriteTrack( seq);
		file.midi()->FreeSeq (seq);

		seq = makeSeq(file.midi(), 67, 1000, 500);
		file.WriteTrack( seq);
		file.midi()->FreeSeq (seq);
		
		/* and we finaly close the file					*/
		file.Close();
	}
	else cerr << "can't create file " << name << endl;
}

int main (int argc, char * argv[])
{
	for (int i=1; i<argc; i++)
		WriteMIDIFile (argv[i]);
	return 0;
}


