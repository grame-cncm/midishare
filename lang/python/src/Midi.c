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
	int ev,val;
	if (!PyArg_ParseTuple(args, "ii", &ev,&val)) return NULL;
	MidiAddField((MidiEvPtr)ev,val);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyAddSeq(PyObject *self, PyObject *args)
{
	int seq,ev;
	if (!PyArg_ParseTuple(args, "ii", &seq,&ev)) return NULL;
	MidiAddSeq((MidiSeqPtr)seq,(MidiEvPtr)ev);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyApplySeq(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyAvailEv(PyObject *self, PyObject *args)
{
	int ref;
	return (!PyArg_ParseTuple(args, "i", &ref)) ? NULL : Py_BuildValue("i", MidiAvailEv(ref));
}

static PyObject *
PyCall(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyClearSeq(PyObject *self, PyObject *args)
{
	int seq;
	if (!PyArg_ParseTuple(args, "i", &seq)) return NULL;
	MidiClearSeq((MidiSeqPtr)seq);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyClose(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref)) return NULL;
	MidiClose(ref);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyConnect(PyObject *self, PyObject *args)
{
	int src,dst,state;
	if (!PyArg_ParseTuple(args, "iii", &src, &dst, &state)) return NULL;
	MidiConnect(src,dst,state);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyCopyEv(PyObject *self, PyObject *args)
{	
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", MidiCopyEv((MidiEvPtr)ev));
}

static PyObject *
PyCountAppls(PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiCountAppls());
}

static PyObject *
PyCountDTasks(PyObject *self, PyObject *args)
{
	int ref;
	return (!PyArg_ParseTuple(args, "i", &ref)) ? NULL :  Py_BuildValue("i", MidiCountDTasks(ref));
}

static PyObject *
PyCountEvs(PyObject *self, PyObject *args)
{
	int ref;
	return (!PyArg_ParseTuple(args, "i", &ref)) ? NULL :  Py_BuildValue("i", MidiCountEvs(ref));
}

static PyObject *
PyCountFields(PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL :  Py_BuildValue("i", MidiCountFields((MidiEvPtr)ev));
}

static PyObject *
PyDTask (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyExec1DTask(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyExt2IntTime(PyObject *self, PyObject *args)
{
	int time;
	return (!PyArg_ParseTuple(args, "i", &time)) ? NULL :  Py_BuildValue("i", MidiExt2IntTime(time));
}

static PyObject *
PyFlushDTasks(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref)) return NULL;
	MidiFlushDTasks (ref);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyFlushEvs(PyObject *self, PyObject *args)
{
	int ref;
	if (!PyArg_ParseTuple(args, "i", &ref)) return NULL;
	MidiFlushEvs (ref);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyForgetTask(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyFreeCell(PyObject *self, PyObject *args)
{
	int ev;
	if (!PyArg_ParseTuple(args, "i", &ev)) return NULL;
	MidiFreeCell ((MidiEvPtr) ev);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyFreeEv(PyObject *self, PyObject *args)
{
	int ev;
	if (!PyArg_ParseTuple(args, "i", &ev)) return NULL;
	MidiFreeEv ((MidiEvPtr) ev);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyFreeSeq(PyObject *self, PyObject *args)
{      
	int seq;
	if (!PyArg_ParseTuple(args, "i", &seq)) return NULL;
	MidiFreeSeq ((MidiSeqPtr) seq);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyFreeSpace (PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiFreeSpace());
}

static PyObject *
PyGetApplAlarm (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyGetEv (PyObject *self, PyObject *args)
{
	/* to be checked : a blocking version ?? */
	int ref;
	return (!PyArg_ParseTuple(args, "i", &ref)) ? NULL : Py_BuildValue("i", MidiGetEv(ref));
}

static PyObject *
PyGetExtTime (PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiGetExtTime());
}

static PyObject *
PyGetField (PyObject *self, PyObject *args)
{
	int ev,i;
	return (!PyArg_ParseTuple(args, "ii", &ev,&i)) ? NULL : Py_BuildValue("i", MidiGetField((MidiEvPtr)ev,i));
}

static PyObject *
PyGetFilter (PyObject *self, PyObject *args)
{
	int ref;
	return (!PyArg_ParseTuple(args, "i", &ref)) ? NULL : Py_BuildValue("i", MidiGetFilter(ref));
}

static PyObject *
PyGetIndAppl (PyObject *self, PyObject *args)
{
	int ref;
	return (!PyArg_ParseTuple(args, "i", &ref)) ? NULL : Py_BuildValue("i", MidiGetIndAppl(ref));
}

static PyObject *
PyGetInfo (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyGetName (PyObject *self, PyObject *args)
{
	/* to be finished */
}

static PyObject *
PyGetNamedAppl (PyObject *self, PyObject *args)
{
	char *name;
	return (!PyArg_ParseTuple(args, "s", &name)) ? NULL : Py_BuildValue("i",MidiGetNamedAppl(name));
}

static PyObject *
PyGetRcvAlarm(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyGetSyncInfo (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyGetTime (PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiGetTime());
}

static PyObject *
PyGetVersion(PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiGetVersion());
}

static PyObject *
PyGrowSpace(PyObject *self, PyObject *args)
{
	int space;
	return (!PyArg_ParseTuple(args, "i", &space)) ? NULL : Py_BuildValue("i",MidiGrowSpace(space));
}

static PyObject *
PyInt2ExtTime (PyObject *self, PyObject *args)
{
	int time;
	return (!PyArg_ParseTuple(args, "i", &time)) ? NULL : Py_BuildValue("i",MidiInt2ExtTime(time));
}

static PyObject *
PyIsConnected (PyObject *self, PyObject *args)
{
	int src,dst;
	return (!PyArg_ParseTuple(args, "ii", &src, &dst)) ? NULL : Py_BuildValue("i",MidiIsConnected(src,dst));
}

static PyObject *
PyNewCell (PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiNewCell());
}

static PyObject *
PyNewEv(PyObject *self, PyObject *args)
{
	int type;
	if (!PyArg_ParseTuple(args, "i", &type)) return NULL;
	return Py_BuildValue("i", MidiNewEv(type));
}

static PyObject *
PyNewSeq (PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiNewSeq());
}

static PyObject *
PyOpen(PyObject *self, PyObject *args)
{
	char *name;
	return (!PyArg_ParseTuple(args, "s", &name)) ? NULL : Py_BuildValue("i", MidiOpen(name));
}

static PyObject *
PySend(PyObject *self, PyObject *args)
{
	int ref,ev;
	if (!PyArg_ParseTuple(args, "ii", &ref, &ev)) return NULL;
	MidiSend(ref,(MidiEvPtr)ev);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySendAt(PyObject *self, PyObject *args)
{
	int ref,ev,date;
	if (!PyArg_ParseTuple(args, "iii", &ref, &ev, &date)) return NULL;
	MidiSendAt(ref,(MidiEvPtr)ev,date);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySendIm(PyObject *self, PyObject *args)
{
	int ref,ev;
	if (!PyArg_ParseTuple(args, "ii", &ref, &ev)) return NULL;
	MidiSendIm(ref,(MidiEvPtr)ev);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetApplAlarm (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetField (PyObject *self, PyObject *args)
{
	int ev,i,val;
	if (!PyArg_ParseTuple(args, "iii", &ev, &i, &val)) return NULL;
	MidiSetField((MidiEvPtr)ev,i,val);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetFilter (PyObject *self, PyObject *args)
{
	int ref,filter;
	if (!PyArg_ParseTuple(args, "ii", &ref, &filter)) return NULL;
	MidiSetFilter(ref,(MidiFilterPtr)filter);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetInfo (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetName (PyObject *self, PyObject *args)
{
	int ref;
	char *name;
	if (!PyArg_ParseTuple(args, "is", &ref,&name)) return NULL;
	MidiSetName(ref,name);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetRcvAlarm (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PySetSyncMode (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyShare (PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiShare());
}

static PyObject *
PySmpte2Time (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyTask (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyTime2Smpte (PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyTotalSpace (PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiTotalSpace());
}

/*--------------------------------------------------------------------------*/
/* FOR COMMON FIELDS MANAGEMENT */
/*--------------------------------------------------------------------------*/

static PyObject *
PyGetLink (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", Link((MidiEvPtr)ev));
}

static PyObject *
PySetLink (PyObject *self, PyObject *args)
{
	int ev,dst;
	if (!PyArg_ParseTuple(args, "ii", &ev,&dst)) return NULL;
	Link((MidiEvPtr)ev) = (MidiEvPtr)dst;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetDate (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", Date((MidiEvPtr)ev));
}

static PyObject *
PySetDate (PyObject *self, PyObject *args)
{
	int ev,date;
	if (!PyArg_ParseTuple(args, "ii", &ev,&date)) return NULL;
	Date((MidiEvPtr)ev) = date;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetRefnum (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", RefNum((MidiEvPtr)ev));
}

static PyObject *
PySetRefnum (PyObject *self, PyObject *args)
{
	int ev,ref;
	if (!PyArg_ParseTuple(args, "ii", &ev,&ref)) return NULL;
	RefNum((MidiEvPtr)ev) = ref;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetType (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", EvType((MidiEvPtr)ev));
}

static PyObject *
PySetType (PyObject *self, PyObject *args)
{
	int ev,type;
	if (!PyArg_ParseTuple(args, "ii", &ev,&type)) return NULL;
	EvType((MidiEvPtr)ev) = type;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetChan (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", Chan((MidiEvPtr)ev));
}

static PyObject *
PySetChan (PyObject *self, PyObject *args)
{
	int ev,chan;
	if (!PyArg_ParseTuple(args, "ii", &ev,&chan)) return NULL;
	Chan((MidiEvPtr)ev) = chan;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetPort (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", Port((MidiEvPtr)ev));
}

static PyObject *
PySetPort (PyObject *self, PyObject *args)
{
	int ev,port;
	if (!PyArg_ParseTuple(args, "ii", &ev,&port)) return NULL;
	Port((MidiEvPtr)ev) = port;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetData0 (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", Data((MidiEvPtr)ev)[0]);
}

static PyObject *
PySetData0 (PyObject *self, PyObject *args)
{
	int ev,val;
	if (!PyArg_ParseTuple(args, "ii", &ev,&val)) return NULL;
	Data((MidiEvPtr)ev)[0] = val;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetData1 (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", Data((MidiEvPtr)ev)[1]);
}

static PyObject *
PySetData1 (PyObject *self, PyObject *args)
{
	int ev,val;
	if (!PyArg_ParseTuple(args, "ii", &ev, &val)) return NULL;
	Data((MidiEvPtr)ev)[1] = val;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetData2 (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", Data((MidiEvPtr)ev)[2]);
}

static PyObject *
PySetData2 (PyObject *self, PyObject *args)
{
	int ev,val;
	if (!PyArg_ParseTuple(args, "ii", &ev, &val)) return NULL;
	Data((MidiEvPtr)ev)[2] = val;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetData3 (PyObject *self, PyObject *args)
{
	int ev;
	return (!PyArg_ParseTuple(args, "i", &ev)) ? NULL : Py_BuildValue("i", Data((MidiEvPtr)ev)[3]);
}

static PyObject *
PySetData3 (PyObject *self, PyObject *args)
{
	int ev,val;
	if (!PyArg_ParseTuple(args, "ii", &ev, &val)) return NULL;
	Data((MidiEvPtr)ev)[3] = val;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetText (PyObject *self, PyObject *args)
{
	 /* to be finished */
}

static PyObject *
PySetText (PyObject *self, PyObject *args)
{
	/* to be finished */
	Py_INCREF(Py_None);
	return Py_None;
}

/*--------------------------------------------------------------------------*/
/* FOR SEQUENCES MANAGEMENT */
/*--------------------------------------------------------------------------*/

static PyObject *
PyGetFirstEv (PyObject *self, PyObject *args)
{
	int seq;
	return (!PyArg_ParseTuple(args, "i", &seq)) ? NULL : Py_BuildValue("i",  FirstEv((MidiSeqPtr)seq));
}

static PyObject *
PySetFirstEv (PyObject *self, PyObject *args)
{
	int seq,ev;
	if (!PyArg_ParseTuple(args, "ii", &seq, &ev)) return NULL;
	FirstEv((MidiSeqPtr)seq) = (MidiEvPtr)ev;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetLastEv (PyObject *self, PyObject *args)
{
	int seq;
	return (!PyArg_ParseTuple(args, "i", &seq)) ? NULL : Py_BuildValue("i",  LastEv((MidiSeqPtr)seq));
}

static PyObject *
PySetLastEv (PyObject *self, PyObject *args)
{
	int seq,ev;
	if (!PyArg_ParseTuple(args, "ii", &seq, &ev)) return NULL;
	LastEv((MidiSeqPtr)seq) = (MidiEvPtr)ev;
	Py_INCREF(Py_None);
	return Py_None;
}

/*--------------------------------------------------------------------------*/
/* FOR FILTER MANAGEMENT */
/*--------------------------------------------------------------------------*/

static PyObject *
PyNewFilter (PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiNewFilter());
}

static PyObject *
PyFreeFilter (PyObject *self, PyObject *args)
{
	int filter;
	if (!PyArg_ParseTuple(args, "i", &filter)) return NULL;
	MidiFreeFilter((MidiFilterPtr) filter);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyAcceptPort (PyObject *self, PyObject *args)
{
	int filter,val,c;
	if (!PyArg_ParseTuple(args, "iii", &filter,&val,&c)) return NULL;
	MidiAcceptPort ((MidiFilterPtr) filter,val,c);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyAcceptChan(PyObject *self, PyObject *args)
{
	int filter,val,c;
	if (!PyArg_ParseTuple(args, "iii", &filter,&val,&c)) return NULL;
	MidiAcceptChan ((MidiFilterPtr) filter,val,c);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyAcceptType(PyObject *self, PyObject *args)
{
	int filter,val,c;
	if (!PyArg_ParseTuple(args, "iii", &filter,&val,&c)) return NULL;
	MidiAcceptType ((MidiFilterPtr) filter,val,c);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyIsAcceptedPort (PyObject *self, PyObject *args)
{
	int filter,port;
	return (!PyArg_ParseTuple(args, "ii", &filter,&port)) 
		? NULL
		: Py_BuildValue("i", MidiIsAcceptedPort((MidiFilterPtr)filter,port));
}

static PyObject *
PyIsAcceptedChan(PyObject *self, PyObject *args)
{
	int filter,chan;
	return (!PyArg_ParseTuple(args, "ii", &filter,&chan)) 
		? NULL
		: Py_BuildValue("i", MidiIsAcceptedChan((MidiFilterPtr)filter,chan));
}

static PyObject *
PyIsAcceptedType(PyObject *self, PyObject *args)
{
	int filter,type;
	return (!PyArg_ParseTuple(args, "ii", &filter,&type)) 
		? NULL
		: Py_BuildValue("i", MidiIsAcceptedType((MidiFilterPtr)filter,type));
}

/*--------------------------------------------------------------------------*/
/* FOR DRIVER MANAGEMENT */
/*--------------------------------------------------------------------------*/

static PyObject *
PyCountDrivers(PyObject *self, PyObject *args)
{
	return Py_BuildValue("i", MidiCountDrivers());
}

static PyObject *
PyGetIndDriver(PyObject *self, PyObject *args)
{
	int index;
	return (!PyArg_ParseTuple(args, "i", &index)) ? NULL : Py_BuildValue("i", MidiGetIndDriver(index));
}

static PyObject *
PyGetDriverInfos(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyAddSlot(PyObject *self, PyObject *args)
{
	/*
	int refnum, slotdir;
	char* slotname;
	SlotRefNum ref;
	if (!PyArg_ParseTuple(args, "isi",&refnum,&slotname,&slotdir)) return NULL; 
	ref = MidiAddSlot(refnum, slotname,(SlotDirection)slotdir);
	return Py_BuildValue("i", (*(jint *)&ref));
	*/
}

static PyObject *
PyGetIndSlot(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PyRemoveSlot(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyGetSlotInfos(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyConnectSlot(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
PyIsSlotConnected(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
}

static PyObject *
PySetSlotName(PyObject *self, PyObject *args)
{
	/* Not yet implemented */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef Midi_methods[] = {
        {"AddField", PyAddField, METH_VARARGS, "MidiAddField"},
        {"AddSeq", PyAddSeq, METH_VARARGS, "MidiAddSeq"},
        {"ApplySeq", PyApplySeq, METH_VARARGS, "MidiApplySeq"},
        {"AvailEv", PyAvailEv, METH_VARARGS, "MidiAvailEv"},
        {"Call", PyCall, METH_VARARGS, "MidiCall"},
        {"ClearSeq", PyClearSeq, METH_VARARGS, "MidiClearSeq"},
        {"Close", PyClose, METH_VARARGS, "MidiClose"},
        {"Connect", PyConnect, METH_VARARGS, "MidiConnect"},
        {"CopyEv", PyCopyEv, METH_VARARGS, "MidiCopyEv"},
        {"CountAppls", PyCountAppls, METH_VARARGS, "MidiCountAppls"},
        {"CountDTasks", PyCountDTasks, METH_VARARGS, "MidiCountDTasks"},
        {"CountEvs", PyCountEvs, METH_VARARGS, "MidiCountEvs"},
        {"CountDTasks", PyCountDTasks, METH_VARARGS, "MidiCountDTasks"},
        {"DTask", PyDTask, METH_VARARGS, "MidiDTask"},
        {"Exec1DTask", PyExec1DTask, METH_VARARGS, "MidiExec1DTask"},
        {"Ext2IntTime", PyExt2IntTime, METH_VARARGS, "MidiExt2IntTime"},
        {"FlushDTasks", PyFlushDTasks, METH_VARARGS, "MidiFlushDTasks"},
        {"FlushEvs", PyFlushEvs, METH_VARARGS, "MidiFlushEvs"},
        {"ForgetTask", PyForgetTask, METH_VARARGS, "MidiForgetTask"},
        {"FreeCell", PyFreeCell, METH_VARARGS, "MidiFreeCell"},
        {"FreeEv", PyFreeEv, METH_VARARGS, "MidiFreeEv"},
        {"FreeSeq", PyFreeSeq, METH_VARARGS, "MidiFreeSeq"},
        {"FreeEv", PyFreeEv, METH_VARARGS, "MidiFreeEv"},
        {"FreeSpace", PyFreeSpace, METH_VARARGS, "MidiFreeSpace"},
        {"GetApplAlarm", PyGetApplAlarm, METH_VARARGS, "MidiGetApplAlarm"},
        {"GetEv", PyGetEv, METH_VARARGS, "MidiGetEv"},
        {"GetExtTime", PyGetExtTime, METH_VARARGS, "MidiGetExtTime"},
        {"GetField", PyGetField, METH_VARARGS, "MidiGetField"},
        {"GetFilter", PyGetFilter, METH_VARARGS, "MidiGetFilter"},
        {"GetIndAppl", PyGetIndAppl, METH_VARARGS, "MidiGetIndAppl"},
        {"GetInfo", PyGetInfo, METH_VARARGS, "MidiGetInfo"},
        {"GetName", PyGetName, METH_VARARGS, "MidiGetName"},
        {"GetNamedAppl", PyGetNamedAppl, METH_VARARGS, "MidiGetNamedAppl"},
        {"GetRcvAlarm", PyGetRcvAlarm, METH_VARARGS, "MidiGetRcvAlarm"},
        {"GetSyncInfo", PyGetSyncInfo, METH_VARARGS, "MidiGetSyncInfo"},
        {"GetTime", PyGetTime, METH_VARARGS, "MidiGetTime"},
        {"GetRcvAlarm", PyGetRcvAlarm, METH_VARARGS, "MidiGetRcvAlarm"},
        {"GetVersion", PyGetVersion, METH_VARARGS, "MidiGetVersion"},
        {"GrowSpace", PyGrowSpace, METH_VARARGS, "MidiGrowSpace"},
        {"Int2ExtTime", PyInt2ExtTime, METH_VARARGS, "MidiInt2ExtTime"},
        {"IsConnected", PyIsConnected, METH_VARARGS, "MidiIsConnected"},
        {"NewCell", PyNewCell, METH_VARARGS, "MidiNewCell"},
        {"NewEv", PyNewEv, METH_VARARGS, "MidiNewEv"},
        {"NewSeq", PyNewSeq, METH_VARARGS, "MidiNewSeq"},
		{"Open", PyOpen, METH_VARARGS, "MidiOpen"},
        {"Send", PySend, METH_VARARGS, "MidiSend"},
        {"SendAt", PySendAt, METH_VARARGS, "MidiSendAt"},
        {"SendIm", PySendIm, METH_VARARGS, "MidiSendIm"},
        {"SetApplAlarm", PySetApplAlarm, METH_VARARGS, "MidiSetApplAlarm"},
        {"SetField", PySetField, METH_VARARGS, "MidiSetField"},
        {"SetFilter", PySetFilter, METH_VARARGS, "MidiSetFilter"},
        {"SetInfo", PySetInfo, METH_VARARGS, "MidiSetInfo"},
        {"SetName", PySetName, METH_VARARGS, "MidiSetName"},
        {"SetRcvAlarm", PySetRcvAlarm, METH_VARARGS, "MidiSetRcvAlarm"},
        {"SetSyncMode", PySetSyncMode, METH_VARARGS, "MidiSetSyncMode"},
        {"Share", PyShare, METH_VARARGS, "MidiShare"},
        {"Smpte2Time", PySmpte2Time, METH_VARARGS, "MidiSmpte2Time"},
        {"Task", PyTask, METH_VARARGS, "MidiTask"},
        {"Time2Smpte", PyTime2Smpte, METH_VARARGS, "MidiTime2Smpte"},
        {"TotalSpace", PySetFilter, METH_VARARGS, "MidiTotalSpace"},
        {"GetLink", PyGetLink, METH_VARARGS, "MidiGetLink"},
        {"SetLink", PySetLink, METH_VARARGS, "MidiSetLink"},
        {"GetDate", PyGetDate, METH_VARARGS, "MidiGetDate"},
        {"SetDate", PySetDate, METH_VARARGS, "MidiSetDate"},
        {"GetRefnum", PyGetRefnum, METH_VARARGS, "MidiGetRefnum"},
        {"GetType", PyGetType, METH_VARARGS, "MidiGetType"},
        {"SetType", PySetType, METH_VARARGS, "MidiSetType"},
        {"GetChan", PyGetChan, METH_VARARGS, "MidiGetChan"},
        {"SetChan", PySetChan, METH_VARARGS, "MidiSetChan"},
        {"GetPort", PyGetPort, METH_VARARGS, "MidiGetPort"},
        {"GetData0", PySetData0, METH_VARARGS, "MidiGetData0"},
        {"SetData0", PySetData0, METH_VARARGS, "MidiSetData0"},
        {"GetData1", PySetData1, METH_VARARGS, "MidiGetData1"},
        {"SetData1", PySetData1, METH_VARARGS, "MidiSetData1"},
        {"GetData2", PySetData2, METH_VARARGS, "MidiGetData2"},
        {"SetData2", PySetData2, METH_VARARGS, "MidiSetData2"},
        {"GetData3", PySetData3, METH_VARARGS, "MidiGetData3"},
        {"SetData3", PySetData3, METH_VARARGS, "MidiSetData3"},
        {"GetTest", PyGetText, METH_VARARGS, "MidiGetTest"},
        {"SetText", PySetText, METH_VARARGS, "MidiSetText"},
        {"GetFirstEv", PyGetFirstEv, METH_VARARGS, "MidiGetFirstEv"},
        {"SetFirstEv", PySetFirstEv, METH_VARARGS, "MidiSetFirstEv"},
        {"GetLastEv", PyGetLastEv, METH_VARARGS, "MidiGetLastEv"},
        {"SetLastEv", PySetLastEv, METH_VARARGS, "MidiSetLastEv"},
        {"NewFilter", PyNewFilter, METH_VARARGS, "MidiNewFilter"},
        {"FreeFilter", PyFreeFilter, METH_VARARGS, "MidiFreeFilter"},
        {"AcceptPort", PyAcceptPort, METH_VARARGS, "MidiAcceptPort"},
        {"AcceptChan", PyAcceptChan, METH_VARARGS, "MidiAcceptChan"},
        {"AcceptType", PyAcceptType, METH_VARARGS, "MidiAcceptType"},
        {"IsAcceptedPort", PyIsAcceptedPort, METH_VARARGS, "MidiIsAcceptedPort"},
        {"IsAcceptedChan", PyIsAcceptedChan, METH_VARARGS, "MidiIsAcceptedChan"},
        {"IsAcceptedType", PyIsAcceptedType, METH_VARARGS, "MidiIsAcceptedType"},
        {"CountDrivers", PyCountDrivers, METH_VARARGS, "MidiCountDrivers"},
        {"GetIndDriver", PyGetIndDriver, METH_VARARGS, "MidiGetIndDriver"},
        {"GetDriverInfos", PyGetDriverInfos, METH_VARARGS, "MidiGetDriverInfos"},
        {"AddSlot", PyAddSlot, METH_VARARGS, "MidiAddSlot"},
        {"GetIndSlot", PyGetIndSlot, METH_VARARGS, "MidiGetIndSlot"},
        {"RemoveSlot", PyRemoveSlot, METH_VARARGS, "MidiRemoveSlot"},
        {"GetSlotInfos", PyGetSlotInfos, METH_VARARGS, "MidiGetSlotInfos"},
        {"ConnectSlot", PyConnectSlot, METH_VARARGS, "MidiConnectSlot"},
        {"IsSlotConnected", PyIsSlotConnected, METH_VARARGS, "MidiIsSlotConnected"},
        {"SetSlotName", PySetSlotName, METH_VARARGS, "MidiSetSlotName"},
	
	{NULL, NULL}
};

void
initMidi(void)
{
	Py_InitModule("Midi", Midi_methods);
}
