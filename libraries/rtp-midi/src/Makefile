
TARGET = librtpmidi.so
LIB = /usr/local/lib/librtpmidi.so
INCF = /usr/local/include

CXX = g++
CXXFLAGS = -Wall -fPIC -g
LIBS = -lMidiShare
#for mac os x only:
#CXXFLAGS = $(CXXFLAGS) -D__Types__
#LIBS = -framework MidiShare

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

# TARGETS

first : $(TARGET)

$(TARGET) : $(OBJ)
	$(CXX) $(LIBS) -shared -W1,soname,$(TARGET) $(OBJ) -o $(TARGET)

install :
	cp $(TARGET) $(LIB).0.5.0
	[ -f $(LIB).0 ] || ln -s $(LIB).0.5.0 $(LIB).0
	[ -f $(LIB) ] || ln -s $(LIB).0 $(LIB)
	[ -d $(INCF) ] || mkdir $(INCF)
	[ -d $(INCF)/rtpmidi ] || mkdir $(INCF)/rtpmidi
	install StreamWriter.h $(INCF)/rtpmidi
	install StreamReader.h $(INCF)/rtpmidi
	install printingUtils.h $(INCF)/rtpmidi
	install eventUtils.h $(INCF)/rtpmidi
	install binaryUtils.h $(INCF)/rtpmidi
	install RtpMidi.h $(INCF)

uninstall :
	[ ! -f $(LIB) ] || rm $(LIB)
	[ ! -f $(LIB).0 ] || rm $(LIB).0
	[ ! -f $(LIB).0.5.0 ] || rm $(LIB).0.5.0
	[ ! -f $(INCF)/rtpmidi/StreamWriter.h ] || rm $(INCF)/rtpmidi/StreamWriter.h
	[ ! -f $(INCF)/rtpmidi/StreamReader.h ] || rm $(INCF)/rtpmidi/StreamReader.h
	[ ! -f $(INCF)/rtpmidi/printingUtils.h ] || rm $(INCF)/rtpmidi/printingUtils.h
	[ ! -f $(INCF)/rtpmidi/eventUtils.h ] || rm $(INCF)/rtpmidi/eventUtils.h
	[ ! -f $(INCF)/rtpmidi/binaryUtils.h ] || rm $(INCF)/rtpmidi/binaryUtils.h
	[ ! -f $(INCF)/rtpmidi/RtpMidi.h ] || rm $(INCF)/RtpMidi.h
	[ ! -d $(INCF)/rtpmidi/rtpmidi ] || rmdir $(INCF)/rtpmidi

clean :
	rm -f $(OBJ) $(TARGET)

depend:
	makedepend -- $(CXXFLAGS) -- $(SRC)
# DO NOT DELETE

binaryUtils.o: binaryUtils.h types.h
ChannelChapterReader.o: ChannelChapterReader.h types.h ChapterReader.h
ChannelChapterReader.o: Chapter.h /usr/include/MidiShare.h RecoveryJournal.h
ChannelChapterReader.o: ChannelReader.h Channel.h RecoveryJournalReader.h
ChannelChapterReader.o: StreamReader.h Stream.h
ChannelChapterWriter.o: ChannelChapterWriter.h types.h ChapterWriter.h
ChannelChapterWriter.o: Chapter.h /usr/include/MidiShare.h RecoveryJournal.h
ChannelChapterWriter.o: binaryUtils.h ChannelWriter.h Channel.h
ChannelChapterWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
ChannelChapterWriter.o: printingUtils.h
Channel.o: Chapter.h /usr/include/MidiShare.h types.h RecoveryJournal.h
Channel.o: Channel.h
ChannelReader.o: ChannelReader.h Channel.h /usr/include/MidiShare.h types.h
ChannelReader.o: ChapterPReader.h ChannelChapterReader.h ChapterReader.h
ChannelReader.o: Chapter.h RecoveryJournal.h RecoveryJournalReader.h
ChannelReader.o: StreamReader.h Stream.h ChapterCReader.h ChapterWReader.h
ChannelReader.o: ChapterNReader.h ChapterTReader.h ChapterAReader.h
ChannelReader.o: binaryUtils.h
ChannelWriter.o: ChannelWriter.h Channel.h /usr/include/MidiShare.h types.h
ChannelWriter.o: ChapterPWriter.h ChannelChapterWriter.h ChapterWriter.h
ChannelWriter.o: Chapter.h RecoveryJournal.h binaryUtils.h
ChannelWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
ChannelWriter.o: ChapterCWriter.h ChapterWWriter.h ChapterNWriter.h
ChannelWriter.o: ChapterTWriter.h ChapterAWriter.h eventUtils.h
ChapterAReader.o: ChapterAReader.h ChannelChapterReader.h types.h
ChapterAReader.o: ChapterReader.h Chapter.h /usr/include/MidiShare.h
ChapterAReader.o: RecoveryJournal.h ChannelReader.h Channel.h
ChapterAReader.o: RecoveryJournalReader.h StreamReader.h Stream.h
ChapterAReader.o: eventUtils.h
ChapterAWriter.o: ChapterAWriter.h ChannelChapterWriter.h types.h
ChapterAWriter.o: ChapterWriter.h Chapter.h /usr/include/MidiShare.h
ChapterAWriter.o: RecoveryJournal.h binaryUtils.h ChannelWriter.h Channel.h
ChapterAWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
Chapter.o: Chapter.h /usr/include/MidiShare.h types.h RecoveryJournal.h
ChapterCReader.o: ChapterCReader.h ChannelChapterReader.h types.h
ChapterCReader.o: ChapterReader.h Chapter.h /usr/include/MidiShare.h
ChapterCReader.o: RecoveryJournal.h ChannelReader.h Channel.h
ChapterCReader.o: RecoveryJournalReader.h StreamReader.h Stream.h
ChapterCReader.o: eventUtils.h
ChapterCWriter.o: ChapterCWriter.h ChannelChapterWriter.h types.h
ChapterCWriter.o: ChapterWriter.h Chapter.h /usr/include/MidiShare.h
ChapterCWriter.o: RecoveryJournal.h binaryUtils.h ChannelWriter.h Channel.h
ChapterCWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
ChapterNReader.o: ChapterNReader.h ChannelChapterReader.h types.h
ChapterNReader.o: ChapterReader.h Chapter.h /usr/include/MidiShare.h
ChapterNReader.o: RecoveryJournal.h ChannelReader.h Channel.h
ChapterNReader.o: RecoveryJournalReader.h StreamReader.h Stream.h
ChapterNReader.o: printingUtils.h eventUtils.h
ChapterNWriter.o: ChapterNWriter.h ChannelChapterWriter.h types.h
ChapterNWriter.o: ChapterWriter.h Chapter.h /usr/include/MidiShare.h
ChapterNWriter.o: RecoveryJournal.h binaryUtils.h ChannelWriter.h Channel.h
ChapterNWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
ChapterNWriter.o: printingUtils.h
ChapterPReader.o: ChapterPReader.h ChannelChapterReader.h types.h
ChapterPReader.o: ChapterReader.h Chapter.h /usr/include/MidiShare.h
ChapterPReader.o: RecoveryJournal.h ChannelReader.h Channel.h
ChapterPReader.o: RecoveryJournalReader.h StreamReader.h Stream.h
ChapterPReader.o: eventUtils.h
ChapterPWriter.o: ChapterPWriter.h ChannelChapterWriter.h types.h
ChapterPWriter.o: ChapterWriter.h Chapter.h /usr/include/MidiShare.h
ChapterPWriter.o: RecoveryJournal.h binaryUtils.h ChannelWriter.h Channel.h
ChapterPWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
ChapterPWriter.o: printingUtils.h
ChapterReader.o: ChapterReader.h types.h Chapter.h /usr/include/MidiShare.h
ChapterReader.o: RecoveryJournal.h
ChapterTReader.o: ChapterTReader.h ChannelChapterReader.h types.h
ChapterTReader.o: ChapterReader.h Chapter.h /usr/include/MidiShare.h
ChapterTReader.o: RecoveryJournal.h ChannelReader.h Channel.h
ChapterTReader.o: RecoveryJournalReader.h StreamReader.h Stream.h
ChapterTReader.o: eventUtils.h
ChapterTWriter.o: ChapterTWriter.h ChannelChapterWriter.h types.h
ChapterTWriter.o: ChapterWriter.h Chapter.h /usr/include/MidiShare.h
ChapterTWriter.o: RecoveryJournal.h binaryUtils.h ChannelWriter.h Channel.h
ChapterTWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
ChapterWReader.o: ChapterWReader.h ChannelChapterReader.h types.h
ChapterWReader.o: ChapterReader.h Chapter.h /usr/include/MidiShare.h
ChapterWReader.o: RecoveryJournal.h ChannelReader.h Channel.h
ChapterWReader.o: RecoveryJournalReader.h StreamReader.h Stream.h
ChapterWReader.o: eventUtils.h
ChapterWriter.o: ChapterWriter.h types.h Chapter.h /usr/include/MidiShare.h
ChapterWriter.o: RecoveryJournal.h binaryUtils.h /usr/include/string.h
ChapterWriter.o: /usr/include/features.h /usr/include/sys/cdefs.h
ChapterWriter.o: /usr/include/gnu/stubs.h
ChapterWriter.o: /usr/lib/gcc-lib/i486-linux/3.3.5/include/stddef.h
ChapterWriter.o: RecoveryJournalWriter.h
ChapterWWriter.o: ChapterWWriter.h ChannelChapterWriter.h types.h
ChapterWWriter.o: ChapterWriter.h Chapter.h /usr/include/MidiShare.h
ChapterWWriter.o: RecoveryJournal.h binaryUtils.h ChannelWriter.h Channel.h
ChapterWWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
CommandFilter.o: CommandFilter.h /usr/include/MidiShare.h types.h
CommandFilter.o: eventUtils.h binaryUtils.h printingUtils.h
deltaTime.o: deltaTime.h types.h
eventUtils.o: eventUtils.h /usr/include/MidiShare.h types.h printingUtils.h
PayloadReader.o: PayloadReader.h types.h /usr/include/MidiShare.h deltaTime.h
PayloadReader.o: eventUtils.h binaryUtils.h /usr/include/stdlib.h
PayloadReader.o: /usr/include/features.h /usr/include/sys/cdefs.h
PayloadReader.o: /usr/include/gnu/stubs.h
PayloadReader.o: /usr/lib/gcc-lib/i486-linux/3.3.5/include/stddef.h
PayloadWriter.o: PayloadWriter.h types.h /usr/include/MidiShare.h
PayloadWriter.o: RecoveryJournalWriter.h RecoveryJournal.h StreamWriter.h
PayloadWriter.o: Stream.h deltaTime.h eventUtils.h binaryUtils.h
PayloadWriter.o: printingUtils.h
printingUtils.o: printingUtils.h types.h /usr/include/MidiShare.h
printingUtils.o: eventUtils.h binaryUtils.h deltaTime.h
RecoveryJournal.o: RecoveryJournal.h /usr/include/MidiShare.h types.h
RecoveryJournal.o: Chapter.h Channel.h
RecoveryJournalReader.o: RecoveryJournalReader.h RecoveryJournal.h
RecoveryJournalReader.o: /usr/include/MidiShare.h types.h StreamReader.h
RecoveryJournalReader.o: Stream.h ChannelReader.h Channel.h
RecoveryJournalReader.o: SystemChapterReader.h ChapterReader.h Chapter.h
RecoveryJournalReader.o: binaryUtils.h
RecoveryJournalWriter.o: RecoveryJournalWriter.h RecoveryJournal.h
RecoveryJournalWriter.o: /usr/include/MidiShare.h types.h StreamWriter.h
RecoveryJournalWriter.o: Stream.h ChannelWriter.h Channel.h
RecoveryJournalWriter.o: SystemChapterWriter.h ChapterWriter.h Chapter.h
RecoveryJournalWriter.o: binaryUtils.h eventUtils.h
Stream.o: Stream.h types.h
StreamReader.o: StreamReader.h Stream.h types.h /usr/include/MidiShare.h
StreamReader.o: PayloadReader.h RecoveryJournalReader.h RecoveryJournal.h
StreamWriter.o: StreamWriter.h Stream.h types.h /usr/include/MidiShare.h
StreamWriter.o: CommandFilter.h PayloadWriter.h RecoveryJournalWriter.h
StreamWriter.o: RecoveryJournal.h eventUtils.h binaryUtils.h
SystemChapterReader.o: SystemChapterReader.h types.h ChapterReader.h
SystemChapterReader.o: Chapter.h /usr/include/MidiShare.h RecoveryJournal.h
SystemChapterReader.o: RecoveryJournalReader.h StreamReader.h Stream.h
SystemChapterWriter.o: SystemChapterWriter.h types.h ChapterWriter.h
SystemChapterWriter.o: Chapter.h /usr/include/MidiShare.h RecoveryJournal.h
SystemChapterWriter.o: binaryUtils.h RecoveryJournalWriter.h StreamWriter.h
SystemChapterWriter.o: Stream.h
