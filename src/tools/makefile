
#OPTIONS = -Dmacintosh -D__Types__
#INC = -I../macosx-mono
#LIB = -framework MidiShare
LIB = -lMidiShare
CFLAGS := -O3 $(OPTIONS) $(INC)
CC    = cc
TESTS = TFonctions TEvents TFilters TDrivers TOutEvents
RES   = results

all : $(TESTS)

TDrivers : TDrivers.c TDrivers.h
	$(CC) $(CFLAGS) $(LIB) TDrivers.c -o TDrivers

TEvents : TEvents.c TEvents.h
	$(CC) $(CFLAGS) $(LIB) TEvents.c -o TEvents

TFilters : TFilters.c TFilters.h
	$(CC) $(CFLAGS) $(LIB) TFilters.c -o TFilters

TFonctions : TFonctions.c TFonctions.h
	$(CC) $(CFLAGS) $(LIB) TFonctions.c -o TFonctions

TOutEvents : TOutEvents.c TOutEvents.h
	$(CC) $(CFLAGS) $(LIB) TOutEvents.c -o TOutEvents

run :
	[ -d $(RES) ] || mkdir $(RES)
	./TFonctions > $(RES)/Tfonctions.txt
	./TEvents    > $(RES)/TEvents.txt
	./TFilters   > $(RES)/TFilters.txt
	./TDrivers   > $(RES)/TDrivers.txt

clean :
	rm $(TESTS)
