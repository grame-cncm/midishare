# Microsoft Developer Studio Generated NMAKE File, Based on msConnect.dsp
!IF "$(CFG)" == ""
CFG=msConnect - Win32 Debug
!MESSAGE No configuration specified. Defaulting to msConnect - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "msConnect - Win32 Release" && "$(CFG)" != "msConnect - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "msConnect.mak" CFG="msConnect - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "msConnect - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "msConnect - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "msConnect - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\msConnect.exe"


CLEAN :
	-@erase "$(INTDIR)\Mscnct.obj"
	-@erase "$(INTDIR)\mscnct32.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\msConnect.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\msConnect.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\mscnct32.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msConnect.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\msConnect.pdb" /machine:I386 /out:"../msConnect.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Mscnct.obj" \
	"$(INTDIR)\mscnct32.res" \
	"..\..\VCProject\Release\mshare32.lib"

"..\msConnect.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "msConnect - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\msConnect.exe" "$(OUTDIR)\msConnect.bsc"


CLEAN :
	-@erase "$(INTDIR)\Mscnct.obj"
	-@erase "$(INTDIR)\Mscnct.sbr"
	-@erase "$(INTDIR)\mscnct32.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\msConnect.bsc"
	-@erase "$(OUTDIR)\msConnect.pdb"
	-@erase ".\msConnect.exe"
	-@erase ".\msConnect.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "../../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\msConnect.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\mscnct32.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msConnect.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Mscnct.sbr"

"$(OUTDIR)\msConnect.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\msConnect.pdb" /debug /machine:I386 /out:"msConnect.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Mscnct.obj" \
	"$(INTDIR)\mscnct32.res" \
	"..\..\VCProject\Release\mshare32.lib"

".\msConnect.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("msConnect.dep")
!INCLUDE "msConnect.dep"
!ELSE 
!MESSAGE Warning: cannot find "msConnect.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "msConnect - Win32 Release" || "$(CFG)" == "msConnect - Win32 Debug"
SOURCE=.\Mscnct.c

!IF  "$(CFG)" == "msConnect - Win32 Release"


"$(INTDIR)\Mscnct.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "msConnect - Win32 Debug"


"$(INTDIR)\Mscnct.obj"	"$(INTDIR)\Mscnct.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mscnct32.rc

"$(INTDIR)\mscnct32.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

