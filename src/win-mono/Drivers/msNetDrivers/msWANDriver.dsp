# Microsoft Developer Studio Project File - Name="msWANDriver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=msWANDriver - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "msWANDriver.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "msWANDriver - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WANRelease"
# PROP Intermediate_Dir "WANRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MSWANDRIVER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W2 /GX /O2 /I "../../include" /I "../../../lib/Network/include" /I "../../../lib/" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WANDriver" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib Ws2_32.lib /nologo /entry:"DllEntryPoint" /dll /machine:I386 /out:"../msWANDriver.dll"

!ELSEIF  "$(CFG)" == "msWANDriver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WANDebug"
# PROP Intermediate_Dir "WANDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MSWANDRIVER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W2 /Gm /GX /ZI /Od /I "../../include" /I "../../../lib/Network/include" /I "../../../lib/" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WANDriver" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib Ws2_32.lib /nologo /entry:"DllEntryPoint" /dll /debug /machine:I386 /out:"msWANDriver.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "msWANDriver - Win32 Release"
# Name "msWANDriver - Win32 Debug"
# Begin Group "common"

# PROP Default_Filter ""
# Begin Group "Midi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\lib\Network\Midi\EventToUDPStream.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Midi\MidiShareAppl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Midi\MidiShareDriver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Midi\MidiShareFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Midi\MidiShareTasks.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Midi\MidiTools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Midi\TMidiSlot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Midi\UDPStreamToEvent.c
# End Source File
# End Group
# Begin Group "state"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\lib\Network\state\ApplState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\state\SlotState.cpp
# End Source File
# End Group
# Begin Group "TCP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\lib\Network\Tcp\TMidiClient.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Tcp\TMidiServer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\Tcp\TWANControler.cpp
# End Source File
# End Group
# Begin Group "sockets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\sockets\TInetAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\sockets\TNetInfos.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\sockets\TTCPSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\lib\sockets\TUDPSocket.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\lib\Network\FTMA.cpp
# End Source File
# Begin Source File

SOURCE=.\Handle.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\SkewControler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TInetControler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TInetTasks.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TMidiInStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TMidiOutStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TMidiRemote.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TRemoteAppl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TRemoteMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\TRemoteSlot.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\INetAlert.cpp
# End Source File
# Begin Source File

SOURCE=.\msWANDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\msWANDriver.rc
# End Source File
# Begin Source File

SOURCE=.\msWANMain.cpp
# End Source File
# Begin Source File

SOURCE=.\NetTools.cpp
# End Source File
# Begin Source File

SOURCE=.\StateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\WinFeedback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\VCProject\Release\mshare32.lib
# End Source File
# End Target
# End Project
