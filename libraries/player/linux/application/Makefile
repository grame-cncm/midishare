
cappl := msplayer
LIB   := -lPlayer
options := $(INC) -Wall -O6 `gtk-config --cflags --libs` $(LIB)
DEST=/usr/local/bin

all : $(cappl)
 
  
$(cappl) : msplayer.c 
	gcc $(options) $< -o $@

clean :
	rm $(cappl)

install :
	cp $(cappl) $(DEST)
	echo chmod +s $(DEST)/$(cappl)
