# Microsoft Developer Studio Generated NMAKE File, Based on TEvents.dsp
!IF "$(CFG)" == ""
CFG=TEvents - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TEvents - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TEvents - Win32 Release" && "$(CFG)" != "TEvents - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TEvents.mak" CFG="TEvents - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TEvents - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TEvents - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TEvents - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\TEvents.exe"


CLEAN :
	-@erase "$(INTDIR)\TEvents.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\TEvents.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\..\include" /I "..\..\..\tools" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /Fp"$(INTDIR)\TEvents.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TEvents.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\TEvents.pdb" /machine:I386 /out:"../TEvents.exe" 
LINK32_OBJS= \
	"$(INTDIR)\TEvents.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TEvents.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TEvents - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\TEvents.exe" "$(OUTDIR)\TEvents.bsc"


CLEAN :
	-@erase "$(INTDIR)\TEvents.obj"
	-@erase "$(INTDIR)\TEvents.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TEvents.bsc"
	-@erase "$(OUTDIR)\TEvents.pdb"
	-@erase "..\TEvents.exe"
	-@erase "..\TEvents.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\tools" /D "_DEBUG" /D "__Windows__" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TEvents.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TEvents.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\TEvents.sbr"

"$(OUTDIR)\TEvents.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\TEvents.pdb" /debug /machine:I386 /out:"../TEvents.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\TEvents.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TEvents.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TEvents.dep")
!INCLUDE "TEvents.dep"
!ELSE 
!MESSAGE Warning: cannot find "TEvents.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TEvents - Win32 Release" || "$(CFG)" == "TEvents - Win32 Debug"
SOURCE=..\..\..\tools\TEvents.c

!IF  "$(CFG)" == "TEvents - Win32 Release"


"$(INTDIR)\TEvents.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TEvents - Win32 Debug"


"$(INTDIR)\TEvents.obj"	"$(INTDIR)\TEvents.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

