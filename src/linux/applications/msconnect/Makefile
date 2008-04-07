cppappl := $(patsubst %.cpp, %, $(wildcard *.cpp))
cappl := $(patsubst %.c, %, $(wildcard *.c))
LIB   := ../lib/msApplsLib.o -Wl,-L../../library -lMidiShare
INC   := -I../../Include -I../lib
options := -Wall -O6 $(INC) `pkg-config --cflags --libs gtk+-2.0` $(LIB)

DEST = /usr/local/bin


all : $(cppappl) $(cappl)
 
$(cppappl) : % : %.cpp  
	gcc $(options) $< -o $@
 
$(cappl) : % : %.c
	gcc $(options) $< -o $@

clean :
	rm $(cppappl) $(cappl)

install :
	cp $(cppappl) $(cappl) $(DEST)
