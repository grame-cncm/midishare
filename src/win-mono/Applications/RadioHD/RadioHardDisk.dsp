# Microsoft Developer Studio Project File - Name="RadioHardDisk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RadioHardDisk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RadioHardDisk.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RadioHardDisk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W2 /GX /O2 /I "../../include" /I "../../../lib/Network/include" /I "../../../lib" /I "../../../common/Headers" /I "../../../common/atomic" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__Windows__" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib Ws2_32.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"../RadioHardDisk.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "RadioHardDisk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W2 /Gm /GX /ZI /Od /I "../../include" /I "../../../lib/Network/include" /I "../../../lib" /I "../../../common/Headers" /I "../../../common/atomic" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "__Windows__" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib Ws2_32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"RadioHardDisk.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RadioHardDisk - Win32 Release"
# Name "RadioHardDisk - Win32 Debug"
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

SOURCE=..\..\Drivers\msNetDrivers\lib\sockets\TInetAddress.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Drivers\msNetDrivers\lib\sockets\TNetInfos.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Drivers\msNetDrivers\lib\sockets\TTCPSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Drivers\msNetDrivers\lib\sockets\TUDPSocket.cpp
# End Source File
# End Group
# Begin Group "atomic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\common\atomic\lflifoIntel.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\lib\Network\FTMA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Drivers\msNetDrivers\Handle.c
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\Network\HostTools.cpp
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

SOURCE=.\bmp1500.bmp
# End Source File
# Begin Source File

SOURCE=.\CheckPort0.c
# End Source File
# Begin Source File

SOURCE=.\icon.ico
# End Source File
# Begin Source File

SOURCE=.\INetAlert.cpp
# End Source File
# Begin Source File

SOURCE=.\RadioHD.cpp
# End Source File
# Begin Source File

SOURCE=.\RadioHD.rc
# End Source File
# Begin Source File

SOURCE=.\RadioHDDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\RadioHDFeedback.cpp
# End Source File
# Begin Source File

SOURCE=.\StateManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\VCProject\Release\mshare32.lib
# End Source File
# End Target
# End Project
