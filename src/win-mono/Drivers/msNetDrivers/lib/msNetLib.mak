# Microsoft Developer Studio Generated NMAKE File, Based on msNetLib.dsp
!IF "$(CFG)" == ""
CFG=msNetLib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to msNetLib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "msNetLib - Win32 Release" && "$(CFG)" != "msNetLib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "msNetLib.mak" CFG="msNetLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "msNetLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "msNetLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "msNetLib - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\msNetLib.lib" "$(OUTDIR)\msNetLib.bsc"


CLEAN :
	-@erase "$(INTDIR)\ApplState.obj"
	-@erase "$(INTDIR)\ApplState.sbr"
	-@erase "$(INTDIR)\EventToUDPStream.obj"
	-@erase "$(INTDIR)\EventToUDPStream.sbr"
	-@erase "$(INTDIR)\FTMA.obj"
	-@erase "$(INTDIR)\FTMA.sbr"
	-@erase "$(INTDIR)\Handle.obj"
	-@erase "$(INTDIR)\Handle.sbr"
	-@erase "$(INTDIR)\HostTools.obj"
	-@erase "$(INTDIR)\HostTools.sbr"
	-@erase "$(INTDIR)\MidiShareAppl.obj"
	-@erase "$(INTDIR)\MidiShareAppl.sbr"
	-@erase "$(INTDIR)\MidiShareDriver.obj"
	-@erase "$(INTDIR)\MidiShareDriver.sbr"
	-@erase "$(INTDIR)\MidiShareFilter.obj"
	-@erase "$(INTDIR)\MidiShareFilter.sbr"
	-@erase "$(INTDIR)\MidiShareTasks.obj"
	-@erase "$(INTDIR)\MidiShareTasks.sbr"
	-@erase "$(INTDIR)\MidiTools.obj"
	-@erase "$(INTDIR)\MidiTools.sbr"
	-@erase "$(INTDIR)\SkewControler.obj"
	-@erase "$(INTDIR)\SkewControler.sbr"
	-@erase "$(INTDIR)\SlotState.obj"
	-@erase "$(INTDIR)\SlotState.sbr"
	-@erase "$(INTDIR)\TInetAddress.obj"
	-@erase "$(INTDIR)\TInetAddress.sbr"
	-@erase "$(INTDIR)\TInetControler.obj"
	-@erase "$(INTDIR)\TInetControler.sbr"
	-@erase "$(INTDIR)\TInetTasks.obj"
	-@erase "$(INTDIR)\TInetTasks.sbr"
	-@erase "$(INTDIR)\TMidiClient.obj"
	-@erase "$(INTDIR)\TMidiClient.sbr"
	-@erase "$(INTDIR)\TMidiInStream.obj"
	-@erase "$(INTDIR)\TMidiInStream.sbr"
	-@erase "$(INTDIR)\TMidiOutStream.obj"
	-@erase "$(INTDIR)\TMidiOutStream.sbr"
	-@erase "$(INTDIR)\TMidiRemote.obj"
	-@erase "$(INTDIR)\TMidiRemote.sbr"
	-@erase "$(INTDIR)\TMidiServer.obj"
	-@erase "$(INTDIR)\TMidiServer.sbr"
	-@erase "$(INTDIR)\TMidiSlot.obj"
	-@erase "$(INTDIR)\TMidiSlot.sbr"
	-@erase "$(INTDIR)\TNetInfos.obj"
	-@erase "$(INTDIR)\TNetInfos.sbr"
	-@erase "$(INTDIR)\TPacket.obj"
	-@erase "$(INTDIR)\TPacket.sbr"
	-@erase "$(INTDIR)\TRemoteAppl.obj"
	-@erase "$(INTDIR)\TRemoteAppl.sbr"
	-@erase "$(INTDIR)\TRemoteMgr.obj"
	-@erase "$(INTDIR)\TRemoteMgr.sbr"
	-@erase "$(INTDIR)\TRemoteSlot.obj"
	-@erase "$(INTDIR)\TRemoteSlot.sbr"
	-@erase "$(INTDIR)\TTCPSocket.obj"
	-@erase "$(INTDIR)\TTCPSocket.sbr"
	-@erase "$(INTDIR)\TUDPSocket.obj"
	-@erase "$(INTDIR)\TUDPSocket.sbr"
	-@erase "$(INTDIR)\TWANControler.obj"
	-@erase "$(INTDIR)\TWANControler.sbr"
	-@erase "$(INTDIR)\UDPStreamToEvent.obj"
	-@erase "$(INTDIR)\UDPStreamToEvent.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\msNetLib.bsc"
	-@erase ".\msNetLib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W2 /GX /O2 /I "../../../include" /I "../../../../lib" /I "../../../../lib/Network/include" /I "../../../../common/Headers" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__Windows__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\msNetLib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msNetLib.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\TInetAddress.sbr" \
	"$(INTDIR)\TNetInfos.sbr" \
	"$(INTDIR)\TTCPSocket.sbr" \
	"$(INTDIR)\TUDPSocket.sbr" \
	"$(INTDIR)\TMidiClient.sbr" \
	"$(INTDIR)\TMidiServer.sbr" \
	"$(INTDIR)\TWANControler.sbr" \
	"$(INTDIR)\ApplState.sbr" \
	"$(INTDIR)\SlotState.sbr" \
	"$(INTDIR)\EventToUDPStream.sbr" \
	"$(INTDIR)\MidiShareAppl.sbr" \
	"$(INTDIR)\MidiShareDriver.sbr" \
	"$(INTDIR)\MidiShareFilter.sbr" \
	"$(INTDIR)\MidiShareTasks.sbr" \
	"$(INTDIR)\MidiTools.sbr" \
	"$(INTDIR)\TMidiSlot.sbr" \
	"$(INTDIR)\UDPStreamToEvent.sbr" \
	"$(INTDIR)\FTMA.sbr" \
	"$(INTDIR)\Handle.sbr" \
	"$(INTDIR)\HostTools.sbr" \
	"$(INTDIR)\SkewControler.sbr" \
	"$(INTDIR)\TInetControler.sbr" \
	"$(INTDIR)\TInetTasks.sbr" \
	"$(INTDIR)\TMidiInStream.sbr" \
	"$(INTDIR)\TMidiOutStream.sbr" \
	"$(INTDIR)\TMidiRemote.sbr" \
	"$(INTDIR)\TPacket.sbr" \
	"$(INTDIR)\TRemoteAppl.sbr" \
	"$(INTDIR)\TRemoteMgr.sbr" \
	"$(INTDIR)\TRemoteSlot.sbr"

"$(OUTDIR)\msNetLib.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"msNetLib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\TInetAddress.obj" \
	"$(INTDIR)\TNetInfos.obj" \
	"$(INTDIR)\TTCPSocket.obj" \
	"$(INTDIR)\TUDPSocket.obj" \
	"$(INTDIR)\TMidiClient.obj" \
	"$(INTDIR)\TMidiServer.obj" \
	"$(INTDIR)\TWANControler.obj" \
	"$(INTDIR)\ApplState.obj" \
	"$(INTDIR)\SlotState.obj" \
	"$(INTDIR)\EventToUDPStream.obj" \
	"$(INTDIR)\MidiShareAppl.obj" \
	"$(INTDIR)\MidiShareDriver.obj" \
	"$(INTDIR)\MidiShareFilter.obj" \
	"$(INTDIR)\MidiShareTasks.obj" \
	"$(INTDIR)\MidiTools.obj" \
	"$(INTDIR)\TMidiSlot.obj" \
	"$(INTDIR)\UDPStreamToEvent.obj" \
	"$(INTDIR)\FTMA.obj" \
	"$(INTDIR)\Handle.obj" \
	"$(INTDIR)\HostTools.obj" \
	"$(INTDIR)\SkewControler.obj" \
	"$(INTDIR)\TInetControler.obj" \
	"$(INTDIR)\TInetTasks.obj" \
	"$(INTDIR)\TMidiInStream.obj" \
	"$(INTDIR)\TMidiOutStream.obj" \
	"$(INTDIR)\TMidiRemote.obj" \
	"$(INTDIR)\TPacket.obj" \
	"$(INTDIR)\TRemoteAppl.obj" \
	"$(INTDIR)\TRemoteMgr.obj" \
	"$(INTDIR)\TRemoteSlot.obj"

".\msNetLib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "msNetLib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\msNetLib.lib" "$(OUTDIR)\msNetLib.bsc"


CLEAN :
	-@erase "$(INTDIR)\ApplState.obj"
	-@erase "$(INTDIR)\ApplState.sbr"
	-@erase "$(INTDIR)\EventToUDPStream.obj"
	-@erase "$(INTDIR)\EventToUDPStream.sbr"
	-@erase "$(INTDIR)\FTMA.obj"
	-@erase "$(INTDIR)\FTMA.sbr"
	-@erase "$(INTDIR)\Handle.obj"
	-@erase "$(INTDIR)\Handle.sbr"
	-@erase "$(INTDIR)\HostTools.obj"
	-@erase "$(INTDIR)\HostTools.sbr"
	-@erase "$(INTDIR)\MidiShareAppl.obj"
	-@erase "$(INTDIR)\MidiShareAppl.sbr"
	-@erase "$(INTDIR)\MidiShareDriver.obj"
	-@erase "$(INTDIR)\MidiShareDriver.sbr"
	-@erase "$(INTDIR)\MidiShareFilter.obj"
	-@erase "$(INTDIR)\MidiShareFilter.sbr"
	-@erase "$(INTDIR)\MidiShareTasks.obj"
	-@erase "$(INTDIR)\MidiShareTasks.sbr"
	-@erase "$(INTDIR)\MidiTools.obj"
	-@erase "$(INTDIR)\MidiTools.sbr"
	-@erase "$(INTDIR)\SkewControler.obj"
	-@erase "$(INTDIR)\SkewControler.sbr"
	-@erase "$(INTDIR)\SlotState.obj"
	-@erase "$(INTDIR)\SlotState.sbr"
	-@erase "$(INTDIR)\TInetAddress.obj"
	-@erase "$(INTDIR)\TInetAddress.sbr"
	-@erase "$(INTDIR)\TInetControler.obj"
	-@erase "$(INTDIR)\TInetControler.sbr"
	-@erase "$(INTDIR)\TInetTasks.obj"
	-@erase "$(INTDIR)\TInetTasks.sbr"
	-@erase "$(INTDIR)\TMidiClient.obj"
	-@erase "$(INTDIR)\TMidiClient.sbr"
	-@erase "$(INTDIR)\TMidiInStream.obj"
	-@erase "$(INTDIR)\TMidiInStream.sbr"
	-@erase "$(INTDIR)\TMidiOutStream.obj"
	-@erase "$(INTDIR)\TMidiOutStream.sbr"
	-@erase "$(INTDIR)\TMidiRemote.obj"
	-@erase "$(INTDIR)\TMidiRemote.sbr"
	-@erase "$(INTDIR)\TMidiServer.obj"
	-@erase "$(INTDIR)\TMidiServer.sbr"
	-@erase "$(INTDIR)\TMidiSlot.obj"
	-@erase "$(INTDIR)\TMidiSlot.sbr"
	-@erase "$(INTDIR)\TNetInfos.obj"
	-@erase "$(INTDIR)\TNetInfos.sbr"
	-@erase "$(INTDIR)\TPacket.obj"
	-@erase "$(INTDIR)\TPacket.sbr"
	-@erase "$(INTDIR)\TRemoteAppl.obj"
	-@erase "$(INTDIR)\TRemoteAppl.sbr"
	-@erase "$(INTDIR)\TRemoteMgr.obj"
	-@erase "$(INTDIR)\TRemoteMgr.sbr"
	-@erase "$(INTDIR)\TRemoteSlot.obj"
	-@erase "$(INTDIR)\TRemoteSlot.sbr"
	-@erase "$(INTDIR)\TTCPSocket.obj"
	-@erase "$(INTDIR)\TTCPSocket.sbr"
	-@erase "$(INTDIR)\TUDPSocket.obj"
	-@erase "$(INTDIR)\TUDPSocket.sbr"
	-@erase "$(INTDIR)\TWANControler.obj"
	-@erase "$(INTDIR)\TWANControler.sbr"
	-@erase "$(INTDIR)\UDPStreamToEvent.obj"
	-@erase "$(INTDIR)\UDPStreamToEvent.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\msNetLib.bsc"
	-@erase "$(OUTDIR)\msNetLib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W2 /Gm /GX /ZI /Od /I "../../../include" /I "../../../../lib" /I "../../../../lib/Network/include" /I "../../../../common/Headers" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__Windows__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\msNetLib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msNetLib.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\TInetAddress.sbr" \
	"$(INTDIR)\TNetInfos.sbr" \
	"$(INTDIR)\TTCPSocket.sbr" \
	"$(INTDIR)\TUDPSocket.sbr" \
	"$(INTDIR)\TMidiClient.sbr" \
	"$(INTDIR)\TMidiServer.sbr" \
	"$(INTDIR)\TWANControler.sbr" \
	"$(INTDIR)\ApplState.sbr" \
	"$(INTDIR)\SlotState.sbr" \
	"$(INTDIR)\EventToUDPStream.sbr" \
	"$(INTDIR)\MidiShareAppl.sbr" \
	"$(INTDIR)\MidiShareDriver.sbr" \
	"$(INTDIR)\MidiShareFilter.sbr" \
	"$(INTDIR)\MidiShareTasks.sbr" \
	"$(INTDIR)\MidiTools.sbr" \
	"$(INTDIR)\TMidiSlot.sbr" \
	"$(INTDIR)\UDPStreamToEvent.sbr" \
	"$(INTDIR)\FTMA.sbr" \
	"$(INTDIR)\Handle.sbr" \
	"$(INTDIR)\HostTools.sbr" \
	"$(INTDIR)\SkewControler.sbr" \
	"$(INTDIR)\TInetControler.sbr" \
	"$(INTDIR)\TInetTasks.sbr" \
	"$(INTDIR)\TMidiInStream.sbr" \
	"$(INTDIR)\TMidiOutStream.sbr" \
	"$(INTDIR)\TMidiRemote.sbr" \
	"$(INTDIR)\TPacket.sbr" \
	"$(INTDIR)\TRemoteAppl.sbr" \
	"$(INTDIR)\TRemoteMgr.sbr" \
	"$(INTDIR)\TRemoteSlot.sbr"

"$(OUTDIR)\msNetLib.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\msNetLib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\TInetAddress.obj" \
	"$(INTDIR)\TNetInfos.obj" \
	"$(INTDIR)\TTCPSocket.obj" \
	"$(INTDIR)\TUDPSocket.obj" \
	"$(INTDIR)\TMidiClient.obj" \
	"$(INTDIR)\TMidiServer.obj" \
	"$(INTDIR)\TWANControler.obj" \
	"$(INTDIR)\ApplState.obj" \
	"$(INTDIR)\SlotState.obj" \
	"$(INTDIR)\EventToUDPStream.obj" \
	"$(INTDIR)\MidiShareAppl.obj" \
	"$(INTDIR)\MidiShareDriver.obj" \
	"$(INTDIR)\MidiShareFilter.obj" \
	"$(INTDIR)\MidiShareTasks.obj" \
	"$(INTDIR)\MidiTools.obj" \
	"$(INTDIR)\TMidiSlot.obj" \
	"$(INTDIR)\UDPStreamToEvent.obj" \
	"$(INTDIR)\FTMA.obj" \
	"$(INTDIR)\Handle.obj" \
	"$(INTDIR)\HostTools.obj" \
	"$(INTDIR)\SkewControler.obj" \
	"$(INTDIR)\TInetControler.obj" \
	"$(INTDIR)\TInetTasks.obj" \
	"$(INTDIR)\TMidiInStream.obj" \
	"$(INTDIR)\TMidiOutStream.obj" \
	"$(INTDIR)\TMidiRemote.obj" \
	"$(INTDIR)\TPacket.obj" \
	"$(INTDIR)\TRemoteAppl.obj" \
	"$(INTDIR)\TRemoteMgr.obj" \
	"$(INTDIR)\TRemoteSlot.obj"

"$(OUTDIR)\msNetLib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("msNetLib.dep")
!INCLUDE "msNetLib.dep"
!ELSE 
!MESSAGE Warning: cannot find "msNetLib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "msNetLib - Win32 Release" || "$(CFG)" == "msNetLib - Win32 Debug"
SOURCE=.\sockets\TInetAddress.cpp

"$(INTDIR)\TInetAddress.obj"	"$(INTDIR)\TInetAddress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\sockets\TNetInfos.cpp

"$(INTDIR)\TNetInfos.obj"	"$(INTDIR)\TNetInfos.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\sockets\TTCPSocket.cpp

"$(INTDIR)\TTCPSocket.obj"	"$(INTDIR)\TTCPSocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\sockets\TUDPSocket.cpp

"$(INTDIR)\TUDPSocket.obj"	"$(INTDIR)\TUDPSocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Tcp\TMidiClient.cpp

"$(INTDIR)\TMidiClient.obj"	"$(INTDIR)\TMidiClient.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Tcp\TMidiServer.cpp

"$(INTDIR)\TMidiServer.obj"	"$(INTDIR)\TMidiServer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Tcp\TWANControler.cpp

"$(INTDIR)\TWANControler.obj"	"$(INTDIR)\TWANControler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\state\ApplState.cpp

"$(INTDIR)\ApplState.obj"	"$(INTDIR)\ApplState.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\state\SlotState.cpp

"$(INTDIR)\SlotState.obj"	"$(INTDIR)\SlotState.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Midi\EventToUDPStream.c

"$(INTDIR)\EventToUDPStream.obj"	"$(INTDIR)\EventToUDPStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Midi\MidiShareAppl.cpp

"$(INTDIR)\MidiShareAppl.obj"	"$(INTDIR)\MidiShareAppl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Midi\MidiShareDriver.cpp

"$(INTDIR)\MidiShareDriver.obj"	"$(INTDIR)\MidiShareDriver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Midi\MidiShareFilter.cpp

"$(INTDIR)\MidiShareFilter.obj"	"$(INTDIR)\MidiShareFilter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Midi\MidiShareTasks.cpp

"$(INTDIR)\MidiShareTasks.obj"	"$(INTDIR)\MidiShareTasks.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Midi\MidiTools.cpp

"$(INTDIR)\MidiTools.obj"	"$(INTDIR)\MidiTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Midi\TMidiSlot.cpp

"$(INTDIR)\TMidiSlot.obj"	"$(INTDIR)\TMidiSlot.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\Midi\UDPStreamToEvent.c

"$(INTDIR)\UDPStreamToEvent.obj"	"$(INTDIR)\UDPStreamToEvent.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\FTMA.cpp

"$(INTDIR)\FTMA.obj"	"$(INTDIR)\FTMA.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Handle.c

"$(INTDIR)\Handle.obj"	"$(INTDIR)\Handle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\HostTools.cpp

"$(INTDIR)\HostTools.obj"	"$(INTDIR)\HostTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\SkewControler.cpp

"$(INTDIR)\SkewControler.obj"	"$(INTDIR)\SkewControler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TInetControler.cpp

"$(INTDIR)\TInetControler.obj"	"$(INTDIR)\TInetControler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TInetTasks.cpp

"$(INTDIR)\TInetTasks.obj"	"$(INTDIR)\TInetTasks.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TMidiInStream.cpp

"$(INTDIR)\TMidiInStream.obj"	"$(INTDIR)\TMidiInStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TMidiOutStream.cpp

"$(INTDIR)\TMidiOutStream.obj"	"$(INTDIR)\TMidiOutStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TMidiRemote.cpp

"$(INTDIR)\TMidiRemote.obj"	"$(INTDIR)\TMidiRemote.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TPacket.cpp

"$(INTDIR)\TPacket.obj"	"$(INTDIR)\TPacket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TRemoteAppl.cpp

"$(INTDIR)\TRemoteAppl.obj"	"$(INTDIR)\TRemoteAppl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TRemoteMgr.cpp

"$(INTDIR)\TRemoteMgr.obj"	"$(INTDIR)\TRemoteMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\lib\Network\TRemoteSlot.cpp

"$(INTDIR)\TRemoteSlot.obj"	"$(INTDIR)\TRemoteSlot.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

