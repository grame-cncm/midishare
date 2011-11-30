
VERSION = 1.16

system := $(shell uname -s)
JAVA	= java
mode	= Release

ifeq ($(system), MINGW32_NT-6.1)
  lib = JMidi.dll
	ifeq ($(ARCH), 64)
		JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"
		lib := $(lib) mshare64.dll
		PREFIX=vs2005-64/$(mode)/
		mspath := ../../../src/$(PREFIX)/mshare64.dll
		F = msjni-64-$(VERSION)
	else 
		lib := $(lib) mshare32.dll
		PREFIX=vs2005/$(mode)/
		mspath := ../../../src/$(PREFIX)/mshare32.dll
		F = msjni-32-$(VERSION)
	endif
	JMIDI := ../jni/$(PREFIX)JMidi-$(VERSION).dll
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
	rm -rf $(F)/grame
	cp -r grame  $(F)
	cp grame.jar $(F)
	cp $(JMIDI) $(F)/JMidi.dll
	cp $(mspath) $(F)
	cp Test.java $(F)
	cp ../jni/src/changelog.txt $(F)

Test : $(lib) Test.class
	$(JAVA) $(JOPT) Test

doc:
	[ -d doc ] || mkdir doc
	javadoc -d doc grame/midishare/*.java  grame/midishare/tools/*.java

grame.jar : $(CLASS)
	jar cvf grame.jar $(OUTCLASS)

clean :
	rm -f $(OUTCLASS)

headers : $(HEADERS)

##################################################################
# platform dependant resources

libJMidi.dylib: ../jni/cmake/$(mode)/libJMidi-$(VERSION).dylib
	cp ../jni/cmake/$(mode)/libJMidi-$(VERSION).dylib .
	ln -sf libJMidi-$(VERSION).dylib libJMidi.dylib

mshare32.dll: $(mspath) 
	cp $(mspath) .

mshare64.dll: $(mspath) 
	cp $(mspath) .

JMidi.dll : $(JMIDI)
	cp $(JMIDI) JMidi.dll

##################################################################
# rule for java compiling
%.class: %.java
	javac $< 

headers/%.h: grame/midishare/%.class
	javah -d headers $(patsubst grame/midishare/%.class, grame.midishare.%, $<) 
