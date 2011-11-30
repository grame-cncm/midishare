
VERSION = 1.16
F = msjni-64-$(VERSION)

system := $(shell uname -s)
JAVA  = java
PREFIX = cmake/lib

ifeq ($(system), MINGW32_NT-6.1)
  lib = JMidi.dll
	ifeq ($(ARCH), 64)
		JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"
		lib := $(lib) mshare64.dll
		PREFIX=vs2005-64/Release/
		mspath := ../../../src/$(PREFIX)/mshare64.dll
	else 
		lib := $(lib) mshare32.dll
		PREFIX=vs2005/Release/
		mspath := ../../../src/$(PREFIX)/mshare32.dll
	endif
else
	lib =  libJMidi.dylib
endif

ifeq ($(ARCH), 32)
	JOPT := -d32
endif


SRC = $(shell find grame -name "*.java")
OUTCLASS = $(shell find grame -name "*.class")
CLASS = $(patsubst %.java, %.class, $(SRC) )
HEADERS = headers/Midi.h headers/MidiAppl.h 

.PHONY: doc 
.PHONY: headers

class : $(CLASS)

jar : grame.jar

zip : grame.jar JMidi.dll
	[ -d $(F) ] || mkdir $(F)
	rm -rf $(F)/*
	cp grame.jar $(F)
	cp JMidi.dll $(F)
	cp mshare64.dll $(F)
	cp ../jni/src/changelog.txt $(F)

Test : $(lib) Test.class
	$(JAVA) $(JOPT) Test

ifeq ($(ARCH), 64)
TestWin : $(lib) Test.class
	"/c/Program Files/Java/jre6/bin/java.exe" Test
else
TestWin : $(lib) Test.class
	java Test
endif

folder :
	-[ -d grameclasses ] && rm -rf grameclasses
	cp -r grame grameclasses
	-find grameclasses -name CVS -exec rm -rf {} \;
	find grameclasses -name "*.java" -exec rm -rf {} \;

grame.jar : $(CLASS)
	jar cvf grame.jar $(OUTCLASS)

clean :
	rm -f $(OUTCLASS)

headers : $(HEADERS)

libJMidi.dylib: ../jni/cmake/Release/libJMidi-$(VERSION).dylib
	cp ../jni/cmake/Release/libJMidi-$(VERSION).dylib .
	ln -sf libJMidi-$(VERSION).dylib libJMidi.dylib

mshare32.dll: $(mspath) 
	cp $(mspath) .

mshare64.dll: $(mspath) 
	cp $(mspath) .

JMidi.dll : ../jni/$(PREFIX)JMidi-$(VERSION).dll
	cp ../jni/$(PREFIX)JMidi-$(VERSION).dll JMidi.dll

##################################################################
# rule for java compiling
%.class: %.java
	javac $< 

headers/%.h: grame/midishare/%.class
	javah -d headers $(patsubst grame/midishare/%.class, grame.midishare.%, $<) 
