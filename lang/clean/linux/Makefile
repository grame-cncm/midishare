
SRC =../src

C = $(CC) -D __Linux__ $(CFLAGS) -O -c

all : Test

MidiShare_Clean.o : $(SRC)/MidiShare_Clean.c
	$C $(SRC)/MidiShare_Clean.c

MidiShare_Clean.icl : $(SRC)/MidiShare_Clean.h
	htoclean $(SRC)/MidiShare_Clean
	
MidiShare_Clean.dcl : $(SRC)/MidiShare_Clean.h
	htoclean $(SRC)/MidiShare_Clean

Test : Test.icl    $(SRC)/MidiShare_Clean.dcl   $(SRC)/MidiShare_Clean.icl MidiShare_Clean.o
	clm  -I $(SRC) -l MidiShare_Clean.o -l /usr/lib/libMidiShare.so  Test -o Test
