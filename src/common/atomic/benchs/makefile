
CFLAGS := $(CFLAGS) -Wall

all : lfbench ffbench fftest

lfbench : lfbench.c ../lflifo.o
	$(CC) $(CFLAGS) -O3 lfbench.c -I.. -lpthread  ../lflifo.o -o lfbench

ffbench : ffbench.c ../lffifo.o
	$(CC) $(CFLAGS) -O3 ffbench.c -I.. -lpthread  ../lffifo.o -o ffbench

fftest : fftest.c ../lffifo.o
	$(CC) $(CFLAGS) -O3 fftest.c -I.. -lpthread  ../lffifo.o -o fftest

clean :
	rm fftest ffbench lfbench ../lffifo.o ../lflifo.o
