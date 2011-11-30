

####################################################
# packaging makefile

VERSION = 1.94
package32 = midishare-32-$(VERSION)
package64 = midishare-64-$(VERSION)

applications	:= Applications/msControl.exe Applications/msDrivers.exe
applications64	:= $(patsubst %.exe, %64.exe, $(applications))

drivers		:= Drivers/msMMSystem.dll
drivers64	:= $(patsubst %.dll, %64.dll, $(drivers))

mshare32	:= ../vs2005/Release/mshare32.dll
mshare64	:= ../vs2005-64/Release/mshare64.dll



win32:
	[ -d $(package32) ] || mkdir $(package32)
	cp $(mshare32) $(package32)
	cp $(applications) $(package32)
	cp $(drivers) $(package32)
	cp midishare.ini  $(package32)


win64:
	[ -d $(package64) ] || mkdir $(package64)
	cp $(mshare64) $(package64)
	cp $(applications64) $(package64)
	cp $(drivers64) $(package64)
	cp midishare64.ini  $(package64)/midishare.ini
