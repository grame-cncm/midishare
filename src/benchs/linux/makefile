#
# midishare benchs makefile (c) Grame 2003
#

DRV = ../msBenchDriver.so
APP = ../msBenchAppl

DRVSRC := ../msBenchDriver.c ../results.c
APPSRC := ../msBenchAppl.c ../results.c
DRVOBJ := $(patsubst %.c, %.o, $(DRVSRC))
APPOBJ := $(patsubst %.c, %.o, $(APPSRC))

SRC := $(DRVSRC) $(APPSRC)
OBJ := $(DRVOBJ) $(APPOBJ)

CFLAGS   := -O3 -Wall
CC = gcc

all: $(DRV) $(APP)

$(DRV) : $(DRVOBJ) makefile
	$(CC) $(DRVOBJ) -g -shared -o $(DRV)

$(APP) : $(APPOBJ) makefile
	$(CC) $(APPOBJ) -lMidiShare -o $(APP)

install: $(TARGET)
	install -C $(DRV) /usr/lib

uninstall: 
	rm -rf /usr/lib/$(DRV)

dep:
	mkdep -a -f makefile $(CFLAGS) $(SRC)

clean:
	rm -f $(OBJ) $(DRV) $(APP)

# DO NOT DELETE
