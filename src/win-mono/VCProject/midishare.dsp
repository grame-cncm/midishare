# Microsoft Developer Studio Project File - Name="midishare" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=midishare - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "midishare.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "midishare.mak" CFG="midishare - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "midishare - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "midishare - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "midishare - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G3 /Zp2 /MT /W3 /GX /Ot /Ob1 /I "..\..\common\headers" /D "NDEBUG" /D "__Windows__" /D "WIN32" /D "_WINDOWS" /D "__EXPORT__" /D "__LittleEndian__" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib winmm.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll /machine:I386 /out:"../mshare32.dll"

!ELSEIF  "$(CFG)" == "midishare - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G3 /Zp2 /MTd /W3 /Gm /GX /Zi /Od /Ob1 /I "..\..\common\headers" /D "_DEBUG" /D "__Windows__" /D "WIN32" /D "_WINDOWS" /D "__EXPORT__" /D "__LittleEndian__" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib winmm.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll /debug /machine:I386 /out:"mshare32.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "midishare - Win32 Release"
# Name "midishare - Win32 Debug"
# Begin Group "common"

# PROP Default_Filter ""
# Begin Group "clients"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Clients\msAlarms.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Clients\msAppls.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Clients\msConnx.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Clients\msFilter.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Clients\msMail.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Clients\msTasks.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Clients\msXmtRcv.c
# End Source File
# End Group
# Begin Group "kernel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Kernel\msHandler.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Kernel\msInit.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Kernel\msSmpte.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Kernel\msTime.c
# End Source File
# End Group
# Begin Group "memory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Memory\msEvents.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Memory\msFields.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Memory\msMem.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Memory\msMemory.c
# End Source File
# Begin Source File

SOURCE=..\..\common\Memory\msSeq.c
# End Source File
# End Group
# Begin Group "sorter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Sorter\msSorter.c
# End Source File
# End Group
# Begin Group "drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Drivers\msDriver.c
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\MidiShare.c
# End Source File
# Begin Source File

SOURCE=..\midishare.def
# End Source File
# Begin Source File

SOURCE=..\midishare.rc
# End Source File
# Begin Source File

SOURCE=..\msPrefs.c
# End Source File
# Begin Source File

SOURCE=..\WinMono.c
# End Source File
# End Target
# End Project
