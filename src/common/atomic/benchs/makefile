

CFLAGS := $(CFLAGS) -Wall -DPROCESSCLOCK 
#CFLAGS := $(CFLAGS) -Wall -DPROCESSCLOCK -m64 -D__x86_64__
THREADS = libcpp/unix/TThreads.o
LIB = $(THREADS) -lpthread -lstdc++

all : lfbench ffbench ffbench2  fflock 
#fftest

lfbench : lfbench.cpp ../lflifo.o $(THREADS)
	$(CC) $(CFLAGS) -O3 lfbench.cpp -I.. $(LIB) ../lflifo.o -o lfbench

ffbench : ffbench.cpp ../lffifo.o
	$(CC) $(CFLAGS) -O3 ffbench.cpp -I..  $(LIB) ../lffifo.o -o ffbench

fflock : ffbench.cpp
	$(CC) $(CFLAGS) -DFFSEM -O3 ffbench.cpp $(LIB)  -I.. -o fflock

ffbench2 : ffbench2.c ../lffifo.o
	$(CC) $(CFLAGS) -O3 ffbench2.c -I.. -lpthread  ../lffifo.o -o ffbench2

fftest : fftest.c ../lffifo.o
	$(CC) $(CFLAGS) -O3 fftest.c -I.. -lpthread  ../lffifo.o -o fftest

clean :
	rm -f fflock fftest ffbench ffbench2 lfbench ../lffifo.o ../lflifo.o
