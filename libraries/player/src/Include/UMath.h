/*

  Copyright © Grame 1996-2004

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
  research@grame.fr

*/

#ifndef __UMATH__
#define __UMATH__

#ifdef __Macintosh__
	#ifndef LONG_MAX
	#define LONG_MAX  0xffffffff
	#endif 
#endif

//--------------
// Class UMath 
//--------------
/*!
\brief	Math utilities functions.
*/

class UMath {

	public:
		
		static long Min (long a, long b) { if (a < b) return a; else return b;}
		static long Max (long a, long b) { if (a >= b) return a; else return b;}
		static unsigned long Min (unsigned long a, unsigned long b) { if (a < b) return a; else return b;}
		static unsigned long Max (unsigned long a, unsigned long b) { if (a > b) return a; else return b;}

		static long Abs (long a) { if (a < 0) return -a; else return a;}
		
#ifdef __Macintosh__ 

	#ifdef  __MacOS9__
		#include <ToolUtils.h>
		#include <FixMath.h>

		#ifdef __MacOS9__
			static long CalcLong (long a, long b, long c) 
			{
				wide res;
				long remainder;
				WideMultiply(a, b, &res);
				return WideDivide(&res,c,&remainder);
			}
		
		#else
			static long CalcLong (long a, long b, long c) 
			{
				long temp;
				Int64Bit res;
				LongMul(a, b, &res);
				temp = res.hiLong*(LONG_MAX%c);
				return res.hiLong * (LONG_MAX/c) + temp/c  + res.loLong/c + ((temp%c + res.loLong%c)>(c>>2) ? 1 : 0) ;
			}
	#endif
	
	#endif
#endif
};

#endif
