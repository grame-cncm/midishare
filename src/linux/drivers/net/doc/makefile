MAN1=/usr/local/man/man1
Target = msInetDriver.1

install :
	[ -f $(Target).bz2 ] || bzip2 $(Target)
	install -d $(MAN1)
	install -m 644 -o root $(Target).bz2 $(MAN1)

uninstall :
	rm -f $(MAN1)/$(Target).bz2
