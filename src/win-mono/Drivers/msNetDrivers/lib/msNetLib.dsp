# Microsoft Developer Studio Project File - Name="msNetLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=msNetLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "msNetLib.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "msNetLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W2 /GX /O2 /I "../../../include" /I "../../../../lib" /I "../../../../lib/Network/include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"msNetLib.lib"

!ELSEIF  "$(CFG)" == "msNetLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W2 /Gm /GX /ZI /Od /I "../../../include" /I "../../../../lib" /I "../../../../lib/Network/include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "msNetLib - Win32 Release"
# Name "msNetLib - Win32 Debug"
# Begin Group "Midi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Midi\EventToUDPStream.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Midi\MidiShareAppl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Midi\MidiShareDriver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Midi\MidiShareFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Midi\MidiShareTasks.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Midi\MidiTools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Midi\TMidiSlot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Midi\UDPStreamToEvent.c
# End Source File
# End Group
# Begin Group "state"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\lib\Network\state\ApplState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\state\SlotState.cpp
# End Source File
# End Group
# Begin Group "TCP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Tcp\TMidiClient.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Tcp\TMidiServer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\Tcp\TWANControler.cpp
# End Source File
# End Group
# Begin Group "sockets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sockets\TInetAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\sockets\TNetInfos.cpp
# End Source File
# Begin Source File

SOURCE=.\sockets\TTCPSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\sockets\TUDPSocket.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\lib\Network\FTMA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\SkewControler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TInetControler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TInetTasks.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TMidiInStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TMidiOutStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TMidiRemote.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TRemoteAppl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TRemoteMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\lib\Network\TRemoteSlot.cpp
# End Source File
# End Target
# End Project
