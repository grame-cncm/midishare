
VERSION = 1.16

system := $(shell uname -s)

JAVA  = java
PREFIX = cmake/lib

ifeq ($(system), MINGW32_NT-6.1)
	lib = JMidi.dll
	ifeq ($(ARCH), 32)
		lib := $(lib) mshare32.dll
	else 
		JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"
		lib := $(lib) mshare64.dll
	endif
else
  lib =  libJMidi.dylib
endif

ifeq ($(ARCH), 32)
	JOPT := -d32
endif

JAR	   = grame.jar
APPLET = AppletFrame.class
PART1  = TutorialPart1.class
PART2  = TutorialPart2.class
path   = -classpath .:grame.jar
java64 = "/c/Program Files/Java/jre6/bin/java.exe"

part1 :  $(lib) $(JAR) $(APPLET) $(PART1)
#		$(JAVA) $(path) $(JOPT) TutorialPart1


part164 :  $(lib) $(JAR) $(APPLET) $(PART1)
		$(JAVA) TutorialPart1

part2 :  $(lib) $(JAR) $(APPLET) $(PART2)
		$(JAVA) $(path) $(JOPT) TutorialPart2

part264 :  $(lib) $(JAR) $(APPLET) $(PART2)
		$(JAVA) TutorialPart2

$(JAR)  : ../../native/grame.jar
	cp ../../native/grame.jar .

../../native/grame.jar:
	make -C ../../native jar
	
libJMidi.dylib: ../../jni/cmake/Release/libJMidi-$(VERSION).dylib
	cp ../../jni/cmake/Release/libJMidi-$(VERSION).dylib .
	ln -sf libJMidi-$(VERSION).dylib libJMidi.dylib

JMidi.dll : ../../jni/$(PREFIX)JMidi-$(VERSION).dll
	cp ../../jni/$(PREFIX)JMidi-$(VERSION).dll JMidi.dll


##################################################################
# rule for java compiling
%.class: %.java
	javac $(path) $< 
