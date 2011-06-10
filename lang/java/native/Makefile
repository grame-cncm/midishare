
VERSION = 1.1.2
F = msjni-$(VERSION)

SRC = $(shell find grame -name "*.java")
CLASS = $(patsubst %.java, %.class, $(SRC) )
JAVA  = java
#JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"

.PHONY: doc

class : $(CLASS)

jar : grame.jar

zip : grame.jar JMidi.dll
	[ -d $(F) ] || mkdir $(F)
	rm -rf $(F)/*
	cp grame.jar $(F)
	cp JMidi.dll $(F)
	cp ../../jni/src/changelog.txt $(F)

Test : Test.class
	java -d32 Test

folder :
	-[ -d grameclasses ] && rm -rf grameclasses
	cp -r grame grameclasses
	-find grameclasses -name CVS -exec rm -rf {} \;
	find grameclasses -name "*.java" -exec rm -rf {} \;

grame.jar : $(CLASS)
	jar cvf grame.jar $(CLASS)

clean :
	rm -f $(CLASS) *.class

##################################################################
# rule for java compiling
%.class: %.java
	javac $< 

