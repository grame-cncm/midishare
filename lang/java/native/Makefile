
VERSION = 1.16
F = msjni-64-$(VERSION)

system := $(shell uname -s)

ifeq ($(system), MINGW32_NT-6.1)
  lib = JMidi.dll
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
JAVA  = java
#JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"

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
	java $(JOPT) Test

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

ifeq ($(ARCH), 64)
JMidi.dll : ../jni/cmake/Release/JMidi64.dll
	cp ../jni/cmake/Release/JMidi64.dll ./JMidi.dll
else
JMidi.dll : ../jni/cmake/Release/JMidi32.dll
	cp ../jni/cmake/Release/JMidi32.dll ./JMidi.dll
endif

##################################################################
# rule for java compiling
%.class: %.java
	javac $< 

headers/%.h: grame/midishare/%.class
	javah -d headers $(patsubst grame/midishare/%.class, grame.midishare.%, $<) 
