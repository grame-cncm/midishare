
CVS = :pserver:anonymous@cvs.sourceforge.net:/cvsroot/midishare
ROOT 	= root
DEVROOT = devroot
DEVKIT	= $(DEVROOT)/Applications/MidiShare/Developer

VERSION =1.88
PACK= midishare/src/macosx-mono/package
SRC = midishare-$(VERSION)-macosx-src
JAVADOC = $(shell find ../../../lang/java/native/grame -name *.java)

common = midishare/doc 				\
		midishare/src/common 			\
		midishare/src/lib 				\
		midishare/src/tools 			\
		midishare/libraries/LGPL.txt 	\
		midishare/libraries/midifile 	\
		midishare/libraries/player/doc 	\
		midishare/libraries/player/src 	\
		midishare/libraries/player/Player_Doxyfile \
		midishare/libraries/seq2score 	\
		midishare/lang/LGPL.txt 		\
		midishare/lang/clean/src 		\
		midishare/lang/java/jni/src 	\
		midishare/lang/java/native/grame \
		midishare/lang/java/samples 	\
		midishare/lang/lisp/			\
		midishare/lang/mzscheme/doc		\
		midishare/lang/mzscheme/src		\
		midishare/lang/python/src

modules = $(common) \
		midishare/src/macosx-mono 			\
		midishare/libraries/player/macosx 	\
		midishare/lang/java/jni/macosx 		\
		midishare/lang/java/native/macosx 	\
		midishare/lang/mzscheme/mac	



package:
	[ -d $(ROOT) ] || cp -R Distribution $(ROOT)
	find $(ROOT) -depth -name CVS -exec rm -rf {} \;
	make dist
	open msPackage.pmsp

dist:
	cd scripts && ./makeContent && sudo ./finalize dist

devpack:
	[ -d $(DEVROOT) ] || cp -R Developer $(DEVROOT)
	find $(ROOT) -depth -name CVS -exec rm -rf {} \;
	make dev
	open msDevPackage.pmsp

dev:
	cd $(DEVKIT)/C_C++ && \
		cvs -d $(CVS) export -D tomorrow -d doc midishare/doc && \
		cvs -d $(CVS) export -D tomorrow -d samples midishare/src/macosx-mono/applications/samples
	cd $(DEVKIT)/C_C++/Libraries && \
		cvs -d $(CVS) export -D tomorrow -d midifile midishare/libraries/midifile
	cd $(DEVKIT)/C_C++/Libraries/player && \
		cvs -d $(CVS) export -D tomorrow -d doc midishare/libraries/player/doc
	cd $(DEVKIT)/Java && \
		cvs -d $(CVS) export -D tomorrow -d tutorial midishare/lang/java/samples/tutorial
	cd $(DEVKIT)/Lisp && \
		cvs -d $(CVS) export -D tomorrow -d openmcl midishare/lang/lisp/openmcl && \
		cvs -d $(CVS) export -D tomorrow -d openmcl midishare/lang/lisp/readme.text
	cd $(DEVKIT)/Lisp/interface && \
		cvs -d $(CVS) export -D tomorrow -d openmcl midishare/lang/lisp/interface/openmcl && \
		cp -R openmcl MCL && flip -m MCL/*.lisp
	cd $(DEVKIT)/Lisp/tutorials && \
		cvs -d $(CVS) export -D tomorrow -d openmcl midishare/lang/lisp/tutorials/openmcl && \
		cp -R openmcl MCL && flip -m MCL/*.lisp
	cd $(DEVROOT)/usr/local && \
		cvs -d $(CVS) export -D tomorrow -d include midishare/src/macosx-mono/include
	cd $(DEVROOT)/usr/local && \
		cvs -d $(CVS) export -D tomorrow -d include midishare/libraries/player/src/Include/Player.h
	javadoc -d $(DEVKIT)/Java/doc $(JAVADOC)
	cd scripts &&  sudo ./finalize dev

source:
	[ -d $(SRC) ] && rm -rf $(SRC) || echo -n
	cvs -d $(CVS) export -D tomorrow $(modules)
	mv midishare $(SRC)
	[ -f $(SRC).tgz ] && rm -f $(SRC).tgz || echo -n
	tar czf $(SRC).tgz $(SRC)

	
clean:
	rm -rf $(ROOT)
	rm -rf $(DEVROOT)
	rm -rf $(SRC) $(SRC).tgz
	