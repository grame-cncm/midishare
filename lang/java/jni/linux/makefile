TARGET=libJMidi.so.1.0

# SET THE PATHNAME  for ../JDKxx/include HERE

INC2=/home/letz/j2sdk1.3/include
INC3=/home/letz/j2sdk1.3/include/linux

SRC =../src
OPTIONS=-D__Linux__ -D_REENTRANT
MODCFLAGS := -O6 -Wall -DLINUX 
FLAGS=-fPIC -c  -g -O2
CC = gcc

OBJ = Midi.o MidiAppl.o


$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -lMidiShare -shared -fPIC -W1,soname,libJMidi.so.1 -o $(TARGET)

	

Midi.o : $(SRC)/Midi.c $(SRC)/Midi.h
	if [ ! -f $(INC2)/jni.h ]; then echo "You must set up the JDK include pathname in the INC2 variable"; exit 1; fi
	if [ ! -f $(INC3)/jni_md.h ]; then echo "You must set up the JDK include pathname in the INC3 variable"; exit 1; fi
	$(CC) $(SRC)/Midi.c     -I$(INC2) -I$(INC3)  $(OPTIONS) $(FLAGS) 
	
MidiAppl.o : $(SRC)/MidiAppl.c $(SRC)/MidiAppl.h
	if [ ! -f $(INC2)/jni.h ]; then echo "You must set up the JDK include pathname in the INC2 variable"; exit 1; fi
	if [ ! -f $(INC3)/jni_md.h ]; then echo "You must set up the JDK include pathname in the INC3 variable"; exit 1; fi
	$(CC) $(SRC)/MidiAppl.c    -I$(INC2) -I$(INC3)  $(OPTIONS) $(FLAGS)  
	
clean : 
	rm *.o
	rm $(TARGET)
