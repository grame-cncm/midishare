FLAGS = ++ccf -D__Linux__ 
TARGET = mzshare.so 
TARGET1= mzplayer.so
SRC =../src

all:	$(TARGET) $(TARGET1)


$(TARGET) : mzshare.o

	mzc --ld mzshare.so mzshare.o /usr/lib/libMidiShare.so

$(TARGET1) :  mzplayer.o
	mzc --ld mzplayer.so mzplayer.o /usr/lib/libPlayer.so


mzshare.o: $(SRC)/mzshare.c $(DEPENDS)
	mzc $(FLAGS)   --cc $(SRC)/mzshare.c 

mzplayer.o: $(SRC)/mzplayer.c $(DEPENDS)
	 mzc $(FLAGS)   --cc $(SRC)/mzplayer.c


clean :
	rm *.o
	rm *.so
