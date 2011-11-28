
#------------------------------------------------------------------------------
# mingw crosscompiler should be installed

CROSS=/usr/local/i386-mingw32-4.3.0/bin/i386-mingw32-
CXX=$(CROSS)g++
AR=$(CROSS)ar
RANLIB=$(CROSS)ranlib
LDFLAGS=
DLLWRAP=$(CROSS)dllwrap --target=i386-mingw32 -mno-cygwin
STRIP=$(CROSS)strip

SRC = $(wildcard ../src/*.cpp)
OBJ = $(patsubst ../src/%.cpp, %.o, $(SRC))  lflifo.o
INC = -I../src -I../src/atomic
CXXFLAGS = $(INC) -DWIN32

lib = midisharelight.lib
exe = read.exe write.exe
targets = $(lib) $(exe)
	
all :  $(targets)
	
$(lib):  $(OBJ)
	$(AR) -r $(lib) $(OBJ)
	$(RANLIB) $(lib)

read.exe : ../test/read.cpp
	$(CXX) $(CXXFLAGS) ../test/read.cpp $(lib) -o read.exe

write.exe  : ../test/write.cpp
	$(CXX) $(CXXFLAGS) ../test/write.cpp $(lib) -o write.exe

strip :
	$(STRIP) $(exe)

lflifo.o : ../src/atomic/lflifo.c
	$(CXX) $(CXXFLAGS) -c $< 

%.o: ../src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< 

clean :
	rm -rf $(OBJ) $(targets)

