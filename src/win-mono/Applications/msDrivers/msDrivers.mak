# Microsoft Developer Studio Generated NMAKE File, Based on msDrivers.dsp
!IF "$(CFG)" == ""
CFG=msDrivers - Win32 Debug
!MESSAGE No configuration specified. Defaulting to msDrivers - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "msDrivers - Win32 Release" && "$(CFG)" != "msDrivers - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "msDrivers.mak" CFG="msDrivers - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "msDrivers - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "msDrivers - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "msDrivers - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\msDrivers.exe"


CLEAN :
	-@erase "$(INTDIR)\msDrivers.obj"
	-@erase "$(INTDIR)\msDrivers.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\msDrivers.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../Include" /I "../../../lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\msDrivers.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\msDrivers.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msDrivers.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\msDrivers.pdb" /machine:I386 /out:"../msDrivers.exe" 
LINK32_OBJS= \
	"$(INTDIR)\msDrivers.obj" \
	"$(INTDIR)\msDrivers.res" \
	"..\..\VCProject\Release\mshare32.lib"

"..\msDrivers.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "msDrivers - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\msDrivers.exe" "$(OUTDIR)\msDrivers.bsc"


CLEAN :
	-@erase "$(INTDIR)\msDrivers.obj"
	-@erase "$(INTDIR)\msDrivers.res"
	-@erase "$(INTDIR)\msDrivers.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\msDrivers.bsc"
	-@erase "$(OUTDIR)\msDrivers.pdb"
	-@erase ".\msDrivers.exe"
	-@erase ".\msDrivers.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /MLd /W3 /Gm /GX /ZI /Od /I "../../Include" /I "../../../lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\msDrivers.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\msDrivers.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msDrivers.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\msDrivers.sbr"

"$(OUTDIR)\msDrivers.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\msDrivers.pdb" /debug /machine:I386 /out:"msDrivers.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\msDrivers.obj" \
	"$(INTDIR)\msDrivers.res" \
	"..\..\VCProject\Release\mshare32.lib"

".\msDrivers.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("msDrivers.dep")
!INCLUDE "msDrivers.dep"
!ELSE 
!MESSAGE Warning: cannot find "msDrivers.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "msDrivers - Win32 Release" || "$(CFG)" == "msDrivers - Win32 Debug"
SOURCE=.\msDrivers.c

!IF  "$(CFG)" == "msDrivers - Win32 Release"


"$(INTDIR)\msDrivers.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "msDrivers - Win32 Debug"


"$(INTDIR)\msDrivers.obj"	"$(INTDIR)\msDrivers.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\msDrivers.rc

"$(INTDIR)\msDrivers.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

