
TARGET = testrtpmidi

CXX = g++
CXXFLAGS = -Wall -I../../src
LIBS = -L../../src -lrtpmidi -lMidiShare

# TARGETS

first : $(TARGET)

clean :
	rm -f $(TARGET) *.o *~

# TEST

test.o : test.cpp ../../src/librtpmidi.so
	$(CXX) $(CXXFLAGS) -c test.cpp -o test.o

$(TARGET) : test.o $(OBJ)
	$(CXX) $(LIBS) test.o $(OBJ) -o $(TARGET)
