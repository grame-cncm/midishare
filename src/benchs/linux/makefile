#
# midishare benchs makefile (c) Grame 2003
#

DRV = msBenchDriver.dylib
APP = msBenchDriver

DRVSRC := msBenchDriver.c results.c
APPSRC := msBenchAppl.c results.c
DRVOBJ := $(patsubst %.c, %.o, $(DRVSRC))
APPOBJ := $(patsubst %.c, %.o, $(APPSRC))

SRC := $(DRVSRC) $(APPSRC)
OBJ := $(DRVOBJ) $(APPOBJ)

CFLAGS   := -O3 -Wall
CC = gcc
#COMP = -compatibility_version 1 -current_version 1

$(DRV) : $(DRVOBJ) msBenchDriver.exp makefile
	ld -dylib -o $(DRV) -flat_namespace $(DRVOBJ) -lSystem -prebind /usr/lib/dylib1.o
#	libtool -dynamic -o $(DRV) -flat_namespace $(DRVOBJ) -lSystem -prebind
#	nmedit -s msBenchDriver.exp $(DRV)
#	libtool -arch_only ppc -noall_load $(COMP) -o $(DRV) -init _Start -flat_namespace -undefined suppress -dynamic $(DRVOBJ) -lcc_dynamic

install: $(TARGET)
	install -C $(DRV) /usr/lib

uninstall: 
	rm -rf /usr/lib/$(DRV)

dep:
	mkdep -a -f makefile $(CFLAGS) $(SRC)

clean:
	rm -f $(OBJ) $(DRV) $(APP)

# DO NOT DELETE
