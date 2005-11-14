/*

  Copyright © Grame 1999

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
  grame@rd.grame.fr

*/


/********************************************************************************/
/* SeekOutputStream class : internal class to manage seekable stream for output
/********************************************************************************/

package grame.midishare.midifile;
import java.io.*;

/**
This class is used internally. 
*/

public class SeekOutputStream extends ByteArrayOutputStream {

    public synchronized void write(int b, int offset) {
    
		int newcount = count + 1;
		
		if (offset > buf.length) {
		    byte newbuf[] = new byte[Math.max(buf.length << 1, offset)];
		    System.arraycopy(buf, 0, newbuf, 0, count);
		    buf = newbuf;
		}
		
		buf[offset] = (byte)b;
	}	 
	 
	public final void writeShort(int v, int offset) throws IOException {
		buf[offset] = (byte) ((v >>> 8) & 0xFF);
		buf[offset+1] = (byte)((v >>> 0) & 0xFF); 
	}
        
    public final void writeInt(int v,  int offset) throws IOException {
		buf[offset] = (byte)((v >>> 24) & 0xFF);
		buf[offset+1]= (byte)((v >>> 16) & 0xFF);
		buf[offset+2]= (byte)((v >>>  8) & 0xFF);
		buf[offset+3]= (byte)((v >>>  0) & 0xFF);
	}

}

