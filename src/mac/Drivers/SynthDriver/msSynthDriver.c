/*

  Copyright © Grame 2002

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


#include <Gestalt.h>
#include <MidiShare.h>
#include "msSynthDriver.h"
#include "SavingState.h"
#include <iiwusynth.h>

/* ----------------------------------*/
/* constants definitions             */

#define MidiShareDrvRef		127
#define StateFile	"\pmsSynthDriver State"	
enum { StateCreator = 'MsST', StateType = 'stST' };	

char* midi_driver = NULL;
char* midi_device = NULL;

/* ----------------------------------*/
/* data structures                   */

typedef struct {
	iiwu_synth_t*		synth ;
	iiwu_midi_handler_t* midi;
	Boolean	*			autoQuit;
	Boolean	*			sflag;
	int             	count;
	Boolean *           loadflag;
	char*				soundfont;
} DriverData, * DriverDataPtr;


/* ----------------------------------*/
/* some macros                       */
/* ----------------------------------*/

DriverData gData;
static inline DriverDataPtr GetData ()	{ return &gData; }

void my_log_function(int level, char* message) {}

/* -----------------------------------------------------------------------------*/
Boolean open_iiwusynth () 
{
	DriverDataPtr data = GetData ();
	iiwu_synth_settings_t settings = {IIWU_SETTINGS_VERSION, 64 , 
										IIWU_AUDIO | IIWU_REVERB, 
										"portaudio", "midishare", 
										 512, 1024, 44100, IIWU_S16_FORMAT 
									 };
	
	/* allocates the synth */
	data->synth = new_iiwu_synth(&settings);
    if (data->synth == NULL) return false;
     
    /* configure log functions */
	iiwu_set_log_function(PANIC, my_log_function);
    iiwu_set_log_function(ERR, my_log_function);
  	iiwu_set_log_function(WARN, my_log_function);
  	iiwu_set_log_function(DBG, my_log_function);
    
    /* load the soundfound */
    if (iiwu_synth_sfload(data->synth, data->soundfont) != 0) return false;
    
	/* start the midi handler and link it to the synth */
	data->midi = new_iiwu_midi_handler(data->synth, midi_driver, midi_device);
	if (data->midi == NULL) return false;
		
	return true;
}

/* -----------------------------------------------------------------------------*/
void close_iiwusynth () 
{
	DriverDataPtr data = GetData ();
	
	if (data->midi) {
    	delete_iiwu_midi_handler(data->midi);
    	data->midi = 0;
  	}
  	if (data->synth) {
    	delete_iiwu_synth(data->synth);
    	data->synth = 0;
  	}
}

/* -----------------------------------------------------------------------------*/
Boolean SetUpMidi (StatePtr state)
{
	DriverDataPtr data = GetData ();
	
	data->autoQuit = &state->autoQuit;
	data->soundfont = state->soundfont;
	data->count = 0;
	data->midi = 0;
	data->synth = 0;
	
	return true;
}

/* -----------------------------------------------------------------------------*/
void CloseMidi ()
{	
	SaveDriverState (MidiGetNamedAppl(SynthDriverName), StateFile, StateCreator, StateType);
	close_iiwusynth ();
}

/* -----------------------------------------------------------------------------*/
Boolean OpenSynth() 
{
	close_iiwusynth ();
	
	if (!open_iiwusynth()){ 
		return false;
	}else {
		RestoreDriverState (MidiGetNamedAppl(SynthDriverName), StateFile);
		return true;
	}
}

/* -----------------------------------------------------------------------------*/
void DoIdle() 
{
	DriverDataPtr data = GetData ();
	int count;
	
	// To detect last application Close
	count = MidiCountAppls();
	if ((count == 0) && (data->count > 0) && *data->autoQuit) {	
		CloseMidi ();
		doneFlag = true;
	}
	data->count = count;
}





