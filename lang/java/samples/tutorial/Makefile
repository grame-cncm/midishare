
VERSION = 1.17

system := $(shell uname -s)
JAVA  = java

ifeq ($(system), MINGW32_NT-6.2)
	ifeq ($(ARCH), 32)
		lib := JMidi.dll mshare32.dll
		PREFIX=vs2005/Release/
		mspath := ../../../../src/$(PREFIX)/mshare32.dll
	else 
#		JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"
		JAVA  = java
		lib := JMidi.dll mshare64.dll
		PREFIX=vs2005-64/Release/
		mspath := ../../../../src/$(PREFIX)/mshare64.dll
	endif
else
  lib =  libJMidi.dylib
  ifeq ($(ARCH), 32)
	JOPT := -d32
  endif
endif

APPLET = AppletFrame.class
path   = -classpath .:../../native

test:
	echo $(lib)

part1 :  $(lib) $(APPLET) TutorialPart1.class
		$(JAVA) $(path) $(JOPT) TutorialPart1

part2 :  $(lib) $(APPLET) TutorialPart2.class
		$(JAVA) $(path) $(JOPT) TutorialPart2

clean:
	rm -f *.class JMidi.dll mshare*.dll 

#libJMidi.dylib: ../../jni/cmake/Release/libJMidi-$(VERSION).dylib
#	cp ../../jni/cmake/Release/libJMidi-$(VERSION).dylib .
#	ln -sf libJMidi-$(VERSION).dylib libJMidi.dylib

#JMidi.dll : ../../jni/$(PREFIX)JMidi-$(VERSION).dll
#	cp ../../jni/$(PREFIX)JMidi-$(VERSION).dll JMidi.dll

mshare32.dll: $(mspath) 
	cp $(mspath) .

#mshare64.dll: $(mspath) 
#	cp $(mspath) .


##################################################################
# rule for java compiling
%.class: %.java
	javac $(path) $< 
