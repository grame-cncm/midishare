
TARGET = testrtpm

CXX = g++
FLAGS = -Wall -I../../src
LIBS = -lrtpmidi -lMidiShare

# TARGETS

first : $(TARGET)

clean :
	rm -f $(TARGET) *.o *~

# TEST

test.o : test.cpp ../../src/librtpmidi.so.1.0
	$(CXX) $(FLAGS) -c test.cpp -o test.o

 $(TARGET) : test.o $(OBJ)
	$(CXX) $(LIBS) test.o $(OBJ) -o test
