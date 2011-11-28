VERSION=1.0
TARGET=MidiShareLight-$(VERSION)

help:
	@echo This makefile is intended for packaging and install only.
	@echo Available targets are: macos linux win32


headers:
	 cp src/MidiShareLight.h $(TARGET)
	 cp src/midifile.h $(TARGET)

macos:
	[ -d $(TARGET) ] || mkdir $(TARGET)
	 cp cmake/Release/libmidisharelight.a $(TARGET)
	 make headers

linux:
	[ -d $(TARGET) ] || mkdir $(TARGET)
	 cp cmake/libmidisharelight.a $(TARGET)
	 make headers

win32:
	[ -d $(TARGET) ] || mkdir $(TARGET)
	 cp cmake/Release/libmidisharelight.a $(TARGET)
	 make headers
