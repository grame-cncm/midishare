
#ifndef __TestEClient__
#define __TestEClient__

enum {
	kOpen,
	kClose,
	kChanEvents,
	kCommonEvents,
	kSystemeEx,
	kStream,
	kPrivate,
	kProcess,
	kQFToMidiFile,
	kMidiFile,
	kReserved
};

#ifndef MODULE
static inline void FOpen()		{ IOCTL(kOpen); }
static inline void FClose()		{ IOCTL(kClose); }
static inline void FChanEvents()	{ IOCTL(kChanEvents); }
static inline void FCommonEvents()	{ IOCTL(kCommonEvents); }
static inline void FSystemeEx()		{ IOCTL(kSystemeEx); }
static inline void FStream()		{ IOCTL(kStream); }
static inline void FPrivate()		{ IOCTL(kPrivate); }
static inline void FProcess()		{ IOCTL(kProcess); }
static inline void FQFToMidiFile()	{ IOCTL(kQFToMidiFile); }
static inline void FMidiFile()		{ IOCTL(kMidiFile); }
static inline void FReserved()		{ IOCTL(kReserved); }
#endif

#endif
