/*
  Copyright © Grame 2002

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
  
  modifications history:
 
*/

#ifndef __TPipesPair__
#define __TPipesPair__

#ifdef WIN32
#	include <windows.h>
#	define sprintf 	wsprintf
#else
#	include <stdio.h>
#endif

#include "TPipe.h"

#define kMaxPipeName	512
typedef struct {
	TPipe  * pipe;
	char 	name[kMaxPipeName];
	int		id;
} PipeDesc;

//___________________________________________________________________
class TPipesPair
{
	public:
		 	 TPipesPair () : fRead(), fWrite() 
		 	 			{ 	fNext = 0; fInfos = 0;
		 	 				fReadDesc.pipe=&fRead;
		 	 				fWriteDesc.pipe=&fWrite;
		 	 			}
	virtual ~TPipesPair ()	{}

		long	Write 	(void *buff, long len) { return fWrite.Write (buff, len); }
		long	Read 	(void *buff, long len) { return fRead.Read (buff, len); }

		PipeDesc * 		GetReadPipe ()		{ return &fReadDesc; }
		PipeDesc * 		GetWritePipe ()		{ return &fWriteDesc; }

		void			Link (TPipesPair * next) 	{ fNext = next; }
		TPipesPair *	Next ()						{ return fNext; }
		
		void	SetInfos (void * infos)		{ fInfos = infos; }
		void *	GetInfos ()					{ return fInfos; }
		int		GetID ()					{ return fReadDesc.id; }

		void	SetName (PipeDesc * d, char *name, int id)
				{ sprintf (d->name, "%s%d", name, id); d->id = id; }
		
	private:
		TPipesPair * fNext;
		TPipe		fRead;
		PipeDesc  	fReadDesc;
		TPipe		fWrite;
		PipeDesc  	fWriteDesc;
		void *  	fInfos;
};

#endif
