The current release of MidiShare for MacOSX uses PortAudio for time synchronization.

PortAudio is a cross platform, open-source, audio I/O library.Ê It lets you write simple audio programs in 'C' that will compile and run on Windows, Macintosh, Unix(OSS), SGI, and BeOS. PortAudio is intended to promote the exchange of audio synthesis software between developers on different platforms, and was recently selected as the audio component of a larger PortMusic project that includes MIDI and sound file support. The PortAudio project and API was proposed by Ross Bencina to the music-dsp mailing list. Many people have since contributed to its development. 

PortAudio provides a very simple API for recording and/or playing sound using a simple callback function.Ê Example programs are included that synthesize sine waves and pink noise, perform fuzz distortion on a guitar, list available audio devices, etc.

For more information see at: http://www.portaudio.com

To get PortAudio source code : 
you can download it from the cvs server:
set CVSROOT to : :pserver:anonymous@www.portaudio.com:/home/cvs
password       : apparently none
module name    : portaudio

Steps to download the source :
1 cd into PortAudio directory
2 log into the cvs repository :
  cvs -d $CVSROOT login
  (no password)
3 checkout the source :
  cvs -d $CVSROOT co -d src portaudio

When you need to update the sources:
just type: cvs update 
