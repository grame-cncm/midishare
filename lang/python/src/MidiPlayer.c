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
  grame@rd.grame.fr

*/

#include "Python.h"
#include <Player.h>



static PyObject *
PyOpen(PyObject *self, PyObject *args)
{
	char *name;
	if (!PyArg_ParseTuple(args, "s", &name))
        return NULL;
	return Py_BuildValue("i", OpenPlayer(name));
}


static PyObject *
PyClose(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref))
        return NULL;
	ClosePlayer(ref);
	Py_INCREF(Py_None);
	return Py_None;
}



static PyObject *
PyStart(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyCont(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyPause(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyStop(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetRecordMode(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyRecord(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetRecordFilter(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetPosBBU(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetPosMs(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetLoop(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetLoopStartBBU(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetLoopEndBBU(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetLoopStartMs(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetLoopEndMs(PyObject *self, PyObject *args)
{
	
}



static PyObject *
PySetSynchroIn(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetSynchroOut(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetTempo(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetSMPTEOffset(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetState(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetEndScore(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetForwardStep(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyBackwardStep(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetAllTrack(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetTrack(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetAllTrack(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetTrack(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetParam(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetParam(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyInsertAllTrack(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyInsertTrack(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySave(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyLoad(PyObject *self, PyObject *args)
{
	
}

static PyMethodDef MidiPlayer_methods[] = {
	{"Open", PyOpen, METH_VARARGS, "OpenPlayer"},
	{"Close", PyClose, METH_VARARGS, "ClosePlayer"},
	{NULL, NULL}
};

void
initMidiPlayer(void)
{
	Py_InitModule("MidiPlayer", MidiPlayer_methods);
}
