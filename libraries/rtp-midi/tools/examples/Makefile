
TARGET = sendmidifile

CXX = g++
CXXFLAGS = -Wall -I../../src
LIBS = -L../../src -lrtpmidi -lPlayer -lMidiShare

# TARGETS

first : $(TARGET)

clean :
	rm -f $(TARGET) *.o *~

# SENDMIDIFILE

sendmidifile.o : sendmidifile.cpp ../../src/librtpmidi.so
	$(CXX) $(CXXFLAGS) -c sendmidifile.cpp -o sendmidifile.o

$(TARGET) : sendmidifile.o $(OBJ)
	$(CXX) $(LIBS) sendmidifile.o $(OBJ) -o $(TARGET)
