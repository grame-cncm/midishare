# Microsoft Developer Studio Generated NMAKE File, Based on msControl32.dsp
!IF "$(CFG)" == ""
CFG=msControl32 - Win32 Release
!MESSAGE No configuration specified. Defaulting to msControl32 - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "msControl32 - Win32 Release" && "$(CFG)" != "msControl32 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "msControl32.mak" CFG="msControl32 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "msControl32 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "msControl32 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "msControl32 - Win32 Release"

OUTDIR=.
INTDIR=.\Release

ALL : "..\msControl32.exe"


CLEAN :
	-@erase "$(INTDIR)\Msctrl.obj"
	-@erase "$(INTDIR)\msCtrl32.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\msControl32.exe"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\msControl32.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\msCtrl32.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msControl32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\msControl32.pdb" /machine:I386 /out:"../msControl32.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Msctrl.obj" \
	"$(INTDIR)\msCtrl32.res" \
	"..\..\VCProject\Release\mshare32.lib"

"..\msControl32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "msControl32 - Win32 Debug"

OUTDIR=.
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : "$(OUTDIR)\msControl32.exe" "$(OUTDIR)\msControl32.bsc"


CLEAN :
	-@erase "$(INTDIR)\Msctrl.obj"
	-@erase "$(INTDIR)\Msctrl.sbr"
	-@erase "$(INTDIR)\msCtrl32.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\msControl32.bsc"
	-@erase "$(OUTDIR)\msControl32.exe"
	-@erase "$(OUTDIR)\msControl32.ilk"
	-@erase "$(OUTDIR)\msControl32.pdb"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "../../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\msControl32.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\msCtrl32.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msControl32.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Msctrl.sbr"

"$(OUTDIR)\msControl32.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\msControl32.pdb" /debug /machine:I386 /out:"$(OUTDIR)\msControl32.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Msctrl.obj" \
	"$(INTDIR)\msCtrl32.res" \
	"..\..\VCProject\Release\mshare32.lib"

"$(OUTDIR)\msControl32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("msControl32.dep")
!INCLUDE "msControl32.dep"
!ELSE 
!MESSAGE Warning: cannot find "msControl32.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "msControl32 - Win32 Release" || "$(CFG)" == "msControl32 - Win32 Debug"
SOURCE=.\Msctrl.c

!IF  "$(CFG)" == "msControl32 - Win32 Release"


"$(INTDIR)\Msctrl.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "msControl32 - Win32 Debug"


"$(INTDIR)\Msctrl.obj"	"$(INTDIR)\Msctrl.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\msCtrl32.rc

"$(INTDIR)\msCtrl32.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

