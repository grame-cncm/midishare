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

#ifndef __msSharedMem__
#define __msSharedMem__

typedef void * SharedMemHandler;
#ifdef WIN32
typedef char * 	ShMemID;
#	define keyMaxSize			16
#	define kShMemId		"msPublicMem"
#else
#	include <sys/shm.h>
#	include <sys/ipc.h>

typedef key_t 	ShMemID;
#	define keyMaxSize	sizeof(ShMemID)
#	define kShMemId		0x6d73506d  /* 'msPm' */
#endif


#define MaxFilters 500   /* allows a maximum of 500 filters for the whole system */

#ifdef __cplusplus
extern "C" {
#endif

    void * msSharedMemCreate          (ShMemID id, unsigned long size);
    void * msSharedMemCreateIndexed   (ShMemID baseid, unsigned long size, int indexlimit);
    void   msSharedMemDelete 		  (void *memPtr);
    void * msSharedMemGetID 		  (void *memPtr);
    SharedMemHandler msSharedMemOpen  (ShMemID id, void ** memPtr);
    void   			 msSharedMemClose (SharedMemHandler shm);
    void   			 msSharedMemForceDelete (SharedMemHandler shm);
    
#ifdef __cplusplus
}
#endif

#endif
