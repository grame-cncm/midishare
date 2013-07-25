subprojects := ../../src/include ../../src/common/atomic ../../src/common/Memory ../../src/common/Headers 
sources = $(wildcard *.cpp) 
objects = $(sources:.cpp=.o) ../../src/common/atomic/lflifo.o

#VPATH = $(subprojects)

system	?= $(shell uname -s)

ifeq ($(system), Darwin)
ARCHFLAGS :=  -arch i386 -arch x86_64
else
ARCHFLAGS := 
endif

CXXFLAGS ?= -O3 -Wall -Wuninitialized $(ARCHFLAGS)
CXXFLAGS += -I. $(addprefix -I, $(subprojects)) -DMSLIGHT

target := ../libmidisharelight.a

all : $(target)

$(target) : $(objects)
	rm -f $@
	ar cq $@ $(objects)
	ranlib $@
	
depend :
	makedepend -fMakefile -w120 -Y -- $(CXXFLAGS) -- $(sources)

clean :
	rm -f $(target) $(objects) 
