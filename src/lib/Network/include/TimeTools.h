/*

  Copyright © Grame 2001
  Copyright © Mil Productions 2001

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/


#ifndef __TimeTools__
#define __TimeTools__

#if macintosh
#include <Timer.h>
#elif WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif


typedef struct {
	long	sec;
	long	usec;
} TimeInfo, *TimeInfoPtr;

//________________________________________________________________________
class TimeTools
{
	public:
		static	void GetTime (TimeInfoPtr time);
		static	long TimeToMls (TimeInfoPtr time);
		static	long DiffTime  (TimeInfoPtr t1, TimeInfoPtr t2);
		static	void AddTime   (TimeInfoPtr t1, long usec);
};

//________________________________________________________________________
inline void TimeTools::GetTime (TimeInfoPtr time)
{
#if macintosh
		UnsignedWide t;
		double tmp;
		Microseconds(&t);
		tmp = (double)t.hi * 0xffffffff;
		tmp += t.lo;
		time->sec = tmp / 1000000;
		time->usec = tmp - ((double)time->sec * 1000000);
#elif WIN32
		DWORD t = GetTickCount ();
		time->sec = t / 1000;
		time->usec = (t % 1000) * 1000;
#else
		gettimeofday ((struct timeval *)time, 0);
#endif
}

inline long TimeTools::TimeToMls (TimeInfoPtr time)
{
	long t = time->sec * 1000;
	return t + (time->usec / 1000);
}

inline long TimeTools::DiffTime (TimeInfoPtr t1, TimeInfoPtr t2)
{
	long t = (t1->sec - t2->sec) * 1000000;
	return t + (t1->usec - t2->usec);
}

inline void TimeTools::AddTime (TimeInfoPtr t, long usec)
{
	t->usec += usec;
	if (t->usec >= 1000000) {
		t->sec += 1;
		t->usec -= 1000000;
	}
}

#endif
