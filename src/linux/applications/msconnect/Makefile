cppappl := $(patsubst %.cpp, %, $(wildcard *.cpp))
cappl := $(patsubst %.c, %, $(wildcard *.c))
LIB   := ../lib/msApplsLib.o -lMidiShare -lpthread
INC   := -I../../Include -I../lib
options := -Wall -O6 $(INC) `gtk-config --cflags --libs` $(LIB)
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
