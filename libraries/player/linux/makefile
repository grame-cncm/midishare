
TARGET = libPlayer.so.1.0
LIB = /usr/lib/libPlayer.so
INC=src/Include 
INC1=./
OPTIONS=-D__Linux__
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
TTimeConverter.o TTimeManager.o
 
 
$(TARGET) :   $(OBJ)
	$(CC)  $(OBJ) -shared -fPIC -W1,soname,libPlayer.so.1 -o $(TARGET)

install: $(LIB).1.0
	./checksu
	if [ ! -f $(LIB) ]; then ln -s $(LIB).1 $(LIB); fi
	if [ ! -f $(LIB).1 ]; then ln -s $(LIB).1.0 $(LIB).1; fi


$(LIB).1.0 : $(TARGET)
	cp $(TARGET)   $(LIB).1.0

	

TChaserIterator.o : src/Chasers/TChaserIterator.cpp
	$(CC) src/Chasers/TChaserIterator.cpp   -I$(INC) -I$(INC1)  $(OPTIONS) $(FLAGS) 

TChaserVisitor.o : src/Chasers/TChaserVisitor.cpp
	$(CC) src/Chasers/TChaserVisitor.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS) 

THashTable.o : src/Chasers/THashTable.cpp
	$(CC) src/Chasers/THashTable.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TClockEventReceiver.o : src/Events/TClockEventReceiver.cpp
	$(CC) src/Events/TClockEventReceiver.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TEventReceiver.o : src/Events/TEventReceiver.cpp
	$(CC) src/Events/TEventReceiver.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TEventRecorder.o : src/Events/TEventRecorder.cpp
	$(CC) src/Events/TEventRecorder.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TExtEventReceiver.o : src/Events/TExtEventReceiver.cpp
	$(CC) src/Events/TExtEventReceiver.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
	
TMidiPlayer.o : src/Events/TMidiPlayer.cpp
	$(CC) src/Events/TMidiPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TSMPTEEventReceiver.o : src/Events/TSMPTEEventReceiver.cpp
	$(CC) src/Events/TSMPTEEventReceiver.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerFactory.o : src/Factory/TPlayerFactory.cpp
	$(CC) src/Factory/TPlayerFactory.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

Player.o : src/Library/Player.cpp
	$(CC) src/Library/Player.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayer.o : src/Library/TPlayer.cpp
	$(CC) src/Library/TPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
	
TLoopManager.o : src/Loop/TLoopManager.cpp
	$(CC) src/Loop/TLoopManager.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TRunningPlayer.o : src/Players/TRunningPlayer.cpp
	$(CC) src/Players/TRunningPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TSMPTEPlayer.o : src/Players/TSMPTEPlayer.cpp
	$(CC) src/Players/TSMPTEPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TSyncInPlayer.o : src/Players/TSyncInPlayer.cpp
	$(CC) src/Players/TSyncInPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TSyncOutPlayer.o : src/Players/TSyncOutPlayer.cpp
	$(CC) src/Players/TSyncOutPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTickPlayer.o : src/Players/TTickPlayer.cpp
	$(CC) src/Players/TTickPlayer.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerScore.o : src/Score/TPlayerScore.cpp
	$(CC) src/Score/TPlayerScore.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TScoreInserter.o : src/Score/TScoreInserter.cpp
	$(CC) src/Score/TScoreInserter.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TClockSender.o : src/Synchroniser/TClockSender.cpp
	$(CC) src/Synchroniser/TClockSender.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerSynchroniserClock.o : src/Synchroniser/TPlayerSynchroniserClock.cpp
	$(CC) src/Synchroniser/TPlayerSynchroniserClock.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerSynchroniserExt.o : src/Synchroniser/TPlayerSynchroniserExt.cpp
	$(CC) src/Synchroniser/TPlayerSynchroniserExt.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TPlayerSynchroniserInt.o : src/Synchroniser/TPlayerSynchroniserInt.cpp
	$(CC) src/Synchroniser/TPlayerSynchroniserInt.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTrackTable.o : src/Tracks/TTrackTable.cpp
	$(CC) src/Tracks/TTrackTable.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

UMidi.o : src/Utils/UMidi.cpp
	$(CC) src/Utils/UMidi.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

UTools.o : src/Utils/UTools.cpp
	$(CC) src/Utils/UTools.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

Tmtrack.o : src/MidiFile/Tmtrack.c
	$(CC) src/MidiFile/Tmtrack.c   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
	
Midifile.o : src/MidiFile/Midifile.c
	$(CC) src/MidiFile/Midifile.c   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TEventFactory.o : src/Framework/Events/TEventFactory.cpp
	$(CC) src/Framework/Events/TEventFactory.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TMidiAppl.o : src/Framework/MidiAppl/TMidiAppl.cpp
	$(CC) src/Framework/MidiAppl/TMidiAppl.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
	
TScheduler.o : src/Framework/Scheduler/TScheduler.cpp
	$(CC) src/Framework/Scheduler/TScheduler.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TScore.o : src/Framework/Score/TScore.cpp
	$(CC) src/Framework/Score/TScore.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TScoreFollower.o : src/Framework/Score/TScoreFollower.cpp
	$(CC) src/Framework/Score/TScoreFollower.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TScoreIterator.o : src/Framework/Score/TScoreIterator.cpp
	$(CC) src/Framework/Score/TScoreIterator.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

UScoreManager.o : src/Framework/Score/UScoreManager.cpp
	$(CC) src/Framework/Score/UScoreManager.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  
		
TTempoConverter.o : src/Framework/Tempo/TTempoConverter.cpp
	$(CC) src/Framework/Tempo/TTempoConverter.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTempoMap.o : src/Framework/Tempo/TTempoMap.cpp
	$(CC) src/Framework/Tempo/TTempoMap.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTempoMapVisitor.o : src/Framework/Tempo/TTempoMapVisitor.cpp
	$(CC) src/Framework/Tempo/TTempoMapVisitor.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTime.o : src/Framework/Tempo/TTime.cpp
	$(CC) src/Framework/Tempo/TTime.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTimeConverter.o : src/Framework/Tempo/TTimeConverter.cpp
	$(CC) src/Framework/Tempo/TTimeConverter.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

TTimeManager.o : src/Framework/Tempo/TTimeManager.cpp
	$(CC) src/Framework/Tempo/TTimeManager.cpp   -I$(INC) -I$(INC1)  $(OPTIONS)  $(FLAGS)  

 clean : 
	rm *.o

