definition module MidiShare_Extensions;

import MidiShare_Clean;

Note:: !Int !Int !Int !Int !Int !State -> (!MidiEvPtr,!State);
KeyOn:: !Int !Int !Int !Int !State -> (!MidiEvPtr,!State);
KeyOff:: !Int !Int !Int !Int !State -> (!MidiEvPtr,!State);
