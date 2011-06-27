
#ifndef _Included_grame_midishare_MidiExt
#define _Included_grame_midishare_MidiExt

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     grame_midishare_Midi
 * Method:    NewString
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_grame_midishare_Midi_NewString
  (JNIEnv *, jclass);

/*
 * Class:     grame_midishare_Midi
 * Method:    FreeString
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_grame_midishare_Midi_FreeString
  (JNIEnv *, jclass, jint);

/*
 * Class:     grame_midishare_Midi
 * Method:    WriteChar
 * Signature: (IIB)V
 */
JNIEXPORT void JNICALL Java_grame_midishare_Midi_WriteChar
  (JNIEnv *, jclass, jint, jint, jbyte);

/*
 * Class:     grame_midishare_Midi
 * Method:    ReadChar
 * Signature: (II)B
 */
JNIEXPORT jbyte JNICALL Java_grame_midishare_Midi_ReadChar
  (JNIEnv *, jclass, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
