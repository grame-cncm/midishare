
cappl := msecho
GTK   := `gtk-config --cflags --libs`
LIB   := -lMidiShare -lpthread
CFLAGS := -I../../Include -Wall -I../lib -O6 `gtk-config --cflags`
options := $(CFLAGS) `gtk-config --libs` $(LIB)
DEST = /usr/local/bin
OBJ  = msecho.o ../lib/msApplsLib.o
CC   = gcc

all : $(cappl)
 
  
$(cappl) : $(OBJ)
	gcc $(options) $(OBJ) -o $(cappl)

clean :
	rm -f $(cappl) $(OBJ)
	
install :
	cp $(cappl) $(DEST)
	echo chmod +s $(DEST)/$(cappl)
