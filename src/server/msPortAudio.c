
#include <stdio.h>
#include <string.h>

#include "msPortAudio.h"
#include "msExtern.h"
#include "msLog.h"
#include "portaudio.h"

#ifdef msTimeBench
# include "../benchs/results.h"
#endif

#define SAMPLE_RATE 44100
#define AUDIO_MS_INT (SAMPLE_RATE/100)

#define AUDIO_DEVICE "Built-in audio controller"

static PortAudioStream * gStream;
static long gFrames = 0;
static long gAudioSize = 0;

#ifdef msTimeBench
    static int benchinitialized=0;
#endif

/*_________________________________________________________________________*/
static int AudioClockHandler( void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             PaTimestamp outTime, void *userData )
{
#ifdef msTimeBench
	static TimeType lastTime;
	TimeType t;

	getTime(t);
    if (benchinitialized)
        storeTime (lastTime, t);
    else benchinitialized=1;
	lastTime = t;
#endif
 	gFrames += gAudioSize; 	
	while (gFrames>=AUDIO_MS_INT) {
		ClockHandler((TMSGlobalPtr)userData);
		gFrames-=AUDIO_MS_INT;
	}	
 	return 0; 
}

/*_________________________________________________________________________*/
static long GetAudioBufFSize (int timeres)
{
	switch (timeres) {
		case 1: return 44;
		case 2: return 88;
		case 3: return 132;
		case 4: return 176;
		case 5: return 220;
		case 6: return 264;
		case 7: return 308;
		case 8: return 352;
		case 9: return 397;
		case 10: return 441;
	}
	LogWrite ("Invalid time resolution %d: forced to 2", timeres);
	return 88;
}

/*_________________________________________________________________________*/
void OpenPortAudio (TMSGlobalPtr g, char *dev)
{
	PaError err;
	const PaDeviceInfo* info;
	int device, maxdev;
    err = Pa_Initialize();
  	if( err != paNoError ) goto error_recovery;

  	// Load audio sizr from the .ini file
  	gAudioSize = GetAudioBufFSize(g->clock.timeRes) * 10;
	if (strlen (dev)) {
		// Look for the internal built-in sound device
		maxdev = Pa_CountDevices();
		for (device= 0 ; device<maxdev; device++) {
			info = Pa_GetDeviceInfo(device);
			if ((strcmp (dev,info->name) == 0) && (info->maxOutputChannels)) break;
		}
	}
	else device = Pa_GetDefaultOutputDeviceID();

	// Open Audio stream
    err = Pa_OpenStream(
				&gStream,
				paNoDevice,		/* default input device */
				0,         		/* no input */
				paFloat32, 	
				NULL,
				device, 			
				2,       		/* stereo output */
				paFloat32,     
				NULL,
				SAMPLE_RATE,
				gAudioSize/10,     /* frames per buffer */
				0,              /* number of buffers, if zero then use default minimum */
				paClipOff,      /* we won't output out of range samples so don't bother clipping them */
				AudioClockHandler,
				g );
 
	 if( err != paNoError ) goto error_recovery;
	 err = Pa_StartStream( gStream );
	 if( err != paNoError ) goto error_recovery;
	 return;
	 
error_recovery:
	 LogWrite ("OpenPortAudio: cannot open audio dev \"%s\"", dev);
	 CloseTimeInterrupts(g);
}

/*_________________________________________________________________________*/
void ClosePortAudio ()
{
	if(gStream) Pa_CloseStream(gStream);
	Pa_Terminate();
#ifdef msTimeBench
   benchinitialized = 0;
#endif
}

