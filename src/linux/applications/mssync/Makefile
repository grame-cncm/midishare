
cappl := mssync
LIB   := ../lib/msApplsLib.o -Wl,-L../../library -lMidiShare
INC   := -I../../Include -I../lib
options := -Wall -O6 $(INC) `pkg-config --cflags --libs gtk+-2.0` $(LIB)
DEST=/usr/local/bin

all : $(cappl)
 
$(cappl) : % : %.c
	gcc $(options) $< -o $@

clean :
	rm $(cappl)

install :
	cp $(cappl) $(DEST)
	echo chmod +s $(DEST)/$(cappl)
