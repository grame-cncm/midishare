# Microsoft Developer Studio Project File - Name="Player32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Player32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Player32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Player32.mak" CFG="Player32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Player32 - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Player32 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Player32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Output"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Zp2 /MT /W3 /GX /Ob1 /I "..\src\Include" /I "." /D "WIN32" /D "__MSWindows__" /D "__Windows__" /D "__UseMSOldInterface__" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Output"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /I "." /D "WIN32" /D "_DEBUG" /D "__MSWindows__" /D "__Windows__" /D "__UseMSOldInterface__" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Player32 - Win32 Release"
# Name "Player32 - Win32 Debug"
# Begin Group "Chasers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Chasers\TChaserIterator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Chasers\TChaserVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Chasers\THashTable.cpp
# End Source File
# End Group
# Begin Group "Events"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Events\TClockEventReceiver.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Events\TEventReceiver.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Events\TEventRecorder.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Events\TExtEventReceiver.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Events\TMidiPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Events\TSMPTEEventReceiver.cpp
# End Source File
# End Group
# Begin Group "Factory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Factory\TPlayerFactory.cpp
# End Source File
# End Group
# Begin Group "Framework"

# PROP Default_Filter ""
# Begin Group "Events No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Framework\Events\TEventFactory.cpp
# End Source File
# End Group
# Begin Group "MidiAppl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Framework\MidiAppl\TMidiAppl.cpp
# End Source File
# End Group
# Begin Group "Scheduler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Framework\Scheduler\TScheduler.cpp
# End Source File
# End Group
# Begin Group "Score No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Framework\Score\TScore.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Framework\Score\TScoreFollower.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Framework\Score\TScoreIterator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Framework\Score\UScoreManager.cpp
# End Source File
# End Group
# Begin Group "Tempo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Framework\Tempo\TTempoConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Framework\Tempo\TTempoMap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Framework\Tempo\TTempoMapVisitor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Framework\Tempo\TTime.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Framework\Tempo\TTimeConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Framework\Tempo\TTimeManager.cpp
# End Source File
# End Group
# End Group
# Begin Group "Library"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Library\Player.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Library\TPlayer.cpp
# End Source File
# End Group
# Begin Group "Loop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Loop\TLoopManager.cpp
# End Source File
# End Group
# Begin Group "MidiFile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\MidiFile\Midifile.c
# End Source File
# Begin Source File

SOURCE=..\src\MidiFile\Tmtrack.c
# End Source File
# End Group
# Begin Group "Players"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Players\TRunningPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Players\TSMPTEPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Players\TSyncInPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Players\TSyncOutPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Players\TTickPlayer.cpp
# End Source File
# End Group
# Begin Group "Score"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Score\TPlayerScore.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Score\TScoreInserter.cpp
# End Source File
# End Group
# Begin Group "Synchroniser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Synchroniser\TClockSender.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Synchroniser\TPlayerSynchroniserClock.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Synchroniser\TPlayerSynchroniserExt.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Synchroniser\TPlayerSynchroniserInt.cpp
# End Source File
# End Group
# Begin Group "Tracks"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Tracks\TTrackTable.cpp
# End Source File
# End Group
# Begin Group "Utils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Utils\UMidi.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /X

!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\Utils\UTools.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /X

!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\Dllmain.c
# End Source File
# Begin Source File

SOURCE=.\mshare32.lib
# End Source File
# Begin Source File

SOURCE=.\Player32.rc
# End Source File
# End Target
# End Project
