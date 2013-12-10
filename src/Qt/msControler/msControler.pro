#-------------------------------------------------
#
# Project created by QtCreator 2010-10-13T16:12:37
#
#-------------------------------------------------

#QT += core gui
QT += widgets printsupport

TARGET = msControler
TEMPLATE = app
DESTDIR = ../bin

SOURCES += 	main.cpp dialog.cpp ../shared/states.c ../../lib/profport.c
HEADERS  += dialog.h ../../lib/profport.h ../shared/states.h
FORMS    += dialog.ui

INCLUDEPATH += ../../include

macx {
	CONFIG+=x86 x86_64
	RC_FILE = msControler.icns
}

win32:LIBS += ../../cmake/release/MidiShare.lib
unix:!macx:LIBS += -L../../cmake -lMidiShare

macx:debug {
	LIBS += -F../../cmake/Debug -framework MidiShare
} 
macx:release {
	LIBS += -F../../cmake/Release -framework MidiShare
}
