
VERSION = 1.17

system := $(shell uname -s)
JAVA	= java
mode	= Release

ifeq ($(system), MINGW32_NT-6.1)
	ifeq ($(ARCH), 64)
		JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"
		lib := JMidi64.dll mshare64.dll
		PREFIX=vs2005-64/$(mode)/
		mspath := ../../../src/$(PREFIX)/mshare64.dll
	else 
		lib := JMidi32.dll mshare32.dll
		PREFIX=vs2005/$(mode)/
		mspath := ../../../src/$(PREFIX)/mshare32.dll
	endif
	F = msjni-$(VERSION)
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

zip : grame.jar
	[ -d $(F) ] || mkdir $(F)
	rm -rf $(F)/grame
	cp -r grame  $(F)
	cp grame.jar $(F)
	cp JMidi??.dll $(F)
	cp mshare??.dll $(F)
	cp msMMSystem.dll $(F)
	cp msMMSystem64.dll $(F)
	cp midishare.ini $(F)
	cp Test.java $(F)
	cp bug.java $(F)
	cp -r doc $(F)
	cp ../jni/src/changelog.txt $(F)

Test : $(lib) Test.class
	$(JAVA) $(JOPT) Test

bug : $(lib) bug.class
	$(JAVA) $(JOPT) bug

BugMS64: $(lib) BugMS64.class
	$(JAVA) $(JOPT) BugMS64

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
macos : libJMidi.dylib

win32 : mshare32.dll JMidi32.dll

win64 : mshare64.dll JMidi64.dll

libJMidi.dylib: ../jni/cmake/$(mode)/libJMidi.$(VERSION).dylib
	cp ../jni/cmake/$(mode)/libJMidi.$(VERSION).dylib .
	ln -sf libJMidi.$(VERSION).dylib libJMidi.dylib
	ln -sf libJMidi.$(VERSION).dylib libJMidi64.dylib

mshare32.dll: $(mspath) 
	cp $(mspath) .

mshare64.dll: $(mspath) 
	cp $(mspath) .


JMidi32.dll: ../jni/$(PREFIX)JMidi32-$(VERSION).dll
	cp ../jni/$(PREFIX)JMidi32-$(VERSION).dll) JMidi32.dll

JMidi64.dll: ../jni/$(PREFIX)JMidi64-$(VERSION).dll 
	cp ../jni/$(PREFIX)JMidi64-$(VERSION).dll) JMidi64.dll

##################################################################
# rule for java compiling
%.class: %.java
	javac $< 

headers/%.h: grame/midishare/%.class
	javah -d headers $(patsubst grame/midishare/%.class, grame.midishare.%, $<) 
