
#ifndef __TDClient__
#define __TDClient__

enum {
	kEnvironment,
	kOpenClose,
	kWakeupSleep,
	kInfos,
	kConnections,
	kSlots,
	kSendingAndReceiving,
};

#ifndef MODULE
static inline void FEnvironment()		{ IOCTL(kEnvironment); }
static inline void FOpenClose()			{ IOCTL(kOpenClose); }
static inline void FWakeupSleep()		{ IOCTL(kWakeupSleep); }
static inline void FInfos()			{ IOCTL(kInfos); }
static inline void FSlots()			{ IOCTL(kSlots); }
static inline void FSendingAndReceiving()	{ IOCTL(kSendingAndReceiving); }
#endif

#endif
