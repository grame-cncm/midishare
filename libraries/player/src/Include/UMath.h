// ===========================================================================
// The Player Library is Copyright (c) Grame, Computer Music Research Laboratory 
// 1996-1999, and is distributed as Open Source software under the Artistic License;
// see the file "Artistic" that is included in the distribution for details.
//
// Grame : Computer Music Research Laboratory
// Web : http://www.grame.fr/Research
// E-mail : MidiShare@rd.grame.fr
// ===========================================================================


#ifndef __UMATH__
#define __UMATH__


#ifdef __Macintosh__
	#define LONG_MAX  0xffffffff
#endif


class UMath {

	public :
		
	static long Min (long a, long b) { if (a < b) return a; else return b;}
	static long Max (long a, long b) { if (a > b) return a; else return b;}
	static long Abs (long a) { if (a < 0) return -a; else return a;}
	
	#ifdef __Macintosh__
	
		#include <ToolUtils.h>
		#include <FixMath.h>

		#if GENERATINGCFM
			
			static long CalcLong (long a, long b, long c) {
				wide res;
				long remainder;
			
				WideMultiply(a, b, &res);
				return WideDivide(&res,c,&remainder);
			}
		
		#else
			static long CalcLong (long a, long b, long c) {
				long temp;
				Int64Bit res;
			
				LongMul(a, b, &res);
				temp = res.hiLong*(LONG_MAX%c);

				return res.hiLong * (LONG_MAX/c) + temp/c  + res.loLong/c + ((temp%c + res.loLong%c)>(c>>2) ? 1 : 0) ;
			}
		
		
		#endif
	#endif
};


#endif
