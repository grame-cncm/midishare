
#ifndef __TestFiltersClient__
#define __TestFiltersClient__

enum {
	kOpen,
	kPort,
	kCanaux,
	kEvents,
	kClose
};

#ifndef MODULE
static inline void FOpen()	{ IOCTL(kOpen, 0); }
static inline void FPorts()	{ IOCTL(kPort, 0); }
static inline void FCanaux()	{ IOCTL(kCanaux, 0); }
static inline void FEvents()	{ IOCTL(kEvents, 0); }
static inline void FClose()	{ IOCTL(kClose, 0); }
#endif

#endif
