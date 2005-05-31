
ALL = sendmidifile

CXX = g++
FLAGS = -Wall
LIBS = -lrtpmidi -lPlayer -lMidiShare

# TARGETS

first : $(ALL)

clean :
	rm *.o *~

# SENDMIDIFILE

sendmidifile.o : sendmidifile.cpp ../src/librtpmidi.so.1.0
	$(CXX) $(FLAGS) -c sendmidifile.cpp -o sendmidifile.o

sendmidifile : sendmidifile.o $(OBJ)
	$(CXX) $(LIBS) sendmidifile.o $(OBJ) -o sendmidifile
