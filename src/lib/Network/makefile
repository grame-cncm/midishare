
CXX  = gcc
INC = -I../../linux/Include/ -I./include -I../../common/Headers
CXXFLAGS = -g $(INC) -D__Pentium__ -O3 -Wall
CFLAGS = $(CXXFLAGS)

OBJ =	FTMA.o \
	HostTools.o \
	SkewControler.o \
	TInetControler.o \
	TInetTasks.o \
	TMidiInStream.o \
	TMidiOutStream.o \
	TMidiRemote.o \
	TPacket.o \
	TRemoteAppl.o \
	TRemoteMgr.o \
	TRemoteSlot.o \
	./Midi/MidiShareAppl.o \
	./Midi/MidiShareDriver.o \
	./Midi/MidiShareFilter.o \
	./Midi/MidiShareTasks.o \
	./Midi/MidiTools.o \
	./Midi/TMidiSlot.o \
	./state/ApplState.o \
	./state/SlotState.o \
	./TCP/TMidiClient.o \
	./TCP/TMidiServer.o \
	./TCP/TWANControler.o \
	./Midi/EventToUDPStream.o \
	./Midi/UDPStreamToEvent.o 

SRC =	FTMA.cpp \
	HostTools.cpp \
	SkewControler.cpp \
	TInetControler.cpp \
	TInetTasks.cpp \
	TMidiInStream.cpp \
	TMidiOutStream.cpp \
	TMidiRemote.cpp \
	TPacket.cpp \
	TRemoteAppl.cpp \
	TRemoteMgr.cpp \
	TRemoteSlot.cpp \
	./Midi/MidiShareAppl.cpp \
	./Midi/MidiShareDriver.cpp \
	./Midi/MidiShareFilter.cpp \
	./Midi/MidiShareTasks.cpp \
	./Midi/MidiTools.cpp \
	./Midi/TMidiSlot.cpp \
	./state/ApplState.cpp \
	./state/SlotState.cpp \
	./TCP/TMidiClient.cpp \
	./TCP/TMidiServer.cpp \
	./TCP/TWANControler.cpp \
	./Midi/EventToUDPStream.c \
	./Midi/UDPStreamToEvent.c 

Target = msNetDriverLib.o

all : $(Target)

$(Target): $(OBJ)
	ld -m elf_i386 -Ur -o $(Target) $(OBJ)
	
clean : 
	rm -f $(OBJ) $(Target)

dep:
	makedepend -- $(CXXFLAGS) -- -Y -m $(SRC)


# DO NOT DELETE

FTMA.o: ./include/FTMA.h ./include/InetTypes.h
HostTools.o: ./include/HostTools.h ./include/InetTypes.h
SkewControler.o: ./include/SkewControler.h ./include/InetTypes.h
SkewControler.o: ./include/FTMA.h ./include/MidiTools.h
SkewControler.o: ../../linux/Include/MidiShare.h
TInetControler.o: ./include/TimeTools.h ./include/TInetControler.h
TInetControler.o: ./include/INetAlert.h ./include/InetTypes.h
TInetControler.o: ../../linux/Include/MidiShare.h ./include/StrConstants.h
TInetControler.o: ./include/TInetAddress.h ./include/TInetTasks.h
TInetControler.o: ./include/MidiShareTasks.h ./include/MidiShareDriver.h
TInetControler.o: ./include/MidiShareAppl.h ./include/MidiShareFilter.h
TInetControler.o: ../../linux/Include/FilterUtils.h ./include/TNetInfos.h
TInetControler.o: ./include/TRemoteMgr.h ../../common/Headers/lflifo.h
TInetControler.o: ./include/TMidiRemote.h ./include/TMidiOutStream.h
TInetControler.o: ./include/TMidiPacket.h ./include/TPacket.h
TInetControler.o: ./include/EventToUDPStream.h ./include/TMidiInStream.h
TInetControler.o: ./include/UDPStreamToEvent.h ./include/TRemote.h
TInetControler.o: ./include/TRemoteSlot.h ./include/TMidiSlot.h
TInetControler.o: ./include/StatesManager.h ./include/Handle.h
TInetControler.o: ./include/SkewControler.h ./include/FTMA.h
TInetControler.o: ./include/TUDPSocket.h ./include/TSocket.h
TInetControler.o: ./include/TUDPListener.h ./include/MidiTools.h
TInetTasks.o: ./include/TInetTasks.h ./include/InetTypes.h
TInetTasks.o: ./include/MidiShareTasks.h ../../linux/Include/MidiShare.h
TInetTasks.o: ./include/TWANControler.h ./include/TInetControler.h
TInetTasks.o: ./include/INetAlert.h ./include/StrConstants.h
TInetTasks.o: ./include/TInetAddress.h ./include/MidiShareDriver.h
TInetTasks.o: ./include/MidiShareAppl.h ./include/MidiShareFilter.h
TInetTasks.o: ../../linux/Include/FilterUtils.h ./include/TNetInfos.h
TInetTasks.o: ./include/TRemoteMgr.h ../../common/Headers/lflifo.h
TInetTasks.o: ./include/TMidiRemote.h ./include/TMidiOutStream.h
TInetTasks.o: ./include/TMidiPacket.h ./include/TPacket.h
TInetTasks.o: ./include/TimeTools.h ./include/EventToUDPStream.h
TInetTasks.o: ./include/TMidiInStream.h ./include/UDPStreamToEvent.h
TInetTasks.o: ./include/TRemote.h ./include/TRemoteSlot.h
TInetTasks.o: ./include/TMidiSlot.h ./include/StatesManager.h
TInetTasks.o: ./include/Handle.h ./include/SkewControler.h ./include/FTMA.h
TInetTasks.o: ./include/TUDPSocket.h ./include/TSocket.h
TInetTasks.o: ./include/TUDPListener.h ./include/FeedbackProvider.h
TMidiInStream.o: ./include/TMidiInStream.h ../../linux/Include/MidiShare.h
TMidiInStream.o: ./include/InetTypes.h ./include/TMidiPacket.h
TMidiInStream.o: ./include/TPacket.h ./include/TimeTools.h
TMidiInStream.o: ./include/UDPStreamToEvent.h ./include/TUDPSocket.h
TMidiInStream.o: ./include/TSocket.h ./include/TUDPListener.h
TMidiOutStream.o: ./include/TMidiOutStream.h ../../linux/Include/MidiShare.h
TMidiOutStream.o: ./include/InetTypes.h ./include/TMidiPacket.h
TMidiOutStream.o: ./include/TPacket.h ./include/TimeTools.h
TMidiOutStream.o: ./include/EventToUDPStream.h ./include/TSocket.h
TMidiRemote.o: ./include/MidiTools.h ../../linux/Include/MidiShare.h
TMidiRemote.o: ./include/TNetInfos.h ./include/InetTypes.h
TMidiRemote.o: ./include/TMidiRemote.h ./include/TInetTasks.h
TMidiRemote.o: ./include/MidiShareTasks.h ./include/MidiShareAppl.h
TMidiRemote.o: ./include/MidiShareFilter.h ../../linux/Include/FilterUtils.h
TMidiRemote.o: ./include/TMidiOutStream.h ./include/TMidiPacket.h
TMidiRemote.o: ./include/TPacket.h ./include/TimeTools.h
TMidiRemote.o: ./include/EventToUDPStream.h ./include/TMidiInStream.h
TMidiRemote.o: ./include/UDPStreamToEvent.h ./include/TRemote.h
TMidiRemote.o: ./include/TRemoteSlot.h ./include/TMidiSlot.h
TMidiRemote.o: ./include/StatesManager.h ./include/Handle.h
TMidiRemote.o: ./include/SkewControler.h ./include/FTMA.h
TMidiRemote.o: ./include/TRemoteAppl.h ./include/TUDPSocket.h
TMidiRemote.o: ./include/TSocket.h ./include/TUDPListener.h
TMidiRemote.o: ./include/TTCPSocket.h ./include/TTCPListener.h
TPacket.o: ./include/TPacket.h ./include/InetTypes.h ./include/TimeTools.h
TPacket.o: ../../linux/Include/MidiShare.h ./include/HostTools.h
TPacket.o: ./include/TSocket.h
TRemoteAppl.o: ./include/TRemoteAppl.h ./include/MidiShareAppl.h
TRemoteAppl.o: ../../linux/Include/MidiShare.h ./include/MidiShareFilter.h
TRemoteAppl.o: ../../linux/Include/FilterUtils.h ./include/MidiShareTasks.h
TRemoteAppl.o: ./include/StatesManager.h ./include/InetTypes.h
TRemoteAppl.o: ./include/Handle.h ./include/TMidiRemote.h
TRemoteAppl.o: ./include/TInetTasks.h ./include/TMidiOutStream.h
TRemoteAppl.o: ./include/TMidiPacket.h ./include/TPacket.h
TRemoteAppl.o: ./include/TimeTools.h ./include/EventToUDPStream.h
TRemoteAppl.o: ./include/TMidiInStream.h ./include/UDPStreamToEvent.h
TRemoteAppl.o: ./include/TRemote.h ./include/TRemoteSlot.h
TRemoteAppl.o: ./include/TMidiSlot.h ./include/SkewControler.h
TRemoteAppl.o: ./include/FTMA.h
TRemoteMgr.o: ./include/TRemoteMgr.h ./include/InetTypes.h
TRemoteMgr.o: ../../common/Headers/lflifo.h ./include/TMidiRemote.h
TRemoteMgr.o: ./include/TInetTasks.h ./include/MidiShareTasks.h
TRemoteMgr.o: ../../linux/Include/MidiShare.h ./include/MidiShareAppl.h
TRemoteMgr.o: ./include/MidiShareFilter.h ../../linux/Include/FilterUtils.h
TRemoteMgr.o: ./include/TMidiOutStream.h ./include/TMidiPacket.h
TRemoteMgr.o: ./include/TPacket.h ./include/TimeTools.h
TRemoteMgr.o: ./include/EventToUDPStream.h ./include/TMidiInStream.h
TRemoteMgr.o: ./include/UDPStreamToEvent.h ./include/TRemote.h
TRemoteMgr.o: ./include/TRemoteSlot.h ./include/TMidiSlot.h
TRemoteMgr.o: ./include/StatesManager.h ./include/Handle.h
TRemoteMgr.o: ./include/SkewControler.h ./include/FTMA.h
TRemoteMgr.o: ./include/TMidiClient.h ./include/TInetAddress.h
TRemoteMgr.o: ./include/TTCPListener.h ./include/TTCPSocket.h
TRemoteMgr.o: ./include/TSocket.h
TRemoteSlot.o: ./include/TRemoteSlot.h ./include/MidiShareAppl.h
TRemoteSlot.o: ../../linux/Include/MidiShare.h ./include/MidiShareFilter.h
TRemoteSlot.o: ../../linux/Include/FilterUtils.h ./include/MidiShareTasks.h
TRemoteSlot.o: ./include/TMidiSlot.h ./include/StatesManager.h
TRemoteSlot.o: ./include/InetTypes.h ./include/Handle.h
./Midi/MidiShareAppl.o: ./include/MidiShareAppl.h
./Midi/MidiShareAppl.o: ../../linux/Include/MidiShare.h
./Midi/MidiShareAppl.o: ./include/MidiShareFilter.h
./Midi/MidiShareAppl.o: ../../linux/Include/FilterUtils.h
./Midi/MidiShareAppl.o: ./include/MidiShareTasks.h
./Midi/MidiShareDriver.o: ./include/MidiShareDriver.h
./Midi/MidiShareDriver.o: ./include/MidiShareAppl.h
./Midi/MidiShareDriver.o: ../../linux/Include/MidiShare.h
./Midi/MidiShareDriver.o: ./include/MidiShareFilter.h
./Midi/MidiShareDriver.o: ../../linux/Include/FilterUtils.h
./Midi/MidiShareDriver.o: ./include/MidiShareTasks.h
./Midi/MidiShareTasks.o: ./include/MidiShareTasks.h
./Midi/MidiShareTasks.o: ../../linux/Include/MidiShare.h
./Midi/MidiTools.o: ./include/MidiTools.h ../../linux/Include/MidiShare.h
./Midi/TMidiSlot.o: ./include/TMidiSlot.h ../../linux/Include/MidiShare.h
./state/ApplState.o: ./include/StatesManager.h ./include/InetTypes.h
./state/ApplState.o: ../../linux/Include/MidiShare.h ./include/Handle.h
./state/ApplState.o: ./include/TInetAddress.h
./state/SlotState.o: ./include/StatesManager.h ./include/InetTypes.h
./state/SlotState.o: ../../linux/Include/MidiShare.h ./include/Handle.h
./state/SlotState.o: ./include/TRemoteSlot.h ./include/MidiShareAppl.h
./state/SlotState.o: ./include/MidiShareFilter.h
./state/SlotState.o: ../../linux/Include/FilterUtils.h
./state/SlotState.o: ./include/MidiShareTasks.h ./include/TMidiSlot.h
./TCP/TMidiClient.o: ./include/TPacket.h ./include/InetTypes.h
./TCP/TMidiClient.o: ./include/TimeTools.h ../../linux/Include/MidiShare.h
./TCP/TMidiClient.o: ./include/TMidiClient.h ./include/TRemote.h
./TCP/TMidiClient.o: ./include/TInetAddress.h ./include/TInetTasks.h
./TCP/TMidiClient.o: ./include/MidiShareTasks.h ./include/TTCPListener.h
./TCP/TMidiClient.o: ./include/TTCPSocket.h ./include/TSocket.h
./TCP/TMidiClient.o: ./include/TWANControler.h ./include/TInetControler.h
./TCP/TMidiClient.o: ./include/INetAlert.h ./include/StrConstants.h
./TCP/TMidiClient.o: ./include/MidiShareDriver.h ./include/MidiShareAppl.h
./TCP/TMidiClient.o: ./include/MidiShareFilter.h
./TCP/TMidiClient.o: ../../linux/Include/FilterUtils.h ./include/TNetInfos.h
./TCP/TMidiClient.o: ./include/TRemoteMgr.h ../../common/Headers/lflifo.h
./TCP/TMidiClient.o: ./include/TMidiRemote.h ./include/TMidiOutStream.h
./TCP/TMidiClient.o: ./include/TMidiPacket.h ./include/EventToUDPStream.h
./TCP/TMidiClient.o: ./include/TMidiInStream.h ./include/UDPStreamToEvent.h
./TCP/TMidiClient.o: ./include/TRemoteSlot.h ./include/TMidiSlot.h
./TCP/TMidiClient.o: ./include/StatesManager.h ./include/Handle.h
./TCP/TMidiClient.o: ./include/SkewControler.h ./include/FTMA.h
./TCP/TMidiClient.o: ./include/TUDPSocket.h ./include/TUDPListener.h
./TCP/TMidiClient.o: ./include/FeedbackProvider.h ./include/HostTools.h
./TCP/TMidiClient.o: ./include/MidiTools.h
./TCP/TMidiServer.o: ./include/StrConstants.h ./include/TMidiClient.h
./TCP/TMidiServer.o: ./include/TPacket.h ./include/InetTypes.h
./TCP/TMidiServer.o: ./include/TimeTools.h ../../linux/Include/MidiShare.h
./TCP/TMidiServer.o: ./include/TRemote.h ./include/TInetAddress.h
./TCP/TMidiServer.o: ./include/TInetTasks.h ./include/MidiShareTasks.h
./TCP/TMidiServer.o: ./include/TTCPListener.h ./include/TTCPSocket.h
./TCP/TMidiServer.o: ./include/TSocket.h ./include/TMidiServer.h
./TCP/TMidiServer.o: ./include/TUDPListener.h ./include/TWANControler.h
./TCP/TMidiServer.o: ./include/TInetControler.h ./include/INetAlert.h
./TCP/TMidiServer.o: ./include/MidiShareDriver.h ./include/MidiShareAppl.h
./TCP/TMidiServer.o: ./include/MidiShareFilter.h
./TCP/TMidiServer.o: ../../linux/Include/FilterUtils.h ./include/TNetInfos.h
./TCP/TMidiServer.o: ./include/TRemoteMgr.h ../../common/Headers/lflifo.h
./TCP/TMidiServer.o: ./include/TMidiRemote.h ./include/TMidiOutStream.h
./TCP/TMidiServer.o: ./include/TMidiPacket.h ./include/EventToUDPStream.h
./TCP/TMidiServer.o: ./include/TMidiInStream.h ./include/UDPStreamToEvent.h
./TCP/TMidiServer.o: ./include/TRemoteSlot.h ./include/TMidiSlot.h
./TCP/TMidiServer.o: ./include/StatesManager.h ./include/Handle.h
./TCP/TMidiServer.o: ./include/SkewControler.h ./include/FTMA.h
./TCP/TMidiServer.o: ./include/TUDPSocket.h ./include/FeedbackProvider.h
./TCP/TMidiServer.o: ./include/HostTools.h
./TCP/TWANControler.o: ./include/TWANControler.h ./include/TInetControler.h
./TCP/TWANControler.o: ./include/INetAlert.h ./include/InetTypes.h
./TCP/TWANControler.o: ../../linux/Include/MidiShare.h
./TCP/TWANControler.o: ./include/StrConstants.h ./include/TInetAddress.h
./TCP/TWANControler.o: ./include/TInetTasks.h ./include/MidiShareTasks.h
./TCP/TWANControler.o: ./include/MidiShareDriver.h ./include/MidiShareAppl.h
./TCP/TWANControler.o: ./include/MidiShareFilter.h
./TCP/TWANControler.o: ../../linux/Include/FilterUtils.h
./TCP/TWANControler.o: ./include/TNetInfos.h ./include/TRemoteMgr.h
./TCP/TWANControler.o: ../../common/Headers/lflifo.h ./include/TMidiRemote.h
./TCP/TWANControler.o: ./include/TMidiOutStream.h ./include/TMidiPacket.h
./TCP/TWANControler.o: ./include/TPacket.h ./include/TimeTools.h
./TCP/TWANControler.o: ./include/EventToUDPStream.h ./include/TMidiInStream.h
./TCP/TWANControler.o: ./include/UDPStreamToEvent.h ./include/TRemote.h
./TCP/TWANControler.o: ./include/TRemoteSlot.h ./include/TMidiSlot.h
./TCP/TWANControler.o: ./include/StatesManager.h ./include/Handle.h
./TCP/TWANControler.o: ./include/SkewControler.h ./include/FTMA.h
./TCP/TWANControler.o: ./include/TUDPSocket.h ./include/TSocket.h
./TCP/TWANControler.o: ./include/TUDPListener.h ./include/FeedbackProvider.h
./TCP/TWANControler.o: ./include/TMidiServer.h ./include/TTCPListener.h
./TCP/TWANControler.o: ./include/TTCPSocket.h ./include/TMidiClient.h
./TCP/TWANControler.o: ./include/MidiTools.h
./Midi/EventToUDPStream.o: ./include/EventToUDPStream.h
./Midi/EventToUDPStream.o: ../../linux/Include/MidiShare.h
./Midi/UDPStreamToEvent.o: ./include/UDPStreamToEvent.h
./Midi/UDPStreamToEvent.o: ../../linux/Include/MidiShare.h
