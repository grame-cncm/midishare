=============================
MidiShare interface for Java
=============================

The MidiShare/Java package is a set of Java classes which give access to most of MidiShare
functions. It allows to develop Midi applications and Midi applets which can be embeded in
Web pages. Midi applets/applications will be able to send and receive Midi events, schedules
tasks and communicate in real-time with other Java Midi applets/applications or with non-Java
MidiShare applications. 

The package is organized as follows:

- The JMidi native library contains most of MidiShare functions. 
- The JPlayer native library implement a multi-tracks and synchronisable MidiShare
  sequencer. 

===================
The natives classes
===================

The functions defined in these two native libraries are accessed in Java with the following
classes:

- The Midi class gives access to the MidiShare functions through the JMidi native library. 
- The MidiPlayer class gives access to the Player functions through the JPlayer native library. 

Because of the Java the security features, theses files must be installed on the client machine
before it can use Midi applets.

================
Other classes
================

The MidiAppl, MidiTask and MidiFileStream classes provide additional features to develop Midi 
applets/applications in Java:

- The MidiAppl class is a Midi application skeleton and will be derived to implement
  concrete Midi applets/applications. 
- The MidiTask allows Midi applets/applications to schedule tasks to be executed in
  the future. 
- The MidiFileStream allows a Midi applet/application to read or write MIDIFiles. 


===========================
Publication of source code
===========================

The "native" folder contains all the Java classes needed to produce natives classes. 
The "jni" folder contains sources to produce the JNI interface.