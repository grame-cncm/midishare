APPLET = AppletFrame.class
PART1  = TutorialPart1.class
PART2  = TutorialPart2.class
path   = -classpath .:grame.jar
java64 = "/c/Program Files/Java/jre6/bin/java.exe"

part1 : $(APPLET) $(PART1)
		java $(path) TutorialPart1

part132 : $(APPLET) $(PART1)
		java -d32 $(path) TutorialPart1

part164 : $(APPLET) $(PART1)
		$(java64) TutorialPart1

part2 : $(APPLET) $(PART2)
		java $(path) TutorialPart2

part264 : $(APPLET) $(PART2)
		$(java64) TutorialPart2

part232 : $(APPLET) $(PART1)
		java -d32 $(path) TutorialPart2

jar :
	cp ../../native/grame.jar .

macos: 
	cp ../../jni/cmake/Release/libJMidi-1.15.dylib .
	ln -sf libJMidi-1.15.dylib libJMidi.dylib

win32: 
	cp ../../jni/cmake/Release/JMidi64.dll ./JMidi.dll


##################################################################
# rule for java compiling
%.class: %.java
	javac $(path) $< 
