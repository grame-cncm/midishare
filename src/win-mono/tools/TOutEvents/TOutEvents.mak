# Microsoft Developer Studio Generated NMAKE File, Based on TOutEvents.dsp
!IF "$(CFG)" == ""
CFG=TOutEvents - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TOutEvents - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TOutEvents - Win32 Release" && "$(CFG)" != "TOutEvents - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TOutEvents.mak" CFG="TOutEvents - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TOutEvents - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TOutEvents - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TOutEvents - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\TOutEvents.exe"


CLEAN :
	-@erase "$(INTDIR)\TOutEvents.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\TOutEvents.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\..\include" /I "..\..\..\tools" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /Fp"$(INTDIR)\TOutEvents.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TOutEvents.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\TOutEvents.pdb" /machine:I386 /out:"../TOutEvents.exe" 
LINK32_OBJS= \
	"$(INTDIR)\TOutEvents.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TOutEvents.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TOutEvents - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\TOutEvents.exe"


CLEAN :
	-@erase "$(INTDIR)\TOutEvents.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TOutEvents.pdb"
	-@erase ".\TOutEvents.exe"
	-@erase ".\TOutEvents.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\tools" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /Fp"$(INTDIR)\TOutEvents.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TOutEvents.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\TOutEvents.pdb" /debug /machine:I386 /out:"TOutEvents.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\TOutEvents.obj" \
	"..\..\VCProject\Release\mshare32.lib"

".\TOutEvents.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TOutEvents.dep")
!INCLUDE "TOutEvents.dep"
!ELSE 
!MESSAGE Warning: cannot find "TOutEvents.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TOutEvents - Win32 Release" || "$(CFG)" == "TOutEvents - Win32 Debug"
SOURCE=..\..\..\tools\TOutEvents.c

"$(INTDIR)\TOutEvents.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

