
ToolSrc := $(wildcard *.c)
ToolSet := $(patsubst %.c, %, $(ToolSrc))

InstallDir = $(HOME)/bin
PublicDir  = /usr/local/bin
ManDir     = /usr/local/man/man1

all : $(ToolSet) 

%: %.c
	gcc $(CFLAGS) $< -lMidiShare -o $@

install : $(ToolSet)
	strip $(ToolSet)
	cp $(ToolSet) $(PublicDir)
	bzip2 -k -f man1/*.1
	install -d $(ManDir)
	mv -f man1/*.bz2 $(ManDir)

linstall : $(ToolSet)
	strip $(ToolSet)
	cp $(ToolSet) $(HOME)/bin

clean : 
	rm -f $(ToolSet)
