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
#include <MidiShare.h>


static PyObject *
PyAddField(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyAddSeq(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyApplySeq(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyAvailEv(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyCall(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyClearSeq(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyClose(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref))
        return NULL;
	MidiClose(ref);
	Py_INCREF(Py_None);
	return Py_None;
}


static PyObject *
PyConnect(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyCopyEv(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyCountAppls(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyCountDTasks(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyCountEvs(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyCountFields(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyDTask (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyExec1DTask(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyExt2IntTime(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyFlushDTasks(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyFlushEvs(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyForgetTask(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyFreeCell(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyFreeEv(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyFreeSeq(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyFreeSpace (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetApplAlarm (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetEv (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetExtTime (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetField (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetFilter (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetIndAppl (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetInfo (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetName (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetNamedAppl (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetRcvAlarm(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetSyncInfo (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetTime (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetVersion(PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiGetVersion());
}


static PyObject *
PyGrowSpace(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyInt2ExtTime (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyIsConnected (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyNewCell (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyNewEv(PyObject *self, PyObject *args)
{
	int type;
	if (!PyArg_ParseTuple(args, "i", &type))
        return NULL;
	return Py_BuildValue("i", MidiNewEv(type));
}

static PyObject *
PyNewSeq (PyObject *self, PyObject *args)
{
	
}




static PyObject *
PyOpen(PyObject *self, PyObject *args)
{
	char *name;
	if (!PyArg_ParseTuple(args, "s", &name))
        return NULL;
	return Py_BuildValue("i", MidiOpen(name));
}


static PyObject *
PySend(PyObject *self, PyObject *args)
{
	int ref,ev;
	if (!PyArg_ParseTuple(args, "ii", &ref, &ev))
        return NULL;
	MidiSend(ref,(MidiEvPtr)ev);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySendAt(PyObject *self, PyObject *args)
{
	int ref,ev,date;
	if (!PyArg_ParseTuple(args, "iii", &ref, &ev,&date))
        return NULL;
	MidiSendAt(ref,(MidiEvPtr)ev,date);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySendIm(PyObject *self, PyObject *args)
{
	int ref,ev;
	if (!PyArg_ParseTuple(args, "ii", &ref, &ev))
        return NULL;
	MidiSendIm(ref,(MidiEvPtr)ev);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetApplAlarm (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetField (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetFilter (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetInfo (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetName (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetRcvAlarm (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetSyncMode (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyShare (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySmpte2Time (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyTask (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyTime2Smpte (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyTotalSpace (PyObject *self, PyObject *args)
{
	
}


/*--------------------------------------------------------------------------*/
/* FOR COMMON FIELDS MANAGEMENT */
/*--------------------------------------------------------------------------*/


static PyObject *
PyGetLink (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetLink (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetDate (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetDate (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetRefnum (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetRefnum (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetType (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetType (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetChan (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetChan (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetPort (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetPort (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetData0 (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetData0 (PyObject *self, PyObject *args)
{
	
}
static PyObject *
PyGetData1 (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetData1 (PyObject *self, PyObject *args)
{
	
}
static PyObject *
PyGetData2 (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetData2 (PyObject *self, PyObject *args)
{
	
}
static PyObject *
PyGetData3 (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetData3 (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetText (PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetText (PyObject *self, PyObject *args)
{
	
}

/*--------------------------------------------------------------------------*/
/* FOR SEQUENCES MANAGEMENT */
/*--------------------------------------------------------------------------*/

static PyObject *
PyGetFirstEv (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetFirstEv (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetLastEv (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetLastEv (PyObject *self, PyObject *args)
{
	
}

/*--------------------------------------------------------------------------*/
/* FOR FILTER MANAGEMENT */
/*--------------------------------------------------------------------------*/

static PyObject *
PyNewFilter (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyFreeFilter (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyAcceptPort (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyAcceptChan(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyAcceptType(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyIsAcceptedPort (PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyIsAcceptedChan(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyIsAcceptedType(PyObject *self, PyObject *args)
{
	
}

/*--------------------------------------------------------------------------*/
/* FOR DRIVER MANAGEMENT */
/*--------------------------------------------------------------------------*/

static PyObject *
PyCountDrivers(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetIndDriver(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetDriverInfos(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyAddSlot(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyGetIndSlot(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PyRemoveSlot(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyGetSlotInfos(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyConnectSlot(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PyIsSlotConnected(PyObject *self, PyObject *args)
{
	
}


static PyObject *
PySetSlotName(PyObject *self, PyObject *args)
{
	
}
static PyMethodDef Midi_methods[] = {
	{"GetVersion", PyGetVersion, METH_VARARGS, "MidiGetVersion"},
	{"Open", PyOpen, METH_VARARGS, "MidiOpen"},
	{"Close", PyClose, METH_VARARGS, "MidiClose"},
	{"NewEv", PyNewEv, METH_VARARGS, "MidiNewEv"},
	{"SendIm", PySendIm, METH_VARARGS, "MidiSendIm"},
	{NULL, NULL}
};

void
initMidi(void)
{
	Py_InitModule("Midi", Midi_methods);
}
