# Microsoft Developer Studio Generated NMAKE File, Based on Player32.dsp
!IF "$(CFG)" == ""
CFG=Player32 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Player32 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Player32 - Win32 Release" && "$(CFG)" !=\
 "Player32 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Player32 - Win32 Release"

OUTDIR=.\Output
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Output
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Player32.dll"

!ELSE 

ALL : "$(OUTDIR)\Player32.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Dllmain.obj"
	-@erase "$(INTDIR)\Midifile.obj"
	-@erase "$(INTDIR)\Player.obj"
	-@erase "$(INTDIR)\Player32.res"
	-@erase "$(INTDIR)\TChaserIterator.obj"
	-@erase "$(INTDIR)\TChaserVisitor.obj"
	-@erase "$(INTDIR)\TClockEventReceiver.obj"
	-@erase "$(INTDIR)\TClockSender.obj"
	-@erase "$(INTDIR)\TEventFactory.obj"
	-@erase "$(INTDIR)\TEventReceiver.obj"
	-@erase "$(INTDIR)\TEventRecorder.obj"
	-@erase "$(INTDIR)\TExtEventReceiver.obj"
	-@erase "$(INTDIR)\THashTable.obj"
	-@erase "$(INTDIR)\TLoopManager.obj"
	-@erase "$(INTDIR)\TMidiAppl.obj"
	-@erase "$(INTDIR)\TMidiPlayer.obj"
	-@erase "$(INTDIR)\Tmtrack.obj"
	-@erase "$(INTDIR)\TPlayer.obj"
	-@erase "$(INTDIR)\TPlayerFactory.obj"
	-@erase "$(INTDIR)\TPlayerScore.obj"
	-@erase "$(INTDIR)\TPlayerSynchroniserClock.obj"
	-@erase "$(INTDIR)\TPlayerSynchroniserExt.obj"
	-@erase "$(INTDIR)\TPlayerSynchroniserInt.obj"
	-@erase "$(INTDIR)\TRunningPlayer.obj"
	-@erase "$(INTDIR)\TScheduler.obj"
	-@erase "$(INTDIR)\TScore.obj"
	-@erase "$(INTDIR)\TScoreFollower.obj"
	-@erase "$(INTDIR)\TScoreInserter.obj"
	-@erase "$(INTDIR)\TScoreIterator.obj"
	-@erase "$(INTDIR)\TSMPTEEventReceiver.obj"
	-@erase "$(INTDIR)\TSMPTEPlayer.obj"
	-@erase "$(INTDIR)\TSyncInPlayer.obj"
	-@erase "$(INTDIR)\TSyncOutPlayer.obj"
	-@erase "$(INTDIR)\TTempoConverter.obj"
	-@erase "$(INTDIR)\TTempoMap.obj"
	-@erase "$(INTDIR)\TTempoMapVisitor.obj"
	-@erase "$(INTDIR)\TTickPlayer.obj"
	-@erase "$(INTDIR)\TTime.obj"
	-@erase "$(INTDIR)\TTimeConverter.obj"
	-@erase "$(INTDIR)\TTimeManager.obj"
	-@erase "$(INTDIR)\TTrackTable.obj"
	-@erase "$(INTDIR)\UMidi.obj"
	-@erase "$(INTDIR)\UScoreManager.obj"
	-@erase "$(INTDIR)\UTools.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Player32.dll"
	-@erase "$(OUTDIR)\Player32.exp"
	-@erase "$(OUTDIR)\Player32.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp2 /MT /W3 /GX /Ob1 /I "..\src\Include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x410 /fo"$(INTDIR)\Player32.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Player32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\Player32.pdb" /machine:I386 /out:"$(OUTDIR)\Player32.dll"\
 /implib:"$(OUTDIR)\Player32.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Dllmain.obj" \
	"$(INTDIR)\Midifile.obj" \
	"$(INTDIR)\Player.obj" \
	"$(INTDIR)\Player32.res" \
	"$(INTDIR)\TChaserIterator.obj" \
	"$(INTDIR)\TChaserVisitor.obj" \
	"$(INTDIR)\TClockEventReceiver.obj" \
	"$(INTDIR)\TClockSender.obj" \
	"$(INTDIR)\TEventFactory.obj" \
	"$(INTDIR)\TEventReceiver.obj" \
	"$(INTDIR)\TEventRecorder.obj" \
	"$(INTDIR)\TExtEventReceiver.obj" \
	"$(INTDIR)\THashTable.obj" \
	"$(INTDIR)\TLoopManager.obj" \
	"$(INTDIR)\TMidiAppl.obj" \
	"$(INTDIR)\TMidiPlayer.obj" \
	"$(INTDIR)\Tmtrack.obj" \
	"$(INTDIR)\TPlayer.obj" \
	"$(INTDIR)\TPlayerFactory.obj" \
	"$(INTDIR)\TPlayerScore.obj" \
	"$(INTDIR)\TPlayerSynchroniserClock.obj" \
	"$(INTDIR)\TPlayerSynchroniserExt.obj" \
	"$(INTDIR)\TPlayerSynchroniserInt.obj" \
	"$(INTDIR)\TRunningPlayer.obj" \
	"$(INTDIR)\TScheduler.obj" \
	"$(INTDIR)\TScore.obj" \
	"$(INTDIR)\TScoreFollower.obj" \
	"$(INTDIR)\TScoreInserter.obj" \
	"$(INTDIR)\TScoreIterator.obj" \
	"$(INTDIR)\TSMPTEEventReceiver.obj" \
	"$(INTDIR)\TSMPTEPlayer.obj" \
	"$(INTDIR)\TSyncInPlayer.obj" \
	"$(INTDIR)\TSyncOutPlayer.obj" \
	"$(INTDIR)\TTempoConverter.obj" \
	"$(INTDIR)\TTempoMap.obj" \
	"$(INTDIR)\TTempoMapVisitor.obj" \
	"$(INTDIR)\TTickPlayer.obj" \
	"$(INTDIR)\TTime.obj" \
	"$(INTDIR)\TTimeConverter.obj" \
	"$(INTDIR)\TTimeManager.obj" \
	"$(INTDIR)\TTrackTable.obj" \
	"$(INTDIR)\UMidi.obj" \
	"$(INTDIR)\UScoreManager.obj" \
	"$(INTDIR)\UTools.obj" \
	".\mshare32.lib"

"$(OUTDIR)\Player32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

OUTDIR=.\Output
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Output
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Player32.dll"

!ELSE 

ALL : "$(OUTDIR)\Player32.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Dllmain.obj"
	-@erase "$(INTDIR)\Midifile.obj"
	-@erase "$(INTDIR)\Player.obj"
	-@erase "$(INTDIR)\Player32.res"
	-@erase "$(INTDIR)\TChaserIterator.obj"
	-@erase "$(INTDIR)\TChaserVisitor.obj"
	-@erase "$(INTDIR)\TClockEventReceiver.obj"
	-@erase "$(INTDIR)\TClockSender.obj"
	-@erase "$(INTDIR)\TEventFactory.obj"
	-@erase "$(INTDIR)\TEventReceiver.obj"
	-@erase "$(INTDIR)\TEventRecorder.obj"
	-@erase "$(INTDIR)\TExtEventReceiver.obj"
	-@erase "$(INTDIR)\THashTable.obj"
	-@erase "$(INTDIR)\TLoopManager.obj"
	-@erase "$(INTDIR)\TMidiAppl.obj"
	-@erase "$(INTDIR)\TMidiPlayer.obj"
	-@erase "$(INTDIR)\Tmtrack.obj"
	-@erase "$(INTDIR)\TPlayer.obj"
	-@erase "$(INTDIR)\TPlayerFactory.obj"
	-@erase "$(INTDIR)\TPlayerScore.obj"
	-@erase "$(INTDIR)\TPlayerSynchroniserClock.obj"
	-@erase "$(INTDIR)\TPlayerSynchroniserExt.obj"
	-@erase "$(INTDIR)\TPlayerSynchroniserInt.obj"
	-@erase "$(INTDIR)\TRunningPlayer.obj"
	-@erase "$(INTDIR)\TScheduler.obj"
	-@erase "$(INTDIR)\TScore.obj"
	-@erase "$(INTDIR)\TScoreFollower.obj"
	-@erase "$(INTDIR)\TScoreInserter.obj"
	-@erase "$(INTDIR)\TScoreIterator.obj"
	-@erase "$(INTDIR)\TSMPTEEventReceiver.obj"
	-@erase "$(INTDIR)\TSMPTEPlayer.obj"
	-@erase "$(INTDIR)\TSyncInPlayer.obj"
	-@erase "$(INTDIR)\TSyncOutPlayer.obj"
	-@erase "$(INTDIR)\TTempoConverter.obj"
	-@erase "$(INTDIR)\TTempoMap.obj"
	-@erase "$(INTDIR)\TTempoMapVisitor.obj"
	-@erase "$(INTDIR)\TTickPlayer.obj"
	-@erase "$(INTDIR)\TTime.obj"
	-@erase "$(INTDIR)\TTimeConverter.obj"
	-@erase "$(INTDIR)\TTimeManager.obj"
	-@erase "$(INTDIR)\TTrackTable.obj"
	-@erase "$(INTDIR)\UMidi.obj"
	-@erase "$(INTDIR)\UScoreManager.obj"
	-@erase "$(INTDIR)\UTools.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Player32.dll"
	-@erase "$(OUTDIR)\Player32.exp"
	-@erase "$(OUTDIR)\Player32.ilk"
	-@erase "$(OUTDIR)\Player32.lib"
	-@erase "$(OUTDIR)\Player32.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /I "." /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x410 /fo"$(INTDIR)\Player32.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Player32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\Player32.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Player32.dll" /implib:"$(OUTDIR)\Player32.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Dllmain.obj" \
	"$(INTDIR)\Midifile.obj" \
	"$(INTDIR)\Player.obj" \
	"$(INTDIR)\Player32.res" \
	"$(INTDIR)\TChaserIterator.obj" \
	"$(INTDIR)\TChaserVisitor.obj" \
	"$(INTDIR)\TClockEventReceiver.obj" \
	"$(INTDIR)\TClockSender.obj" \
	"$(INTDIR)\TEventFactory.obj" \
	"$(INTDIR)\TEventReceiver.obj" \
	"$(INTDIR)\TEventRecorder.obj" \
	"$(INTDIR)\TExtEventReceiver.obj" \
	"$(INTDIR)\THashTable.obj" \
	"$(INTDIR)\TLoopManager.obj" \
	"$(INTDIR)\TMidiAppl.obj" \
	"$(INTDIR)\TMidiPlayer.obj" \
	"$(INTDIR)\Tmtrack.obj" \
	"$(INTDIR)\TPlayer.obj" \
	"$(INTDIR)\TPlayerFactory.obj" \
	"$(INTDIR)\TPlayerScore.obj" \
	"$(INTDIR)\TPlayerSynchroniserClock.obj" \
	"$(INTDIR)\TPlayerSynchroniserExt.obj" \
	"$(INTDIR)\TPlayerSynchroniserInt.obj" \
	"$(INTDIR)\TRunningPlayer.obj" \
	"$(INTDIR)\TScheduler.obj" \
	"$(INTDIR)\TScore.obj" \
	"$(INTDIR)\TScoreFollower.obj" \
	"$(INTDIR)\TScoreInserter.obj" \
	"$(INTDIR)\TScoreIterator.obj" \
	"$(INTDIR)\TSMPTEEventReceiver.obj" \
	"$(INTDIR)\TSMPTEPlayer.obj" \
	"$(INTDIR)\TSyncInPlayer.obj" \
	"$(INTDIR)\TSyncOutPlayer.obj" \
	"$(INTDIR)\TTempoConverter.obj" \
	"$(INTDIR)\TTempoMap.obj" \
	"$(INTDIR)\TTempoMapVisitor.obj" \
	"$(INTDIR)\TTickPlayer.obj" \
	"$(INTDIR)\TTime.obj" \
	"$(INTDIR)\TTimeConverter.obj" \
	"$(INTDIR)\TTimeManager.obj" \
	"$(INTDIR)\TTrackTable.obj" \
	"$(INTDIR)\UMidi.obj" \
	"$(INTDIR)\UScoreManager.obj" \
	"$(INTDIR)\UTools.obj" \
	".\mshare32.lib"

"$(OUTDIR)\Player32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Player32 - Win32 Release" || "$(CFG)" ==\
 "Player32 - Win32 Debug"
SOURCE=..\src\Chasers\TChaserIterator.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TCHAS=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	
NODEP_CPP_TCHAS=\
	"..\src\include\MidiShare.h"\
	
CPP_SWITCHES=/nologo /Zp2 /MT /W3 /GX /Ob1 /I "..\src\Include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TChaserIterator.obj" : $(SOURCE) $(DEP_CPP_TCHAS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TCHAS=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	".\midishare.h"\
	
CPP_SWITCHES=/nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /I "." /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TChaserIterator.obj" : $(SOURCE) $(DEP_CPP_TCHAS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\Chasers\TChaserVisitor.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TCHASE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	
NODEP_CPP_TCHASE=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TChaserVisitor.obj" : $(SOURCE) $(DEP_CPP_TCHASE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TCHASE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	".\midishare.h"\
	

"$(INTDIR)\TChaserVisitor.obj" : $(SOURCE) $(DEP_CPP_TCHASE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Chasers\THashTable.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_THASH=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\umidi.h"\
	
NODEP_CPP_THASH=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\THashTable.obj" : $(SOURCE) $(DEP_CPP_THASH) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_THASH=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\umidi.h"\
	".\midishare.h"\
	

"$(INTDIR)\THashTable.obj" : $(SOURCE) $(DEP_CPP_THASH) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Events\TClockEventReceiver.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TCLOC=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TCLOC=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TClockEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TCLOC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TCLOC=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TClockEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TCLOC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Events\TEventReceiver.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TEVEN=\
	"..\src\include\cstruct.h"\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tdestructor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayer.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorestate.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TEVEN=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TEVEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TEVEN=\
	"..\src\include\cstruct.h"\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tdestructor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayer.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorestate.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TEVEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Events\TEventRecorder.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TEVENT=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	
NODEP_CPP_TEVENT=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TEventRecorder.obj" : $(SOURCE) $(DEP_CPP_TEVENT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TEVENT=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	".\midishare.h"\
	

"$(INTDIR)\TEventRecorder.obj" : $(SOURCE) $(DEP_CPP_TEVENT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Events\TExtEventReceiver.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TEXTE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TEXTE=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TExtEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TEXTE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TEXTE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TExtEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TEXTE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Events\TMidiPlayer.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TMIDI=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TMIDI=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TMidiPlayer.obj" : $(SOURCE) $(DEP_CPP_TMIDI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TMIDI=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TMidiPlayer.obj" : $(SOURCE) $(DEP_CPP_TMIDI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Events\TSMPTEEventReceiver.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSMPT=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TSMPT=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TSMPTEEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TSMPT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSMPT=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TSMPTEEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TSMPT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Factory\TPlayerFactory.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TPLAY=\
	"..\src\include\cstruct.h"\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tdestructor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayer.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerfactory.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorestate.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TPLAY=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TPlayerFactory.obj" : $(SOURCE) $(DEP_CPP_TPLAY) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TPLAY=\
	"..\src\include\cstruct.h"\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tdestructor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayer.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerfactory.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorestate.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TPlayerFactory.obj" : $(SOURCE) $(DEP_CPP_TPLAY) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Events\TEventFactory.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TEVENTF=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	
NODEP_CPP_TEVENTF=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TEventFactory.obj" : $(SOURCE) $(DEP_CPP_TEVENTF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TEVENTF=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	".\midishare.h"\
	

"$(INTDIR)\TEventFactory.obj" : $(SOURCE) $(DEP_CPP_TEVENTF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\MidiAppl\TMidiAppl.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TMIDIA=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	
NODEP_CPP_TMIDIA=\
	"..\src\include\MidiShare.h"\
	
CPP_SWITCHES=/nologo /Zp2 /MT /W3 /GX /Ob1 /I "..\src\Include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TMidiAppl.obj" : $(SOURCE) $(DEP_CPP_TMIDIA) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TMIDIA=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	".\midishare.h"\
	
CPP_SWITCHES=/nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /I "." /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\TMidiAppl.obj" : $(SOURCE) $(DEP_CPP_TMIDIA) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\Framework\Scheduler\TScheduler.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSCHE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\umidi.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TSCHE=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TScheduler.obj" : $(SOURCE) $(DEP_CPP_TSCHE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSCHE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\umidi.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TScheduler.obj" : $(SOURCE) $(DEP_CPP_TSCHE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Score\TScore.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSCOR=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TSCOR=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TScore.obj" : $(SOURCE) $(DEP_CPP_TSCOR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSCOR=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TScore.obj" : $(SOURCE) $(DEP_CPP_TSCOR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Score\TScoreFollower.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSCORE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	
NODEP_CPP_TSCORE=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TScoreFollower.obj" : $(SOURCE) $(DEP_CPP_TSCORE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSCORE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	".\midishare.h"\
	

"$(INTDIR)\TScoreFollower.obj" : $(SOURCE) $(DEP_CPP_TSCORE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Score\TScoreIterator.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSCOREI=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TSCOREI=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TScoreIterator.obj" : $(SOURCE) $(DEP_CPP_TSCOREI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSCOREI=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TScoreIterator.obj" : $(SOURCE) $(DEP_CPP_TSCOREI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Score\UScoreManager.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_USCOR=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\uscoremanager.h"\
	
NODEP_CPP_USCOR=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\UScoreManager.obj" : $(SOURCE) $(DEP_CPP_USCOR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_USCOR=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\uscoremanager.h"\
	".\midishare.h"\
	

"$(INTDIR)\UScoreManager.obj" : $(SOURCE) $(DEP_CPP_USCOR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Tempo\TTempoConverter.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TTEMP=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\udebug.h"\
	"..\src\include\umath.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TTEMP=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TTempoConverter.obj" : $(SOURCE) $(DEP_CPP_TTEMP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TTEMP=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\udebug.h"\
	"..\src\include\umath.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TTempoConverter.obj" : $(SOURCE) $(DEP_CPP_TTEMP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Tempo\TTempoMap.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TTEMPO=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TTEMPO=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TTempoMap.obj" : $(SOURCE) $(DEP_CPP_TTEMPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TTEMPO=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TTempoMap.obj" : $(SOURCE) $(DEP_CPP_TTEMPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Tempo\TTempoMapVisitor.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TTEMPOM=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TTEMPOM=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TTempoMapVisitor.obj" : $(SOURCE) $(DEP_CPP_TTEMPOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TTEMPOM=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TTempoMapVisitor.obj" : $(SOURCE) $(DEP_CPP_TTEMPOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Tempo\TTime.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TTIME=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttime.h"\
	
NODEP_CPP_TTIME=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TTime.obj" : $(SOURCE) $(DEP_CPP_TTIME) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TTIME=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttime.h"\
	".\midishare.h"\
	

"$(INTDIR)\TTime.obj" : $(SOURCE) $(DEP_CPP_TTIME) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Tempo\TTimeConverter.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TTIMEC=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	
NODEP_CPP_TTIMEC=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TTimeConverter.obj" : $(SOURCE) $(DEP_CPP_TTIMEC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TTIMEC=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	".\midishare.h"\
	

"$(INTDIR)\TTimeConverter.obj" : $(SOURCE) $(DEP_CPP_TTIMEC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Framework\Tempo\TTimeManager.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TTIMEM=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapbuilder.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\uscoremanager.h"\
	
NODEP_CPP_TTIMEM=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TTimeManager.obj" : $(SOURCE) $(DEP_CPP_TTIMEM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TTIMEM=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapbuilder.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\uscoremanager.h"\
	".\midishare.h"\
	

"$(INTDIR)\TTimeManager.obj" : $(SOURCE) $(DEP_CPP_TTIMEM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Library\Player.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_PLAYE=\
	"..\src\include\cstruct.h"\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tdestructor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayer.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerfactory.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorestate.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_PLAYE=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\Player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_PLAYE=\
	"..\src\include\cstruct.h"\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tdestructor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayer.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerfactory.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorestate.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\Player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Library\TPlayer.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TPLAYE=\
	"..\src\include\cstruct.h"\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tdestructor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayer.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerfactory.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayermemento.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorestate.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapbuilder.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\udebug.h"\
	"..\src\include\umath.h"\
	"..\src\include\umidi.h"\
	"..\src\include\uscoremanager.h"\
	
NODEP_CPP_TPLAYE=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TPlayer.obj" : $(SOURCE) $(DEP_CPP_TPLAYE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TPLAYE=\
	"..\src\include\cstruct.h"\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tdestructor.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\teventreceiver.h"\
	"..\src\include\teventreceiverinterface.h"\
	"..\src\include\teventrecorder.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayer.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tmidiplayer.h"\
	"..\src\include\tplayer.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerfactory.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayermemento.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorestate.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapbuilder.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\ttimemanager.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	"..\src\include\udebug.h"\
	"..\src\include\umath.h"\
	"..\src\include\umidi.h"\
	"..\src\include\uscoremanager.h"\
	".\midishare.h"\
	

"$(INTDIR)\TPlayer.obj" : $(SOURCE) $(DEP_CPP_TPLAYE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Loop\TLoopManager.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TLOOP=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TLOOP=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TLoopManager.obj" : $(SOURCE) $(DEP_CPP_TLOOP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TLOOP=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\tloopmanager.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscoremarker.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TLoopManager.obj" : $(SOURCE) $(DEP_CPP_TLOOP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\MidiFile\Midifile.c

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_MIDIF=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\midifile.h"\
	
NODEP_CPP_MIDIF=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\Midifile.obj" : $(SOURCE) $(DEP_CPP_MIDIF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_MIDIF=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\midifile.h"\
	".\midishare.h"\
	

"$(INTDIR)\Midifile.obj" : $(SOURCE) $(DEP_CPP_MIDIF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\MidiFile\Tmtrack.c

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TMTRA=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\midifile.h"\
	"..\src\include\tmtrack.h"\
	
NODEP_CPP_TMTRA=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\Tmtrack.obj" : $(SOURCE) $(DEP_CPP_TMTRA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TMTRA=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\midifile.h"\
	"..\src\include\tmtrack.h"\
	".\midishare.h"\
	

"$(INTDIR)\Tmtrack.obj" : $(SOURCE) $(DEP_CPP_TMTRA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Players\TRunningPlayer.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TRUNN=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TRUNN=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TRunningPlayer.obj" : $(SOURCE) $(DEP_CPP_TRUNN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TRUNN=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningplayer.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TRunningPlayer.obj" : $(SOURCE) $(DEP_CPP_TRUNN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Players\TSMPTEPlayer.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSMPTE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TSMPTE=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TSMPTEPlayer.obj" : $(SOURCE) $(DEP_CPP_TSMPTE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSMPTE=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\tgenericplayerinterface.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsmpteinfos.h"\
	"..\src\include\tsmpteplayer.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TSMPTEPlayer.obj" : $(SOURCE) $(DEP_CPP_TSMPTE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Players\TSyncInPlayer.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSYNC=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TSYNC=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TSyncInPlayer.obj" : $(SOURCE) $(DEP_CPP_TSYNC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSYNC=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TSyncInPlayer.obj" : $(SOURCE) $(DEP_CPP_TSYNC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Players\TSyncOutPlayer.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSYNCO=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	
NODEP_CPP_TSYNCO=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TSyncOutPlayer.obj" : $(SOURCE) $(DEP_CPP_TSYNCO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSYNCO=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tchaseriterator.h"\
	"..\src\include\tchaservisitor.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\teventtable.h"\
	"..\src\include\thashtable.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerinterface.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\tsyncinplayer.h"\
	"..\src\include\tsyncoutplayer.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	".\midishare.h"\
	

"$(INTDIR)\TSyncOutPlayer.obj" : $(SOURCE) $(DEP_CPP_TSYNCO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Players\TTickPlayer.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TTICK=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TTICK=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TTickPlayer.obj" : $(SOURCE) $(DEP_CPP_TTICK) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TTICK=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tslicevisitor.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttickplayer.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TTickPlayer.obj" : $(SOURCE) $(DEP_CPP_TTICK) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Score\TPlayerScore.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TPLAYER=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\udebug.h"\
	"..\src\include\umath.h"\
	"..\src\include\umidi.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TPLAYER=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TPlayerScore.obj" : $(SOURCE) $(DEP_CPP_TPLAYER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TPLAYER=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventfactory.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\udebug.h"\
	"..\src\include\umath.h"\
	"..\src\include\umidi.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TPlayerScore.obj" : $(SOURCE) $(DEP_CPP_TPLAYER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Score\TScoreInserter.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TSCOREIN=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\umidi.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TSCOREIN=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TScoreInserter.obj" : $(SOURCE) $(DEP_CPP_TSCOREIN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TSCOREIN=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayerscore.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreinserter.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\umidi.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TScoreInserter.obj" : $(SOURCE) $(DEP_CPP_TSCOREIN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Synchroniser\TClockSender.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TCLOCK=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TCLOCK=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TClockSender.obj" : $(SOURCE) $(DEP_CPP_TCLOCK) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TCLOCK=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tclocksender.h"\
	"..\src\include\tevent.h"\
	"..\src\include\teventsenderinterface.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TClockSender.obj" : $(SOURCE) $(DEP_CPP_TCLOCK) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Synchroniser\TPlayerSynchroniserClock.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TPLAYERS=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TPLAYERS=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TPlayerSynchroniserClock.obj" : $(SOURCE) $(DEP_CPP_TPLAYERS)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TPLAYERS=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TPlayerSynchroniserClock.obj" : $(SOURCE) $(DEP_CPP_TPLAYERS)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Synchroniser\TPlayerSynchroniserExt.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TPLAYERSY=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TPLAYERSY=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TPlayerSynchroniserExt.obj" : $(SOURCE) $(DEP_CPP_TPLAYERSY)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TPLAYERSY=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TPlayerSynchroniserExt.obj" : $(SOURCE) $(DEP_CPP_TPLAYERSY)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Synchroniser\TPlayerSynchroniserInt.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TPLAYERSYN=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_TPLAYERSYN=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TPlayerSynchroniserInt.obj" : $(SOURCE) $(DEP_CPP_TPLAYERSYN)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TPLAYERSYN=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\MidiFilter.h"\
	"..\src\include\tclockconverter.h"\
	"..\src\include\tevent.h"\
	"..\src\include\tmidiappl.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\tplayersynchroniser.h"\
	"..\src\include\trunningstate.h"\
	"..\src\include\tscheduler.h"\
	"..\src\include\tscore.h"\
	"..\src\include\tscorefollower.h"\
	"..\src\include\tscoreiterator.h"\
	"..\src\include\tscorevisitorinterface.h"\
	"..\src\include\tsynchroniserinterface.h"\
	"..\src\include\ttempoconverter.h"\
	"..\src\include\ttempomap.h"\
	"..\src\include\ttempomapvisitor.h"\
	"..\src\include\ttime.h"\
	"..\src\include\ttimeconverter.h"\
	"..\src\include\udebug.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	

"$(INTDIR)\TPlayerSynchroniserInt.obj" : $(SOURCE) $(DEP_CPP_TPLAYERSYN)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Tracks\TTrackTable.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_TTRAC=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	
NODEP_CPP_TTRAC=\
	"..\src\include\MidiShare.h"\
	

"$(INTDIR)\TTrackTable.obj" : $(SOURCE) $(DEP_CPP_TTRAC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_TTRAC=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\ttrack.h"\
	"..\src\include\ttracktable.h"\
	".\midishare.h"\
	

"$(INTDIR)\TTrackTable.obj" : $(SOURCE) $(DEP_CPP_TTRAC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\src\Utils\UMidi.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_UMIDI=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\umidi.h"\
	
NODEP_CPP_UMIDI=\
	"..\src\include\MidiShare.h"\
	
CPP_SWITCHES=/nologo /Zp2 /MT /W3 /GX /Ob1 /I "..\src\Include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\UMidi.obj" : $(SOURCE) $(DEP_CPP_UMIDI) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_UMIDI=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\umidi.h"\
	".\midishare.h"\
	
CPP_SWITCHES=/nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /I "." /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\UMidi.obj" : $(SOURCE) $(DEP_CPP_UMIDI) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\Utils\UTools.cpp

!IF  "$(CFG)" == "Player32 - Win32 Release"

DEP_CPP_UTOOL=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\utools.h"\
	
NODEP_CPP_UTOOL=\
	"..\src\include\MidiShare.h"\
	
CPP_SWITCHES=/nologo /Zp2 /MT /W3 /GX /Ob1 /I "..\src\Include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\UTools.obj" : $(SOURCE) $(DEP_CPP_UTOOL) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

DEP_CPP_UTOOL=\
	"..\src\include\genericmidishare.h"\
	"..\src\include\tplayerconstants.h"\
	"..\src\include\utools.h"\
	".\midishare.h"\
	
CPP_SWITCHES=/nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /I "." /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\UTools.obj" : $(SOURCE) $(DEP_CPP_UTOOL) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Dllmain.c

"$(INTDIR)\Dllmain.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Player32.rc

"$(INTDIR)\Player32.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

