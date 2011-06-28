
#ifndef _MidiPtrType__
#define _MidiPtrType__

#ifdef __x86_64__
#define javaptr		jlong
#define javadate	jlong
#else
#define __x86_64__
#define javaptr		jlong
#define javadate	jlong
#endif

#endif
