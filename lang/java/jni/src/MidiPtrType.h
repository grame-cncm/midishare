
#ifndef _MidiPtrType__
#define _MidiPtrType__

#ifdef __x86_64__
#define javaptr		jlong
#define javadate	jlong
#else
#define javaptr		jint
#define javadate	jint
#endif

#endif
