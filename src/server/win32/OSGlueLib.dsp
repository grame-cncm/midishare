# Microsoft Developer Studio Project File - Name="OSGlueLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=OSGlueLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OSGlueLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OSGlueLib.mak" CFG="OSGlueLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OSGlueLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "OSGlueLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OSGlueLib - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "../../osglue/include" /I "../../osglue/glue" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"OSGlueLib.lib"

!ELSEIF  "$(CFG)" == "OSGlueLib - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../osglue/include" /I "../../osglue/glue" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"OSGlueLib.lib"

!ENDIF 

# Begin Target

# Name "OSGlueLib - Win32 Release"
# Name "OSGlueLib - Win32 Debug"
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\osglue\win32\TLog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\osglue\win32\TMsgChan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\osglue\win32\TMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\osglue\win32\TPipe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\osglue\win32\TShMem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\osglue\win32\TThreads.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\osglue\msCommChans.cpp
# End Source File
# Begin Source File

SOURCE=..\..\osglue\msLog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\osglue\msSharedMem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\osglue\msThreads.cpp
# End Source File
# End Target
# End Project
