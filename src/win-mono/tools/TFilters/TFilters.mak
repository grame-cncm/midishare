# Microsoft Developer Studio Generated NMAKE File, Based on TFilters.dsp
!IF "$(CFG)" == ""
CFG=TFilters - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TFilters - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TFilters - Win32 Release" && "$(CFG)" != "TFilters - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TFilters.mak" CFG="TFilters - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TFilters - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TFilters - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TFilters - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "..\TFilters.exe" "$(OUTDIR)\TFilters.bsc"


CLEAN :
	-@erase "$(INTDIR)\TFilters.obj"
	-@erase "$(INTDIR)\TFilters.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TFilters.bsc"
	-@erase "..\TFilters.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\..\include" /I "..\..\..\tools" /I "..\..\..\lib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TFilters.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TFilters.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\TFilters.sbr"

"$(OUTDIR)\TFilters.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\TFilters.pdb" /machine:I386 /out:"../TFilters.exe" 
LINK32_OBJS= \
	"$(INTDIR)\TFilters.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TFilters.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TFilters - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\TFilters.exe" "$(OUTDIR)\TFilters.bsc"


CLEAN :
	-@erase "$(INTDIR)\TFilters.obj"
	-@erase "$(INTDIR)\TFilters.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TFilters.bsc"
	-@erase "$(OUTDIR)\TFilters.pdb"
	-@erase "..\TFilters.exe"
	-@erase "..\TFilters.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\tools" /I "..\..\..\lib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TFilters.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TFilters.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\TFilters.sbr"

"$(OUTDIR)\TFilters.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\TFilters.pdb" /debug /machine:I386 /out:"../TFilters.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\TFilters.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TFilters.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TFilters.dep")
!INCLUDE "TFilters.dep"
!ELSE 
!MESSAGE Warning: cannot find "TFilters.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TFilters - Win32 Release" || "$(CFG)" == "TFilters - Win32 Debug"
SOURCE=..\..\..\tools\TFilters.c

"$(INTDIR)\TFilters.obj"	"$(INTDIR)\TFilters.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

