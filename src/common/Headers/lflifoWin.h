/*

  Copyright © Grame 2001

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public 
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/

#ifndef __LFLIFOWIN__
#define __LFLIFOWIN__

#ifdef __SMP__
#define LOCK lock ; 
#else
#define LOCK
#endif

static inline void lfpush (lifo * lf, cell * cell) 
{
	__asm 
	{
		push	eax
		push	ebx
		push	ecx
		push	edx
		push	esi
		mov		esi, lf
		mov		eax, dword ptr [esi]
		mov		ecx, cell
		mov		edx, dword ptr 4[esi]
	_loop:
		mov		ebx, eax
		inc		ebx
		mov		[ecx], edx
		LOCK cmpxchg8b qword ptr [esi]
		jnz		_loop
		pop		esi
		pop		edx
		pop		ecx
		pop		ebx
		pop		eax
	}
}

static inline cell* lfpop (lifo * lf) 
{
	__asm 
	{
		push	ebx
		push	ecx
		push	edx
		push	esi
		mov		esi, lf
		add		esi, 4
		mov 	edx, dword ptr [esi+4]
		mov  	eax, dword ptr [esi]	
		test	eax, eax
		jz		_end
	_loop:
		mov		ebx, dword ptr [eax]
		mov		ecx, edx
		inc		ecx
		LOCK cmpxchg8b qword ptr [esi]
		jz		_end
		test	eax, eax
		jnz		_loop
	_end:
		pop		esi
		pop		edx
		pop		ecx
		pop		ebx
	}
}

static inline unsigned long lfsize (lifo * lf) 
{
	return lf->ic - lf->oc;
}

#endif
