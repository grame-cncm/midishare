# Microsoft Developer Studio Generated NMAKE File, Based on TDrivers.dsp
!IF "$(CFG)" == ""
CFG=TDrivers - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TDrivers - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TDrivers - Win32 Release" && "$(CFG)" != "TDrivers - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TDrivers.mak" CFG="TDrivers - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TDrivers - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TDrivers - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TDrivers - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\TDrivers.exe"


CLEAN :
	-@erase "$(INTDIR)\TDrivers.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\TDrivers.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\..\include" /I "..\..\..\tools" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /Fp"$(INTDIR)\TDrivers.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TDrivers.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\TDrivers.pdb" /machine:I386 /out:"../TDrivers.exe" 
LINK32_OBJS= \
	"$(INTDIR)\TDrivers.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TDrivers.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TDrivers - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\TDrivers.exe" "$(OUTDIR)\TDrivers.bsc"


CLEAN :
	-@erase "$(INTDIR)\TDrivers.obj"
	-@erase "$(INTDIR)\TDrivers.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TDrivers.bsc"
	-@erase "$(OUTDIR)\TDrivers.pdb"
	-@erase "..\TDrivers.exe"
	-@erase "..\TDrivers.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\tools" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TDrivers.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TDrivers.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\TDrivers.sbr"

"$(OUTDIR)\TDrivers.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\TDrivers.pdb" /debug /machine:I386 /out:"../TDrivers.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\TDrivers.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TDrivers.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TDrivers.dep")
!INCLUDE "TDrivers.dep"
!ELSE 
!MESSAGE Warning: cannot find "TDrivers.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TDrivers - Win32 Release" || "$(CFG)" == "TDrivers - Win32 Debug"
SOURCE=..\..\..\tools\TDrivers.c

!IF  "$(CFG)" == "TDrivers - Win32 Release"


"$(INTDIR)\TDrivers.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TDrivers - Win32 Debug"


"$(INTDIR)\TDrivers.obj"	"$(INTDIR)\TDrivers.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

