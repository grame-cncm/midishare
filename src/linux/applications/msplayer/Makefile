
cappl := msplayer
options := -I../../Include -Wall -O6 `gtk-config --cflags --libs` -lMidiShare -lpthread -lPlayer
DEST=/usr/local/bin

all : $(cappl)
 
  
$(cappl) : msplayer.c Player.h
	gcc $(options) $< -o $@

clean :
	rm $(cappl)

install :
	cp $(cappl) $(DEST)
	echo chmod +s $(DEST)/$(cappl)
