APPLET = AppletFrame.class
PART1  = TutorialPart1.class
PART2  = TutorialPart2.class
path   = -classpath .:grame.jar

part1 : $(APPLET) $(PART1)
		java $(path) TutorialPart1

part132 : $(APPLET) $(PART1)
		java -d32 $(path) TutorialPart1

part2 : $(APPLET) $(PART2)
		java $(path) TutorialPart2

part232 : $(APPLET) $(PART1)
		java -d32 $(path) TutorialPart2

jar :
	cp ../../native/grame.jar .

macos : 
	cp ../../jni/cmake/Release/libJMidi-1.14.dylib .

##################################################################
# rule for java compiling
%.class: %.java
	javac $(path) $< 
