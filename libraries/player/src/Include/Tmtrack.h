/* ===========================================================================
* The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
* 1996-1999, and is distributed as Open Source software under the Artistic License;
* see the file "Artistic" that is included in the distribution for details.
*
* Grame : Computer Music Research Laboratory
* Web : http://www.grame.fr/Research
* E-mail : MidiShare@rd.grame.fr
* ===========================================================================
*/


#ifndef __TMTRACK__
#define __TMTRACK__

#include "Midifile.h"

/*********************************** fin ************************************/


#define maxTrack		256			/* le nombre de pistes possibles	*/

/*--------------- constantes définissant les codes d'erreur ------------*/
#define ErrOpen		1			/* ouverture du fichier impossible 	*/
#define ErrRead		2			/* erreur de lecture du fichier		*/
#define ErrWrite	3			/* erreur d'écriture du fichier		*/
#define ErrVol		4			/* erreur détermination du volume 	*/
#define ErrGetInfo 	5			/* erreur lors de GetFInfo			*/
#define ErrSetInfo	6			/* erreur lors de SetFInfo			*/
#define ErrMidiFileFormat	7	/* mauvais format MidiFile 			*/

/*--------------------------------------------------------------------------*/

typedef struct MidiFileInfos  FAR * MidiFileInfosPtr;
typedef struct MidiFileInfos
{
	long format;					/* format du fichier				*/
	long timedef;					/* définition du temps				*/
	long clicks;					/* nbre de pas d'horloge/noire/frame*/
	long tracks;               		/* nombre de tracks                 */

}MidiFileInfos;

#ifdef __cplusplus
extern "C" {
#endif

void SetSeqRef( MidiSeqPtr seq, register short refNum);
char *GetBeginKey( register char *buff);
MidiEvPtr RestoreSeqName( char *buff);
MidiEvPtr GetTrackName( MidiSeqPtr seq, MidiEvPtr *prec);
short GetEvRef( char *buff, int keyLen, short numPiste);
short GetSeqRef( MidiSeqPtr seq, short numPiste);
void DelEndTrack( MidiSeqPtr seq);
MidiEvPtr AddSeq( register MidiEvPtr e1, register MidiEvPtr e2);
void MixeSeq( MidiSeqPtr src, MidiSeqPtr dest);
void UseTrack (MidiSeqPtr seq, MidiSeqPtr dest , int i);
int ReadTracks( register midiFILE *fd, MidiSeqPtr dest);
void InitTrackListe( void);
Boolean WriteEndTrack( register midiFILE *fd, register MidiEvPtr prev);
Boolean WriteSeqName( midiFILE *fd, MidiEvPtr ev, short ref, short numPiste);
Boolean WriteTrackFormat2( register midiFILE *fd, MidiSeqPtr seq,
								  register short ref, short numPiste);
								  
 Boolean WriteTrackFormat1( register midiFILE *fd, MidiSeqPtr seq,
								  register short ref, short numPiste);
								  
Boolean WriteTempoMap( register midiFILE *fd, MidiSeqPtr seq); 
void AnalyseSeq( midiFILE *fd, MidiSeqPtr seq);			
void pCopy( register char *dest, register char * src);			
void cCopy( register char *dest, register char * src);
Boolean WriteTracks( register midiFILE *fd, register MidiSeqPtr seq);	
void SetLoadDates( MidiFileInfosPtr infos, MidiSeqPtr s);
int TryToReadTrack ( register midiFILE *fd, MidiSeqPtr dest, int i);
void ReturnTimeInfos( register midiFILE *fd, MidiFileInfosPtr infos);

void SetSeqPort (MidiSeqPtr se);
short GetPortFromString(MidiEvPtr e);
MidiSeqPtr WritePortInfo (MidiSeqPtr seq);
Boolean IsNormalEv(MidiEvPtr e);
Boolean CheckPort(MidiEvPtr e);
void BuildTable(MidiSeqPtr seq, MidiEvPtr* table);
MidiSeqPtr WriteTempoAndTimeSign (MidiSeqPtr seq);
MidiEvPtr CheckEvType (MidiSeqPtr src, short type);
Boolean MFAPI MidiFileWriteTrack1( MIDIFilePtr fd, MidiSeqPtr seq);
							  
#ifdef __Macintosh__
	#pragma export on
#endif

	int  EXPORT MidiFileSave( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos);
	int  EXPORT MidiFileLoad( char * name, MidiSeqPtr seq, MidiFileInfosPtr infos);

	
#ifdef __Macintosh__
	#pragma export off
#endif

	
#ifdef __cplusplus
}
#endif


#endif


