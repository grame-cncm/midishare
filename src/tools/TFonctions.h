/*

  Copyright © Grame 1999

  This program is free software; you can redistribute it and modify it under 
  the terms of the GNU General Public License as published by the 
  Free Software Foundation version 2 of the License.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
  more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr


*/


#ifndef __TestFunctions__
#define __TestFunctions__

short Environment();
void OpenClose();
void ApplConfiguration();
void Connections(short freeMem);
void Events(short freeMem);
void Sequences(short freeMem);
void Time();
void Sending();
void Mail();
void Tasks(short freeMem);
void Synchro();
void Tolerance();
void NoMem();
void Close();

#endif
