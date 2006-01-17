
TARGET = librtpmidi.so
LIB = /usr/local/lib/librtpmidi.so
INCF = /usr/local/include

CXX = g++
CXXFLAGS = -Wall -fPIC -g -D__Types__
LIBS = -lMidiShare
#for mac os x only:
#CXXFLAGS = $(CXXFLAGS) -D__Types__
#LIBS = -framework MidiShare

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

# TARGETS

first : $(TARGET)

$(TARGET) : $(OBJ)
	$(CXX) $(LIBS) -shared -W1,soname,$(TARGET) $(OBJ) -o $(TARGET)

install :
	cp -f $(TARGET) $(LIB).0.5.0
	[ -f $(LIB).0 ] || ln -s $(LIB).0.5.0 $(LIB).0
	[ -f $(LIB) ] || ln -s $(LIB).0 $(LIB)
	[ -d $(INCF) ] || mkdir $(INCF)
	[ -d $(INCF)/rtpmidi ] || mkdir $(INCF)/rtpmidi
	install Stream.h $(INCF)/rtpmidi
	install StreamWriter.h $(INCF)/rtpmidi
	install StreamReader.h $(INCF)/rtpmidi
	install types.h $(INCF)/rtpmidi
	install printingUtils.h $(INCF)/rtpmidi
	install eventUtils.h $(INCF)/rtpmidi
	install binaryUtils.h $(INCF)/rtpmidi
	install RtpMidi.h $(INCF)

uninstall :
	[ ! -f $(LIB) ] || rm $(LIB)
	[ ! -f $(LIB).0 ] || rm $(LIB).0
	[ ! -f $(LIB).0.5.0 ] || rm $(LIB).0.5.0
	[ ! -f $(INCF)/rtpmidi/Stream.h ] || rm $(INCF)/rtpmidi/Stream.h
	[ ! -f $(INCF)/rtpmidi/StreamWriter.h ] || rm $(INCF)/rtpmidi/StreamWriter.h
	[ ! -f $(INCF)/rtpmidi/StreamReader.h ] || rm $(INCF)/rtpmidi/StreamReader.h
	[ ! -f $(INCF)/rtpmidi/types.h ] || rm $(INCF)/rtpmidi/types.h
	[ ! -f $(INCF)/rtpmidi/printingUtils.h ] || rm $(INCF)/rtpmidi/printingUtils.h
	[ ! -f $(INCF)/rtpmidi/eventUtils.h ] || rm $(INCF)/rtpmidi/eventUtils.h
	[ ! -f $(INCF)/rtpmidi/binaryUtils.h ] || rm $(INCF)/rtpmidi/binaryUtils.h
	[ ! -f $(INCF)/rtpmidi/RtpMidi.h ] || rm $(INCF)/RtpMidi.h
	[ ! -d $(INCF)/rtpmidi/rtpmidi ] || rmdir $(INCF)/rtpmidi

clean :
	rm -f $(OBJ) $(TARGET)

depend:
	makedepend -- $(CXXFLAGS) -- $(SRC)
