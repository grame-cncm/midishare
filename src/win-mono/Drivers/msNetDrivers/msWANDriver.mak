# Microsoft Developer Studio Generated NMAKE File, Based on msWANDriver.dsp
!IF "$(CFG)" == ""
CFG=msWANDriver - Win32 Debug
!MESSAGE No configuration specified. Defaulting to msWANDriver - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "msWANDriver - Win32 Release" && "$(CFG)" != "msWANDriver - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "msWANDriver.mak" CFG="msWANDriver - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "msWANDriver - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "msWANDriver - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "msWANDriver - Win32 Release"

OUTDIR=.\WANRelease
INTDIR=.\WANRelease

ALL : "..\msWANDriver.dll"


CLEAN :
	-@erase "$(INTDIR)\ApplState.obj"
	-@erase "$(INTDIR)\EventToUDPStream.obj"
	-@erase "$(INTDIR)\FTMA.obj"
	-@erase "$(INTDIR)\Handle.obj"
	-@erase "$(INTDIR)\INetAlert.obj"
	-@erase "$(INTDIR)\MidiShareAppl.obj"
	-@erase "$(INTDIR)\MidiShareDriver.obj"
	-@erase "$(INTDIR)\MidiShareFilter.obj"
	-@erase "$(INTDIR)\MidiShareTasks.obj"
	-@erase "$(INTDIR)\MidiTools.obj"
	-@erase "$(INTDIR)\msWANDriver.obj"
	-@erase "$(INTDIR)\msWANDriver.res"
	-@erase "$(INTDIR)\msWANMain.obj"
	-@erase "$(INTDIR)\NetTools.obj"
	-@erase "$(INTDIR)\SkewControler.obj"
	-@erase "$(INTDIR)\SlotState.obj"
	-@erase "$(INTDIR)\StateManager.obj"
	-@erase "$(INTDIR)\TInetAddress.obj"
	-@erase "$(INTDIR)\TInetControler.obj"
	-@erase "$(INTDIR)\TInetTasks.obj"
	-@erase "$(INTDIR)\TMidiClient.obj"
	-@erase "$(INTDIR)\TMidiInStream.obj"
	-@erase "$(INTDIR)\TMidiOutStream.obj"
	-@erase "$(INTDIR)\TMidiRemote.obj"
	-@erase "$(INTDIR)\TMidiServer.obj"
	-@erase "$(INTDIR)\TMidiSlot.obj"
	-@erase "$(INTDIR)\TNetInfos.obj"
	-@erase "$(INTDIR)\TPacket.obj"
	-@erase "$(INTDIR)\TRemoteAppl.obj"
	-@erase "$(INTDIR)\TRemoteMgr.obj"
	-@erase "$(INTDIR)\TRemoteSlot.obj"
	-@erase "$(INTDIR)\TTCPSocket.obj"
	-@erase "$(INTDIR)\TUDPSocket.obj"
	-@erase "$(INTDIR)\TWANControler.obj"
	-@erase "$(INTDIR)\UDPStreamToEvent.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WinFeedback.obj"
	-@erase "$(OUTDIR)\msWANDriver.exp"
	-@erase "$(OUTDIR)\msWANDriver.lib"
	-@erase "..\msWANDriver.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W2 /GX /O2 /I "../../include" /I "../../../lib/Network/include" /I "../../../lib/" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WANDriver" /Fp"$(INTDIR)\msWANDriver.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\msWANDriver.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msWANDriver.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib Ws2_32.lib /nologo /entry:"DllEntryPoint" /dll /incremental:no /pdb:"$(OUTDIR)\msWANDriver.pdb" /machine:I386 /out:"../msWANDriver.dll" /implib:"$(OUTDIR)\msWANDriver.lib" 
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
	"$(INTDIR)\INetAlert.obj" \
	"$(INTDIR)\msWANDriver.obj" \
	"$(INTDIR)\msWANMain.obj" \
	"$(INTDIR)\NetTools.obj" \
	"$(INTDIR)\StateManager.obj" \
	"$(INTDIR)\WinFeedback.obj" \
	"$(INTDIR)\msWANDriver.res" \
	"..\..\VCProject\Release\mshare32.lib"

"..\msWANDriver.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "msWANDriver - Win32 Debug"

OUTDIR=.\WANDebug
INTDIR=.\WANDebug

ALL : ".\msWANDriver.dll"


CLEAN :
	-@erase "$(INTDIR)\ApplState.obj"
	-@erase "$(INTDIR)\EventToUDPStream.obj"
	-@erase "$(INTDIR)\FTMA.obj"
	-@erase "$(INTDIR)\Handle.obj"
	-@erase "$(INTDIR)\INetAlert.obj"
	-@erase "$(INTDIR)\MidiShareAppl.obj"
	-@erase "$(INTDIR)\MidiShareDriver.obj"
	-@erase "$(INTDIR)\MidiShareFilter.obj"
	-@erase "$(INTDIR)\MidiShareTasks.obj"
	-@erase "$(INTDIR)\MidiTools.obj"
	-@erase "$(INTDIR)\msWANDriver.obj"
	-@erase "$(INTDIR)\msWANDriver.res"
	-@erase "$(INTDIR)\msWANMain.obj"
	-@erase "$(INTDIR)\NetTools.obj"
	-@erase "$(INTDIR)\SkewControler.obj"
	-@erase "$(INTDIR)\SlotState.obj"
	-@erase "$(INTDIR)\StateManager.obj"
	-@erase "$(INTDIR)\TInetAddress.obj"
	-@erase "$(INTDIR)\TInetControler.obj"
	-@erase "$(INTDIR)\TInetTasks.obj"
	-@erase "$(INTDIR)\TMidiClient.obj"
	-@erase "$(INTDIR)\TMidiInStream.obj"
	-@erase "$(INTDIR)\TMidiOutStream.obj"
	-@erase "$(INTDIR)\TMidiRemote.obj"
	-@erase "$(INTDIR)\TMidiServer.obj"
	-@erase "$(INTDIR)\TMidiSlot.obj"
	-@erase "$(INTDIR)\TNetInfos.obj"
	-@erase "$(INTDIR)\TPacket.obj"
	-@erase "$(INTDIR)\TRemoteAppl.obj"
	-@erase "$(INTDIR)\TRemoteMgr.obj"
	-@erase "$(INTDIR)\TRemoteSlot.obj"
	-@erase "$(INTDIR)\TTCPSocket.obj"
	-@erase "$(INTDIR)\TUDPSocket.obj"
	-@erase "$(INTDIR)\TWANControler.obj"
	-@erase "$(INTDIR)\UDPStreamToEvent.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WinFeedback.obj"
	-@erase "$(OUTDIR)\msWANDriver.exp"
	-@erase "$(OUTDIR)\msWANDriver.lib"
	-@erase "$(OUTDIR)\msWANDriver.pdb"
	-@erase ".\msWANDriver.dll"
	-@erase ".\msWANDriver.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W2 /Gm /GX /ZI /Od /I "../../include" /I "../../../lib/Network/include" /I "../../../lib/" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WANDriver" /Fp"$(INTDIR)\msWANDriver.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\msWANDriver.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msWANDriver.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib Ws2_32.lib /nologo /entry:"DllEntryPoint" /dll /incremental:yes /pdb:"$(OUTDIR)\msWANDriver.pdb" /debug /machine:I386 /out:"msWANDriver.dll" /implib:"$(OUTDIR)\msWANDriver.lib" /pdbtype:sept 
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
	"$(INTDIR)\INetAlert.obj" \
	"$(INTDIR)\msWANDriver.obj" \
	"$(INTDIR)\msWANMain.obj" \
	"$(INTDIR)\NetTools.obj" \
	"$(INTDIR)\StateManager.obj" \
	"$(INTDIR)\WinFeedback.obj" \
	"$(INTDIR)\msWANDriver.res" \
	"..\..\VCProject\Release\mshare32.lib"

".\msWANDriver.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("msWANDriver.dep")
!INCLUDE "msWANDriver.dep"
!ELSE 
!MESSAGE Warning: cannot find "msWANDriver.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "msWANDriver - Win32 Release" || "$(CFG)" == "msWANDriver - Win32 Debug"
SOURCE=..\..\..\lib\Network\Midi\EventToUDPStream.c

"$(INTDIR)\EventToUDPStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiShareAppl.cpp

"$(INTDIR)\MidiShareAppl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiShareDriver.cpp

"$(INTDIR)\MidiShareDriver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiShareFilter.cpp

"$(INTDIR)\MidiShareFilter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiShareTasks.cpp

"$(INTDIR)\MidiShareTasks.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\MidiTools.cpp

"$(INTDIR)\MidiTools.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\TMidiSlot.cpp

"$(INTDIR)\TMidiSlot.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Midi\UDPStreamToEvent.c

"$(INTDIR)\UDPStreamToEvent.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\state\ApplState.cpp

"$(INTDIR)\ApplState.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\state\SlotState.cpp

"$(INTDIR)\SlotState.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Tcp\TMidiClient.cpp

"$(INTDIR)\TMidiClient.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Tcp\TMidiServer.cpp

"$(INTDIR)\TMidiServer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\Tcp\TWANControler.cpp

"$(INTDIR)\TWANControler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\sockets\TInetAddress.cpp

"$(INTDIR)\TInetAddress.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\sockets\TNetInfos.cpp

"$(INTDIR)\TNetInfos.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\sockets\TTCPSocket.cpp

"$(INTDIR)\TTCPSocket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\lib\sockets\TUDPSocket.cpp

"$(INTDIR)\TUDPSocket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\FTMA.cpp

"$(INTDIR)\FTMA.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Handle.c

"$(INTDIR)\Handle.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\..\lib\Network\SkewControler.cpp

"$(INTDIR)\SkewControler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TInetControler.cpp

"$(INTDIR)\TInetControler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TInetTasks.cpp

"$(INTDIR)\TInetTasks.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TMidiInStream.cpp

"$(INTDIR)\TMidiInStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TMidiOutStream.cpp

"$(INTDIR)\TMidiOutStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TMidiRemote.cpp

"$(INTDIR)\TMidiRemote.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TPacket.cpp

"$(INTDIR)\TPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TRemoteAppl.cpp

"$(INTDIR)\TRemoteAppl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TRemoteMgr.cpp

"$(INTDIR)\TRemoteMgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\lib\Network\TRemoteSlot.cpp

"$(INTDIR)\TRemoteSlot.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\INetAlert.cpp

"$(INTDIR)\INetAlert.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\msWANDriver.cpp

"$(INTDIR)\msWANDriver.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\msWANDriver.rc

"$(INTDIR)\msWANDriver.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\msWANMain.cpp

"$(INTDIR)\msWANMain.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\NetTools.cpp

"$(INTDIR)\NetTools.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StateManager.cpp

"$(INTDIR)\StateManager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WinFeedback.cpp

"$(INTDIR)\WinFeedback.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

