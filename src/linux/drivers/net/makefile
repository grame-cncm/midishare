
CXX  = gcc
INC = -I../../Include -I../../../lib/Network/include -I../../../common/Headers
NETLIB = ../../../lib/Network/msNetDriverLib.o
LIB = $(NETLIB) -Wl,-L../../library -lMidiShare -lpthread
CXXFLAGS = -g $(INC) -D__Pentium__
CFLAGS = $(CXXFLAGS)

BIN=/usr/local/bin

OBJ =	Handle.o \
	INetAlert.o \
	LinuxFeedback.o \
	msInetDriver.o \
	StateManager.o \
	./sockets/TInetAddress.o \
	./sockets/TNetInfos.o \
	./sockets/TTCPSocket.o \
	./sockets/TUDPSocket.o 

SRC =	Handle.c \
	INetAlert.cpp \
	LinuxFeedback.cpp \
	msInetDriver.cpp \
	StateManager.cpp \
	./sockets/TInetAddress.cpp \
	./sockets/TNetInfos.cpp \
	./sockets/TTCPSocket.cpp \
	./sockets/TUDPSocket.cpp 

Target = msInetDriver

all : $(Target)

$(Target) : $(OBJ) $(NETLIB)
	gcc $(OBJ) $(LIB) -o $(Target)

$(NETLIB) :
	make -C ../../../lib/Network
	
clean : 
	rm -f $(Target) $(OBJ)

install :
	make -C doc install
	strip $(Target)
	install -m 6755 -s $(Target) $(BIN)

uninstall :
	make -C doc uninstall
	rm -f $(BIN)/$(Target)

dep:
	makedepend -- $(CXXFLAGS) -- -Y -m $(SRC)
# DO NOT DELETE

Handle.o: ../../../lib/Network/include/Handle.h
INetAlert.o: ../../../lib/Network/include/INetAlert.h
INetAlert.o: ../../../lib/Network/include/InetTypes.h
LinuxFeedback.o: LinuxFeedback.h
LinuxFeedback.o: ../../../lib/Network/include/FeedbackProvider.h
LinuxFeedback.o: ../../../lib/Network/include/StrConstants.h
LinuxFeedback.o: ../../../lib/Network/include/InetTypes.h
LinuxFeedback.o: ../../../lib/Network/include/TInetAddress.h
LinuxFeedback.o: ../../../lib/Network/include/TMidiRemote.h
LinuxFeedback.o: ../../../lib/Network/include/TInetTasks.h
LinuxFeedback.o: ../../../lib/Network/include/MidiShareTasks.h
LinuxFeedback.o: ../../Include/MidiShare.h
LinuxFeedback.o: ../../../lib/Network/include/MidiShareAppl.h
LinuxFeedback.o: ../../../lib/Network/include/MidiShareFilter.h
LinuxFeedback.o: ../../Include/FilterUtils.h
LinuxFeedback.o: ../../../lib/Network/include/TMidiOutStream.h
LinuxFeedback.o: ../../../lib/Network/include/TMidiPacket.h
LinuxFeedback.o: ../../../lib/Network/include/TPacket.h
LinuxFeedback.o: ../../../lib/Network/include/TimeTools.h
LinuxFeedback.o: ../../../lib/Network/include/EventToUDPStream.h
LinuxFeedback.o: ../../../lib/Network/include/TMidiInStream.h
LinuxFeedback.o: ../../../lib/Network/include/UDPStreamToEvent.h
LinuxFeedback.o: ../../../lib/Network/include/TRemote.h
LinuxFeedback.o: ../../../lib/Network/include/TRemoteSlot.h
LinuxFeedback.o: ../../../lib/Network/include/TMidiSlot.h
LinuxFeedback.o: ../../../lib/Network/include/StatesManager.h
LinuxFeedback.o: ../../../lib/Network/include/Handle.h
LinuxFeedback.o: ../../../lib/Network/include/SkewControler.h
LinuxFeedback.o: ../../../lib/Network/include/FTMA.h
msInetDriver.o: ../../../lib/Network/include/TWANControler.h
msInetDriver.o: ../../../lib/Network/include/TInetControler.h
msInetDriver.o: ../../../lib/Network/include/INetAlert.h
msInetDriver.o: ../../../lib/Network/include/InetTypes.h
msInetDriver.o: ../../Include/MidiShare.h
msInetDriver.o: ../../../lib/Network/include/StrConstants.h
msInetDriver.o: ../../../lib/Network/include/TInetAddress.h
msInetDriver.o: ../../../lib/Network/include/TInetTasks.h
msInetDriver.o: ../../../lib/Network/include/MidiShareTasks.h
msInetDriver.o: ../../../lib/Network/include/MidiShareDriver.h
msInetDriver.o: ../../../lib/Network/include/MidiShareAppl.h
msInetDriver.o: ../../../lib/Network/include/MidiShareFilter.h
msInetDriver.o: ../../Include/FilterUtils.h
msInetDriver.o: ../../../lib/Network/include/TNetInfos.h
msInetDriver.o: ../../../lib/Network/include/TRemoteMgr.h
msInetDriver.o: ../../../lib/Network/include/TMidiRemote.h
msInetDriver.o: ../../../lib/Network/include/TMidiOutStream.h
msInetDriver.o: ../../../lib/Network/include/TMidiPacket.h
msInetDriver.o: ../../../lib/Network/include/TPacket.h
msInetDriver.o: ../../../lib/Network/include/TimeTools.h
msInetDriver.o: ../../../lib/Network/include/EventToUDPStream.h
msInetDriver.o: ../../../lib/Network/include/TMidiInStream.h
msInetDriver.o: ../../../lib/Network/include/UDPStreamToEvent.h
msInetDriver.o: ../../../lib/Network/include/TRemote.h
msInetDriver.o: ../../../lib/Network/include/TRemoteSlot.h
msInetDriver.o: ../../../lib/Network/include/TMidiSlot.h
msInetDriver.o: ../../../lib/Network/include/StatesManager.h
msInetDriver.o: ../../../lib/Network/include/Handle.h
msInetDriver.o: ../../../lib/Network/include/SkewControler.h
msInetDriver.o: ../../../lib/Network/include/FTMA.h
msInetDriver.o: ../../../lib/Network/include/TUDPSocket.h
msInetDriver.o: ../../../lib/Network/include/TSocket.h
msInetDriver.o: ../../../lib/Network/include/TUDPListener.h
msInetDriver.o: ../../../lib/Network/include/FeedbackProvider.h
msInetDriver.o: LinuxFeedback.h ../../../lib/Network/include/TMidiServer.h
msInetDriver.o: ../../../lib/Network/include/TTCPListener.h
msInetDriver.o: ../../../lib/Network/include/TTCPSocket.h
StateManager.o: ../../../lib/Network/include/StatesManager.h
StateManager.o: ../../../lib/Network/include/InetTypes.h
StateManager.o: ../../Include/MidiShare.h
StateManager.o: ../../../lib/Network/include/Handle.h
./sockets/TInetAddress.o: ../../../lib/Network/include/TInetAddress.h
./sockets/TInetAddress.o: ../../../lib/Network/include/InetTypes.h
./sockets/TNetInfos.o: ../../../lib/Network/include/TNetInfos.h
./sockets/TNetInfos.o: ../../../lib/Network/include/InetTypes.h
./sockets/TNetInfos.o: ../../../lib/Network/include/TInetAddress.h
./sockets/TTCPSocket.o: ../../../lib/Network/include/TTCPSocket.h
./sockets/TTCPSocket.o: ../../../lib/Network/include/TSocket.h
./sockets/TTCPSocket.o: ../../../lib/Network/include/InetTypes.h
./sockets/TTCPSocket.o: ../../../lib/Network/include/TPacket.h
./sockets/TTCPSocket.o: ../../../lib/Network/include/TimeTools.h
./sockets/TTCPSocket.o: ../../Include/MidiShare.h
./sockets/TTCPSocket.o: ../../../lib/Network/include/TTCPListener.h
./sockets/TUDPSocket.o: ../../../lib/Network/include/TUDPSocket.h
./sockets/TUDPSocket.o: ../../../lib/Network/include/TSocket.h
./sockets/TUDPSocket.o: ../../../lib/Network/include/InetTypes.h
./sockets/TUDPSocket.o: ../../../lib/Network/include/TPacket.h
./sockets/TUDPSocket.o: ../../../lib/Network/include/TimeTools.h
./sockets/TUDPSocket.o: ../../Include/MidiShare.h
./sockets/TUDPSocket.o: ../../../lib/Network/include/TUDPListener.h
