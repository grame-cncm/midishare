
all: player.dll

PATHENV = C:\BC45
PATHBIN = C:\BC45\BIN
PATHLIB = C:\BC45\LIB
PATHINC = C:\BC45\INCLUDE

CC 	= Bcc +BccW16.cfg
LINK 	= TLink
BRC   = Brc
SRC   = ..\src
RC    = c:\mstools\binw16\rc

DLL_CFLAGS 		= -P- -I$(PATHINC);$(SRC)\include;. -D_RTLDLL;__MSWindows__;__Windows__;__IncludeThunking__;__UseMSOldInterface__ -w-pia
DLLMFLAGS   	= /c /DIS_16 /W3 /Zi /nologo
DLL_LINKFLAGS 	= -P=32000 -Twd -3 -c -C 
BRC_FLAGS		= -31 -I$(PATHINC) -D_RTLDLL

TARGET = player.dll

#
# Dependency List
#
player : BccW16.cfg  player.dll makefile

Dependances =  makefile player.def\
   OUT16\playThk.obj\
   OUT16\init.obj\
   OUT16\obj1.obj\
   OUT16\obj2.obj\
   OUT16\obj3.obj\
   OUT16\obj4.obj\
   OUT16\obj5.obj\
   OUT16\obj6.obj\
   OUT16\obj7.obj\
   OUT16\obj8.obj\
   OUT16\obj9.obj\
   OUT16\obj10.obj\
   OUT16\obj11.obj\
   OUT16\obj12.obj\
   OUT16\obj13.obj\
   OUT16\obj14.obj\
   OUT16\obj15.obj\
   OUT16\obj16.obj\
   OUT16\obj17.obj\
   OUT16\obj18.obj\
   OUT16\obj19.obj\
   OUT16\obj20.obj\
   OUT16\obj21.obj\
   OUT16\obj22.obj\
   OUT16\obj23.obj\
   OUT16\obj24.obj\
   OUT16\obj25.obj\
   OUT16\obj26.obj\
   OUT16\obj27.obj\
   OUT16\obj28.obj\
   OUT16\obj29.obj\
   OUT16\obj30.obj\
   OUT16\obj31.obj\
   OUT16\obj32.obj\
   OUT16\obj33.obj\
   OUT16\obj34.obj\
   OUT16\obj35.obj\
   OUT16\obj36.obj\
   OUT16\obj37.obj\
   OUT16\obj38.obj\
   OUT16\obj39.obj\
   OUT16\obj40.obj\
   OUT16\obj41.obj\
   OUT16\obj42.obj\
   OUT16\utools.obj\
   OUT16\umidi.obj\
   OUT16\player.res\
   player.rc\

$(TARGET) : $(Dependances) makefile
	 $(LINK) @<<
	 $(DLL_LINKFLAGS) +
			$(PATHLIB)\c0dl.obj+
			 OUT16\playThk+
   OUT16\init+
   OUT16\obj1+
   OUT16\obj2+
   OUT16\obj3+
   OUT16\obj4+
   OUT16\obj5+
   OUT16\obj6+
   OUT16\obj7+
   OUT16\obj8+
   OUT16\obj9+
   OUT16\obj10+
   OUT16\obj11+
   OUT16\obj12+
   OUT16\obj13+
   OUT16\obj14+
   OUT16\obj15+
   OUT16\obj16+
   OUT16\obj17+
   OUT16\obj18+
   OUT16\obj19+
   OUT16\obj20+
   OUT16\obj21+
   OUT16\obj22+
   OUT16\obj23+
   OUT16\obj24+
   OUT16\obj25+
   OUT16\obj26+
   OUT16\obj27+
   OUT16\obj28+
   OUT16\obj29+
   OUT16\obj30+
   OUT16\obj31+
   OUT16\obj32+
   OUT16\obj33+
   OUT16\obj34+
   OUT16\obj35+
   OUT16\obj36+
   OUT16\obj37+
   OUT16\obj38+
   OUT16\obj39+
   OUT16\obj40+
   OUT16\obj41+
   OUT16\obj42+
   OUT16\utools+
   OUT16\umidi
  	$(TARGET)
   player.map
		$(PATHLIB)\import.lib+
      $(PATHLIB)\crtldll.lib+
		mshare.lib
		player.def
<<
	 $(RC) -40 OUT16\player.res $(TARGET)
	 implib player.lib $(TARGET)


playerThunk.asm :  player.thk
	thunk -t PLAYER -o  playerThunk.asm  player.thk

OUT16\playThk.obj :  playerThunk.asm
	ml $(DLLMFLAGS) /Fo OUT16\playThk.obj  playerThunk.asm

OUT16\player.res :  player.rc
  $(BRC) $(BRC_FLAGS) -R -FO$@ player.rc

OUT16\init.obj:  init.c
	 $(CC) -c $(DLL_CFLAGS) -o$@ init.c

OUT16\obj1.obj:  $(SRC)\Chasers\tchaseriterator.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Chasers\tchaseriterator.cpp

OUT16\obj2.obj:  $(SRC)\Chasers\tchaservisitor.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Chasers\tchaservisitor.cpp

OUT16\obj3.obj:  $(SRC)\Chasers\thashtable.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Chasers\thashtable.cpp

OUT16\obj4.obj:  $(SRC)\Events\texteventreceiver.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Events\texteventreceiver.cpp

OUT16\obj5.obj:  $(SRC)\Events\tsmpteeventreceiver.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Events\tsmpteeventreceiver.cpp

OUT16\obj6.obj:  $(SRC)\Events\teventreceiver.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Events\teventreceiver.cpp

OUT16\obj7.obj:  $(SRC)\Events\tmidiplayer.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Events\tmidiplayer.cpp

OUT16\obj8.obj:  $(SRC)\Events\tclockeventreceiver.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Events\tclockeventreceiver.cpp

OUT16\obj9.obj:  $(SRC)\Events\teventrecorder.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Events\teventrecorder.cpp


OUT16\obj10.obj:  $(SRC)\Factory\tplayerfactory.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Factory\tplayerfactory.cpp

OUT16\obj11.obj:  $(SRC)\Framework\Events\teventfactory.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Events\teventfactory.cpp

OUT16\obj12.obj:  $(SRC)\Framework\MidiAppl\midifilter.c
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\MidiAppl\midifilter.c

OUT16\obj13.obj:  $(SRC)\Framework\MidiAppl\tmidiappl.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\MidiAppl\tmidiappl.cpp

OUT16\obj14.obj:  $(SRC)\Framework\Scheduler\tscheduler.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Scheduler\tscheduler.cpp

OUT16\obj15.obj:  $(SRC)\Framework\Score\uscoremanager.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Score\uscoremanager.cpp

OUT16\obj16.obj:  $(SRC)\Framework\Score\tscore.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Score\tscore.cpp

OUT16\obj17.obj:  $(SRC)\Framework\Score\tscorefollower.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Score\tscorefollower.cpp

OUT16\obj18.obj:  $(SRC)\Framework\Score\tscoreiterator.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Score\tscoreiterator.cpp

OUT16\obj19.obj:  $(SRC)\Framework\Tempo\ttempomapvisitor.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Tempo\ttempomapvisitor.cpp

OUT16\obj20.obj:  $(SRC)\Framework\Tempo\ttempomap.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Tempo\ttempomap.cpp

OUT16\obj21.obj:  $(SRC)\Framework\Tempo\ttimemanager.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Tempo\ttimemanager.cpp

OUT16\obj22.obj:  $(SRC)\Framework\Tempo\ttimeconverter.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Tempo\ttimeconverter.cpp

OUT16\obj23.obj:  $(SRC)\Framework\Tempo\ttime.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Tempo\ttime.cpp

OUT16\obj24.obj:  $(SRC)\Framework\Tempo\ttempoconverter.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Framework\Tempo\ttempoconverter.cpp

OUT16\obj25.obj:  $(SRC)\Library\player.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Library\player.cpp

OUT16\obj26.obj:  $(SRC)\Library\tplayer.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Library\tplayer.cpp

OUT16\obj27.obj:  $(SRC)\Loop\tloopmanager.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Loop\tloopmanager.cpp

OUT16\obj28.obj:  $(SRC)\Midifile\tmtrack.c
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Midifile\tmtrack.c

OUT16\obj29.obj:  $(SRC)\Midifile\midifile.c
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Midifile\midifile.c

OUT16\obj30.obj:  $(SRC)\Players\ttickplayer.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Players\ttickplayer.cpp

OUT16\obj31.obj:  $(SRC)\Players\trunningplayer.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Players\trunningplayer.cpp

OUT16\obj32.obj:  $(SRC)\Players\tsyncinplayer.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Players\tsyncinplayer.cpp

OUT16\obj33.obj:  $(SRC)\Players\tsyncoutplayer.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Players\tsyncoutplayer.cpp

OUT16\obj34.obj:  $(SRC)\Players\tsmpteplayer.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Players\tsmpteplayer.cpp

OUT16\obj35.obj:  $(SRC)\Score\tplayerscore.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Score\tplayerscore.cpp

OUT16\obj36.obj:  $(SRC)\Score\tscoreinserter.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Score\tscoreinserter.cpp

OUT16\obj37.obj:  $(SRC)\Synchroniser\tplayersynchroniserext.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Synchroniser\tplayersynchroniserext.cpp

OUT16\obj38.obj:  $(SRC)\Synchroniser\tplayersynchroniserclock.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Synchroniser\tplayersynchroniserclock.cpp

OUT16\obj39.obj:  $(SRC)\Synchroniser\tplayersynchroniserint.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Synchroniser\tplayersynchroniserint.cpp

OUT16\obj40.obj:  $(SRC)\Synchroniser\tclocksender.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Synchroniser\tclocksender.cpp

OUT16\obj41.obj:  $(SRC)\Tracks\ttracktable.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Tracks\ttracktable.cpp

OUT16\utools.obj:  $(SRC)\Utils\utools.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Utils\utools.cpp

OUT16\umidi.obj:  $(SRC)\Utils\umidi.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ $(SRC)\Utils\umidi.cpp

OUT16\obj42.obj:  playerwin16.cpp
	 $(CC) -c $(DLL_CFLAGS) -o$@ playerwin16.cpp
