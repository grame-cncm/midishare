
VERSION = 1.1.2
F = msjni-$(VERSION)

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
	cp ../../jni/src/changelog.txt $(F)

Test : Test.class
	java Test

Test32 : Test.class
	java -d32 Test

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


##################################################################
# rule for java compiling
%.class: %.java
	javac $< 

headers/%.h: grame/midishare/%.class
	javah -d headers $(patsubst grame/midishare/%.class, grame.midishare.%, $<) 
