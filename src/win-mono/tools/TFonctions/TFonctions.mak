# Microsoft Developer Studio Generated NMAKE File, Based on TFonctions.dsp
!IF "$(CFG)" == ""
CFG=TFonctions - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TFonctions - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TFonctions - Win32 Release" && "$(CFG)" != "TFonctions - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TFonctions.mak" CFG="TFonctions - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TFonctions - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TFonctions - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TFonctions - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\TFonctions.exe"


CLEAN :
	-@erase "$(INTDIR)\TFonctions.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\TFonctions.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\..\include" /I "..\..\..\tools" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /Fp"$(INTDIR)\TFonctions.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TFonctions.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\TFonctions.pdb" /machine:I386 /out:"../TFonctions.exe" 
LINK32_OBJS= \
	"$(INTDIR)\TFonctions.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TFonctions.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TFonctions - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\TFonctions.exe"


CLEAN :
	-@erase "$(INTDIR)\TFonctions.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TFonctions.pdb"
	-@erase "..\TFonctions.exe"
	-@erase "..\TFonctions.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\tools" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__Windows__" /Fp"$(INTDIR)\TFonctions.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TFonctions.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\TFonctions.pdb" /debug /machine:I386 /out:"../TFonctions.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\TFonctions.obj" \
	"..\..\VCProject\Release\mshare32.lib"

"..\TFonctions.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TFonctions.dep")
!INCLUDE "TFonctions.dep"
!ELSE 
!MESSAGE Warning: cannot find "TFonctions.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TFonctions - Win32 Release" || "$(CFG)" == "TFonctions - Win32 Debug"
SOURCE=..\..\..\tools\TFonctions.c

"$(INTDIR)\TFonctions.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

