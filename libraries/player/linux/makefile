
TARGET = libPlayer1.so.1.0
TARGETJAVA = libJPlayer1.so

LIB = /usr/lib/libPlayer1.so
INC=../src/Include 
INC1=./

# SET THE PATHNAME  for ../JDKxx/include HERE

INC2=/home/letz/jdk1.2.2/include
INC3=/home/letz/jdk1.2.2/include/linux

SRC =../src
OPTIONS=-D__Linux__ -D_REENTRANT
MODCFLAGS := -O6 -Wall -DLINUX 
FLAGS=-fPIC -c  -g -O2
CC = gcc


OBJ = TChaserIterator.o TChaserVisitor.o THashTable.o TClockEventReceiver.o TEventReceiver.o \
TEventRecorder.o TExtEventReceiver.o TMidiPlayer.o TSMPTEEventReceiver.o TPlayerFactory.o \
Player.o TPlayer.o TLoopManager.o TRunningPlayer.o TSMPTEPlayer.o TSyncInPlayer.o \
TSyncOutPlayer.o TTickPlayer.o TPlayerScore.o TScoreInserter.o \
TClockSender.o TPlayerSynchroniserClock.o TPlayerSynchroniserExt.o TPlayerSynchroniserInt.o \
TTrackTable.o UMidi.o UTools.o Tmtrack.o Midifile.o \
TEventFactory.o  TMidiAppl.o TScheduler.o TScore.o TScoreFollower.o TScoreIterator.o\
UScoreManager.o TTempoConverter.o TTempoMap.o TTempoMapVisitor.o TTime.o \
TTimeConverter.o TTimeManager.o TEventModifier.o
 
OBJJAVA = $(OBJ) MidiPlayer.o
 
$(TARGET) :   $(OBJ)
	$(CC)  $(OBJ) -lMidiShare -shared -fPIC -W1,soname,libPlayer1.so.1 -o $(TARGET)

install: $(LIB).1.0
	if [ ! -f $(LIB) ]; then ln -s $(LIB).1 $(LIB); fi
	if [ ! -f $(LIB).1 ]; then ln -s $(LIB).1.0 $(LIB).1; fi

java : $(OBJJAVA)
	$(CC)  $(OBJJAVA) -lMidiShare -shared -fPIC -W1,soname,libJPlayer1.so.1 -o $(TARGETJAVA)

$(LIB).1.0 : $(TARGET)
	cp $(TARGET)   $(LIB).1.0

MidiPlayer.o : $(SRC)/Java/MidiPlayer.c $(SRC)/Java/MidiPlayer.h
	if [ ! -f $(INC2)/jni.h ]; then echo "You must set up the JDK include pathname in the INC2 variable"; exit 1; fi
	if [ ! -f $(INC3)/jni_md.h ]; then echo "You must set up the JDK include pathname in the INC3 variable"; exit 1; fi
	$(CC) $(SRC)/Java/MidiPlayer.c   -I$(INC) -I$(INC1) -I$(INC2) -I$(INC3)  $(OPTIONS) $(FLAGS) 
	

TChaserIterator.o : $(SRC)/Chasers/TChaserIterator.cpp
	$(CC) $(SRC)/Chasers/TChaserIterator.cpp   -I$(INC) -I$(INC1)  $(OPTIONS) $(FLAGS) 

TChaserVisitor.o : $(SRC)/Chasers/TChaserVisitor.cpp
	$(CC) $(SRC)/Chasers/TChaserVisitor.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS) 

THashTable.o : $(SRC)/Chasers/THashTable.cpp
	$(CC) $(SRC)/Chasers/THashTable.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TClockEventReceiver.o : $(SRC)/Events/TClockEventReceiver.cpp
	$(CC) $(SRC)/Events/TClockEventReceiver.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TEventReceiver.o : $(SRC)/Events/TEventReceiver.cpp
	$(CC) $(SRC)/Events/TEventReceiver.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TEventRecorder.o : $(SRC)/Events/TEventRecorder.cpp
	$(CC) $(SRC)/Events/TEventRecorder.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TExtEventReceiver.o : $(SRC)/Events/TExtEventReceiver.cpp
	$(CC) $(SRC)/Events/TExtEventReceiver.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
	
TMidiPlayer.o : $(SRC)/Events/TMidiPlayer.cpp
	$(CC) $(SRC)/Events/TMidiPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TSMPTEEventReceiver.o : $(SRC)/Events/TSMPTEEventReceiver.cpp
	$(CC) $(SRC)/Events/TSMPTEEventReceiver.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerFactory.o : $(SRC)/Factory/TPlayerFactory.cpp
	$(CC) $(SRC)/Factory/TPlayerFactory.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

Player.o : $(SRC)/Library/Player.cpp
	$(CC) $(SRC)/Library/Player.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayer.o : $(SRC)/Library/TPlayer.cpp
	$(CC) $(SRC)/Library/TPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
	
TLoopManager.o : $(SRC)/Loop/TLoopManager.cpp
	$(CC) $(SRC)/Loop/TLoopManager.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TRunningPlayer.o : $(SRC)/Players/TRunningPlayer.cpp
	$(CC) $(SRC)/Players/TRunningPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TSMPTEPlayer.o : $(SRC)/Players/TSMPTEPlayer.cpp
	$(CC) $(SRC)/Players/TSMPTEPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TSyncInPlayer.o : $(SRC)/Players/TSyncInPlayer.cpp
	$(CC) $(SRC)/Players/TSyncInPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TSyncOutPlayer.o : $(SRC)/Players/TSyncOutPlayer.cpp
	$(CC) $(SRC)/Players/TSyncOutPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTickPlayer.o : $(SRC)/Players/TTickPlayer.cpp
	$(CC) $(SRC)/Players/TTickPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerScore.o : $(SRC)/Score/TPlayerScore.cpp
	$(CC) $(SRC)/Score/TPlayerScore.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TScoreInserter.o : $(SRC)/Score/TScoreInserter.cpp
	$(CC) $(SRC)/Score/TScoreInserter.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TClockSender.o : $(SRC)/Synchroniser/TClockSender.cpp
	$(CC) $(SRC)/Synchroniser/TClockSender.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerSynchroniserClock.o : $(SRC)/Synchroniser/TPlayerSynchroniserClock.cpp
	$(CC) $(SRC)/Synchroniser/TPlayerSynchroniserClock.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerSynchroniserExt.o : $(SRC)/Synchroniser/TPlayerSynchroniserExt.cpp
	$(CC) $(SRC)/Synchroniser/TPlayerSynchroniserExt.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerSynchroniserInt.o : $(SRC)/Synchroniser/TPlayerSynchroniserInt.cpp
	$(CC) $(SRC)/Synchroniser/TPlayerSynchroniserInt.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTrackTable.o : $(SRC)/Tracks/TTrackTable.cpp
	$(CC) $(SRC)/Tracks/TTrackTable.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

UMidi.o : $(SRC)/Utils/UMidi.cpp
	$(CC) $(SRC)/Utils/UMidi.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

UTools.o : $(SRC)/Utils/UTools.cpp
	$(CC) $(SRC)/Utils/UTools.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

Tmtrack.o : $(SRC)/MidiFile/Tmtrack.c
	$(CC) $(SRC)/MidiFile/Tmtrack.c   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
	
Midifile.o : $(SRC)/MidiFile/Midifile.c
	$(CC) $(SRC)/MidiFile/Midifile.c   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TEventFactory.o : $(SRC)/Framework/Events/TEventFactory.cpp
	$(CC) $(SRC)/Framework/Events/TEventFactory.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TMidiAppl.o : $(SRC)/Framework/MidiAppl/TMidiAppl.cpp
	$(CC) $(SRC)/Framework/MidiAppl/TMidiAppl.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
	
TScheduler.o : $(SRC)/Framework/Scheduler/TScheduler.cpp
	$(CC) $(SRC)/Framework/Scheduler/TScheduler.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TScore.o : $(SRC)/Framework/Score/TScore.cpp
	$(CC) $(SRC)/Framework/Score/TScore.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TScoreFollower.o : $(SRC)/Framework/Score/TScoreFollower.cpp
	$(CC) $(SRC)/Framework/Score/TScoreFollower.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TScoreIterator.o : $(SRC)/Framework/Score/TScoreIterator.cpp
	$(CC) $(SRC)/Framework/Score/TScoreIterator.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

UScoreManager.o : $(SRC)/Framework/Score/UScoreManager.cpp
	$(CC) $(SRC)/Framework/Score/UScoreManager.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
		
TTempoConverter.o : $(SRC)/Framework/Tempo/TTempoConverter.cpp
	$(CC) $(SRC)/Framework/Tempo/TTempoConverter.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTempoMap.o : $(SRC)/Framework/Tempo/TTempoMap.cpp
	$(CC) $(SRC)/Framework/Tempo/TTempoMap.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTempoMapVisitor.o : $(SRC)/Framework/Tempo/TTempoMapVisitor.cpp
	$(CC) $(SRC)/Framework/Tempo/TTempoMapVisitor.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTime.o : $(SRC)/Framework/Tempo/TTime.cpp
	$(CC) $(SRC)/Framework/Tempo/TTime.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTimeConverter.o : $(SRC)/Framework/Tempo/TTimeConverter.cpp
	$(CC) $(SRC)/Framework/Tempo/TTimeConverter.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTimeManager.o : $(SRC)/Framework/Tempo/TTimeManager.cpp
	$(CC) $(SRC)/Framework/Tempo/TTimeManager.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TEventModifier.o : $(SRC)/Events/TEventModifier.cpp
	$(CC) $(SRC)/Events/TEventModifier.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

 clean : 
	rm *.o
	rm $(TARGET)
	rm $(TARGETJAVA)

