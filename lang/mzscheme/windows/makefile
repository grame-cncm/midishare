INCLUDEDIR = c:/develop/include/
DEPENDS = $(INCLUDEDIR)msType.h $(INCLUDEDIR)midishare.h $(INCLUDEDIR)player.h
THUNK = ++ccf -D__UseMSThunking__ 

all:	mz3216 mz32
mz3216:	mzplayer3216.dll mzshare3216.dll  
mz32:	mzshare32.dll  
mzp32:	mzplayer32.dll

mzplayer3216.dll:	mzplayer3216.obj
	mzc  --ld mzplayer3216.dll mzplayer3216.obj player3216.lib

mzplayer3216.obj:	mzplayer.c $(DEPENDS)
	cp mzplayer.c mzplayer3216.c
	mzc  $(THUNK) ++ccf -DWIN32  --cc mzplayer3216.c 

mzshare3216.dll:	mzshare3216.obj
	mzc --ld mzshare3216.dll mzshare3216.obj mshare3216.lib

mzshare3216.obj:	mzshare.c $(DEPENDS)
	cp mzshare.c mzshare3216.c
	mzc $(THUNK) ++ccf -DWIN32  --cc mzshare3216.c 

mzplayer32.dll:	mzplayer32.obj
	mzc  --ld mzplayer32.dll mzplayer.obj player32.lib

mzplayer32.obj:	mzplayer.c $(DEPENDS)
	mzc  ++ccf -DWIN32   --cc mzplayer.c 

mzshare32.dll:	mzshare32.obj
	mzc --ld mzshare32.dll mzshare.obj mshare32.lib

mzshare32.obj:	mzshare.c $(DEPENDS)
	mzc   ++ccf -DWIN32  --cc mzshare.c 