# Microsoft Developer Studio Generated NMAKE File, Based on midishare.dsp
!IF "$(CFG)" == ""
CFG=midishare - Win32 Debug
!MESSAGE No configuration specified. Defaulting to midishare - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "midishare - Win32 Release" && "$(CFG)" != "midishare - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "midishare.mak" CFG="midishare - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "midishare - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "midishare - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "midishare - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\mshare32.dll" "$(OUTDIR)\midishare.bsc"


CLEAN :
	-@erase "$(INTDIR)\MidiShare.obj"
	-@erase "$(INTDIR)\midishare.res"
	-@erase "$(INTDIR)\MidiShare.sbr"
	-@erase "$(INTDIR)\msAlarms.obj"
	-@erase "$(INTDIR)\msAlarms.sbr"
	-@erase "$(INTDIR)\msAppls.obj"
	-@erase "$(INTDIR)\msAppls.sbr"
	-@erase "$(INTDIR)\msConnx.obj"
	-@erase "$(INTDIR)\msConnx.sbr"
	-@erase "$(INTDIR)\msDriver.obj"
	-@erase "$(INTDIR)\msDriver.sbr"
	-@erase "$(INTDIR)\msEvents.obj"
	-@erase "$(INTDIR)\msEvents.sbr"
	-@erase "$(INTDIR)\msFields.obj"
	-@erase "$(INTDIR)\msFields.sbr"
	-@erase "$(INTDIR)\msFilter.obj"
	-@erase "$(INTDIR)\msFilter.sbr"
	-@erase "$(INTDIR)\msHandler.obj"
	-@erase "$(INTDIR)\msHandler.sbr"
	-@erase "$(INTDIR)\msInit.obj"
	-@erase "$(INTDIR)\msInit.sbr"
	-@erase "$(INTDIR)\msMail.obj"
	-@erase "$(INTDIR)\msMail.sbr"
	-@erase "$(INTDIR)\msMem.obj"
	-@erase "$(INTDIR)\msMem.sbr"
	-@erase "$(INTDIR)\msMemory.obj"
	-@erase "$(INTDIR)\msMemory.sbr"
	-@erase "$(INTDIR)\msPrefs.obj"
	-@erase "$(INTDIR)\msPrefs.sbr"
	-@erase "$(INTDIR)\msSeq.obj"
	-@erase "$(INTDIR)\msSeq.sbr"
	-@erase "$(INTDIR)\msSmpte.obj"
	-@erase "$(INTDIR)\msSmpte.sbr"
	-@erase "$(INTDIR)\msSorter.obj"
	-@erase "$(INTDIR)\msSorter.sbr"
	-@erase "$(INTDIR)\msTasks.obj"
	-@erase "$(INTDIR)\msTasks.sbr"
	-@erase "$(INTDIR)\msTime.obj"
	-@erase "$(INTDIR)\msTime.sbr"
	-@erase "$(INTDIR)\msXmtRcv.obj"
	-@erase "$(INTDIR)\msXmtRcv.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WinMono.obj"
	-@erase "$(INTDIR)\WinMono.sbr"
	-@erase "$(OUTDIR)\midishare.bsc"
	-@erase "$(OUTDIR)\mshare32.exp"
	-@erase "$(OUTDIR)\mshare32.lib"
	-@erase "..\mshare32.dll"
	-@erase "..\mshare32.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /Ot /Ob2 /I "..\..\common\headers" /D "NDEBUG" /D "__Windows__" /D "WIN32" /D "_WINDOWS" /D "__EXPORT__" /D "__LittleEndian__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\midishare.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\midishare.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\midishare.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\msAlarms.sbr" \
	"$(INTDIR)\msAppls.sbr" \
	"$(INTDIR)\msConnx.sbr" \
	"$(INTDIR)\msFilter.sbr" \
	"$(INTDIR)\msMail.sbr" \
	"$(INTDIR)\msTasks.sbr" \
	"$(INTDIR)\msXmtRcv.sbr" \
	"$(INTDIR)\msHandler.sbr" \
	"$(INTDIR)\msInit.sbr" \
	"$(INTDIR)\msSmpte.sbr" \
	"$(INTDIR)\msTime.sbr" \
	"$(INTDIR)\msEvents.sbr" \
	"$(INTDIR)\msFields.sbr" \
	"$(INTDIR)\msMem.sbr" \
	"$(INTDIR)\msMemory.sbr" \
	"$(INTDIR)\msSeq.sbr" \
	"$(INTDIR)\msSorter.sbr" \
	"$(INTDIR)\msDriver.sbr" \
	"$(INTDIR)\MidiShare.sbr" \
	"$(INTDIR)\msPrefs.sbr" \
	"$(INTDIR)\WinMono.sbr"

"$(OUTDIR)\midishare.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib winmm.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\mshare32.pdb" /machine:I386 /out:"../mshare32.dll" /implib:"$(OUTDIR)\mshare32.lib" 
LINK32_OBJS= \
	"$(INTDIR)\msAlarms.obj" \
	"$(INTDIR)\msAppls.obj" \
	"$(INTDIR)\msConnx.obj" \
	"$(INTDIR)\msFilter.obj" \
	"$(INTDIR)\msMail.obj" \
	"$(INTDIR)\msTasks.obj" \
	"$(INTDIR)\msXmtRcv.obj" \
	"$(INTDIR)\msHandler.obj" \
	"$(INTDIR)\msInit.obj" \
	"$(INTDIR)\msSmpte.obj" \
	"$(INTDIR)\msTime.obj" \
	"$(INTDIR)\msEvents.obj" \
	"$(INTDIR)\msFields.obj" \
	"$(INTDIR)\msMem.obj" \
	"$(INTDIR)\msMemory.obj" \
	"$(INTDIR)\msSeq.obj" \
	"$(INTDIR)\msSorter.obj" \
	"$(INTDIR)\msDriver.obj" \
	"$(INTDIR)\MidiShare.obj" \
	"$(INTDIR)\msPrefs.obj" \
	"$(INTDIR)\WinMono.obj" \
	"$(INTDIR)\midishare.res"

"..\mshare32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "midishare - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\mshare32.dll" "$(OUTDIR)\midishare.bsc"


CLEAN :
	-@erase "$(INTDIR)\MidiShare.obj"
	-@erase "$(INTDIR)\midishare.res"
	-@erase "$(INTDIR)\MidiShare.sbr"
	-@erase "$(INTDIR)\msAlarms.obj"
	-@erase "$(INTDIR)\msAlarms.sbr"
	-@erase "$(INTDIR)\msAppls.obj"
	-@erase "$(INTDIR)\msAppls.sbr"
	-@erase "$(INTDIR)\msConnx.obj"
	-@erase "$(INTDIR)\msConnx.sbr"
	-@erase "$(INTDIR)\msDriver.obj"
	-@erase "$(INTDIR)\msDriver.sbr"
	-@erase "$(INTDIR)\msEvents.obj"
	-@erase "$(INTDIR)\msEvents.sbr"
	-@erase "$(INTDIR)\msFields.obj"
	-@erase "$(INTDIR)\msFields.sbr"
	-@erase "$(INTDIR)\msFilter.obj"
	-@erase "$(INTDIR)\msFilter.sbr"
	-@erase "$(INTDIR)\msHandler.obj"
	-@erase "$(INTDIR)\msHandler.sbr"
	-@erase "$(INTDIR)\msInit.obj"
	-@erase "$(INTDIR)\msInit.sbr"
	-@erase "$(INTDIR)\msMail.obj"
	-@erase "$(INTDIR)\msMail.sbr"
	-@erase "$(INTDIR)\msMem.obj"
	-@erase "$(INTDIR)\msMem.sbr"
	-@erase "$(INTDIR)\msMemory.obj"
	-@erase "$(INTDIR)\msMemory.sbr"
	-@erase "$(INTDIR)\msPrefs.obj"
	-@erase "$(INTDIR)\msPrefs.sbr"
	-@erase "$(INTDIR)\msSeq.obj"
	-@erase "$(INTDIR)\msSeq.sbr"
	-@erase "$(INTDIR)\msSmpte.obj"
	-@erase "$(INTDIR)\msSmpte.sbr"
	-@erase "$(INTDIR)\msSorter.obj"
	-@erase "$(INTDIR)\msSorter.sbr"
	-@erase "$(INTDIR)\msTasks.obj"
	-@erase "$(INTDIR)\msTasks.sbr"
	-@erase "$(INTDIR)\msTime.obj"
	-@erase "$(INTDIR)\msTime.sbr"
	-@erase "$(INTDIR)\msXmtRcv.obj"
	-@erase "$(INTDIR)\msXmtRcv.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WinMono.obj"
	-@erase "$(INTDIR)\WinMono.sbr"
	-@erase "$(OUTDIR)\midishare.bsc"
	-@erase "$(OUTDIR)\mshare32.exp"
	-@erase "$(OUTDIR)\mshare32.lib"
	-@erase "$(OUTDIR)\mshare32.pdb"
	-@erase ".\mshare32.dll"
	-@erase ".\mshare32.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /Gm /GX /Zi /Od /I "..\..\common\headers" /D "_DEBUG" /D "__Windows__" /D "WIN32" /D "_WINDOWS" /D "__EXPORT__" /D "__LittleEndian__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\midishare.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\midishare.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\midishare.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\msAlarms.sbr" \
	"$(INTDIR)\msAppls.sbr" \
	"$(INTDIR)\msConnx.sbr" \
	"$(INTDIR)\msFilter.sbr" \
	"$(INTDIR)\msMail.sbr" \
	"$(INTDIR)\msTasks.sbr" \
	"$(INTDIR)\msXmtRcv.sbr" \
	"$(INTDIR)\msHandler.sbr" \
	"$(INTDIR)\msInit.sbr" \
	"$(INTDIR)\msSmpte.sbr" \
	"$(INTDIR)\msTime.sbr" \
	"$(INTDIR)\msEvents.sbr" \
	"$(INTDIR)\msFields.sbr" \
	"$(INTDIR)\msMem.sbr" \
	"$(INTDIR)\msMemory.sbr" \
	"$(INTDIR)\msSeq.sbr" \
	"$(INTDIR)\msSorter.sbr" \
	"$(INTDIR)\msDriver.sbr" \
	"$(INTDIR)\MidiShare.sbr" \
	"$(INTDIR)\msPrefs.sbr" \
	"$(INTDIR)\WinMono.sbr"

"$(OUTDIR)\midishare.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib winmm.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\mshare32.pdb" /debug /machine:I386 /out:"mshare32.dll" /implib:"$(OUTDIR)\mshare32.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\msAlarms.obj" \
	"$(INTDIR)\msAppls.obj" \
	"$(INTDIR)\msConnx.obj" \
	"$(INTDIR)\msFilter.obj" \
	"$(INTDIR)\msMail.obj" \
	"$(INTDIR)\msTasks.obj" \
	"$(INTDIR)\msXmtRcv.obj" \
	"$(INTDIR)\msHandler.obj" \
	"$(INTDIR)\msInit.obj" \
	"$(INTDIR)\msSmpte.obj" \
	"$(INTDIR)\msTime.obj" \
	"$(INTDIR)\msEvents.obj" \
	"$(INTDIR)\msFields.obj" \
	"$(INTDIR)\msMem.obj" \
	"$(INTDIR)\msMemory.obj" \
	"$(INTDIR)\msSeq.obj" \
	"$(INTDIR)\msSorter.obj" \
	"$(INTDIR)\msDriver.obj" \
	"$(INTDIR)\MidiShare.obj" \
	"$(INTDIR)\msPrefs.obj" \
	"$(INTDIR)\WinMono.obj" \
	"$(INTDIR)\midishare.res"

".\mshare32.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("midishare.dep")
!INCLUDE "midishare.dep"
!ELSE 
!MESSAGE Warning: cannot find "midishare.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "midishare - Win32 Release" || "$(CFG)" == "midishare - Win32 Debug"
SOURCE=..\..\common\Clients\msAlarms.c

"$(INTDIR)\msAlarms.obj"	"$(INTDIR)\msAlarms.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Clients\msAppls.c

"$(INTDIR)\msAppls.obj"	"$(INTDIR)\msAppls.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Clients\msConnx.c

"$(INTDIR)\msConnx.obj"	"$(INTDIR)\msConnx.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Clients\msFilter.c

"$(INTDIR)\msFilter.obj"	"$(INTDIR)\msFilter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Clients\msMail.c

"$(INTDIR)\msMail.obj"	"$(INTDIR)\msMail.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Clients\msTasks.c

"$(INTDIR)\msTasks.obj"	"$(INTDIR)\msTasks.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Clients\msXmtRcv.c

"$(INTDIR)\msXmtRcv.obj"	"$(INTDIR)\msXmtRcv.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Kernel\msHandler.c

"$(INTDIR)\msHandler.obj"	"$(INTDIR)\msHandler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Kernel\msInit.c

"$(INTDIR)\msInit.obj"	"$(INTDIR)\msInit.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Kernel\msSmpte.c

"$(INTDIR)\msSmpte.obj"	"$(INTDIR)\msSmpte.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Kernel\msTime.c

"$(INTDIR)\msTime.obj"	"$(INTDIR)\msTime.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Memory\msEvents.c

"$(INTDIR)\msEvents.obj"	"$(INTDIR)\msEvents.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Memory\msFields.c

"$(INTDIR)\msFields.obj"	"$(INTDIR)\msFields.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Memory\msMem.c

"$(INTDIR)\msMem.obj"	"$(INTDIR)\msMem.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Memory\msMemory.c

"$(INTDIR)\msMemory.obj"	"$(INTDIR)\msMemory.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Memory\msSeq.c

"$(INTDIR)\msSeq.obj"	"$(INTDIR)\msSeq.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Sorter\msSorter.c

"$(INTDIR)\msSorter.obj"	"$(INTDIR)\msSorter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\common\Drivers\msDriver.c

"$(INTDIR)\msDriver.obj"	"$(INTDIR)\msDriver.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\MidiShare.c

"$(INTDIR)\MidiShare.obj"	"$(INTDIR)\MidiShare.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\midishare.rc

!IF  "$(CFG)" == "midishare - Win32 Release"


"$(INTDIR)\midishare.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x40c /fo"$(INTDIR)\midishare.res" /i "\Documents and Settings\fober\Documents\src\win-mono" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "midishare - Win32 Debug"


"$(INTDIR)\midishare.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x40c /fo"$(INTDIR)\midishare.res" /i "\Documents and Settings\fober\Documents\src\win-mono" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\msPrefs.c

"$(INTDIR)\msPrefs.obj"	"$(INTDIR)\msPrefs.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\WinMono.c

"$(INTDIR)\WinMono.obj"	"$(INTDIR)\WinMono.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

