
#include <iostream>
#include <map>

#include "midifile.h"

using namespace std;

const char * typeNames[] = {
	"Note",
	"KeyOn",
	"KeyOff",
	"KeyPress",
	"CtrlChange",
	"ProgChange",
	"ChanPress",
	"PitchWheel",
	"SongPos",
	"SongSel",
	"Clock",
	"Start",
	"Continue",
	"Stop",
	"Tune",
	"ActiveSens",
	"Reset",
	"SysEx",
	"Stream"
};
//Private         19   /* 19..127 Private events for applications internal use */

const char * typeExtNames[] = {			// from 130 to 149
	"QuarterFrame",
	"Ctrl14b",
	"NonRegParam",
	"RegParam",
	"SeqNum",
	"Textual",
	"Copyright",
	"SeqName",
	"InstrName",
	"Lyric",
	"Marker",
	"CuePoint",
	"ChanPrefix",
	"EndTrack",
	"Tempo",
	"SMPTEOffset",
	"TimeSign",
	"KeySign",
	"Specific",
	"PortPrefix"
};

/*--------------------------------------------------------------------------
 *
 * a typical example of code to read a MIDI file might be 
 * the following one. 
 *
 * Warning! take care of that this code doesn't check for errors
 *
 *--------------------------------------------------------------------------*/
static int count(MidiSeqPtr seq, int& dur)
{
	int n = 0;
	dur = 0;
	MidiEvPtr e = seq->first;
	while (e) {
		n++;
		dur = Date(e);
		e = Link(e);
	}
	return n;
}

static void print(MidiSeqPtr seq)
{
	MidiEvPtr e = seq->first;
	const char * name = "unexpected unknown event type";
	unsigned int t = (unsigned int)EvType(e);
	if (t < typePrivate)			name = typeNames[t];
	else if (t < 128)				name = "unexpected private event";
	else if ((t >= typeQuarterFrame) && (t <= typePortPrefix))
									name = typeExtNames[t-typeQuarterFrame];
	while (e) {
		cout << "  " << Date(e) << " : type " << name ;
		switch (t) {
			case typeNote:
				cout << " duration: " << Dur(e);
			case typeKeyOn:
			case typeKeyOff:
				cout << " pitch: " << int(Pitch(e)) << " vel: " << int(Vel(e));
				break;
		}
		cout << endl;
		e = Link(e);
	}
}

static void ReadMidiFile( const char *name)
{
	MIDIFile file;
	if (file.Open(name, MidiFileRead)) {
		const midiFILE& mf = file.infos();
		int n= mf.ntrks;					/* get the number of tracks */
		cout << name << ": MIDI file format " << mf.format << " - " << n << " tracks" << endl;

		int i = 1;
		while( n--) {
			MidiSeqPtr seq = file.ReadTrack();	/* read every track			*/
			if (seq) {
				int dur, c = count(seq, dur);
				cout << " track " << i++ << ": " << c << " events, duration: " << dur << endl;
				print (seq);
			}
			else cout << " track " << i++ << " null seq" << endl;
			file.midi()->FreeSeq( seq);
		}
		file.Close();
	}
	else cerr << "can't open file " << name << endl;
}

int main (int argc, char * argv[])
{
	for (int i=1; i<argc; i++)
		ReadMidiFile (argv[i]);
	return 0;
}

