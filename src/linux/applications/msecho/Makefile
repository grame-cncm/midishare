
cappl := msecho
LIB   := ../lib/msApplsLib.o -lMidiShare -lpthread
INC   := -I../../Include -I../lib
options := -Wall -O6 $(INC) `gtk-config --cflags --libs` $(LIB)
DEST = /usr/local/bin

all : $(cappl)
 
$(cappl) : % : %.c
	gcc $(options) $< -o $@

clean :
	rm -f $(cappl)
	
install :
	cp $(cappl) $(DEST)
	echo chmod +s $(DEST)/$(cappl)
