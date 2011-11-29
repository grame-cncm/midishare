
VERSION = 1.16

system := $(shell uname -s)

ifeq ($(system), MINGW32_NT-6.1)
  lib = JMidi.dll
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
		java $(path) $(JOPT) TutorialPart1


part164 :  $(lib) $(JAR) $(APPLET) $(PART1)
		$(java64) TutorialPart1

part2 :  $(lib) $(JAR) $(APPLET) $(PART2)
		java $(path) $(JOPT) TutorialPart2

part264 :  $(lib) $(JAR) $(APPLET) $(PART2)
		$(java64) TutorialPart2

$(JAR)  : ../../native/grame.jar
	cp ../../native/grame.jar .

../../native/grame.jar:
	make -C ../../native jar
	
libJMidi.dylib: ../../jni/cmake/Release/libJMidi-$(VERSION).dylib
	cp ../../jni/cmake/Release/libJMidi-$(VERSION).dylib .
	ln -sf libJMidi-$(VERSION).dylib libJMidi.dylib

ifeq ($(ARCH), 64)
JMidi.dll : ../../jni/cmake/Release/JMidi64.dll
	cp ../../jni/cmake/Release/JMidi64.dll ./JMidi.dll
else
JMidi.dll : ../../jni/cmake/Release/JMidi32.dll
	cp ../../jni/cmake/Release/JMidi32.dll ./JMidi.dll
endif


##################################################################
# rule for java compiling
%.class: %.java
	javac $(path) $< 
