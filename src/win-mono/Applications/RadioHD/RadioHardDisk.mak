# Microsoft Developer Studio Generated NMAKE File, Based on RadioHardDisk.dsp
!IF "$(CFG)" == ""
CFG=RadioHardDisk - Win32 Debug
!MESSAGE No configuration specified. Defaulting to RadioHardDisk - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "RadioHardDisk - Win32 Release" && "$(CFG)" != "RadioHardDisk - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RadioHardDisk.mak" CFG="RadioHardDisk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RadioHardDisk - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RadioHardDisk - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "RadioHardDisk - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\RadioHardDisk.exe" "$(OUTDIR)\RadioHardDisk.bsc"


CLEAN :
	-@erase "$(INTDIR)\ApplState.obj"
	-@erase "$(INTDIR)\ApplState.sbr"
	-@erase "$(INTDIR)\CheckPort0.obj"
	-@erase "$(INTDIR)\CheckPort0.sbr"
	-@erase "$(INTDIR)\EventToUDPStream.obj"
	-@erase "$(INTDIR)\EventToUDPStream.sbr"
	-@erase "$(INTDIR)\FTMA.obj"
	-@erase "$(INTDIR)\FTMA.sbr"
	-@erase "$(INTDIR)\Handle.obj"
	-@erase "$(INTDIR)\Handle.sbr"
	-@erase "$(INTDIR)\HostTools.obj"
	-@erase "$(INTDIR)\HostTools.sbr"
	-@erase "$(INTDIR)\INetAlert.obj"
	-@erase "$(INTDIR)\INetAlert.sbr"
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
	-@erase "$(INTDIR)\RadioHD.obj"
	-@erase "$(INTDIR)\RadioHD.res"
	-@erase "$(INTDIR)\RadioHD.sbr"
	-@erase "$(INTDIR)\RadioHDDriver.obj"
	-@erase "$(INTDIR)\RadioHDDriver.sbr"
	-@erase "$(INTDIR)\RadioHDFeedback.obj"
	-@erase "$(INTDIR)\RadioHDFeedback.sbr"
	-@erase "$(INTDIR)\SkewControler.obj"
	-@erase "$(INTDIR)\SkewControler.sbr"
	-@erase "$(INTDIR)\SlotState.obj"
	-@erase "$(INTDIR)\SlotState.sbr"
	-@erase "$(INTDIR)\StateManager.obj"
	-@erase "$(INTDIR)\StateManager.sbr"
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
	-@erase "$(OUTDIR)\RadioHardDisk.bsc"
	-@erase "..\RadioHardDisk.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W2 /GX /O2 /I "../../include" /I "../../../lib/Network/include" /I "../../../lib" /I "../../../common/Headers" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__Windows__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RadioHardDisk.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\RadioHD.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RadioHardDisk.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\EventToUDPStream.sbr" \
	"$(INTDIR)\MidiShareAppl.sbr" \
	"$(INTDIR)\MidiShareDriver.sbr" \
	"$(INTDIR)\MidiShareFilter.sbr" \
	"$(INTDIR)\MidiShareTasks.sbr" \
	"$(INTDIR)\MidiTools.sbr" \
	"$(INTDIR)\TMidiSlot.sbr" \
	"$(INTDIR)\UDPStreamToEvent.sbr" \
	"$(INTDIR)\ApplState.sbr" \
	"$(INTDIR)\SlotState.sbr" \
	"$(INTDIR)\TMidiClient.sbr" \
	"$(INTDIR)\TMidiServer.sbr" \
	"$(INTDIR)\TWANControler.sbr" \
	"$(INTDIR)\TInetAddress.sbr" \
	"$(INTDIR)\TNetInfos.sbr" \
	"$(INTDIR)\TTCPSocket.sbr" \
	"$(INTDIR)\TUDPSocket.sbr" \
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
	"$(INTDIR)\TRemoteSlot.sbr" \
	"$(INTDIR)\CheckPort0.sbr" \
	"$(INTDIR)\INetAlert.sbr" \
	"$(INTDIR)\RadioHD.sbr" \
	"$(INTDIR)\RadioHDDriver.sbr" \
	"$(INTDIR)\RadioHDFeedback.sbr" \
	"$(INTDIR)\StateManager.sbr"

"$(OUTDIR)\RadioHardDisk.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib Ws2_32.lib winmm.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\RadioHardDisk.pdb" /machine:I386 /out:"../RadioHardDisk.exe" 
LINK32_OBJS= \
	"$(INTDIR)\EventToUDPStream.obj" \
	"$(INTDIR)\MidiShareAppl.obj" \
	"$(INTDIR)\MidiShareDriver.obj" \
	"$(INTDIR)\MidiShareFilter.obj" \
	"$(INTDIR)\MidiShareTasks.obj" \
	"$(INTDIR)\MidiTools.obj" \
	"$(INTDIR)\TMidiSlot.obj" \
	"$(INTDIR)\UDPStreamToEvent.obj" \
	"$(INTDIR)\ApplState.obj" \
	"$(INTDIR)\SlotState.obj" \
	"$(INTDIR)\TMidiClient.obj" \
	"$(INTDIR)\TMidiServer.obj" \
	"$(INTDIR)\TWANControler.obj" \
	"$(INTDIR)\TInetAddress.obj" \
	"$(INTDIR)\TNetInfos.obj" \
	"$(INTDIR)\TTCPSocket.obj" \
	"$(INTDIR)\TUDPSocket.obj" \
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
	"$(INTDIR)\TRemoteSlot.obj" \
	"$(INTDIR)\CheckPort0.obj" \
	"$(INTDIR)\INetAlert.obj" \
	"$(INTDIR)\RadioHD.obj" \
	"$(INTDIR)\RadioHDDriver.obj" \
	"$(INTDIR)\RadioHDFeedback.obj" \
	"$(INTDIR)\StateManager.obj" \
	"$(INTDIR)\RadioHD.res" \
	"..\..\VCProject\Release\mshare32.lib"

"..\RadioHardDisk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RadioHardDisk - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\RadioHardDisk.exe" "$(OUTDIR)\RadioHardDisk.bsc"


CLEAN :
	-@erase "$(INTDIR)\ApplState.obj"
	-@erase "$(INTDIR)\ApplState.sbr"
	-@erase "$(INTDIR)\CheckPort0.obj"
	-@erase "$(INTDIR)\CheckPort0.sbr"
	-@erase "$(INTDIR)\EventToUDPStream.obj"
	-@erase "$(INTDIR)\EventToUDPStream.sbr"
	-@erase "$(INTDIR)\FTMA.obj"
	-@erase "$(INTDIR)\FTMA.sbr"
	-@erase "$(INTDIR)\Handle.obj"
	-@erase "$(INTDIR)\Handle.sbr"
	-@erase "$(INTDIR)\HostTools.obj"
	-@erase "$(INTDIR)\HostTools.sbr"
	-@erase "$(INTDIR)\INetAlert.obj"
	-@erase "$(INTDIR)\INetAlert.sbr"
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
	-@erase "$(INTDIR)\RadioHD.obj"
	-@erase "$(INTDIR)\RadioHD.res"
	-@erase "$(INTDIR)\RadioHD.sbr"
	-@erase "$(INTDIR)\RadioHDDriver.obj"
	-@erase "$(INTDIR)\RadioHDDriver.sbr"
	-@erase "$(INTDIR)\RadioHDFeedback.obj"
	-@erase "$(INTDIR)\RadioHDFeedback.sbr"
	-@erase "$(INTDIR)\SkewControler.obj"
	-@erase "$(INTDIR)\SkewControler.sbr"
	-@erase "$(INTDIR)\SlotState.obj"
	-@erase "$(INTDIR)\SlotState.sbr"
	-@erase "$(INTDIR)\StateManager.obj"
	-@erase "$(INTDIR)\StateManager.sbr"
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
	-@erase "$(OUTDIR)\RadioHardDisk.bsc"
	-@erase "$(OUTDIR)\RadioHardDisk.pdb"
	-@erase ".\RadioHardDisk.exe"
	-@erase ".\RadioHardDisk.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W2 /Gm /GX /ZI /Od /I "../../include" /I "../../../lib/Network/include" /I "../../../lib" /I "../../../common/Headers" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__Windows__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RadioHardDisk.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\RadioHD.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RadioHardDisk.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\EventToUDPStream.sbr" \
	"$(INTDIR)\MidiShareAppl.sbr" \
	"$(INTDIR)\MidiShareDriver.sbr" \
	"$(INTDIR)\MidiShareFilter.sbr" \
	"$(INTDIR)\MidiShareTasks.sbr" \
	"$(INTDIR)\MidiTools.sbr" \
	"$(INTDIR)\TMidiSlot.sbr" \
	"$(INTDIR)\UDPStreamToEvent.sbr" \
	"$(INTDIR)\ApplState.sbr" \
	"$(INTDIR)\SlotState.sbr" \
	"$(INTDIR)\TMidiClient.sbr" \
	"$(INTDIR)\TMidiServer.sbr" \
	"$(INTDIR)\TWANControler.sbr" \
	"$(INTDIR)\TInetAddress.sbr" \
	"$(INTDIR)\TNetInfos.sbr" \
	"$(INTDIR)\TTCPSocket.sbr" \
	"$(INTDIR)\TUDPSocket.sbr" \
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
	"$(INTDIR)\TRemoteSlot.sbr" \
	"$(INTDIR)\CheckPort0.sbr" \
	"$(INTDIR)\INetAlert.sbr" \
	"$(INTDIR)\RadioHD.sbr" \
	"$(INTDIR)\RadioHDDriver.sbr" \
	"$(INTDIR)\RadioHDFeedback.sbr" \
	"$(INTDIR)\StateManager.sbr"

"$(OUTDIR)\RadioHardDisk.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib Ws2_32.lib winmm.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\RadioHardDisk.pdb" /debug /machine:I386 /out:"RadioHardDisk.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\EventToUDPStream.obj" \
	"$(INTDIR)\MidiShareAppl.obj" \
	"$(INTDIR)\MidiShareDriver.obj" \
	"$(INTDIR)\MidiShareFilter.obj" \
	"$(INTDIR)\MidiShareTasks.obj" \
	"$(INTDIR)\MidiTools.obj" \
	"$(INTDIR)\TMidiSlot.obj" \
	"$(INTDIR)\UDPStreamToEvent.obj" \
	"$(INTDIR)\ApplState.obj" \
	"$(INTDIR)\SlotState.obj" \
	"$(INTDIR)\TMidiClient.obj" \
	"$(INTDIR)\TMidiServer.obj" \
	"$(INTDIR)\TWANControler.obj" \
	"$(INTDIR)\TInetAddress.obj" \
	"$(INTDIR)\TNetInfos.obj" \
	"$(INTDIR)\TTCPSocket.obj" \
	"$(INTDIR)\TUDPSocket.obj" \
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
	"$(INTDIR)\TRemoteSlot.obj" \
	"$(INTDIR)\CheckPort0.obj" \
	"$(INTDIR)\INetAlert.obj" \
	"$(INTDIR)\RadioHD.obj" \
	"$(INTDIR)\RadioHDDriver.obj" \
	"$(INTDIR)\RadioHDFeedback.obj" \
	"$(INTDIR)\StateManager.obj" \
	"$(INTDIR)\RadioHD.res" \
	"..\..\VCProject\Release\mshare32.lib"

".\RadioHardDisk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RadioHardDisk.dep")
!INCLUDE "RadioHardDisk.dep"
!ELSE 
!MESSAGE Warning: cannot find "RadioHardDisk.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RadioHardDisk - Win32 Release" || "$(CFG)" == "RadioHardDisk - Win32 Debug"
SOURCE=..\..\..\lib\Network\Midi\EventToUDPStream.c

"$(INTDIR)\EventToUDPStream.obj"	"$(INTDIR)\EventToUDPStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiShareAppl.cpp

"$(INTDIR)\MidiShareAppl.obj"	"$(INTDIR)\MidiShareAppl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiShareDriver.cpp

"$(INTDIR)\MidiShareDriver.obj"	"$(INTDIR)\MidiShareDriver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiShareFilter.cpp

"$(INTDIR)\MidiShareFilter.obj"	"$(INTDIR)\MidiShareFilter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiShareTasks.cpp

"$(INTDIR)\MidiShareTasks.obj"	"$(INTDIR)\MidiShareTasks.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiTools.cpp

"$(INTDIR)\MidiTools.obj"	"$(INTDIR)\MidiTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\TMidiSlot.cpp

"$(INTDIR)\TMidiSlot.obj"	"$(INTDIR)\TMidiSlot.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\UDPStreamToEvent.c

"$(INTDIR)\UDPStreamToEvent.obj"	"$(INTDIR)\UDPStreamToEvent.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\state\ApplState.cpp

"$(INTDIR)\ApplState.obj"	"$(INTDIR)\ApplState.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\state\SlotState.cpp

"$(INTDIR)\SlotState.obj"	"$(INTDIR)\SlotState.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Tcp\TMidiClient.cpp

"$(INTDIR)\TMidiClient.obj"	"$(INTDIR)\TMidiClient.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Tcp\TMidiServer.cpp

"$(INTDIR)\TMidiServer.obj"	"$(INTDIR)\TMidiServer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Tcp\TWANControler.cpp

"$(INTDIR)\TWANControler.obj"	"$(INTDIR)\TWANControler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Drivers\msNetDrivers\lib\sockets\TInetAddress.cpp

"$(INTDIR)\TInetAddress.obj"	"$(INTDIR)\TInetAddress.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Drivers\msNetDrivers\lib\sockets\TNetInfos.cpp

"$(INTDIR)\TNetInfos.obj"	"$(INTDIR)\TNetInfos.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Drivers\msNetDrivers\lib\sockets\TTCPSocket.cpp

"$(INTDIR)\TTCPSocket.obj"	"$(INTDIR)\TTCPSocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Drivers\msNetDrivers\lib\sockets\TUDPSocket.cpp

"$(INTDIR)\TUDPSocket.obj"	"$(INTDIR)\TUDPSocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\FTMA.cpp

"$(INTDIR)\FTMA.obj"	"$(INTDIR)\FTMA.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Drivers\msNetDrivers\Handle.c

"$(INTDIR)\Handle.obj"	"$(INTDIR)\Handle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\HostTools.cpp

"$(INTDIR)\HostTools.obj"	"$(INTDIR)\HostTools.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\SkewControler.cpp

"$(INTDIR)\SkewControler.obj"	"$(INTDIR)\SkewControler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TInetControler.cpp

"$(INTDIR)\TInetControler.obj"	"$(INTDIR)\TInetControler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TInetTasks.cpp

"$(INTDIR)\TInetTasks.obj"	"$(INTDIR)\TInetTasks.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TMidiInStream.cpp

"$(INTDIR)\TMidiInStream.obj"	"$(INTDIR)\TMidiInStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TMidiOutStream.cpp

"$(INTDIR)\TMidiOutStream.obj"	"$(INTDIR)\TMidiOutStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TMidiRemote.cpp

"$(INTDIR)\TMidiRemote.obj"	"$(INTDIR)\TMidiRemote.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TPacket.cpp

"$(INTDIR)\TPacket.obj"	"$(INTDIR)\TPacket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TRemoteAppl.cpp

"$(INTDIR)\TRemoteAppl.obj"	"$(INTDIR)\TRemoteAppl.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TRemoteMgr.cpp

"$(INTDIR)\TRemoteMgr.obj"	"$(INTDIR)\TRemoteMgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TRemoteSlot.cpp

"$(INTDIR)\TRemoteSlot.obj"	"$(INTDIR)\TRemoteSlot.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\CheckPort0.c

"$(INTDIR)\CheckPort0.obj"	"$(INTDIR)\CheckPort0.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\INetAlert.cpp

"$(INTDIR)\INetAlert.obj"	"$(INTDIR)\INetAlert.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadioHD.cpp

"$(INTDIR)\RadioHD.obj"	"$(INTDIR)\RadioHD.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadioHD.rc

"$(INTDIR)\RadioHD.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\RadioHDDriver.cpp

"$(INTDIR)\RadioHDDriver.obj"	"$(INTDIR)\RadioHDDriver.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RadioHDFeedback.cpp

"$(INTDIR)\RadioHDFeedback.obj"	"$(INTDIR)\RadioHDFeedback.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StateManager.cpp

"$(INTDIR)\StateManager.obj"	"$(INTDIR)\StateManager.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

