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
#include <Carbon/Carbon.h>

#include "iiwusynth.h"
#include "MidiShare.h"
#include "msDriverState.h"

/* ----------------------------------*/
/* constants definitions             */


#define	SynthDriverName "iiwusynth"
#define MidiShareDrvRef	127

#define kProfileName "msSynthDriver.ini"
#define kDefaultSoundFont  "Vintage.sf2"

static char* midi_driver = NULL;
static char* midi_device = NULL;
static char* midi_id = NULL;

/* ----------------------------------*/
/* data structures                   */

typedef struct {
	iiwu_synth_t*		synth ;
	iiwu_midi_handler_t* 	midi;
        char*  			soundfont;
        int  			buffersize;
 } DriverData, * DriverDataPtr;

/* ----------------------------------*/
/* some macros                       */
/* ----------------------------------*/

DriverData gData;
static inline DriverDataPtr GetData ()	{ return &gData; }
static void close_iiwusynth ();

/* -----------------------------------------------------------------------------*/
void my_log_function(int level, char* message) {}

/* -----------------------------------------------------------------------------*/
static Boolean open_iiwusynth () 
{
	DriverDataPtr data = GetData ();
    	
	iiwu_synth_settings_t settings = {IIWU_SETTINGS_VERSION, 64 , IIWU_AUDIO | IIWU_REVERB, 
                                        "portaudio", "midishare",  64, 2048, 44100, IIWU_FLOAT_FORMAT  };
									 
        settings.bufsize = data->buffersize;	
     
	/* allocates the synth */
	data->synth = new_iiwu_synth(&settings);
        if (data->synth == NULL) goto error;
        
    /* configure log functions */
	//iiwu_set_log_function(PANIC, my_log_function);
    //iiwu_set_log_function(ERR, my_log_function);
  	//iiwu_set_log_function(WARN, my_log_function);
  	//iiwu_set_log_function(DBG, my_log_function);
         
    /* load the soundfound */
    if (iiwu_synth_sfload(data->synth, GetProfileFullName(data->soundfont)) != 0) goto error;
         
    	/* start the midi handler and link it to the synth */
	data->midi = new_iiwu_midi_handler(data->synth, midi_driver, midi_device,midi_id);
	if (data->midi == NULL) goto error;
        
   	return true;
        
    error:
        close_iiwusynth();
        return false;
}

/* -----------------------------------------------------------------------------*/
static void close_iiwusynth () 
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
Boolean OpenSynth() 
{
        DriverDataPtr data = GetData ();
   
        /* use MidiShare audio buffer size*/ 
        data->buffersize = LoadBufferSize();
        data->soundfont = LoadConfig("Configuration", "SoundFont", GetProfileFullName(kProfileName),kDefaultSoundFont);
       
	if (open_iiwusynth()) {
             LoadSlot ("Output Slots", GetProfileFullName(kProfileName),SynthDriverName);
             return true;
        }else {
             /* tries to use default soundfont */
            data->soundfont = kDefaultSoundFont;
            if (open_iiwusynth()) {
                LoadSlot ("Output Slots", GetProfileFullName(kProfileName),SynthDriverName);
                return true;
            }else{
                return false;
            }
	}
}

/* -----------------------------------------------------------------------------*/
void CloseSynth ()
{	
        DriverDataPtr data = GetData ();
        
        SaveConfig ("Configuration", "SoundFont", data->soundfont,  GetProfileFullName(kProfileName));
        SaveSlot ("Output Slots", GetProfileFullName(kProfileName),SynthDriverName);
	close_iiwusynth ();
}

/* -----------------------------------------------------------------------------*/
Boolean Start() {return OpenSynth();}
void Stop() { CloseSynth ();}

