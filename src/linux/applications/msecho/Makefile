
cappl := msecho
options := -I../../Include -Wall -O6 `gtk-config --cflags --libs` -lMidiShare -lpthread
DEST=/usr/local/bin

all : $(cappl)
 
  
$(cappl) : % : %.c
	gcc $(options) $< -o $@

clean :
	rm $(cappl)

install :
	cp $(cappl) $(DEST)
	echo chmod +s $(DEST)/$(cappl)
