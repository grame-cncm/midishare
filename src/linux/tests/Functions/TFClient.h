
#ifndef __TestFClient__
#define __TestFClient__

enum {
	kEnvironment,
	kOpenClose,
	kApplConfiguration,
	kConnections,
	kEvents,
	kSequences,
	kTime,
	kSending,
	kMail,
	kTasks,
	kSynchro,
	kTolerance,
	kNoMem,
	kClose
};

#ifndef MODULE
static inline void FEnvironment()		{ IOCTL(kEnvironment, 0); }
static inline void FOpenClose()			{ IOCTL(kOpenClose, 0); }
static inline void FApplConfiguration()		{ IOCTL(kApplConfiguration, 0); }
static inline void FConnections(short free)	{ IOCTL(kConnections, free); }
static inline void FEvents(short free)		{ IOCTL(kEvents, free); }
static inline void FSequences(short free)	{ IOCTL(kSequences, free); }
static inline void FTime()			{ IOCTL(kTime, 0); }
static inline void FSending()			{ IOCTL(kSending, 0); }
static inline void FMail()			{ IOCTL(kMail, 0); }
static inline void FTasks(short free)		{ IOCTL(kTasks, free); }
static inline void FSynchro()			{ IOCTL(kSynchro, 0); }
static inline void FTolerance()			{ IOCTL(kTolerance, 0); }
static inline void FNoMem()			{ IOCTL(kNoMem, 0); }
static inline void FClose()			{ IOCTL(kClose, 0); }
#endif

#endif
