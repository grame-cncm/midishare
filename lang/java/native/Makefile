SRC = $(shell find grame -name "*.java")
CLASS = $(patsubst %.java, %.class, $(SRC) )
JAVA  = java
#JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"

.PHONY: doc

class : $(CLASS)

jar : grame.jar

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

