
ScriptSet=MidiGetStat msQuit
PublicDir=/usr/local/bin
ManDir=/usr/local/man/man1


install : $(ScriptSet)
	cp $(ScriptSet) $(PublicDir)
	bzip2 -k -f man1/*.1
	mv -f man1/*.bz2 $(ManDir)
