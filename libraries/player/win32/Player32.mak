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
	-@erase "$(INTDIR)\Filter.obj"
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
	"$(INTDIR)\Filter.obj" \
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
	-@erase "$(INTDIR)\Filter.obj"
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
CPP_PROJ=/nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
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
	"$(INTDIR)\Filter.obj" \
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
DEP_CPP_TCHAS=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	

"$(INTDIR)\TChaserIterator.obj" : $(SOURCE) $(DEP_CPP_TCHAS) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Chasers\TChaserVisitor.cpp
DEP_CPP_TCHASE=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	

"$(INTDIR)\TChaserVisitor.obj" : $(SOURCE) $(DEP_CPP_TCHASE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Chasers\THashTable.cpp
DEP_CPP_THASH=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\UMidi.h"\
	

"$(INTDIR)\THashTable.obj" : $(SOURCE) $(DEP_CPP_THASH) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Events\TClockEventReceiver.cpp
DEP_CPP_TCLOC=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TEventReceiver.h"\
	"..\src\Include\TEventReceiverInterface.h"\
	"..\src\Include\TLoopManager.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreMarker.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TClockEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TCLOC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Events\TEventReceiver.cpp
DEP_CPP_TEVEN=\
	"..\src\Include\CStruct.h"\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TClockSender.h"\
	"..\src\Include\TDestructor.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TEventReceiver.h"\
	"..\src\Include\TEventReceiverInterface.h"\
	"..\src\Include\TEventRecorder.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\TGenericPlayer.h"\
	"..\src\Include\TGenericPlayerInterface.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TLoopManager.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TMidiPlayer.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningPlayer.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreInserter.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreMarker.h"\
	"..\src\Include\TScoreState.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSMPTEInfos.h"\
	"..\src\Include\TSMPTEPlayer.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TSyncInPlayer.h"\
	"..\src\Include\TSyncOutPlayer.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\TTimeManager.h"\
	"..\src\Include\TTrack.h"\
	"..\src\Include\TTrackTable.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TEVEN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Events\TEventRecorder.cpp
DEP_CPP_TEVENT=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TEventReceiverInterface.h"\
	"..\src\Include\TEventRecorder.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	

"$(INTDIR)\TEventRecorder.obj" : $(SOURCE) $(DEP_CPP_TEVENT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Events\TExtEventReceiver.cpp
DEP_CPP_TEXTE=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TEventReceiver.h"\
	"..\src\Include\TEventReceiverInterface.h"\
	"..\src\Include\TLoopManager.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreMarker.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TExtEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TEXTE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Events\TMidiPlayer.cpp
DEP_CPP_TMIDI=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TMidiPlayer.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTrack.h"\
	"..\src\Include\TTrackTable.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TMidiPlayer.obj" : $(SOURCE) $(DEP_CPP_TMIDI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Events\TSMPTEEventReceiver.cpp
DEP_CPP_TSMPT=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TEventReceiver.h"\
	"..\src\Include\TEventReceiverInterface.h"\
	"..\src\Include\TLoopManager.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreMarker.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TSMPTEEventReceiver.obj" : $(SOURCE) $(DEP_CPP_TSMPT) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Factory\TPlayerFactory.cpp
DEP_CPP_TPLAY=\
	"..\src\Include\CStruct.h"\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TClockSender.h"\
	"..\src\Include\TDestructor.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TEventReceiver.h"\
	"..\src\Include\TEventReceiverInterface.h"\
	"..\src\Include\TEventRecorder.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\TGenericPlayer.h"\
	"..\src\Include\TGenericPlayerInterface.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TLoopManager.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TMidiPlayer.h"\
	"..\src\Include\TPlayer.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerFactory.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningPlayer.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreInserter.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreMarker.h"\
	"..\src\Include\TScoreState.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSMPTEInfos.h"\
	"..\src\Include\TSMPTEPlayer.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TSyncInPlayer.h"\
	"..\src\Include\TSyncOutPlayer.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\TTimeManager.h"\
	"..\src\Include\TTrack.h"\
	"..\src\Include\TTrackTable.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TPlayerFactory.obj" : $(SOURCE) $(DEP_CPP_TPLAY) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Events\TEventFactory.cpp
DEP_CPP_TEVENTF=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	

"$(INTDIR)\TEventFactory.obj" : $(SOURCE) $(DEP_CPP_TEVENTF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\MidiAppl\Filter.c
DEP_CPP_FILTE=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	

"$(INTDIR)\Filter.obj" : $(SOURCE) $(DEP_CPP_FILTE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\MidiAppl\TMidiAppl.cpp
DEP_CPP_TMIDIA=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	

"$(INTDIR)\TMidiAppl.obj" : $(SOURCE) $(DEP_CPP_TMIDIA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Scheduler\TScheduler.cpp
DEP_CPP_TSCHE=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\UMidi.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TScheduler.obj" : $(SOURCE) $(DEP_CPP_TSCHE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Score\TScore.cpp
DEP_CPP_TSCOR=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\UDebug.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TScore.obj" : $(SOURCE) $(DEP_CPP_TSCOR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Score\TScoreFollower.cpp
DEP_CPP_TSCORE=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	

"$(INTDIR)\TScoreFollower.obj" : $(SOURCE) $(DEP_CPP_TSCORE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Score\TScoreIterator.cpp
DEP_CPP_TSCOREI=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\UDebug.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TScoreIterator.obj" : $(SOURCE) $(DEP_CPP_TSCOREI) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Score\UScoreManager.cpp
DEP_CPP_USCOR=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\UScoreManager.h"\
	

"$(INTDIR)\UScoreManager.obj" : $(SOURCE) $(DEP_CPP_USCOR) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Tempo\TTempoConverter.cpp
DEP_CPP_TTEMP=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\UDebug.h"\
	"..\src\Include\UMath.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TTempoConverter.obj" : $(SOURCE) $(DEP_CPP_TTEMP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Tempo\TTempoMap.cpp
DEP_CPP_TTEMPO=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TTempoMap.obj" : $(SOURCE) $(DEP_CPP_TTEMPO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Tempo\TTempoMapVisitor.cpp
DEP_CPP_TTEMPOM=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TTempoMapVisitor.obj" : $(SOURCE) $(DEP_CPP_TTEMPOM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Tempo\TTime.cpp
DEP_CPP_TTIME=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TTime.h"\
	

"$(INTDIR)\TTime.obj" : $(SOURCE) $(DEP_CPP_TTIME) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Tempo\TTimeConverter.cpp
DEP_CPP_TTIMEC=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	

"$(INTDIR)\TTimeConverter.obj" : $(SOURCE) $(DEP_CPP_TTIMEC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Framework\Tempo\TTimeManager.cpp
DEP_CPP_TTIMEM=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapBuilder.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\TTimeManager.h"\
	"..\src\Include\UScoreManager.h"\
	

"$(INTDIR)\TTimeManager.obj" : $(SOURCE) $(DEP_CPP_TTIMEM) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Library\Player.cpp
DEP_CPP_PLAYE=\
	"..\src\Include\CStruct.h"\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TClockSender.h"\
	"..\src\Include\TDestructor.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TEventReceiver.h"\
	"..\src\Include\TEventReceiverInterface.h"\
	"..\src\Include\TEventRecorder.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\TGenericPlayer.h"\
	"..\src\Include\TGenericPlayerInterface.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TLoopManager.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TMidiPlayer.h"\
	"..\src\Include\TPlayer.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerFactory.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningPlayer.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreInserter.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreMarker.h"\
	"..\src\Include\TScoreState.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSMPTEInfos.h"\
	"..\src\Include\TSMPTEPlayer.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TSyncInPlayer.h"\
	"..\src\Include\TSyncOutPlayer.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\TTimeManager.h"\
	"..\src\Include\TTrack.h"\
	"..\src\Include\TTrackTable.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\Player.obj" : $(SOURCE) $(DEP_CPP_PLAYE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Library\TPlayer.cpp
DEP_CPP_TPLAYE=\
	"..\src\Include\CStruct.h"\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TClockSender.h"\
	"..\src\Include\TDestructor.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TEventReceiver.h"\
	"..\src\Include\TEventReceiverInterface.h"\
	"..\src\Include\TEventRecorder.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\TGenericPlayer.h"\
	"..\src\Include\TGenericPlayerInterface.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TLoopManager.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TMidiPlayer.h"\
	"..\src\Include\TPlayer.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerFactory.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerMemento.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningPlayer.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreInserter.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreMarker.h"\
	"..\src\Include\TScoreState.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSMPTEInfos.h"\
	"..\src\Include\TSMPTEPlayer.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TSyncInPlayer.h"\
	"..\src\Include\TSyncOutPlayer.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapBuilder.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\TTimeManager.h"\
	"..\src\Include\TTrack.h"\
	"..\src\Include\TTrackTable.h"\
	"..\src\Include\UDebug.h"\
	"..\src\Include\UMath.h"\
	"..\src\Include\UMidi.h"\
	"..\src\Include\UScoreManager.h"\
	

"$(INTDIR)\TPlayer.obj" : $(SOURCE) $(DEP_CPP_TPLAYE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Loop\TLoopManager.cpp
DEP_CPP_TLOOP=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TLoopManager.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreMarker.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TLoopManager.obj" : $(SOURCE) $(DEP_CPP_TLOOP) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\MidiFile\Midifile.c
DEP_CPP_MIDIF=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\MidiFile.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	

"$(INTDIR)\Midifile.obj" : $(SOURCE) $(DEP_CPP_MIDIF) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\MidiFile\Tmtrack.c
DEP_CPP_TMTRA=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\MidiFile.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TMTrack.h"\
	

"$(INTDIR)\Tmtrack.obj" : $(SOURCE) $(DEP_CPP_TMTRA) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Players\TRunningPlayer.cpp
DEP_CPP_TRUNN=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningPlayer.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TSyncInPlayer.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TRunningPlayer.obj" : $(SOURCE) $(DEP_CPP_TRUNN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Players\TSMPTEPlayer.cpp
DEP_CPP_TSMPTE=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\TGenericPlayerInterface.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSMPTEInfos.h"\
	"..\src\Include\TSMPTEPlayer.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TSyncInPlayer.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TSMPTEPlayer.obj" : $(SOURCE) $(DEP_CPP_TSMPTE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Players\TSyncInPlayer.cpp
DEP_CPP_TSYNC=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TSyncInPlayer.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TSyncInPlayer.obj" : $(SOURCE) $(DEP_CPP_TSYNC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Players\TSyncOutPlayer.cpp
DEP_CPP_TSYNCO=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TChaserIterator.h"\
	"..\src\Include\TChaserVisitor.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TClockSender.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TEventTable.h"\
	"..\src\Include\THashTable.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerInterface.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TSyncInPlayer.h"\
	"..\src\Include\TSyncOutPlayer.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	

"$(INTDIR)\TSyncOutPlayer.obj" : $(SOURCE) $(DEP_CPP_TSYNCO) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Players\TTickPlayer.cpp
DEP_CPP_TTICK=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSliceVisitor.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TTickPlayer.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TTickPlayer.obj" : $(SOURCE) $(DEP_CPP_TTICK) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Score\TPlayerScore.cpp
DEP_CPP_TPLAYER=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventFactory.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\UDebug.h"\
	"..\src\Include\UMath.h"\
	"..\src\Include\UMidi.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TPlayerScore.obj" : $(SOURCE) $(DEP_CPP_TPLAYER) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Score\TScoreInserter.cpp
DEP_CPP_TSCOREIN=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerScore.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreInserter.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UMidi.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TScoreInserter.obj" : $(SOURCE) $(DEP_CPP_TSCOREIN) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Synchroniser\TClockSender.cpp
DEP_CPP_TCLOCK=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TClockSender.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TEventSenderInterface.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TClockSender.obj" : $(SOURCE) $(DEP_CPP_TCLOCK) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Synchroniser\TPlayerSynchroniserClock.cpp
DEP_CPP_TPLAYERS=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TPlayerSynchroniserClock.obj" : $(SOURCE) $(DEP_CPP_TPLAYERS)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Synchroniser\TPlayerSynchroniserExt.cpp
DEP_CPP_TPLAYERSY=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TPlayerSynchroniserExt.obj" : $(SOURCE) $(DEP_CPP_TPLAYERSY)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Synchroniser\TPlayerSynchroniserInt.cpp
DEP_CPP_TPLAYERSYN=\
	"..\src\Include\filter.h"\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TClockConverter.h"\
	"..\src\Include\TEvent.h"\
	"..\src\Include\TMidiAppl.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TPlayerSynchroniser.h"\
	"..\src\Include\TRunningState.h"\
	"..\src\Include\TScheduler.h"\
	"..\src\Include\TScore.h"\
	"..\src\Include\TScoreFollower.h"\
	"..\src\Include\TScoreIterator.h"\
	"..\src\Include\TScoreVisitorInterface.h"\
	"..\src\Include\TSynchroniserInterface.h"\
	"..\src\Include\TTempoConverter.h"\
	"..\src\Include\TTempoMap.h"\
	"..\src\Include\TTempoMapVisitor.h"\
	"..\src\Include\TTime.h"\
	"..\src\Include\TTimeConverter.h"\
	"..\src\Include\UDebug.h"\
	"..\src\Include\UTools.h"\
	

"$(INTDIR)\TPlayerSynchroniserInt.obj" : $(SOURCE) $(DEP_CPP_TPLAYERSYN)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Tracks\TTrackTable.cpp
DEP_CPP_TTRAC=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\TTrack.h"\
	"..\src\Include\TTrackTable.h"\
	

"$(INTDIR)\TTrackTable.obj" : $(SOURCE) $(DEP_CPP_TTRAC) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\Utils\UMidi.cpp
DEP_CPP_UMIDI=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\UMidi.h"\
	

!IF  "$(CFG)" == "Player32 - Win32 Release"

CPP_SWITCHES=/nologo /Zp2 /MT /W3 /GX /Ob1 /I "..\src\Include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\UMidi.obj" : $(SOURCE) $(DEP_CPP_UMIDI) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

CPP_SWITCHES=/nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /D "WIN32"\
 /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\UMidi.obj" : $(SOURCE) $(DEP_CPP_UMIDI) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\Utils\UTools.cpp
DEP_CPP_UTOOL=\
	"..\src\Include\GenericMidiShare.h"\
	"..\src\Include\midishare.h"\
	"..\src\Include\Playerwin32.h"\
	"..\src\Include\TPlayerConstants.h"\
	"..\src\Include\UTools.h"\
	

!IF  "$(CFG)" == "Player32 - Win32 Release"

CPP_SWITCHES=/nologo /Zp2 /MT /W3 /GX /Ob1 /I "..\src\Include" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\UTools.obj" : $(SOURCE) $(DEP_CPP_UTOOL) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Player32 - Win32 Debug"

CPP_SWITCHES=/nologo /Zp2 /MTd /W3 /GX /Zi /Od /I "..\src\Include" /D "WIN32"\
 /D "_DEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\Player32.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

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

