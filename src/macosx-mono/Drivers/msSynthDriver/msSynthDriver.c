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
#include "profport.h"
#include "FilterUtils.h"

/* ----------------------------------*/
/* constants definitions             */

#define kMaxEntryLen	1024
#define PortMaxEntry	10
#define CnxError	-1

#define	SynthDriverName "iiwusynth"
#define MidiShareDrvRef	127
#define MidiShareDirectory "MidiShare"

static char * kProfileName = "msSynthDriver.ini";
static char * kDefaultSoundFont = "Vintage.sf2";

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


static unsigned short CountCnx (char * cnxString);
static short GetCnx (char * cnxString, short index);
static __inline Boolean CnxSeparator(c) { return ((c)==' ') || ((c)=='	'); }
static void close_iiwusynth ();

//________________________________________________________________________
static char * GetProfileFullName (char * name)
{
	static char  buff [1024];
	const char* home = getenv("HOME");
	if (home) {
		sprintf (buff, "%s/%s/%s", home, MidiShareDirectory, name);
		return buff;
	}
	return name;
}

//________________________________________________________________________
static char* LoadConfig (char * section, char* key, char* fullname)
{
	static char buff[kMaxEntryLen];
        int n = get_private_profile_string (section, key, "", buff, kMaxEntryLen, fullname);
        return (n==0) ? NULL : buff ;
}

//________________________________________________________________________
static void SaveConfig (char * section, char* key, char* name, char* fullname)
{
        write_private_profile_string (section, key, name, fullname);
}

//________________________________________________________________________
static void LoadSlot (char * section, char* fullname)
{
        SlotRefNum refNum = MidiGetIndSlot(MidiGetNamedAppl(SynthDriverName),1);
        TSlotInfos infos;
        
	  	if (MidiGetSlotInfos (refNum, &infos)) {
			char buff[kMaxEntryLen];
			unsigned long n;
			n= get_private_profile_string (section, infos.name, "", buff, kMaxEntryLen, fullname);
	          	if (n) {
				unsigned short i, c = CountCnx (buff);
				for (i=0; i<c; i++) {
					short port = GetCnx (buff, i);
					if (port != CnxError) {
						MidiConnectSlot (port, refNum, true);
					}
				}
			}
		}
}
//________________________________________________________________________
static void SaveSlot (char * section, char* fullname)
{
        SlotRefNum refNum = MidiGetIndSlot(MidiGetNamedAppl(SynthDriverName),1);
        TSlotInfos infos;
        
        if (MidiGetSlotInfos (refNum, &infos)) {
		char buff[kMaxEntryLen]; int i;
		buff[0] = 0;
                
		for (i=0; i<256; i++) {
			if (IsAcceptedBit (infos.cnx, i)) {
				char numStr[10];
				sprintf (numStr, "%d ", i);
				strcat (buff, numStr);
			}
		}
		write_private_profile_string (section, infos.name, buff, fullname);
	}
}

//________________________________________________________________________
static char * NextCnx (char *ptr, Boolean first)
{
	Boolean skipped = first;
	while (*ptr) {
		if (CnxSeparator(*ptr))	skipped = true;
		else if (skipped) return ptr;
		ptr++;
	}
	return 0;
}

//________________________________________________________________________
static unsigned short CountCnx (char * cnxString)
{
	unsigned short count = 0;
	char * ptr = NextCnx (cnxString, true);
	while (ptr) {
		count++;
		ptr = NextCnx (ptr, false);
	}
	return count;
}

//________________________________________________________________________
static Boolean NullString (char * ptr)
{
	while (*ptr)
		if (*ptr++ != '0') return false;
	return true;
}

//________________________________________________________________________
static short GetCnx (char * cnxString, short index)
{
	short cnx, bufsize=PortMaxEntry; char buff[PortMaxEntry];
	char * dst = buff;
	char * ptr = NextCnx (cnxString, true);
	while (index-- && ptr)
		ptr = NextCnx (ptr, false);
	if (!ptr) return CnxError;
	
	while (*ptr && !CnxSeparator(*ptr) && --bufsize)
		*dst++ = *ptr++;
	if (!bufsize) return CnxError;
	*dst = 0;
	cnx = (short)atol (buff);
	if (!cnx && !NullString (buff)) return CnxError;
	return cnx;
}

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
	iiwu_set_log_function(PANIC, my_log_function);
        iiwu_set_log_function(ERR, my_log_function);
  	iiwu_set_log_function(WARN, my_log_function);
  	iiwu_set_log_function(DBG, my_log_function);
         
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
        data->soundfont = LoadConfig("Configuration", "SoundFont", GetProfileFullName(kProfileName));
        
        /* if the soundfont name is not found in the .ini file, use default soundfont*/
        if (data->soundfont == NULL) data->soundfont = kDefaultSoundFont;
       
	if (open_iiwusynth()) {
             LoadSlot ("Output Slots", GetProfileFullName(kProfileName));
             return true;
        }else {
             /* tries to use default soundfont */
            data->soundfont = kDefaultSoundFont;
            if (open_iiwusynth()) {
                LoadSlot ("Output Slots", GetProfileFullName(kProfileName));
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
        SaveSlot ("Output Slots", GetProfileFullName(kProfileName));
	close_iiwusynth ();
}

/* -----------------------------------------------------------------------------*/
Boolean Start() {return OpenSynth();}
void Stop() { CloseSynth ();}

