
TARGET = librtpmidi.so.1.0
LIB = /usr/lib/librtpmidi.so
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

install: $(LIB).1.0
	if [ ! -f $(LIB) ]; then ln -s $(LIB).1 $(LIB); fi
	if [ ! -f $(LIB).1 ]; then ln -s $(LIB).1.0 $(LIB).1; fi

$(LIB).1.0 : $(TARGET)
	cp $(TARGET) $(LIB).1.0

clean :
	rm -f $(OBJ) $(TARGET)*

depend:
	makedepend -- $(CXXFLAGS) -- $(SRC)

# DO NOT DELETE

Channel.o: Chapter.h types.h RecoveryJournal.h Channel.h
ChannelChapterReader.o: ChannelChapterReader.h types.h ChapterReader.h
ChannelChapterReader.o: Chapter.h RecoveryJournal.h ChannelReader.h Channel.h
ChannelChapterReader.o: RecoveryJournalReader.h StreamReader.h Stream.h
ChannelChapterWriter.o: ChannelChapterWriter.h types.h ChapterWriter.h
ChannelChapterWriter.o: Chapter.h RecoveryJournal.h binaryUtils.h
ChannelChapterWriter.o: ChannelWriter.h Channel.h RecoveryJournalWriter.h
ChannelChapterWriter.o: StreamWriter.h Stream.h printingUtils.h
ChannelReader.o: ChannelReader.h Channel.h types.h ChapterPReader.h
ChannelReader.o: ChannelChapterReader.h ChapterReader.h Chapter.h
ChannelReader.o: RecoveryJournal.h RecoveryJournalReader.h StreamReader.h
ChannelReader.o: Stream.h ChapterCReader.h ChapterWReader.h ChapterNReader.h
ChannelReader.o: ChapterTReader.h ChapterAReader.h binaryUtils.h
ChannelWriter.o: ChannelWriter.h Channel.h types.h ChapterPWriter.h
ChannelWriter.o: ChannelChapterWriter.h ChapterWriter.h Chapter.h
ChannelWriter.o: RecoveryJournal.h binaryUtils.h RecoveryJournalWriter.h
ChannelWriter.o: StreamWriter.h Stream.h ChapterCWriter.h ChapterWWriter.h
ChannelWriter.o: ChapterNWriter.h ChapterTWriter.h ChapterAWriter.h
ChannelWriter.o: eventUtils.h
Chapter.o: Chapter.h types.h RecoveryJournal.h
ChapterAReader.o: ChapterAReader.h ChannelChapterReader.h types.h
ChapterAReader.o: ChapterReader.h Chapter.h RecoveryJournal.h ChannelReader.h
ChapterAReader.o: Channel.h RecoveryJournalReader.h StreamReader.h Stream.h
ChapterAReader.o: eventUtils.h
ChapterAWriter.o: ChapterAWriter.h ChannelChapterWriter.h types.h
ChapterAWriter.o: ChapterWriter.h Chapter.h RecoveryJournal.h binaryUtils.h
ChapterAWriter.o: ChannelWriter.h Channel.h RecoveryJournalWriter.h
ChapterAWriter.o: StreamWriter.h Stream.h
ChapterCReader.o: ChapterCReader.h ChannelChapterReader.h types.h
ChapterCReader.o: ChapterReader.h Chapter.h RecoveryJournal.h ChannelReader.h
ChapterCReader.o: Channel.h RecoveryJournalReader.h StreamReader.h Stream.h
ChapterCReader.o: eventUtils.h
ChapterCWriter.o: ChapterCWriter.h ChannelChapterWriter.h types.h
ChapterCWriter.o: ChapterWriter.h Chapter.h RecoveryJournal.h binaryUtils.h
ChapterCWriter.o: ChannelWriter.h Channel.h RecoveryJournalWriter.h
ChapterCWriter.o: StreamWriter.h Stream.h
ChapterNReader.o: ChapterNReader.h ChannelChapterReader.h types.h
ChapterNReader.o: ChapterReader.h Chapter.h RecoveryJournal.h ChannelReader.h
ChapterNReader.o: Channel.h RecoveryJournalReader.h StreamReader.h Stream.h
ChapterNReader.o: printingUtils.h eventUtils.h
ChapterNWriter.o: ChapterNWriter.h ChannelChapterWriter.h types.h
ChapterNWriter.o: ChapterWriter.h Chapter.h RecoveryJournal.h binaryUtils.h
ChapterNWriter.o: ChannelWriter.h Channel.h RecoveryJournalWriter.h
ChapterNWriter.o: StreamWriter.h Stream.h printingUtils.h
ChapterPReader.o: ChapterPReader.h ChannelChapterReader.h types.h
ChapterPReader.o: ChapterReader.h Chapter.h RecoveryJournal.h ChannelReader.h
ChapterPReader.o: Channel.h RecoveryJournalReader.h StreamReader.h Stream.h
ChapterPReader.o: eventUtils.h
ChapterPWriter.o: ChapterPWriter.h ChannelChapterWriter.h types.h
ChapterPWriter.o: ChapterWriter.h Chapter.h RecoveryJournal.h binaryUtils.h
ChapterPWriter.o: ChannelWriter.h Channel.h RecoveryJournalWriter.h
ChapterPWriter.o: StreamWriter.h Stream.h printingUtils.h
ChapterReader.o: ChapterReader.h types.h Chapter.h RecoveryJournal.h
ChapterTReader.o: ChapterTReader.h ChannelChapterReader.h types.h
ChapterTReader.o: ChapterReader.h Chapter.h RecoveryJournal.h ChannelReader.h
ChapterTReader.o: Channel.h RecoveryJournalReader.h StreamReader.h Stream.h
ChapterTReader.o: eventUtils.h
ChapterTWriter.o: ChapterTWriter.h ChannelChapterWriter.h types.h
ChapterTWriter.o: ChapterWriter.h Chapter.h RecoveryJournal.h binaryUtils.h
ChapterTWriter.o: ChannelWriter.h Channel.h RecoveryJournalWriter.h
ChapterTWriter.o: StreamWriter.h Stream.h
ChapterWReader.o: ChapterWReader.h ChannelChapterReader.h types.h
ChapterWReader.o: ChapterReader.h Chapter.h RecoveryJournal.h ChannelReader.h
ChapterWReader.o: Channel.h RecoveryJournalReader.h StreamReader.h Stream.h
ChapterWReader.o: eventUtils.h
ChapterWWriter.o: ChapterWWriter.h ChannelChapterWriter.h types.h
ChapterWWriter.o: ChapterWriter.h Chapter.h RecoveryJournal.h binaryUtils.h
ChapterWWriter.o: ChannelWriter.h Channel.h RecoveryJournalWriter.h
ChapterWWriter.o: StreamWriter.h Stream.h
ChapterWriter.o: ChapterWriter.h types.h Chapter.h RecoveryJournal.h
ChapterWriter.o: binaryUtils.h /usr/include/string.h
ChapterWriter.o: /usr/include/machine/ansi.h /usr/include/ppc/ansi.h
ChapterWriter.o: /usr/include/sys/cdefs.h RecoveryJournalWriter.h
CommandFilter.o: CommandFilter.h types.h eventUtils.h binaryUtils.h
CommandFilter.o: printingUtils.h
PayloadReader.o: PayloadReader.h types.h deltaTime.h eventUtils.h
PayloadReader.o: binaryUtils.h /usr/include/stdlib.h
PayloadReader.o: /usr/include/machine/ansi.h /usr/include/ppc/ansi.h
PayloadReader.o: /usr/include/machine/types.h /usr/include/ppc/types.h
PayloadReader.o: /usr/include/sys/cdefs.h /usr/include/alloca.h
PayloadWriter.o: PayloadWriter.h types.h RecoveryJournalWriter.h
PayloadWriter.o: RecoveryJournal.h StreamWriter.h Stream.h deltaTime.h
PayloadWriter.o: eventUtils.h binaryUtils.h printingUtils.h
RecoveryJournal.o: RecoveryJournal.h types.h Chapter.h Channel.h
RecoveryJournalReader.o: RecoveryJournalReader.h RecoveryJournal.h types.h
RecoveryJournalReader.o: StreamReader.h Stream.h ChannelReader.h Channel.h
RecoveryJournalReader.o: SystemChapterReader.h ChapterReader.h Chapter.h
RecoveryJournalReader.o: binaryUtils.h
RecoveryJournalWriter.o: RecoveryJournalWriter.h RecoveryJournal.h types.h
RecoveryJournalWriter.o: StreamWriter.h Stream.h ChannelWriter.h Channel.h
RecoveryJournalWriter.o: SystemChapterWriter.h ChapterWriter.h Chapter.h
RecoveryJournalWriter.o: binaryUtils.h eventUtils.h
Stream.o: Stream.h types.h
StreamReader.o: StreamReader.h Stream.h types.h PayloadReader.h
StreamReader.o: RecoveryJournalReader.h RecoveryJournal.h
StreamWriter.o: StreamWriter.h Stream.h types.h CommandFilter.h
StreamWriter.o: PayloadWriter.h RecoveryJournalWriter.h RecoveryJournal.h
StreamWriter.o: eventUtils.h binaryUtils.h
SystemChapterReader.o: SystemChapterReader.h types.h ChapterReader.h
SystemChapterReader.o: Chapter.h RecoveryJournal.h RecoveryJournalReader.h
SystemChapterReader.o: StreamReader.h Stream.h
SystemChapterWriter.o: SystemChapterWriter.h types.h ChapterWriter.h
SystemChapterWriter.o: Chapter.h RecoveryJournal.h binaryUtils.h
SystemChapterWriter.o: RecoveryJournalWriter.h StreamWriter.h Stream.h
binaryUtils.o: binaryUtils.h types.h
deltaTime.o: deltaTime.h types.h
eventUtils.o: eventUtils.h types.h printingUtils.h
printingUtils.o: printingUtils.h types.h eventUtils.h binaryUtils.h
printingUtils.o: deltaTime.h
