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
#ifndef __TLog__
#define __TLog__

#ifdef WIN32
#	include <windows.h>
#else
#	include <stdio.h>
#	include <string.h>
#	include <errno.h>
#endif

//____________________________________________________________
// log facilities:
// 		TLog objects add messages to the file given as
//		argument to the creator or to the Open method,
//		when no file is specified, messages are written
//		to stderr
//____________________________________________________________
class TLog
{
	public:
		 	 TLog (const char * logpath = 0);
	virtual ~TLog ()	{ Close(); }
			
	static char *	DefaultDir(char *buff, int size);

		void		Open 	(const char * logpath);
		void		Close 	();
		void		Write 	(const char *msg);
		void		WriteErr(const char *msg);

	protected:

	private:
		char * 	DateString (char * buff, short len);
#ifdef WIN32
		char * 	ErrorString ();
		HANDLE 	fLogFile;
#else
		FILE * 	fLogFile;
		char * 	ErrorString ()	{ return strerror(errno); }
#endif
};


#endif
