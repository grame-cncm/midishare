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

#ifndef __msCommInit__
#define __msCommInit__

#ifdef WIN32
#	define kShMemId		"msPublicMem"
#else
#	define kShMemId		0x6d73506d  /* 'msPm' */
#endif

typedef void * CInitHandler;
typedef void * PipesPair;

typedef void (* NewClientProcPtr) (PipesPair p);

#ifdef __cplusplus
extern "C" {
#endif

/* communication initialization */
CInitHandler	CreateClientInit ();
PipesPair		StartClientInit (CInitHandler cih);
void			EndClientInit (CInitHandler cih);

int		ServerMainComInit (NewClientProcPtr proc);
void	ServerMainComStop (int stopThread);

/* pipes management */
void	ClosePipesPair	(PipesPair p);
long	PPWrite 		(PipesPair p, void *buff, long len);
long	PPRead 			(PipesPair p, void *buff, long len);
short	PPID 			(PipesPair p);

void	PPSetInfos 		(PipesPair p, void * infos);
void *	PPGetInfos 		(PipesPair p);


#ifdef __cplusplus
}
#endif

#endif
