cppappl := $(patsubst %.cpp, %, $(wildcard *.cpp))
cappl := $(patsubst %.c, %, $(wildcard *.c))
options := -I../../Include -Wall -O6 `gtk-config --cflags --libs` -lMidiShare -lpthread


all : $(cppappl) $(cappl)
 
$(cppappl) : % : %.cpp  
	gcc $(options) $< -o $@
 
$(cappl) : % : %.c
	gcc $(options) $< -o $@

clean :
	rm $(cppappl) $(cappl)

install :
	cp $(cppappl) $(cappl) /usr/local/bin
