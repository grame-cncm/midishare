#!/bin/sh
h-to-ffi.sh -D__Types__ /System/Library/Frameworks/Midishare.framework/Headers/MidiShare.h
h-to-ffi.sh -D__Types__ -I/System/Library/Frameworks/Midishare.framework/Headers/ /System/Library/Frameworks/Player.framework/Headers/Player.h
