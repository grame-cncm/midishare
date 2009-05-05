
cappl := msplayer
LIB   := ../lib/msApplsLib.o -Wl,-L../../library -lMidiShare -lPlayer
INC   := -I../../Include -I../lib
options := $(INC) -Wall -O6 `pkg-config --cflags --libs gtk+-2.0` $(LIB)
DEST=/usr/local/bin

all : $(cappl)
 
  
$(cappl) : msplayer.c Player.h
	gcc $(options) $< -o $@

clean :
	rm $(cappl)

install :
	cp $(cappl) $(DEST)
	echo chmod +s $(DEST)/$(cappl)
