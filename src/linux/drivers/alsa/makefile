
INC = -I../../Include
UTILS = ../../lib/msUtilities.o
LIB = $(UTILS) -Wl,-L../../library -lMidiShare -lasound
BIN=/usr/local/bin

Target = msAlsaSeq

MAN1=/usr/local/man/man1
ManTarget = msAlsaSeq.1

all : $(Target)

$(Target) : $(Target).c $(UTILS)
	gcc $(Target).c  -g $(INC) $(LIB) -o $(Target)

clean : 
	rm -f $(Target)

install :
	install -m 6755 -s $(Target) $(BIN)
	[ -f $(ManTarget).bz2 ] || bzip2 $(ManTarget)
	install -d $(MAN1)
	install -m 644 -o root $(ManTarget).bz2 $(MAN1)

uninstall :
	rm -f $(BIN)/$(Target)
	rm -f $(MAN1)/$(ManTarget).bz2
