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
	return (!PyArg_ParseTuple(args, "s", &name)) ? NULL : Py_BuildValue("i", OpenPlayer(name));
}


static PyObject *
PyClose(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref)) return NULL;
	ClosePlayer(ref);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyStart(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref)) return NULL;
	StartPlayer(ref);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyCont(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref)) return NULL;
	ContPlayer(ref);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyPause(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref)) return NULL;
	PausePlayer(ref);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyStop(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref)) return NULL;
	StopPlayer(ref);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetRecordMode(PyObject *self, PyObject *args)
{
	int ref,mode;
	if (!PyArg_ParseTuple(args, "ii", &ref, &mode)) return NULL;
	SetRecordModePlayer(ref, mode);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyRecord(PyObject *self, PyObject *args)
{
	int ref,track;
	if (!PyArg_ParseTuple(args, "ii", &ref, &track)) return NULL;
	RecordPlayer(ref, track);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetRecordFilter(PyObject *self, PyObject *args)
{
	int ref,filter;
	if (!PyArg_ParseTuple(args, "ii", &ref, &filter)) return NULL;
	SetRecordFilterPlayer(ref, (MidiFilterPtr)filter);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetPosBBU(PyObject *self, PyObject *args)
{
	
}

static PyObject *
PySetPosMs(PyObject *self, PyObject *args)
{
	int ref,date;
	if (!PyArg_ParseTuple(args, "ii", &ref, &date)) return NULL;
	SetPosMsPlayer(ref, date);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetLoop(PyObject *self, PyObject *args)
{
	int ref,loopstate;
	if (!PyArg_ParseTuple(args, "ii", &ref, &loopstate)) return NULL;
	SetLoopPlayer(ref, loopstate);
	Py_INCREF(Py_None);
	return Py_None;
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
	int ref,date;
	return (!PyArg_ParseTuple(args, "ii", &ref, &date)) 
			? NULL
			:  Py_BuildValue("i", SetLoopStartMsPlayer(ref, date));
}

static PyObject *
PySetLoopEndMs(PyObject *self, PyObject *args)
{
	int ref,date;
	return (!PyArg_ParseTuple(args, "ii", &ref, &date)) 
			? NULL
			:  Py_BuildValue("i",SetLoopEndMsPlayer(ref, date));
}

static PyObject *
PySetSynchroIn(PyObject *self, PyObject *args)
{
	int ref,syncstate;
	if (!PyArg_ParseTuple(args, "ii", &ref, &syncstate)) return NULL;
	SetSynchroInPlayer(ref,  syncstate);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetSynchroOut(PyObject *self, PyObject *args)
{
	int ref,syncstate;
	if (!PyArg_ParseTuple(args, "ii", &ref, &syncstate)) return NULL;
	SetSynchroOutPlayer(ref,  syncstate);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetTempo(PyObject *self, PyObject *args)
{
	int ref,tempo;
	if (!PyArg_ParseTuple(args, "ii", &ref, &tempo)) return NULL;
	SetTempoPlayer(ref,  tempo);
	Py_INCREF(Py_None);
	return Py_None;
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
PyForwardStep(PyObject *self, PyObject *args)
{
	int ref,stepstate;
	if (!PyArg_ParseTuple(args, "ii", &ref, &stepstate)) return NULL;
	ForwardStepPlayer(ref,stepstate);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyBackwardStep(PyObject *self, PyObject *args)
{
	int ref,stepstate;
	if (!PyArg_ParseTuple(args, "ii", &ref, &stepstate)) return NULL;
	BackwardStepPlayer(ref,stepstate);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetAllTrack(PyObject *self, PyObject *args)
{
	int ref;
	return (!PyArg_ParseTuple(args, "i", &ref)) 
			? NULL
			: Py_BuildValue("i",GetAllTrackPlayer(ref));
}

static PyObject *
PyGetTrack(PyObject *self, PyObject *args)
{
	int ref,track;
	return (!PyArg_ParseTuple(args, "ii", &ref ,&track)) 
			? NULL
			: Py_BuildValue("i",GetTrackPlayer(ref,track));

}

static PyObject *
PySetAllTrack(PyObject *self, PyObject *args)
{
	int ref,seq,tpq;
	return (!PyArg_ParseTuple(args, "iii", &ref, &seq, &tpq)) 
			? NULL
			: Py_BuildValue("i",SetAllTrackPlayer(ref, (MidiSeqPtr)seq, tpq));
}


static PyObject *
PySetTrack(PyObject *self, PyObject *args)
{
	int ref,track,seq;
	return (!PyArg_ParseTuple(args, "iii", &ref, &track, &seq)) 
			? NULL
			: Py_BuildValue("i",SetTrackPlayer(ref, track, (MidiSeqPtr)seq));
}

static PyObject *
PySetParam(PyObject *self, PyObject *args)
{
	int ref,track, param ,value;
	if (!PyArg_ParseTuple(args, "iiii", &ref, &track, &param, &value)) return NULL;
	SetParamPlayer(ref, track,  param ,  value);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetParam(PyObject *self, PyObject *args)
{
	int ref,track,param;
	return (!PyArg_ParseTuple(args, "iii", &ref, &track, &param)) 
			? NULL
			: Py_BuildValue("i",GetParamPlayer(ref, track, param));
}

static PyObject *
PyInsertAllTrack(PyObject *self, PyObject *args)
{
	int ref,seq;
	if (!PyArg_ParseTuple(args, "ii", &ref, &seq)) return NULL;
	InsertAllTrackPlayer(ref, (MidiSeqPtr)seq);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyInsertTrack(PyObject *self, PyObject *args)
{
	int ref,track, seq;
	if (!PyArg_ParseTuple(args, "iii", &ref, &track, &seq)) return NULL;
	InsertTrackPlayer(ref, track, (MidiSeqPtr)seq);
	Py_INCREF(Py_None);
	return Py_None;
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
	{"Start", PyStart, METH_VARARGS, "StartPlayer"},
	{"Cont", PyCont, METH_VARARGS, "ContPlayer"},
	{"Pause", PyPause, METH_VARARGS, "PausePlayer"},
	{"Stop", PyStop, METH_VARARGS, "StopPlayer"},
	{"SetRecordMode", PySetRecordMode, METH_VARARGS, "SetRecordModePlayer"},
	{"Record", PyRecord, METH_VARARGS, "RecordPlayer"},
	{"SetRecordFilter", PySetRecordFilter, METH_VARARGS, "SetRecordFilterPlayer"},
	{"SetPosBBU", PySetPosBBU, METH_VARARGS, "SetPosBBUPlayer"},
	{"SetPosMs", PySetPosMs, METH_VARARGS, "SetPosMsPlayer"},
	{"SetLoop", PySetLoop, METH_VARARGS, "SetLoopPlayer"},
	{"SetLoopStartBBU", PySetLoopStartBBU, METH_VARARGS, "SetLoopStartBBUPlayer"},
	{"SetLoopEndBBU", PySetLoopEndBBU, METH_VARARGS, "SetLoopEndBBUPlayer"},
	{"SetLoopStartMs", PySetLoopStartMs, METH_VARARGS, "SetLoopStartMsPlayer"},
	{"SetLoopEndMs", PySetLoopEndMs, METH_VARARGS, "SetLoopEndMsPlayer"},
	{"SetSynchroIn", PySetSynchroIn, METH_VARARGS, "SetSynchroInPlayer"},
	{"SetSynchroOut", PySetSynchroOut, METH_VARARGS, "SetSynchroOutPlayer"},
	{"SetTempo", PySetTempo, METH_VARARGS, "SetTempoPlayer"},
	{"SetSMPTEOffset", PySetSMPTEOffset, METH_VARARGS, "SetSMPTEOffsetPlayer"},
	{"GetState", PyGetState, METH_VARARGS, "GetStatePlayer"},
	{"GetEndScore", PyGetEndScore, METH_VARARGS, "GetEndScorePlayer"},
	{"ForwardStep", PyForwardStep, METH_VARARGS, "ForwardStepPlayer"},
	{"BackwardStep", PyBackwardStep, METH_VARARGS, "BackwardStepPlayer"},
	{"GetAllTrack", PyGetAllTrack, METH_VARARGS, "GetAllTrackPlayer"},
	{"GetTrack", PyGetTrack, METH_VARARGS, "SetGetTrack"},
	{"SetAllTrack", PySetAllTrack, METH_VARARGS, "SetAllTrackPlayer"},
	{"SetParam", PySetParam, METH_VARARGS, "SetParamPlayer"},
	{"GetParam", PyGetParam, METH_VARARGS, "GetParamPlayer"},
	{"InsertAllTrack", PyInsertAllTrack, METH_VARARGS, "InsertAllTrackPlayer"},
	{"InsertTrack", PyInsertTrack, METH_VARARGS, "InsertTrackPlayer"},
	{"Load", PyLoad, METH_VARARGS, "LoadPlayer"},
	{"Save", PySave, METH_VARARGS, "SavePlayer"},

	{NULL, NULL}
};

void
initMidiPlayer(void)
{
	Py_InitModule("MidiPlayer", MidiPlayer_methods);
}
