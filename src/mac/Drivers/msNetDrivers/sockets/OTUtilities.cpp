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


#include "OTUtilities.h"


//___________________________________________________________________________
OSStatus OTUtilities::SetOption (SocketRef ref, OTXTILevel lev, OTXTIName name, UInt32 value)
{
	OSStatus err; 
	TOptMgmt in, out; TOption optIn, optOut;

	optIn.len = sizeof (optIn);	
	optIn.level = lev;		// protocol affected
	optIn.name = name;		// option name
	optIn.status = 0; 		// status value	
	optIn.value[0] = value;	// data goes here

	in.opt.len = sizeof (optIn);
	in.opt.buf = (UInt8 *)&optIn;
	in.flags = T_NEGOTIATE;

	optOut.len = out.opt.maxlen = sizeof (optOut);
	out.opt.buf = (UInt8 *)&optOut;
	out.flags = T_NEGOTIATE;
	err = ref->OptionManagement (&in, &out);
	return err;
}

//___________________________________________________________________________
OSStatus OTUtilities::GetOption (SocketRef ref, OTXTILevel lev, OTXTIName name, UInt32 *value)
{
	OSStatus err; 
	TOptMgmt in, out; TOption optIn, optOut;

	optIn.len = sizeof (optIn);	
	optIn.level = lev;
	optIn.name = name;		// option name
	optIn.status = 0; 		// status value	
	optIn.value[0] = 0;		// data goes here

	in.opt.len = sizeof (optIn);
	in.opt.buf = (UInt8 *)&optIn;
	in.flags = T_CURRENT;

	out.opt.maxlen = sizeof (optOut);
	out.opt.buf = (UInt8 *)&optOut;
	out.flags = T_CURRENT;
	err = ref->OptionManagement (&in, &out);
	if (err != noErr) return err;
	if (optOut.status == T_SUCCESS)
		*value = optOut.value[0];
	return err;
}
