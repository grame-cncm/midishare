SRC = ../../../tools
CFLAGS = -D __Linux__ -I../../Include -I$(SRC)
MODOPTS=-DMODVERSIONS -D__Pentium__ -DCONFIG_KERNELD -DMODULE -D__KERNEL__ -DLINUX -D_LOOSE_KERNEL_NAMES -DCONFIG_KERNELD
MODINC=-I../../../common/Headers
CC  = gcc
LIB = -lMidiShare -lpthread
MODULE = TDModule
APPL = TDrivers
CLIENT = TDClient
LOADER = TDLoader
OBJ = $(APPL).o $(LOADER).o
DEVICE = /dev/TestDrivers

all : $(CLIENT) $(MODULE).o

# user mode not yet supported for drivers
#$(APPL) : $(SRC)/$(APPL).c $(SRC)/$(APPL).h
#	$(CC) $(SRC)/$(APPL).c -g  $(CFLAGS) -o $(APPL) $(LIB)

$(CLIENT) : $(CLIENT).c $(CLIENT).h
	$(CC) $(CLIENT).c $(CFLAGS)  $(LIB) -o $(CLIENT)

$(MODULE).o : $(OBJ)
	ld -m elf_i386 -r $(OBJ) -o $(MODULE).o

$(APPL).o : $(SRC)/$(APPL).c $(SRC)/$(APPL).h $(CLIENT).h
	$(CC) $(SRC)/$(APPL).c $(CFLAGS) $(MODINC) -c  $(MODOPTS) -D__NO_VERSION__

$(LOADER).o : $(LOADER).c $(CLIENT).h  $(SRC)/$(APPL).h
	$(CC) $(LOADER).c $(CFLAGS) $(MODINC) -c  $(MODOPTS)

install :
	if [ ! -c $(DEVICE) ]; then mknod $(DEVICE) c 156 0; fi
	insmod $(MODULE).o

uninstall : 
	if [ -c $(DEVICE) ]; then rm $(DEVICE); fi
	rmmod $(MODULE)
	
clean :
	rm $(APPL) $(CLIENT) $(MODULE).o $(OBJ)
