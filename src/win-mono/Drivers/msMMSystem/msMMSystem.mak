# Microsoft Developer Studio Generated NMAKE File, Based on msMMSystem.dsp
!IF "$(CFG)" == ""
CFG=msMMSystem - Win32 Debug
!MESSAGE No configuration specified. Defaulting to msMMSystem - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "msMMSystem - Win32 Release" && "$(CFG)" != "msMMSystem - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "msMMSystem.mak" CFG="msMMSystem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "msMMSystem - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "msMMSystem - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "msMMSystem - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\msMMSystem.dll"


CLEAN :
	-@erase "$(INTDIR)\EventToMidiStream.obj"
	-@erase "$(INTDIR)\MidiStreamToEvent.obj"
	-@erase "$(INTDIR)\msMMDriver.obj"
	-@erase "$(INTDIR)\msMMDriver.res"
	-@erase "$(INTDIR)\msMMError.obj"
	-@erase "$(INTDIR)\msMMInOut.obj"
	-@erase "$(INTDIR)\msMMMain.obj"
	-@erase "$(INTDIR)\msMMState.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\msMMSystem.exp"
	-@erase "$(OUTDIR)\msMMSystem.lib"
	-@erase "..\msMMSystem.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\..\include" /I "../../../lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\msMMSystem.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\msMMDriver.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msMMSystem.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winmm.lib libc.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\msMMSystem.pdb" /machine:I386 /nodefaultlib /out:"../msMMSystem.dll" /implib:"$(OUTDIR)\msMMSystem.lib" /fixed:no 
LINK32_OBJS= \
	"$(INTDIR)\EventToMidiStream.obj" \
	"$(INTDIR)\MidiStreamToEvent.obj" \
	"$(INTDIR)\msMMDriver.obj" \
	"$(INTDIR)\msMMError.obj" \
	"$(INTDIR)\msMMInOut.obj" \
	"$(INTDIR)\msMMMain.obj" \
	"$(INTDIR)\msMMState.obj" \
	"$(INTDIR)\msMMDriver.res" \
	"..\..\VCProject\Release\mshare32.lib"

"..\msMMSystem.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "msMMSystem - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\msMMSystem.dll" "$(OUTDIR)\msMMSystem.bsc"


CLEAN :
	-@erase "$(INTDIR)\EventToMidiStream.obj"
	-@erase "$(INTDIR)\EventToMidiStream.sbr"
	-@erase "$(INTDIR)\MidiStreamToEvent.obj"
	-@erase "$(INTDIR)\MidiStreamToEvent.sbr"
	-@erase "$(INTDIR)\msMMDriver.obj"
	-@erase "$(INTDIR)\msMMDriver.res"
	-@erase "$(INTDIR)\msMMDriver.sbr"
	-@erase "$(INTDIR)\msMMError.obj"
	-@erase "$(INTDIR)\msMMError.sbr"
	-@erase "$(INTDIR)\msMMInOut.obj"
	-@erase "$(INTDIR)\msMMInOut.sbr"
	-@erase "$(INTDIR)\msMMMain.obj"
	-@erase "$(INTDIR)\msMMMain.sbr"
	-@erase "$(INTDIR)\msMMState.obj"
	-@erase "$(INTDIR)\msMMState.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\msMMSystem.bsc"
	-@erase "$(OUTDIR)\msMMSystem.exp"
	-@erase "$(OUTDIR)\msMMSystem.lib"
	-@erase "$(OUTDIR)\msMMSystem.pdb"
	-@erase ".\msMMSystem.dll"
	-@erase ".\msMMSystem.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G3 /Zp2 /MTd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "../../../lib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\msMMSystem.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x40c /fo"$(INTDIR)\msMMDriver.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\msMMSystem.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\EventToMidiStream.sbr" \
	"$(INTDIR)\MidiStreamToEvent.sbr" \
	"$(INTDIR)\msMMDriver.sbr" \
	"$(INTDIR)\msMMError.sbr" \
	"$(INTDIR)\msMMInOut.sbr" \
	"$(INTDIR)\msMMMain.sbr" \
	"$(INTDIR)\msMMState.sbr"

"$(OUTDIR)\msMMSystem.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winmm.lib libc.lib /nologo /entry:"DllEntryPoint" /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\msMMSystem.pdb" /debug /machine:I386 /nodefaultlib /out:"msMMSystem.dll" /implib:"$(OUTDIR)\msMMSystem.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\EventToMidiStream.obj" \
	"$(INTDIR)\MidiStreamToEvent.obj" \
	"$(INTDIR)\msMMDriver.obj" \
	"$(INTDIR)\msMMError.obj" \
	"$(INTDIR)\msMMInOut.obj" \
	"$(INTDIR)\msMMMain.obj" \
	"$(INTDIR)\msMMState.obj" \
	"$(INTDIR)\msMMDriver.res" \
	"..\..\VCProject\Release\mshare32.lib"

".\msMMSystem.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("msMMSystem.dep")
!INCLUDE "msMMSystem.dep"
!ELSE 
!MESSAGE Warning: cannot find "msMMSystem.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "msMMSystem - Win32 Release" || "$(CFG)" == "msMMSystem - Win32 Debug"
SOURCE=..\..\..\lib\EventToMidiStream.c

!IF  "$(CFG)" == "msMMSystem - Win32 Release"


"$(INTDIR)\EventToMidiStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "msMMSystem - Win32 Debug"


"$(INTDIR)\EventToMidiStream.obj"	"$(INTDIR)\EventToMidiStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\lib\MidiStreamToEvent.c

!IF  "$(CFG)" == "msMMSystem - Win32 Release"


"$(INTDIR)\MidiStreamToEvent.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "msMMSystem - Win32 Debug"


"$(INTDIR)\MidiStreamToEvent.obj"	"$(INTDIR)\MidiStreamToEvent.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\msMMDriver.c

!IF  "$(CFG)" == "msMMSystem - Win32 Release"


"$(INTDIR)\msMMDriver.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "msMMSystem - Win32 Debug"


"$(INTDIR)\msMMDriver.obj"	"$(INTDIR)\msMMDriver.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\msMMDriver.rc

"$(INTDIR)\msMMDriver.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\msMMError.c

!IF  "$(CFG)" == "msMMSystem - Win32 Release"


"$(INTDIR)\msMMError.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "msMMSystem - Win32 Debug"


"$(INTDIR)\msMMError.obj"	"$(INTDIR)\msMMError.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\msMMInOut.c

!IF  "$(CFG)" == "msMMSystem - Win32 Release"


"$(INTDIR)\msMMInOut.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "msMMSystem - Win32 Debug"


"$(INTDIR)\msMMInOut.obj"	"$(INTDIR)\msMMInOut.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\msMMMain.c

!IF  "$(CFG)" == "msMMSystem - Win32 Release"


"$(INTDIR)\msMMMain.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "msMMSystem - Win32 Debug"


"$(INTDIR)\msMMMain.obj"	"$(INTDIR)\msMMMain.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\msMMState.c

!IF  "$(CFG)" == "msMMSystem - Win32 Release"


"$(INTDIR)\msMMState.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "msMMSystem - Win32 Debug"


"$(INTDIR)\msMMState.obj"	"$(INTDIR)\msMMState.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

