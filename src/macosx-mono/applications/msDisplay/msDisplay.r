data 'ALRT' (500, "Errors") {
	$"0064 0064 00B7 0169 01F4 0005"                      /* .d.d.·.i.ô.. */
};

data 'BNDL' (128, purgeable) {
	$"4469 7370 0000 0001 4943 4E23 0000 0000"            /* Disp....ICN#.... */
	$"0080 4652 4546 0000 0000 0080"                      /* .€FREF.....€ */
};

data 'DITL' (128, "Main") {
	$"0000 0000 0000 0004 0004 0119 0115 C002"            /* ..............À. */
	$"0080"                                               /* .€ */
};

data 'DITL' (129, "About", preload) {
	$"0000 0000 0000 FFFF FFF6 00B0 00A9 4002"            /* ......ÿÿÿö.°.©@. */
	$"0081"                                               /* . */
};

data 'DITL' (500, "Errors") {
	$"0002 0000 0000 0034 00BB 0049 00F9 0402"            /* .......4.».I.ù.. */
	$"4F4B 0000 0000 0009 0034 002A 00FF 881C"            /* OK.....Æ.4.*.ÿˆ. */
	$"496D 706F 7373 6962 6C65 2064 6520 6C61"            /* Impossible de la */
	$"6E63 6572 205E 303A 0D5E 312E 0000 0000"            /* ncer ^0:Â^1..... */
	$"0006 0008 0026 0028 A002 01F4"                      /* .....&.( ..ô */
};

data 'DLOG' (128, "Main") {
	$"003C 0014 015C 0132 0004 0000 0100 0000"            /* .<...\.2........ */
	$"0000 0080 096D 7344 6973 706C 6179"                 /* ...€ÆmsDisplay */
};

data 'DLOG' (129, "About") {
	$"003C 0022 00EC 00CA 0002 0100 0100 0000"            /* .<.".ì.Ê........ */
	$"0000 0081 00"                                       /* .... */
};

data 'Disp' (0) {
	$"0B56 6572 7369 6F6E 2031 2E35"                      /* .Version 1.5 */
};

data 'FREF' (128) {
	$"4150 504C 0000 00"                                  /* APPL... */
};

data 'ICN#' (128, "display") {
	$"FFFF FFFF 9000 0001 9000 0001 FFFF FFFF"            /* ÿÿÿÿ......ÿÿÿÿ */
	$"8000 0001 BFFF FFFD A000 0005 A5E7 CFE5"            /* €...¿ÿÿý ...¥çÏå */
	$"A000 0005 A6E6 4FA5 A000 0005 A765 CDE5"            /*  ...¦æO¥ ...§eÍå */
	$"A000 0005 A6E5 CFA5 A000 0005 A7E5 4DE5"            /*  ...¦åÏ¥ ...§åMå */
	$"A000 0005 A7A6 CEE5 A000 0005 A6E7 CF65"            /*  ...§¦Îå ...¦çÏe */
	$"A000 0005 A000 0005 A000 0005 A000 0005"            /*  ... ... ... ... */
	$"A000 0005 A000 0005 A000 0005 A000 0005"            /*  ... ... ... ... */
	$"A000 0005 BFFF FFFD 8000 0001 FFFF FFFF"            /*  ...¿ÿÿý€...ÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
};

data 'ICON' (128) {
	$"FFFF FFFF 9000 0001 9000 0001 FFFF FFFF"            /* ÿÿÿÿ......ÿÿÿÿ */
	$"8000 0001 BFFF FFFD A000 0005 A5E7 CFE5"            /* €...¿ÿÿý ...¥çÏå */
	$"A000 0005 A6E6 4FA5 A000 0005 A765 CDE5"            /*  ...¦æO¥ ...§eÍå */
	$"A000 0005 A6E5 CFA5 A000 0005 A7E5 4DE5"            /*  ...¦åÏ¥ ...§åMå */
	$"A000 0005 A7A6 CEE5 A000 0005 A6E7 CF65"            /*  ...§¦Îå ...¦çÏe */
	$"A000 0005 A000 0005 A000 0005 A000 0005"            /*  ... ... ... ... */
	$"A000 0005 A000 0005 A000 0005 A000 0005"            /*  ... ... ... ... */
	$"A000 0005 BFFF FFFD 8000 0001 FFFF FFFF"            /*  ...¿ÿÿý€...ÿÿÿÿ */
};

data 'ICON' (500) {
	$"FFFF FFFF 8000 0001 8003 0001 8005 F001"            /* ÿÿÿÿ€...€...€.ð. */
	$"801F 8F31 8363 80D9 8580 4379 8780 2C31"            /* €.1ƒc€Ù…€Cy‡€,1 */
	$"8300 30C1 8080 CD01 805B 1601 802C 1E01"            /* ƒ.0Á€€Í.€[..€,.. */
	$"803C 0C01 8018 0001 8000 0001 FFFF FFFF"            /* €<..€...€...ÿÿÿÿ */
	$"B9C4 739D B9C4 739D B9C4 739D B9C4 739D"            /* ¹Äs¹Äs¹Äs¹Äs */
	$"B9C4 739D B9C4 739D B9C4 739D B9C4 739D"            /* ¹Äs¹Äs¹Äs¹Äs */
	$"9084 2109 9084 2109 9084 2109 9084 2109"            /* „!Æ„!Æ„!Æ„!Æ */
	$"9084 2109 9084 2109 9084 2109 FFFF FFFF"            /* „!Æ„!Æ„!Æÿÿÿÿ */
};

data 'MENU' (128, "Apple", preload) {
	$"0080 0000 0000 0000 0000 FFFF FFFB 0114"            /* .€........ÿÿÿû.. */
	$"1641 2070 726F 706F 7320 6465 206D 7344"            /* .A propos de msD */
	$"6973 706C 6179 C900 0000 0001 2D00 0000"            /* isplayÉ.....-... */
	$"0000"                                               /* .. */
};

data 'MENU' (129, "File", preload) {
	$"0081 0000 0000 0000 0000 FFFF FFFF 0746"            /* .........ÿÿÿÿ.F */
	$"6963 6869 6572 0751 7569 7474 6572 0051"            /* ichier.Quitter.Q */
	$"0000 00"                                            /* ... */
};

data 'MENU' (130, "Edit", preload) {
	$"0082 0000 0000 0000 0000 FFFF FFFB 0445"            /* .‚........ÿÿÿû.E */
	$"6469 7407 416E 6E75 6C65 7200 5A00 0001"            /* dit.Annuler.Z... */
	$"2D00 0000 0006 436F 7570 6572 0058 0000"            /* -.....Couper.X.. */
	$"0643 6F70 6965 7200 4300 0006 436F 6C6C"            /* .Copier.C...Coll */
	$"6572 0056 0000 0745 6666 6163 6572 0000"            /* er.V...Effacer.. */
	$"0000 00"                                            /* ... */
};

data 'PICT' (128) {
	$"00A6 FFFF FFFF 0114 0110 1101 A000 82A0"            /* .¦ÿÿÿÿ...... .‚  */
	$"008C 0100 0AFF FFFF FF01 1401 1009 FFFF"            /* .Œ...ÿÿÿÿ....Æÿÿ */
	$"FFFF FFFF FFFF 3000 0D00 0101 1301 1020"            /* ÿÿÿÿÿÿ0.Â......  */
	$"000E 0002 000E 010F 2000 0F00 0200 0F01"            /* ........ ....... */
	$"0E20 000F 0002 0112 0002 2000 1000 0301"            /* . ........ ..... */
	$"1100 03A1 0096 000C 0100 0000 0200 0000"            /* ...¡.–.......... */
	$"0000 0000 0100 0A00 0000 0900 0B00 FD03"            /* ..........Æ...ý. */
	$"0004 0D00 092B 0A09 2368 683A 6D6E 3A73"            /* ..Â.Æ+.Æ#hh:mn:s */
	$"732E 6D6D 6D20 2020 502F 4320 2020 2020"            /* s.mmm   P/C      */
	$"2020 206D 6573 7361 6765 2020 A000 97A0"            /*    message   .—  */
	$"008D A000 83FF"                                     /* . .ƒÿ */
};

data 'PICT' (129) {
	$"5244 0000 0000 00B1 00B3 0011 02FF 0C00"            /* RD.....±.³...ÿ.. */
	$"FFFE 0000 0048 0000 0048 0000 0000 0000"            /* ÿþ...H...H...... */
	$"00B1 00B3 0000 0000 001E 0001 000A 0000"            /* .±.³............ */
	$"0000 00B1 00B3 009B 0000 00FF 82CC 0000"            /* ...±.³.›...ÿ‚Ì.. */
	$"0000 00B1 00B3 0000 0004 0000 0000 0048"            /* ...±.³.........H */
	$"0000 0048 0000 0010 0020 0003 0008 0000"            /* ...H..... ...... */
	$"0000 070D BF2C 0000 0000 0000 0000 00B1"            /* ...Â¿,.........± */
	$"00B3 0000 0000 00B1 00B3 0040 000A 0000"            /* .³.....±.³.@.... */
	$"0000 00B1 00B3 0013 81FF D0FF 0077 81FF"            /* ...±.³..ÿÐÿ.wÿ */
	$"CFFF 0077 81FF CFFF 0177 FF00 1681 FFD1"            /* Ïÿ.wÿÏÿ.wÿ..ÿÑ */
	$"FF01 BB77 81FF D0FF 01BB 7781 FFD0 FF02"            /* ÿ.»wÿÐÿ.»wÿÐÿ. */
	$"BB77 FF00 1981 FFD2 FF02 DDBB 7781 FFD1"            /* »wÿ..ÿÒÿ.Ý»wÿÑ */
	$"FF02 DDBB 7781 FFD1 FF03 DDBB 77FF 0019"            /* ÿ.Ý»wÿÑÿ.Ý»wÿ.. */
	$"81FF D2FF 02DD BB77 81FF D1FF 02DD BB77"            /* ÿÒÿ.Ý»wÿÑÿ.Ý»w */
	$"81FF D1FF 03DD BB77 FF00 1981 FFD2 FF02"            /* ÿÑÿ.Ý»wÿ..ÿÒÿ. */
	$"DDBB 7781 FFD1 FF02 DDBB 7781 FFD1 FF03"            /* Ý»wÿÑÿ.Ý»wÿÑÿ. */
	$"DDBB 77FF 0019 81FF D2FF 02DD BB77 81FF"            /* Ý»wÿ..ÿÒÿ.Ý»wÿ */
	$"D1FF 02DD BB77 81FF D1FF 03DD BB77 FF00"            /* Ñÿ.Ý»wÿÑÿ.Ý»wÿ. */
	$"1981 FFD2 FF02 DDBB 7781 FFD1 FF02 DDBB"            /* .ÿÒÿ.Ý»wÿÑÿ.Ý» */
	$"7781 FFD1 FF03 DDBB 77FF 0019 81FF D2FF"            /* wÿÑÿ.Ý»wÿ..ÿÒÿ */
	$"02DD BB77 81FF D1FF 02DD BB77 81FF D1FF"            /* .Ý»wÿÑÿ.Ý»wÿÑÿ */
	$"03DD BB77 FF00 1981 FFD2 FF02 DDBB 7781"            /* .Ý»wÿ..ÿÒÿ.Ý»w */
	$"FFD1 FF02 DDBB 7781 FFD1 FF03 DDBB 77FF"            /* ÿÑÿ.Ý»wÿÑÿ.Ý»wÿ */
	$"001F 83FF 0000 D1FF 02DD BB77 82FF 0000"            /* ..ƒÿ..Ñÿ.Ý»w‚ÿ.. */
	$"D1FF 02DD BB77 82FF 0000 D1FF 03DD BB77"            /* Ñÿ.Ý»w‚ÿ..Ñÿ.Ý»w */
	$"FF00 1F86 FFFD 00D1 FF02 DDBB 7785 FFFD"            /* ÿ..†ÿý.Ñÿ.Ý»w…ÿý */
	$"00D1 FF02 DDBB 7785 FFFD 00D1 FF03 DDBB"            /* .Ñÿ.Ý»w…ÿý.Ñÿ.Ý» */
	$"77FF 003A BCFF F800 F6FF FE00 E0FF 0100"            /* wÿ.:¼ÿø.öÿþ.àÿ.. */
	$"00D1 FF02 DDBB 77BB FFF8 00F6 FFFE 00E0"            /* .Ñÿ.Ý»w»ÿø.öÿþ.à */
	$"FF01 0000 D1FF 02DD BB77 BBFF F800 F6FF"            /* ÿ...Ñÿ.Ý»w»ÿø.öÿ */
	$"FE00 E0FF 0100 00D1 FF03 DDBB 77FF 0049"            /* þ.àÿ...Ñÿ.Ý»wÿ.I */
	$"BFFF FB00 FBFF 0100 00F8 FFFE 00E0 FF01"            /* ¿ÿû.ûÿ...øÿþ.àÿ. */
	$"0000 D1FF 02DD BB77 BEFF FB00 FBFF 0100"            /* ..Ñÿ.Ý»w¾ÿû.ûÿ.. */
	$"00F8 FFFE 00E0 FF01 0000 D1FF 02DD BB77"            /* .øÿþ.àÿ...Ñÿ.Ý»w */
	$"BEFF FB00 FBFF 0100 00F8 FFFE 00E0 FF01"            /* ¾ÿû.ûÿ...øÿþ.àÿ. */
	$"0000 D1FF 03DD BB77 FF00 4CBC FF01 0000"            /* ..Ñÿ.Ý»wÿ.L¼ÿ... */
	$"F8FF 0100 00FA FFFE 00E0 FF01 0000 D1FF"            /* øÿ...úÿþ.àÿ...Ñÿ */
	$"02DD BB77 BBFF 0100 00F8 FF01 0000 FAFF"            /* .Ý»w»ÿ...øÿ...úÿ */
	$"FE00 E0FF 0100 00D1 FF02 DDBB 77BB FF01"            /* þ.àÿ...Ñÿ.Ý»w»ÿ. */
	$"0000 F8FF 0100 00FA FFFE 00E0 FF01 0000"            /* ..øÿ...úÿþ.àÿ... */
	$"D1FF 03DD BB77 FF00 40BC FF01 0000 F7FF"            /* Ñÿ.Ý»wÿ.@¼ÿ...÷ÿ */
	$"0100 00D7 FF01 0000 D1FF 02DD BB77 BBFF"            /* ...×ÿ...Ñÿ.Ý»w»ÿ */
	$"0100 00F7 FF01 0000 D7FF 0100 00D1 FF02"            /* ...÷ÿ...×ÿ...Ñÿ. */
	$"DDBB 77BB FF01 0000 F7FF 0100 00D7 FF01"            /* Ý»w»ÿ...÷ÿ...×ÿ. */
	$"0000 D1FF 03DD BB77 FF00 40BC FF01 0000"            /* ..Ñÿ.Ý»wÿ.@¼ÿ... */
	$"F6FF 0100 00D8 FF01 0000 D1FF 02DD BB77"            /* öÿ...Øÿ...Ñÿ.Ý»w */
	$"BBFF 0100 00F6 FF01 0000 D8FF 0100 00D1"            /* »ÿ...öÿ...Øÿ...Ñ */
	$"FF02 DDBB 77BB FF01 0000 F6FF 0100 00D8"            /* ÿ.Ý»w»ÿ...öÿ...Ø */
	$"FF01 0000 D1FF 03DD BB77 FF00 40BC FF01"            /* ÿ...Ñÿ.Ý»wÿ.@¼ÿ. */
	$"0000 F6FF 0100 00D8 FF01 0000 D1FF 02DD"            /* ..öÿ...Øÿ...Ñÿ.Ý */
	$"BB77 BBFF 0100 00F6 FF01 0000 D8FF 0100"            /* »w»ÿ...öÿ...Øÿ.. */
	$"00D1 FF02 DDBB 77BB FF01 0000 F6FF 0100"            /* .Ñÿ.Ý»w»ÿ...öÿ.. */
	$"00D8 FF01 0000 D1FF 03DD BB77 FF00 40BC"            /* .Øÿ...Ñÿ.Ý»wÿ.@¼ */
	$"FF01 0000 F5FF 0100 00D9 FF01 0000 D1FF"            /* ÿ...õÿ...Ùÿ...Ñÿ */
	$"02DD BB77 BBFF 0100 00F5 FF01 0000 D9FF"            /* .Ý»w»ÿ...õÿ...Ùÿ */
	$"0100 00D1 FF02 DDBB 77BB FF01 0000 F5FF"            /* ...Ñÿ.Ý»w»ÿ...õÿ */
	$"0100 00D9 FF01 0000 D1FF 03DD BB77 FF00"            /* ...Ùÿ...Ñÿ.Ý»wÿ. */
	$"C4DF FF02 00FF FFFD 00FD FFFD 00F9 FFFC"            /* Äßÿ..ÿÿý.ýÿý.ùÿü */
	$"00FA FF01 0000 F5FF 0100 00FB FF00 00FC"            /* .úÿ...õÿ...ûÿ..ü */
	$"FFFC 00F9 FF00 00FE FFFD 00FA FF01 0000"            /* ÿü.ùÿ..þÿý.úÿ... */
	$"FAFF FC00 FEFF FB00 FEFF FC00 EEFF 02DD"            /* úÿü.þÿû.þÿü.îÿ.Ý */
	$"BB77 DEFF 0200 FFFF FD00 FDFF FD00 F9FF"            /* »wÞÿ..ÿÿý.ýÿý.ùÿ */
	$"FC00 FAFF 0100 00F5 FF01 0000 FBFF 0000"            /* ü.úÿ...õÿ...ûÿ.. */
	$"FCFF FC00 F9FF 0000 FEFF FD00 FAFF 0100"            /* üÿü.ùÿ..þÿý.úÿ.. */
	$"00FA FFFC 00FE FFFB 00FE FFFC 00EE FF02"            /* .úÿü.þÿû.þÿü.îÿ. */
	$"DDBB 77DE FF02 00FF FFFD 00FD FFFD 00F9"            /* Ý»wÞÿ..ÿÿý.ýÿý.ù */
	$"FFFC 00FA FF01 0000 F5FF 0100 00FB FF00"            /* ÿü.úÿ...õÿ...ûÿ. */
	$"00FC FFFC 00F9 FF00 00FE FFFD 00FA FF01"            /* .üÿü.ùÿ..þÿý.úÿ. */
	$"0000 FAFF FC00 FEFF FB00 FEFF FC00 EEFF"            /* ..úÿü.þÿû.þÿü.îÿ */
	$"03DD BB77 FF01 1BE0 FF03 0000 FF00 FEFF"            /* .Ý»wÿ..àÿ...ÿ.þÿ */
	$"0400 00FF FF00 FEFF 0100 00FB FF05 0000"            /* ...ÿÿ.þÿ...ûÿ... */
	$"FFFF 0000 FAFF 0100 00F4 FF01 0000 FDFF"            /* ÿÿ..úÿ...ôÿ...ýÿ */
	$"0100 00FD FF05 0000 FFFF 0000 FAFF 0400"            /* ...ýÿ...ÿÿ..úÿ.. */
	$"00FF 0000 FEFF FE00 FCFF 0100 00FC FF01"            /* .ÿ..þÿþ.üÿ...üÿ. */
	$"0000 FEFF FE00 FEFF FE00 FAFF 0100 00ED"            /* ..þÿþ.þÿþ.úÿ...í */
	$"FF02 DDBB 77DF FF03 0000 FF00 FEFF 0400"            /* ÿ.Ý»wßÿ...ÿ.þÿ.. */
	$"00FF FF00 FEFF 0100 00FB FF05 0000 FFFF"            /* .ÿÿ.þÿ...ûÿ...ÿÿ */
	$"0000 FAFF 0100 00F4 FF01 0000 FDFF 0100"            /* ..úÿ...ôÿ...ýÿ.. */
	$"00FD FF05 0000 FFFF 0000 FAFF 0400 00FF"            /* .ýÿ...ÿÿ..úÿ...ÿ */
	$"0000 FEFF FE00 FCFF 0100 00FC FF01 0000"            /* ..þÿþ.üÿ...üÿ... */
	$"FEFF FE00 FEFF FE00 FAFF 0100 00ED FF02"            /* þÿþ.þÿþ.úÿ...íÿ. */
	$"DDBB 77DF FF03 0000 FF00 FEFF 0400 00FF"            /* Ý»wßÿ...ÿ.þÿ...ÿ */
	$"FF00 FEFF 0100 00FB FF05 0000 FFFF 0000"            /* ÿ.þÿ...ûÿ...ÿÿ.. */
	$"FAFF 0100 00F4 FF01 0000 FDFF 0100 00FD"            /* úÿ...ôÿ...ýÿ...ý */
	$"FF05 0000 FFFF 0000 FAFF 0400 00FF 0000"            /* ÿ...ÿÿ..úÿ...ÿ.. */
	$"FEFF FE00 FCFF 0100 00FC FF01 0000 FEFF"            /* þÿþ.üÿ...üÿ...þÿ */
	$"FE00 FEFF FE00 FAFF 0100 00ED FF03 DDBB"            /* þ.þÿþ.úÿ...íÿ.Ý» */
	$"77FF 0103 E2FF FC00 FDFF 0300 00FF 00FD"            /* wÿ..âÿü.ýÿ...ÿ.ý */
	$"FF01 0000 FCFF 0100 00FD FF00 00FA FF01"            /* ÿ...üÿ...ýÿ..úÿ. */
	$"0000 F4FF 0300 00FF FFFD 00FE FF01 0000"            /* ..ôÿ...ÿÿý.þÿ... */
	$"FDFF 0000 FCFF FC00 FBFF FE00 FDFF 0100"            /* ýÿ..üÿü.ûÿþ.ýÿ.. */
	$"00FD FF01 0000 FCFF 0100 00FD FF01 0000"            /* .ýÿ...üÿ...ýÿ... */
	$"FAFF 0000 ECFF 02DD BB77 E1FF FC00 FDFF"            /* úÿ..ìÿ.Ý»wáÿü.ýÿ */
	$"0300 00FF 00FD FF01 0000 FCFF 0100 00FD"            /* ...ÿ.ýÿ...üÿ...ý */
	$"FF00 00FA FF01 0000 F4FF 0300 00FF FFFD"            /* ÿ..úÿ...ôÿ...ÿÿý */
	$"00FE FF01 0000 FDFF 0000 FCFF FC00 FBFF"            /* .þÿ...ýÿ..üÿü.ûÿ */
	$"FE00 FDFF 0100 00FD FF01 0000 FCFF 0100"            /* þ.ýÿ...ýÿ...üÿ.. */
	$"00FD FF01 0000 FAFF 0000 ECFF 02DD BB77"            /* .ýÿ...úÿ..ìÿ.Ý»w */
	$"E1FF FC00 FDFF 0300 00FF 00FD FF01 0000"            /* áÿü.ýÿ...ÿ.ýÿ... */
	$"FCFF 0100 00FD FF00 00FA FF01 0000 F4FF"            /* üÿ...ýÿ..úÿ...ôÿ */
	$"0300 00FF FFFD 00FE FF01 0000 FDFF 0000"            /* ...ÿÿý.þÿ...ýÿ.. */
	$"FCFF FC00 FBFF FE00 FDFF 0100 00FD FF01"            /* üÿü.ûÿþ.ýÿ...ýÿ. */
	$"0000 FCFF 0100 00FD FF01 0000 FAFF 0000"            /* ..üÿ...ýÿ...úÿ.. */
	$"ECFF 03DD BB77 FF01 09E0 FF01 0000 FBFF"            /* ìÿ.Ý»wÿ.Æàÿ...ûÿ */
	$"0100 00FB FF01 0000 FDFF 0100 00FD FF00"            /* ...ûÿ...ýÿ...ýÿ. */
	$"00FA FF01 0000 F4FF 0100 00FD FF01 0000"            /* .úÿ...ôÿ...ýÿ... */
	$"FEFF 0100 00FD FF00 00FA FF01 0000 F9FF"            /* þÿ...ýÿ..úÿ...ùÿ */
	$"0100 00FD FF01 0000 FEFF 0100 00FB FF01"            /* ...ýÿ...þÿ...ûÿ. */
	$"0000 FDFF 0100 00FB FF00 00EB FF02 DDBB"            /* ..ýÿ...ûÿ..ëÿ.Ý» */
	$"77DF FF01 0000 FBFF 0100 00FB FF01 0000"            /* wßÿ...ûÿ...ûÿ... */
	$"FDFF 0100 00FD FF00 00FA FF01 0000 F4FF"            /* ýÿ...ýÿ..úÿ...ôÿ */
	$"0100 00FD FF01 0000 FEFF 0100 00FD FF00"            /* ...ýÿ...þÿ...ýÿ. */
	$"00FA FF01 0000 F9FF 0100 00FD FF01 0000"            /* .úÿ...ùÿ...ýÿ... */
	$"FEFF 0100 00FB FF01 0000 FDFF 0100 00FB"            /* þÿ...ûÿ...ýÿ...û */
	$"FF00 00EB FF02 DDBB 77DF FF01 0000 FBFF"            /* ÿ..ëÿ.Ý»wßÿ...ûÿ */
	$"0100 00FB FF01 0000 FDFF 0100 00FD FF00"            /* ...ûÿ...ýÿ...ýÿ. */
	$"00FA FF01 0000 F4FF 0100 00FD FF01 0000"            /* .úÿ...ôÿ...ýÿ... */
	$"FEFF 0100 00FD FF00 00FA FF01 0000 F9FF"            /* þÿ...ýÿ..úÿ...ùÿ */
	$"0100 00FD FF01 0000 FEFF 0100 00FB FF01"            /* ...ýÿ...þÿ...ûÿ. */
	$"0000 FDFF 0100 00FB FF00 00EB FF03 DDBB"            /* ..ýÿ...ûÿ..ëÿ.Ý» */
	$"77FF 00EE E0FF 0100 00FB FF01 0000 FBFF"            /* wÿ.îàÿ...ûÿ...ûÿ */
	$"0100 00FD FF01 0000 F5FF 0100 00F4 FF01"            /* ...ýÿ...õÿ...ôÿ. */
	$"0000 FDFF 0100 00FE FF01 0000 F5FF 0100"            /* ..ýÿ...þÿ...õÿ.. */
	$"00F9 FFFE 00FE FF01 0000 FEFF 0100 00FB"            /* .ùÿþ.þÿ...þÿ...û */
	$"FF01 0000 FCFF 0100 00FC FF00 00EB FF02"            /* ÿ...üÿ...üÿ..ëÿ. */
	$"DDBB 77DF FF01 0000 FBFF 0100 00FB FF01"            /* Ý»wßÿ...ûÿ...ûÿ. */
	$"0000 FDFF 0100 00F5 FF01 0000 F4FF 0100"            /* ..ýÿ...õÿ...ôÿ.. */
	$"00FD FF01 0000 FEFF 0100 00F5 FF01 0000"            /* .ýÿ...þÿ...õÿ... */
	$"F9FF FE00 FEFF 0100 00FE FF01 0000 FBFF"            /* ùÿþ.þÿ...þÿ...ûÿ */
	$"0100 00FC FF01 0000 FCFF 0000 EBFF 02DD"            /* ...üÿ...üÿ..ëÿ.Ý */
	$"BB77 DFFF 0100 00FB FF01 0000 FBFF 0100"            /* »wßÿ...ûÿ...ûÿ.. */
	$"00FD FF01 0000 F5FF 0100 00F4 FF01 0000"            /* .ýÿ...õÿ...ôÿ... */
	$"FDFF 0100 00FE FF01 0000 F5FF 0100 00F9"            /* ýÿ...þÿ...õÿ...ù */
	$"FFFE 00FE FF01 0000 FEFF 0100 00FB FF01"            /* ÿþ.þÿ...þÿ...ûÿ. */
	$"0000 FCFF 0100 00FC FF00 00EB FF03 DDBB"            /* ..üÿ...üÿ..ëÿ.Ý» */
	$"77FF 00DC E0FF 0100 00FB FF01 0000 FBFF"            /* wÿ.Üàÿ...ûÿ...ûÿ */
	$"0100 00FD FFFE 00F6 FF01 0000 F4FF 0100"            /* ...ýÿþ.öÿ...ôÿ.. */
	$"00FD FF01 0000 FEFF FE00 F6FF 0100 00F8"            /* .ýÿ...þÿþ.öÿ...ø */
	$"FF01 0000 FEFF 0100 00F6 FF01 0000 FCFF"            /* ÿ...þÿ...öÿ...üÿ */
	$"0100 00FC FF00 00EB FF02 DDBB 77DF FF01"            /* ...üÿ..ëÿ.Ý»wßÿ. */
	$"0000 FBFF 0100 00FB FF01 0000 FDFF FE00"            /* ..ûÿ...ûÿ...ýÿþ. */
	$"F6FF 0100 00F4 FF01 0000 FDFF 0100 00FE"            /* öÿ...ôÿ...ýÿ...þ */
	$"FFFE 00F6 FF01 0000 F8FF 0100 00FE FF01"            /* ÿþ.öÿ...øÿ...þÿ. */
	$"0000 F6FF 0100 00FC FF01 0000 FCFF 0000"            /* ..öÿ...üÿ...üÿ.. */
	$"EBFF 02DD BB77 DFFF 0100 00FB FF01 0000"            /* ëÿ.Ý»wßÿ...ûÿ... */
	$"FBFF 0100 00FD FFFE 00F6 FF01 0000 F4FF"            /* ûÿ...ýÿþ.öÿ...ôÿ */
	$"0100 00FD FF01 0000 FEFF FE00 F6FF 0100"            /* ...ýÿ...þÿþ.öÿ.. */
	$"00F8 FF01 0000 FEFF 0100 00F6 FF01 0000"            /* .øÿ...þÿ...öÿ... */
	$"FCFF 0100 00FC FF00 00EB FF03 DDBB 77FF"            /* üÿ...üÿ..ëÿ.Ý»wÿ */
	$"00D9 E0FF 0100 00FB FF01 0000 FBFF 0100"            /* .Ùàÿ...ûÿ...ûÿ.. */
	$"00FC FFFD 00F8 FF01 0000 F4FF 0100 00FD"            /* .üÿý.øÿ...ôÿ...ý */
	$"FF01 0000 FDFF FD00 F8FF 0100 00F8 FF01"            /* ÿ...ýÿý.øÿ...øÿ. */
	$"0000 FEFF 0100 00F7 FFFE 00FC FF01 0000"            /* ..þÿ...÷ÿþ.üÿ... */
	$"FDFF 0000 EAFF 02DD BB77 DFFF 0100 00FB"            /* ýÿ..êÿ.Ý»wßÿ...û */
	$"FF01 0000 FBFF 0100 00FC FFFD 00F8 FF01"            /* ÿ...ûÿ...üÿý.øÿ. */
	$"0000 F4FF 0100 00FD FF01 0000 FDFF FD00"            /* ..ôÿ...ýÿ...ýÿý. */
	$"F8FF 0100 00F8 FF01 0000 FEFF 0100 00F7"            /* øÿ...øÿ...þÿ...÷ */
	$"FFFE 00FC FF01 0000 FDFF 0000 EAFF 02DD"            /* ÿþ.üÿ...ýÿ..êÿ.Ý */
	$"BB77 DFFF 0100 00FB FF01 0000 FBFF 0100"            /* »wßÿ...ûÿ...ûÿ.. */
	$"00FC FFFD 00F8 FF01 0000 F4FF 0100 00FD"            /* .üÿý.øÿ...ôÿ...ý */
	$"FF01 0000 FDFF FD00 F8FF 0100 00F8 FF01"            /* ÿ...ýÿý.øÿ...øÿ. */
	$"0000 FEFF 0100 00F7 FFFE 00FC FF01 0000"            /* ..þÿ...÷ÿþ.üÿ... */
	$"FDFF 0000 EAFF 03DD BB77 FF00 E5E0 FF01"            /* ýÿ..êÿ.Ý»wÿ.åàÿ. */
	$"0000 FBFF 0100 00FB FF01 0000 FBFF FD00"            /* ..ûÿ...ûÿ...ûÿý. */
	$"F9FF 0100 00F4 FF01 0000 FDFF 0100 00FC"            /* ùÿ...ôÿ...ýÿ...ü */
	$"FFFD 00F9 FF01 0000 F8FF 0100 00FE FF01"            /* ÿý.ùÿ...øÿ...þÿ. */
	$"0000 F9FF 0400 00FF 0000 FBFF 0100 00FE"            /* ..ùÿ...ÿ..ûÿ...þ */
	$"FF00 00EA FF02 DDBB 77DF FF01 0000 FBFF"            /* ÿ..êÿ.Ý»wßÿ...ûÿ */
	$"0100 00FB FF01 0000 FBFF FD00 F9FF 0100"            /* ...ûÿ...ûÿý.ùÿ.. */
	$"00F4 FF01 0000 FDFF 0100 00FC FFFD 00F9"            /* .ôÿ...ýÿ...üÿý.ù */
	$"FF01 0000 F8FF 0100 00FE FF01 0000 F9FF"            /* ÿ...øÿ...þÿ...ùÿ */
	$"0400 00FF 0000 FBFF 0100 00FE FF00 00EA"            /* ...ÿ..ûÿ...þÿ..ê */
	$"FF02 DDBB 77DF FF01 0000 FBFF 0100 00FB"            /* ÿ.Ý»wßÿ...ûÿ...û */
	$"FF01 0000 FBFF FD00 F9FF 0100 00F4 FF01"            /* ÿ...ûÿý.ùÿ...ôÿ. */
	$"0000 FDFF 0100 00FC FFFD 00F9 FF01 0000"            /* ..ýÿ...üÿý.ùÿ... */
	$"F8FF 0100 00FE FF01 0000 F9FF 0400 00FF"            /* øÿ...þÿ...ùÿ...ÿ */
	$"0000 FBFF 0100 00FE FF00 00EA FF03 DDBB"            /* ..ûÿ...þÿ..êÿ.Ý» */
	$"77FF 00E5 E0FF 0100 00FB FF01 0000 FBFF"            /* wÿ.åàÿ...ûÿ...ûÿ */
	$"0100 00F9 FFFE 00FA FF01 0000 F5FF FE00"            /* ...ùÿþ.úÿ...õÿþ. */
	$"FDFF 0100 00FA FFFE 00FA FF01 0000 F8FF"            /* ýÿ...úÿþ.úÿ...øÿ */
	$"0100 00FE FF01 0000 FAFF 0000 FEFF 0100"            /* ...þÿ...úÿ..þÿ.. */
	$"00FB FF01 0000 FEFF 0000 EAFF 02DD BB77"            /* .ûÿ...þÿ..êÿ.Ý»w */
	$"DFFF 0100 00FB FF01 0000 FBFF 0100 00F9"            /* ßÿ...ûÿ...ûÿ...ù */
	$"FFFE 00FA FF01 0000 F5FF FE00 FDFF 0100"            /* ÿþ.úÿ...õÿþ.ýÿ.. */
	$"00FA FFFE 00FA FF01 0000 F8FF 0100 00FE"            /* .úÿþ.úÿ...øÿ...þ */
	$"FF01 0000 FAFF 0000 FEFF 0100 00FB FF01"            /* ÿ...úÿ..þÿ...ûÿ. */
	$"0000 FEFF 0000 EAFF 02DD BB77 DFFF 0100"            /* ..þÿ..êÿ.Ý»wßÿ.. */
	$"00FB FF01 0000 FBFF 0100 00F9 FFFE 00FA"            /* .ûÿ...ûÿ...ùÿþ.ú */
	$"FF01 0000 F5FF FE00 FDFF 0100 00FA FFFE"            /* ÿ...õÿþ.ýÿ...úÿþ */
	$"00FA FF01 0000 F8FF 0100 00FE FF01 0000"            /* .úÿ...øÿ...þÿ... */
	$"FAFF 0000 FEFF 0100 00FB FF01 0000 FEFF"            /* úÿ..þÿ...ûÿ...þÿ */
	$"0000 EAFF 03DD BB77 FF00 E8E0 FF01 0000"            /* ..êÿ.Ý»wÿ.èàÿ... */
	$"FBFF 0100 00FB FF01 0000 F8FF FE00 FBFF"            /* ûÿ...ûÿ...øÿþ.ûÿ */
	$"0100 00F5 FF01 0000 FCFF 0100 00F9 FFFE"            /* ...õÿ...üÿ...ùÿþ */
	$"00FB FF01 0000 F8FF 0100 00FE FF01 0000"            /* .ûÿ...øÿ...þÿ... */
	$"FCFF 0100 00FD FF01 0000 FBFF 0400 00FF"            /* üÿ...ýÿ...ûÿ...ÿ */
	$"FF00 E9FF 02DD BB77 DFFF 0100 00FB FF01"            /* ÿ.éÿ.Ý»wßÿ...ûÿ. */
	$"0000 FBFF 0100 00F8 FFFE 00FB FF01 0000"            /* ..ûÿ...øÿþ.ûÿ... */
	$"F5FF 0100 00FC FF01 0000 F9FF FE00 FBFF"            /* õÿ...üÿ...ùÿþ.ûÿ */
	$"0100 00F8 FF01 0000 FEFF 0100 00FC FF01"            /* ...øÿ...þÿ...üÿ. */
	$"0000 FDFF 0100 00FB FF04 0000 FFFF 00E9"            /* ..ýÿ...ûÿ...ÿÿ.é */
	$"FF02 DDBB 77DF FF01 0000 FBFF 0100 00FB"            /* ÿ.Ý»wßÿ...ûÿ...û */
	$"FF01 0000 F8FF FE00 FBFF 0100 00F5 FF01"            /* ÿ...øÿþ.ûÿ...õÿ. */
	$"0000 FCFF 0100 00F9 FFFE 00FB FF01 0000"            /* ..üÿ...ùÿþ.ûÿ... */
	$"F8FF 0100 00FE FF01 0000 FCFF 0100 00FD"            /* øÿ...þÿ...üÿ...ý */
	$"FF01 0000 FBFF 0400 00FF FF00 E9FF 03DD"            /* ÿ...ûÿ...ÿÿ.éÿ.Ý */
	$"BB77 FF00 EBE0 FF01 0000 FBFF 0100 00FB"            /* »wÿ.ëàÿ...ûÿ...û */
	$"FF01 0000 F7FF 0100 00FB FF01 0000 F5FF"            /* ÿ...÷ÿ...ûÿ...õÿ */
	$"0100 00FC FF01 0000 F8FF 0100 00FB FF01"            /* ...üÿ...øÿ...ûÿ. */
	$"0000 F8FF 0100 00FE FF01 0000 FDFF 0100"            /* ..øÿ...þÿ...ýÿ.. */
	$"00FC FF01 0000 FBFF FE00 01FF 00E9 FF02"            /* .üÿ...ûÿþ..ÿ.éÿ. */
	$"DDBB 77DF FF01 0000 FBFF 0100 00FB FF01"            /* Ý»wßÿ...ûÿ...ûÿ. */
	$"0000 F7FF 0100 00FB FF01 0000 F5FF 0100"            /* ..÷ÿ...ûÿ...õÿ.. */
	$"00FC FF01 0000 F8FF 0100 00FB FF01 0000"            /* .üÿ...øÿ...ûÿ... */
	$"F8FF 0100 00FE FF01 0000 FDFF 0100 00FC"            /* øÿ...þÿ...ýÿ...ü */
	$"FF01 0000 FBFF FE00 01FF 00E9 FF02 DDBB"            /* ÿ...ûÿþ..ÿ.éÿ.Ý» */
	$"77DF FF01 0000 FBFF 0100 00FB FF01 0000"            /* wßÿ...ûÿ...ûÿ... */
	$"F7FF 0100 00FB FF01 0000 F5FF 0100 00FC"            /* ÷ÿ...ûÿ...õÿ...ü */
	$"FF01 0000 F8FF 0100 00FB FF01 0000 F8FF"            /* ÿ...øÿ...ûÿ...øÿ */
	$"0100 00FE FF01 0000 FDFF 0100 00FC FF01"            /* ...þÿ...ýÿ...üÿ. */
	$"0000 FBFF FE00 01FF 00E9 FF03 DDBB 77FF"            /* ..ûÿþ..ÿ.éÿ.Ý»wÿ */
	$"0100 E0FF 0100 00FB FF01 0000 FBFF 0100"            /* ..àÿ...ûÿ...ûÿ.. */
	$"00FE FF00 00FB FF01 0000 FBFF 0100 00F6"            /* .þÿ..ûÿ...ûÿ...ö */
	$"FF01 0000 FBFF 0400 00FF FF00 FBFF 0100"            /* ÿ...ûÿ...ÿÿ.ûÿ.. */
	$"00FB FF01 0000 F9FF 0100 00FD FF01 0000"            /* .ûÿ...ùÿ...ýÿ... */
	$"FEFF 0100 00FB FF01 0000 FAFF 0300 00FF"            /* þÿ...ûÿ...úÿ...ÿ */
	$"00E9 FF02 DDBB 77DF FF01 0000 FBFF 0100"            /* .éÿ.Ý»wßÿ...ûÿ.. */
	$"00FB FF01 0000 FEFF 0000 FBFF 0100 00FB"            /* .ûÿ...þÿ..ûÿ...û */
	$"FF01 0000 F6FF 0100 00FB FF04 0000 FFFF"            /* ÿ...öÿ...ûÿ...ÿÿ */
	$"00FB FF01 0000 FBFF 0100 00F9 FF01 0000"            /* .ûÿ...ûÿ...ùÿ... */
	$"FDFF 0100 00FE FF01 0000 FBFF 0100 00FA"            /* ýÿ...þÿ...ûÿ...ú */
	$"FF03 0000 FF00 E9FF 02DD BB77 DFFF 0100"            /* ÿ...ÿ.éÿ.Ý»wßÿ.. */
	$"00FB FF01 0000 FBFF 0100 00FE FF00 00FB"            /* .ûÿ...ûÿ...þÿ..û */
	$"FF01 0000 FBFF 0100 00F6 FF01 0000 FBFF"            /* ÿ...ûÿ...öÿ...ûÿ */
	$"0400 00FF FF00 FBFF 0100 00FB FF01 0000"            /* ...ÿÿ.ûÿ...ûÿ... */
	$"F9FF 0100 00FD FF01 0000 FEFF 0100 00FB"            /* ùÿ...ýÿ...þÿ...û */
	$"FF01 0000 FAFF 0300 00FF 00E9 FF03 DDBB"            /* ÿ...úÿ...ÿ.éÿ.Ý» */
	$"77FF 00F4 E0FF 0100 00FB FF01 0000 FBFF"            /* wÿ.ôàÿ...ûÿ...ûÿ */
	$"0100 00FE FF00 00FB FF01 0000 FBFF 0100"            /* ...þÿ..ûÿ...ûÿ.. */
	$"00F7 FFFE 00FB FF04 0000 FFFF 00FB FF01"            /* .÷ÿþ.ûÿ...ÿÿ.ûÿ. */
	$"0000 FBFF 0100 00F9 FF01 0000 FDFF 0100"            /* ..ûÿ...ùÿ...ýÿ.. */
	$"00FE FF01 0000 FBFF 0100 00FA FFFE 00E8"            /* .þÿ...ûÿ...úÿþ.è */
	$"FF02 DDBB 77DF FF01 0000 FBFF 0100 00FB"            /* ÿ.Ý»wßÿ...ûÿ...û */
	$"FF01 0000 FEFF 0000 FBFF 0100 00FB FF01"            /* ÿ...þÿ..ûÿ...ûÿ. */
	$"0000 F7FF FE00 FBFF 0400 00FF FF00 FBFF"            /* ..÷ÿþ.ûÿ...ÿÿ.ûÿ */
	$"0100 00FB FF01 0000 F9FF 0100 00FD FF01"            /* ...ûÿ...ùÿ...ýÿ. */
	$"0000 FEFF 0100 00FB FF01 0000 FAFF FE00"            /* ..þÿ...ûÿ...úÿþ. */
	$"E8FF 02DD BB77 DFFF 0100 00FB FF01 0000"            /* èÿ.Ý»wßÿ...ûÿ... */
	$"FBFF 0100 00FE FF00 00FB FF01 0000 FBFF"            /* ûÿ...þÿ..ûÿ...ûÿ */
	$"0100 00F7 FFFE 00FB FF04 0000 FFFF 00FB"            /* ...÷ÿþ.ûÿ...ÿÿ.û */
	$"FF01 0000 FBFF 0100 00F9 FF01 0000 FDFF"            /* ÿ...ûÿ...ùÿ...ýÿ */
	$"0100 00FE FF01 0000 FBFF 0100 00FA FFFE"            /* ...þÿ...ûÿ...úÿþ */
	$"00E8 FF03 DDBB 77FF 00EB E0FF 0100 00FB"            /* .èÿ.Ý»wÿ.ëàÿ...û */
	$"FF01 0000 FBFF 0100 00FE FF00 00FB FF00"            /* ÿ...ûÿ...þÿ..ûÿ. */
	$"00FA FF01 0000 F8FF FE00 FAFF 0400 00FF"            /* .úÿ...øÿþ.úÿ...ÿ */
	$"FF00 FBFF 0000 FAFF 0100 00FA FF01 0000"            /* ÿ.ûÿ..úÿ...úÿ... */
	$"FCFF 0100 00FE FF01 0000 FCFF FE00 FAFF"            /* üÿ...þÿ...üÿþ.úÿ */
	$"FE00 E8FF 02DD BB77 DFFF 0100 00FB FF01"            /* þ.èÿ.Ý»wßÿ...ûÿ. */
	$"0000 FBFF 0100 00FE FF00 00FB FF00 00FA"            /* ..ûÿ...þÿ..ûÿ..ú */
	$"FF01 0000 F8FF FE00 FAFF 0400 00FF FF00"            /* ÿ...øÿþ.úÿ...ÿÿ. */
	$"FBFF 0000 FAFF 0100 00FA FF01 0000 FCFF"            /* ûÿ..úÿ...úÿ...üÿ */
	$"0100 00FE FF01 0000 FCFF FE00 FAFF FE00"            /* ...þÿ...üÿþ.úÿþ. */
	$"E8FF 02DD BB77 DFFF 0100 00FB FF01 0000"            /* èÿ.Ý»wßÿ...ûÿ... */
	$"FBFF 0100 00FE FF00 00FB FF00 00FA FF01"            /* ûÿ...þÿ..ûÿ..úÿ. */
	$"0000 F8FF FE00 FAFF 0400 00FF FF00 FBFF"            /* ..øÿþ.úÿ...ÿÿ.ûÿ */
	$"0000 FAFF 0100 00FA FF01 0000 FCFF 0100"            /* ..úÿ...úÿ...üÿ.. */
	$"00FE FF01 0000 FCFF FE00 FAFF FE00 E8FF"            /* .þÿ...üÿþ.úÿþ.èÿ */
	$"03DD BB77 FF01 03E0 FF01 0000 FBFF 0100"            /* .Ý»wÿ..àÿ...ûÿ.. */
	$"00FB FF01 0000 FEFF 0100 00FD FF01 0000"            /* .ûÿ...þÿ...ýÿ... */
	$"FAFF FE00 FBFF FE00 F8FF 0500 00FF FF00"            /* úÿþ.ûÿþ.øÿ...ÿÿ. */
	$"00FD FF01 0000 FAFF FD00 FDFF 0100 00FB"            /* .ýÿ...úÿý.ýÿ...û */
	$"FF01 0000 FEFF FE00 FEFF 0100 FFFE 0001"            /* ÿ...þÿþ.þÿ..ÿþ.. */
	$"FF00 FCFF 0100 00E8 FF02 DDBB 77DF FF01"            /* ÿ.üÿ...èÿ.Ý»wßÿ. */
	$"0000 FBFF 0100 00FB FF01 0000 FEFF 0100"            /* ..ûÿ...ûÿ...þÿ.. */
	$"00FD FF01 0000 FAFF FE00 FBFF FE00 F8FF"            /* .ýÿ...úÿþ.ûÿþ.øÿ */
	$"0500 00FF FF00 00FD FF01 0000 FAFF FD00"            /* ...ÿÿ..ýÿ...úÿý. */
	$"FDFF 0100 00FB FF01 0000 FEFF FE00 FEFF"            /* ýÿ...ûÿ...þÿþ.þÿ */
	$"0100 FFFE 0001 FF00 FCFF 0100 00E8 FF02"            /* ..ÿþ..ÿ.üÿ...èÿ. */
	$"DDBB 77DF FF01 0000 FBFF 0100 00FB FF01"            /* Ý»wßÿ...ûÿ...ûÿ. */
	$"0000 FEFF 0100 00FD FF01 0000 FAFF FE00"            /* ..þÿ...ýÿ...úÿþ. */
	$"FBFF FE00 F8FF 0500 00FF FF00 00FD FF01"            /* ûÿþ.øÿ...ÿÿ..ýÿ. */
	$"0000 FAFF FD00 FDFF 0100 00FB FF01 0000"            /* ..úÿý.ýÿ...ûÿ... */
	$"FEFF FE00 FEFF 0100 FFFE 0001 FF00 FCFF"            /* þÿþ.þÿ..ÿþ..ÿ.üÿ */
	$"0100 00E8 FF03 DDBB 77FF 00BE E2FF FB00"            /* ...èÿ.Ý»wÿ.¾âÿû. */
	$"01FF FFFB 0001 FFFF FB00 01FF FFFC 00FB"            /* .ÿÿû..ÿÿû..ÿÿü.û */
	$"FFF4 00F8 FFFB 0000 FFFC 00F8 FF03 0000"            /* ÿô.øÿû..ÿü.øÿ... */
	$"FFFF FC00 FCFF FB00 01FF FFFC 00FE FFFE"            /* ÿÿü.üÿû..ÿÿü.þÿþ */
	$"00FB FF00 00E7 FF02 DDBB 77E1 FFFB 0001"            /* .ûÿ..çÿ.Ý»wáÿû.. */
	$"FFFF FB00 01FF FFFB 0001 FFFF FC00 FBFF"            /* ÿÿû..ÿÿû..ÿÿü.ûÿ */
	$"F400 F8FF FB00 00FF FC00 F8FF 0300 00FF"            /* ô.øÿû..ÿü.øÿ...ÿ */
	$"FFFC 00FC FFFB 0001 FFFF FC00 FEFF FE00"            /* ÿü.üÿû..ÿÿü.þÿþ. */
	$"FBFF 0000 E7FF 02DD BB77 E1FF FB00 01FF"            /* ûÿ..çÿ.Ý»wáÿû..ÿ */
	$"FFFB 0001 FFFF FB00 01FF FFFC 00FB FFF4"            /* ÿû..ÿÿû..ÿÿü.ûÿô */
	$"00F8 FFFB 0000 FFFC 00F8 FF03 0000 FFFF"            /* .øÿû..ÿü.øÿ...ÿÿ */
	$"FC00 FCFF FB00 01FF FFFC 00FE FFFE 00FB"            /* ü.üÿû..ÿÿü.þÿþ.û */
	$"FF00 00E7 FF03 DDBB 77FF 002E 94FF 0100"            /* ÿ..çÿ.Ý»wÿ..”ÿ.. */
	$"00DC FF00 00E7 FF02 DDBB 7793 FF01 0000"            /* .Üÿ..çÿ.Ý»w“ÿ... */
	$"DCFF 0000 E7FF 02DD BB77 93FF 0100 00DC"            /* Üÿ..çÿ.Ý»w“ÿ...Ü */
	$"FF00 00E7 FF03 DDBB 77FF 002E 94FF 0100"            /* ÿ..çÿ.Ý»wÿ..”ÿ.. */
	$"00DC FF00 00E7 FF02 DDBB 7793 FF01 0000"            /* .Üÿ..çÿ.Ý»w“ÿ... */
	$"DCFF 0000 E7FF 02DD BB77 93FF 0100 00DC"            /* Üÿ..çÿ.Ý»w“ÿ...Ü */
	$"FF00 00E7 FF03 DDBB 77FF 002E 94FF 0100"            /* ÿ..çÿ.Ý»wÿ..”ÿ.. */
	$"00DD FF00 00E6 FF02 DDBB 7793 FF01 0000"            /* .Ýÿ..æÿ.Ý»w“ÿ... */
	$"DDFF 0000 E6FF 02DD BB77 93FF 0100 00DD"            /* Ýÿ..æÿ.Ý»w“ÿ...Ý */
	$"FF00 00E6 FF03 DDBB 77FF 003D A5FF 0146"            /* ÿ..æÿ.Ý»wÿ.=¥ÿ.F */
	$"46F2 FF01 0000 DDFF 0000 E6FF 02DD BB77"            /* Fòÿ...Ýÿ..æÿ.Ý»w */
	$"A4FF 0100 00F2 FF01 0000 DDFF 0000 E6FF"            /* ¤ÿ...òÿ...Ýÿ..æÿ */
	$"02DD BB77 A4FF 01A5 A5F2 FF01 0000 DDFF"            /* .Ý»w¤ÿ.¥¥òÿ...Ýÿ */
	$"0000 E6FF 03DD BB77 FF00 47A6 FF03 46FF"            /* ..æÿ.Ý»wÿ.G¦ÿ.Fÿ */
	$"4646 FE00 F6FF 0100 00DD FF00 00E6 FF02"            /* FFþ.öÿ...Ýÿ..æÿ. */
	$"DDBB 77A5 FF01 00FF FC00 F6FF 0100 00DD"            /* Ý»w¥ÿ..ÿü.öÿ...Ý */
	$"FF00 00E6 FF02 DDBB 77A5 FF03 A5FF A5A5"            /* ÿ..æÿ.Ý»w¥ÿ.¥ÿ¥¥ */
	$"FE00 F6FF 0100 00DD FF00 00E6 FF03 DDBB"            /* þ.öÿ...Ýÿ..æÿ.Ý» */
	$"77FF 005E A8FF 0100 00FD 46FE FFFD 0003"            /* wÿ.^¨ÿ...ýFþÿý.. */
	$"FFFF 1F1F FEFF 0100 00DF FF01 0000 E5FF"            /* ÿÿ..þÿ...ßÿ...åÿ */
	$"02DD BB77 A7FF FB00 FEFF FD00 03FF FFB7"            /* .Ý»w§ÿû.þÿý..ÿÿ· */
	$"B7FE FF01 0000 DFFF 0100 00E5 FF02 DDBB"            /* ·þÿ...ßÿ...åÿ.Ý» */
	$"77A7 FF01 0000 FDA5 FEFF FD00 03FF FF14"            /* w§ÿ...ý¥þÿý..ÿÿ. */
	$"14FE FF01 0000 DFFF 0100 00E5 FF03 DDBB"            /* .þÿ...ßÿ...åÿ.Ý» */
	$"77FF 006B ADFF 04DD DDFF 0000 FEFF 0246"            /* wÿ.k­ÿ.ÝÝÿ..þÿ.F */
	$"4600 FAFF 0800 1FFF 1F1F FFFF 0000 E0FF"            /* F.úÿ...ÿ..ÿÿ..àÿ */
	$"FE00 E5FF 02DD BB77 ACFF 0408 08FF 0000"            /* þ.åÿ.Ý»w¬ÿ...ÿ.. */
	$"FEFF FE00 FAFF 0800 B7FF B7B7 FFFF 0000"            /* þÿþ.úÿ..·ÿ··ÿÿ.. */
	$"E0FF FE00 E5FF 02DD BB77 ACFF 0406 06FF"            /* àÿþ.åÿ.Ý»w¬ÿ...ÿ */
	$"0000 FEFF 02A5 A500 FAFF 0800 14FF 1414"            /* ..þÿ.¥¥.úÿ...ÿ.. */
	$"FFFF 0000 E0FF FE00 E5FF 03DD BB77 FF00"            /* ÿÿ..àÿþ.åÿ.Ý»wÿ. */
	$"5EAE FF03 DDFF DDDD F9FF 0000 FDFF 0200"            /* ^®ÿ.ÝÿÝÝùÿ..ýÿ.. */
	$"00FF FD1F FB00 E2FF FE00 E5FF 02DD BB77"            /* .ÿý.û.âÿþ.åÿ.Ý»w */
	$"ADFF 0308 FF08 08F9 FF00 00FD FF02 0000"            /* ­ÿ..ÿ..ùÿ..ýÿ... */
	$"FFFD B7FB 00E2 FFFE 00E5 FF02 DDBB 77AD"            /* ÿý·û.âÿþ.åÿ.Ý»w­ */
	$"FF03 06FF 0606 F9FF 0000 FDFF 0200 00FF"            /* ÿ..ÿ..ùÿ..ýÿ...ÿ */
	$"FD14 FB00 E2FF FE00 E5FF 03DD BB77 FF00"            /* ý.û.âÿþ.åÿ.Ý»wÿ. */
	$"43AE FFFD DDF8 FF03 00FF 0000 FDFF 011F"            /* C®ÿýÝøÿ..ÿ..ýÿ.. */
	$"1FBC FF02 DDBB 77AD FFFD 08F8 FF03 00FF"            /* .¼ÿ.Ý»w­ÿý.øÿ..ÿ */
	$"0000 FDFF 01B7 B7BC FF02 DDBB 77AD FFFD"            /* ..ýÿ.··¼ÿ.Ý»w­ÿý */
	$"06F8 FF03 00FF 0000 FDFF 0114 14BC FF03"            /* .øÿ..ÿ..ýÿ...¼ÿ. */
	$"DDBB 77FF 0040 ADFF 01DD DDF7 FF01 0000"            /* Ý»wÿ.@­ÿ.ÝÝ÷ÿ... */
	$"FDFF 0100 00BA FF02 DDBB 77AC FF01 0808"            /* ýÿ...ºÿ.Ý»w¬ÿ... */
	$"F7FF 0100 00FD FF01 0000 BAFF 02DD BB77"            /* ÷ÿ...ýÿ...ºÿ.Ý»w */
	$"ACFF 0106 06F7 FF01 0000 FDFF 0100 00BA"            /* ¬ÿ...÷ÿ...ýÿ...º */
	$"FF03 DDBB 77FF 003E ABFF 0000 FAFF 0100"            /* ÿ.Ý»wÿ.>«ÿ..úÿ.. */
	$"00FC FF00 00B8 FF02 DDBB 77AA FF00 00FA"            /* .üÿ..¸ÿ.Ý»wªÿ..ú */
	$"FF07 0000 FFFF 6464 FF00 B8FF 02DD BB77"            /* ÿ...ÿÿddÿ.¸ÿ.Ý»w */
	$"AAFF 0000 FAFF 0700 00FF FF02 02FF 00B8"            /* ªÿ..úÿ...ÿÿ..ÿ.¸ */
	$"FF03 DDBB 77FF 003F AAFF 0600 FF00 00FF"            /* ÿ.Ý»wÿ.?ªÿ..ÿ..ÿ */
	$"0000 B0FF 02DD BB77 A9FF 0600 FF00 00FF"            /* ..°ÿ.Ý»w©ÿ..ÿ..ÿ */
	$"0000 FEFF 0364 FF64 64B7 FF02 DDBB 77A9"            /* ..þÿ.dÿdd·ÿ.Ý»w© */
	$"FF06 00FF D4D4 FF00 00FE FF03 02FF 0202"            /* ÿ..ÿÔÔÿ..þÿ..ÿ.. */
	$"B7FF 03DD BB77 FF00 30A9 FF03 00FF 0000"            /* ·ÿ.Ý»wÿ.0©ÿ..ÿ.. */
	$"AEFF 02DD BB77 A8FF 0300 FF00 00FC FFFD"            /* ®ÿ.Ý»w¨ÿ..ÿ..üÿý */
	$"64B7 FF02 DDBB 77A8 FF03 D4FF D4D4 FCFF"            /* d·ÿ.Ý»w¨ÿ.ÔÿÔÔüÿ */
	$"FD02 B7FF 03DD BB77 FF00 29A9 FFFD 00AE"            /* ý.·ÿ.Ý»wÿ.)©ÿý.® */
	$"FF02 DDBB 77A8 FFFD 00FB FF01 6464 B6FF"            /* ÿ.Ý»w¨ÿý.ûÿ.dd¶ÿ */
	$"02DD BB77 A8FF FDD4 FBFF 0102 02B6 FF03"            /* .Ý»w¨ÿýÔûÿ...¶ÿ. */
	$"DDBB 77FF 0022 A8FF 0100 00AD FF02 DDBB"            /* Ý»wÿ."¨ÿ...­ÿ.Ý» */
	$"77A7 FF01 0000 ADFF 02DD BB77 A7FF 01D4"            /* w§ÿ...­ÿ.Ý»w§ÿ.Ô */
	$"D4AD FF03 DDBB 77FF 0019 81FF D2FF 02DD"            /* Ô­ÿ.Ý»wÿ..ÿÒÿ.Ý */
	$"BB77 81FF D1FF 02DD BB77 81FF D1FF 03DD"            /* »wÿÑÿ.Ý»wÿÑÿ.Ý */
	$"BB77 FF00 1981 FFD2 FF02 DDBB 7781 FFD1"            /* »wÿ..ÿÒÿ.Ý»wÿÑ */
	$"FF02 DDBB 7781 FFD1 FF03 DDBB 77FF 0019"            /* ÿ.Ý»wÿÑÿ.Ý»wÿ.. */
	$"81FF D2FF 02DD BB77 81FF D1FF 02DD BB77"            /* ÿÒÿ.Ý»wÿÑÿ.Ý»w */
	$"81FF D1FF 03DD BB77 FF00 4FC8 FF01 0000"            /* ÿÑÿ.Ý»wÿ.OÈÿ... */
	$"F8FF 0100 00BC FF01 0000 FCFF 0100 00E6"            /* øÿ...¼ÿ...üÿ...æ */
	$"FF02 DDBB 77C7 FF01 0000 F8FF 0100 00BC"            /* ÿ.Ý»wÇÿ...øÿ...¼ */
	$"FF01 0000 FCFF 0100 00E6 FF02 DDBB 77C7"            /* ÿ...üÿ...æÿ.Ý»wÇ */
	$"FF01 0000 F8FF 0100 00BC FF01 0000 FCFF"            /* ÿ...øÿ...¼ÿ...üÿ */
	$"0100 00E6 FF03 DDBB 77FF 007C DAFF FE00"            /* ...æÿ.Ý»wÿ.|Úÿþ. */
	$"FCFF 0300 00FF 00FA FF02 00FF 00FE FFFD"            /* üÿ...ÿ.úÿ..ÿ.þÿý */
	$"0001 FF00 BEFF 0300 FFFF 00FB FF00 00E6"            /* ..ÿ.¾ÿ..ÿÿ.ûÿ..æ */
	$"FF02 DDBB 77D9 FFFE 00FC FF03 0000 FF00"            /* ÿ.Ý»wÙÿþ.üÿ...ÿ. */
	$"FAFF 0200 FF00 FEFF FD00 01FF 00BE FF03"            /* úÿ..ÿ.þÿý..ÿ.¾ÿ. */
	$"00FF FF00 FBFF 0000 E6FF 02DD BB77 D9FF"            /* .ÿÿ.ûÿ..æÿ.Ý»wÙÿ */
	$"FE00 FCFF 0300 00FF 00FA FF02 00FF 00FE"            /* þ.üÿ...ÿ.úÿ..ÿ.þ */
	$"FFFD 0001 FF00 BEFF 0300 FFFF 00FB FF00"            /* ÿý..ÿ.¾ÿ..ÿÿ.ûÿ. */
	$"00E6 FF03 DDBB 77FF 0094 D9FF 0100 00FD"            /* .æÿ.Ý»wÿ.”Ùÿ...ý */
	$"FF04 0000 FFFF 00FA FF05 00FF 00FF FF00"            /* ÿ...ÿÿ.úÿ..ÿ.ÿÿ. */
	$"FEFF 0200 FF00 C2FF 0000 FEFF 0300 FFFF"            /* þÿ..ÿ.Âÿ..þÿ..ÿÿ */
	$"00FB FF00 00E6 FF02 DDBB 77D8 FF01 0000"            /* .ûÿ..æÿ.Ý»wØÿ... */
	$"FDFF 0400 00FF FF00 FAFF 0500 FF00 FFFF"            /* ýÿ...ÿÿ.úÿ..ÿ.ÿÿ */
	$"00FE FF02 00FF 00C2 FF00 00FE FF03 00FF"            /* .þÿ..ÿ.Âÿ..þÿ..ÿ */
	$"FF00 FBFF 0000 E6FF 02DD BB77 D8FF 0100"            /* ÿ.ûÿ..æÿ.Ý»wØÿ.. */
	$"00FD FF04 0000 FFFF 00FA FF05 00FF 00FF"            /* .ýÿ...ÿÿ.úÿ..ÿ.ÿ */
	$"FF00 FEFF 0200 FF00 C2FF 0000 FEFF 0300"            /* ÿ.þÿ..ÿ.Âÿ..þÿ.. */
	$"FFFF 00FB FF00 00E6 FF03 DDBB 77FF 007C"            /* ÿÿ.ûÿ..æÿ.Ý»wÿ.| */
	$"D9FF 0200 FF00 FEFF 0100 00F7 FF00 00FD"            /* Ùÿ..ÿ.þÿ...÷ÿ..ý */
	$"FF00 00FC FF00 00C2 FF00 00FB FF00 00FB"            /* ÿ..üÿ..Âÿ..ûÿ..û */
	$"FF00 00E6 FF02 DDBB 77D8 FF02 00FF 00FE"            /* ÿ..æÿ.Ý»wØÿ..ÿ.þ */
	$"FF01 0000 F7FF 0000 FDFF 0000 FCFF 0000"            /* ÿ...÷ÿ..ýÿ..üÿ.. */
	$"C2FF 0000 FBFF 0000 FBFF 0000 E6FF 02DD"            /* Âÿ..ûÿ..ûÿ..æÿ.Ý */
	$"BB77 D8FF 0200 FF00 FEFF 0100 00F7 FF00"            /* »wØÿ..ÿ.þÿ...÷ÿ. */
	$"00FD FF00 00FC FF00 00C2 FF00 00FB FF00"            /* .ýÿ..üÿ..Âÿ..ûÿ. */
	$"00FB FF00 00E6 FF03 DDBB 77FF 013C E2FF"            /* .ûÿ..æÿ.Ý»wÿ.<âÿ */
	$"FD00 FCFF 0A00 FF00 FFFF 00FF 00FF 0000"            /* ý.üÿ..ÿ.ÿÿ.ÿ.ÿ.. */
	$"FEFF FA00 FEFF 0000 FDFF 0300 FF00 00FE"            /* þÿú.þÿ..ýÿ..ÿ..þ */
	$"FFFD 0007 FF00 00FF 0000 FFFF FE00 FBFF"            /* ÿý..ÿ..ÿ..ÿÿþ.ûÿ */
	$"FD00 01FF FFFD 000E FF00 00FF 0000 FFFF"            /* ý..ÿÿý..ÿ..ÿ..ÿÿ */
	$"0000 FFFF 0000 FFFE 0001 FFFF FD00 00FF"            /* ..ÿÿ..ÿþ..ÿÿý..ÿ */
	$"FB00 03FF FF00 FFFE 0002 FFFF 00FE FFFE"            /* û..ÿÿ.ÿþ..ÿÿ.þÿþ */
	$"00EC FF02 DDBB 77E1 FFFD 00FC FF0A 00FF"            /* .ìÿ.Ý»wáÿý.üÿ..ÿ */
	$"00FF FF00 FF00 FF00 00FE FFFA 00FE FF00"            /* .ÿÿ.ÿ.ÿ..þÿú.þÿ. */
	$"00FD FF03 00FF 0000 FEFF FD00 07FF 0000"            /* .ýÿ..ÿ..þÿý..ÿ.. */
	$"FF00 00FF FFFE 00FB FFFD 0001 FFFF FD00"            /* ÿ..ÿÿþ.ûÿý..ÿÿý. */
	$"0EFF 0000 FF00 00FF FF00 00FF FF00 00FF"            /* .ÿ..ÿ..ÿÿ..ÿÿ..ÿ */
	$"FE00 01FF FFFD 0000 FFFB 0003 FFFF 00FF"            /* þ..ÿÿý..ÿû..ÿÿ.ÿ */
	$"FE00 02FF FF00 FEFF FE00 ECFF 02DD BB77"            /* þ..ÿÿ.þÿþ.ìÿ.Ý»w */
	$"E1FF FD00 FCFF 0A00 FF00 FFFF 00FF 00FF"            /* áÿý.üÿ..ÿ.ÿÿ.ÿ.ÿ */
	$"0000 FEFF FA00 FEFF 0000 FDFF 0300 FF00"            /* ..þÿú.þÿ..ýÿ..ÿ. */
	$"00FE FFFD 0007 FF00 00FF 0000 FFFF FE00"            /* .þÿý..ÿ..ÿ..ÿÿþ. */
	$"FBFF FD00 01FF FFFD 000E FF00 00FF 0000"            /* ûÿý..ÿÿý..ÿ..ÿ.. */
	$"FFFF 0000 FFFF 0000 FFFE 0001 FFFF FD00"            /* ÿÿ..ÿÿ..ÿþ..ÿÿý. */
	$"00FF FB00 03FF FF00 FFFE 0002 FFFF 00FE"            /* .ÿû..ÿÿ.ÿþ..ÿÿ.þ */
	$"FFFE 00EC FF03 DDBB 77FF 016C DFFF 0000"            /* ÿþ.ìÿ.Ý»wÿ.lßÿ.. */
	$"FCFF 0D00 FF00 FFFF 00FF 00FF FF00 FFFF"            /* üÿÂ.ÿ.ÿÿ.ÿ.ÿÿ.ÿÿ */
	$"00FD FF02 00FF 00FD FF00 00FE FF04 0000"            /* .ýÿ..ÿ.ýÿ..þÿ... */
	$"FFFF 00FC FF08 00FF FF00 00FF 00FF 00FE"            /* ÿÿ.üÿ..ÿÿ..ÿ.ÿ.þ */
	$"FF00 00FD FF00 00FE FF02 00FF 00FD FF0E"            /* ÿ..ýÿ..þÿ..ÿ.ýÿ. */
	$"00FF 0000 FFFF 0000 FFFF 00FF FF00 00FE"            /* .ÿ..ÿÿ..ÿÿ.ÿÿ..þ */
	$"FF00 00FD FF03 00FF FF00 FEFF 0400 FFFF"            /* ÿ..ýÿ..ÿÿ.þÿ..ÿÿ */
	$"0000 FEFF 0500 FF00 FFFF 00FE FF00 00ED"            /* ..þÿ..ÿ.ÿÿ.þÿ..í */
	$"FF02 DDBB 77DE FF00 00FC FF0D 00FF 00FF"            /* ÿ.Ý»wÞÿ..üÿÂ.ÿ.ÿ */
	$"FF00 FF00 FFFF 00FF FF00 FDFF 0200 FF00"            /* ÿ.ÿ.ÿÿ.ÿÿ.ýÿ..ÿ. */
	$"FDFF 0000 FEFF 0400 00FF FF00 FCFF 0800"            /* ýÿ..þÿ...ÿÿ.üÿ.. */
	$"FFFF 0000 FF00 FF00 FEFF 0000 FDFF 0000"            /* ÿÿ..ÿ.ÿ.þÿ..ýÿ.. */
	$"FEFF 0200 FF00 FDFF 0E00 FF00 00FF FF00"            /* þÿ..ÿ.ýÿ..ÿ..ÿÿ. */
	$"00FF FF00 FFFF 0000 FEFF 0000 FDFF 0300"            /* .ÿÿ.ÿÿ..þÿ..ýÿ.. */
	$"FFFF 00FE FF04 00FF FF00 00FE FF05 00FF"            /* ÿÿ.þÿ..ÿÿ..þÿ..ÿ */
	$"00FF FF00 FEFF 0000 EDFF 02DD BB77 DEFF"            /* .ÿÿ.þÿ..íÿ.Ý»wÞÿ */
	$"0000 FCFF 0D00 FF00 FFFF 00FF 00FF FF00"            /* ..üÿÂ.ÿ.ÿÿ.ÿ.ÿÿ. */
	$"FFFF 00FD FF02 00FF 00FD FF00 00FE FF04"            /* ÿÿ.ýÿ..ÿ.ýÿ..þÿ. */
	$"0000 FFFF 00FC FF08 00FF FF00 00FF 00FF"            /* ..ÿÿ.üÿ..ÿÿ..ÿ.ÿ */
	$"00FE FF00 00FD FF00 00FE FF02 00FF 00FD"            /* .þÿ..ýÿ..þÿ..ÿ.ý */
	$"FF0E 00FF 0000 FFFF 0000 FFFF 00FF FF00"            /* ÿ..ÿ..ÿÿ..ÿÿ.ÿÿ. */
	$"00FE FF00 00FD FF03 00FF FF00 FEFF 0400"            /* .þÿ..ýÿ..ÿÿ.þÿ.. */
	$"FFFF 0000 FEFF 0500 FF00 FFFF 00FE FF00"            /* ÿÿ..þÿ..ÿ.ÿÿ.þÿ. */
	$"00ED FF03 DDBB 77FF 014E E0FF 0100 00FC"            /* .íÿ.Ý»wÿ.Nàÿ...ü */
	$"FF0D 00FF FF00 FF00 FF00 FFFF 00FF FF00"            /* ÿÂ.ÿÿ.ÿ.ÿ.ÿÿ.ÿÿ. */
	$"FDFF 0200 FF00 FCFF 0300 FFFF 00FE FF00"            /* ýÿ..ÿ.üÿ..ÿÿ.þÿ. */
	$"00FD FF04 0000 FFFF 00FD FFFC 00FD FF00"            /* .ýÿ...ÿÿ.ýÿü.ýÿ. */
	$"00FC FF00 00FD FF02 00FF 00FE FF00 00FE"            /* .üÿ..ýÿ..ÿ.þÿ..þ */
	$"FF03 00FF FF00 FDFF 0000 FEFF 0400 00FF"            /* ÿ..ÿÿ.ýÿ..þÿ...ÿ */
	$"FF00 FEFF 0300 FFFF 00FD FF04 00FF 00FF"            /* ÿ.þÿ..ÿÿ.ýÿ..ÿ.ÿ */
	$"FFFC 00ED FF02 DDBB 77DF FF01 0000 FCFF"            /* ÿü.íÿ.Ý»wßÿ...üÿ */
	$"0D00 FFFF 00FF 00FF 00FF FF00 FFFF 00FD"            /* Â.ÿÿ.ÿ.ÿ.ÿÿ.ÿÿ.ý */
	$"FF02 00FF 00FC FF03 00FF FF00 FEFF 0000"            /* ÿ..ÿ.üÿ..ÿÿ.þÿ.. */
	$"FDFF 0400 00FF FF00 FDFF FC00 FDFF 0000"            /* ýÿ...ÿÿ.ýÿü.ýÿ.. */
	$"FCFF 0000 FDFF 0200 FF00 FEFF 0000 FEFF"            /* üÿ..ýÿ..ÿ.þÿ..þÿ */
	$"0300 FFFF 00FD FF00 00FE FF04 0000 FFFF"            /* ..ÿÿ.ýÿ..þÿ...ÿÿ */
	$"00FE FF03 00FF FF00 FDFF 0400 FF00 FFFF"            /* .þÿ..ÿÿ.ýÿ..ÿ.ÿÿ */
	$"FC00 EDFF 02DD BB77 DFFF 0100 00FC FF0D"            /* ü.íÿ.Ý»wßÿ...üÿÂ */
	$"00FF FF00 FF00 FF00 FFFF 00FF FF00 FDFF"            /* .ÿÿ.ÿ.ÿ.ÿÿ.ÿÿ.ýÿ */
	$"0200 FF00 FCFF 0300 FFFF 00FE FF00 00FD"            /* ..ÿ.üÿ..ÿÿ.þÿ..ý */
	$"FF04 0000 FFFF 00FD FFFC 00FD FF00 00FC"            /* ÿ...ÿÿ.ýÿü.ýÿ..ü */
	$"FF00 00FD FF02 00FF 00FE FF00 00FE FF03"            /* ÿ..ýÿ..ÿ.þÿ..þÿ. */
	$"00FF FF00 FDFF 0000 FEFF 0400 00FF FF00"            /* .ÿÿ.ýÿ..þÿ...ÿÿ. */
	$"FEFF 0300 FFFF 00FD FF04 00FF 00FF FFFC"            /* þÿ..ÿÿ.ýÿ..ÿ.ÿÿü */
	$"00ED FF03 DDBB 77FF 014E E1FF 0200 FF00"            /* .íÿ.Ý»wÿ.Náÿ..ÿ. */
	$"FCFF 0D00 FFFF 0000 FFFF 00FF FF00 FFFF"            /* üÿÂ.ÿÿ..ÿÿ.ÿÿ.ÿÿ */
	$"00FD FF02 00FF 00FB FF02 00FF 00FE FF00"            /* .ýÿ..ÿ.ûÿ..ÿ.þÿ. */
	$"00FE FF05 00FF 00FF FF00 FDFF 0000 F9FF"            /* .þÿ..ÿ.ÿÿ.ýÿ..ùÿ */
	$"0000 FCFF 0000 FDFF 0200 FF00 FEFF 0000"            /* ..üÿ..ýÿ..ÿ.þÿ.. */
	$"FEFF 0300 FFFF 00FD FF08 00FF FF00 FF00"            /* þÿ..ÿÿ.ýÿ..ÿÿ.ÿ. */
	$"FFFF 00FE FF03 00FF FF00 FDFF 0500 FF00"            /* ÿÿ.þÿ..ÿÿ.ýÿ..ÿ. */
	$"FFFF 00E9 FF02 DDBB 77E0 FF02 00FF 00FC"            /* ÿÿ.éÿ.Ý»wàÿ..ÿ.ü */
	$"FF0D 00FF FF00 00FF FF00 FFFF 00FF FF00"            /* ÿÂ.ÿÿ..ÿÿ.ÿÿ.ÿÿ. */
	$"FDFF 0200 FF00 FBFF 0200 FF00 FEFF 0000"            /* ýÿ..ÿ.ûÿ..ÿ.þÿ.. */
	$"FEFF 0500 FF00 FFFF 00FD FF00 00F9 FF00"            /* þÿ..ÿ.ÿÿ.ýÿ..ùÿ. */
	$"00FC FF00 00FD FF02 00FF 00FE FF00 00FE"            /* .üÿ..ýÿ..ÿ.þÿ..þ */
	$"FF03 00FF FF00 FDFF 0800 FFFF 00FF 00FF"            /* ÿ..ÿÿ.ýÿ..ÿÿ.ÿ.ÿ */
	$"FF00 FEFF 0300 FFFF 00FD FF05 00FF 00FF"            /* ÿ.þÿ..ÿÿ.ýÿ..ÿ.ÿ */
	$"FF00 E9FF 02DD BB77 E0FF 0200 FF00 FCFF"            /* ÿ.éÿ.Ý»wàÿ..ÿ.üÿ */
	$"0D00 FFFF 0000 FFFF 00FF FF00 FFFF 00FD"            /* Â.ÿÿ..ÿÿ.ÿÿ.ÿÿ.ý */
	$"FF02 00FF 00FB FF02 00FF 00FE FF00 00FE"            /* ÿ..ÿ.ûÿ..ÿ.þÿ..þ */
	$"FF05 00FF 00FF FF00 FDFF 0000 F9FF 0000"            /* ÿ..ÿ.ÿÿ.ýÿ..ùÿ.. */
	$"FCFF 0000 FDFF 0200 FF00 FEFF 0000 FEFF"            /* üÿ..ýÿ..ÿ.þÿ..þÿ */
	$"0300 FFFF 00FD FF08 00FF FF00 FF00 FFFF"            /* ..ÿÿ.ýÿ..ÿÿ.ÿ.ÿÿ */
	$"00FE FF03 00FF FF00 FDFF 0500 FF00 FFFF"            /* .þÿ..ÿÿ.ýÿ..ÿ.ÿÿ */
	$"00E9 FF03 DDBB 77FF 015A E2FF 0300 FFFF"            /* .éÿ.Ý»wÿ.Zâÿ..ÿÿ */
	$"00FC FF00 00FE FF09 00FF FF00 FFFF 00FF"            /* .üÿ..þÿÆ.ÿÿ.ÿÿ.ÿ */
	$"FF00 FDFF 0500 FF00 FFFF 00FE FF02 00FF"            /* ÿ.ýÿ..ÿ.ÿÿ.þÿ..ÿ */
	$"00FE FF09 00FF FF00 FFFF 00FF FF00 FDFF"            /* .þÿÆ.ÿÿ.ÿÿ.ÿÿ.ýÿ */
	$"0000 F9FF 0000 FCFF 0000 FDFF 0200 FF00"            /* ..ùÿ..üÿ..ýÿ..ÿ. */
	$"FEFF 0000 FEFF 0300 FFFF 00FD FF08 00FF"            /* þÿ..þÿ..ÿÿ.ýÿ..ÿ */
	$"00FF FF00 FFFF 00FE FF03 00FF FF00 FDFF"            /* .ÿÿ.ÿÿ.þÿ..ÿÿ.ýÿ */
	$"0500 FF00 FFFF 00E9 FF02 DDBB 77E1 FF03"            /* ..ÿ.ÿÿ.éÿ.Ý»wáÿ. */
	$"00FF FF00 FCFF 0000 FEFF 0900 FFFF 00FF"            /* .ÿÿ.üÿ..þÿÆ.ÿÿ.ÿ */
	$"FF00 FFFF 00FD FF05 00FF 00FF FF00 FEFF"            /* ÿ.ÿÿ.ýÿ..ÿ.ÿÿ.þÿ */
	$"0200 FF00 FEFF 0900 FFFF 00FF FF00 FFFF"            /* ..ÿ.þÿÆ.ÿÿ.ÿÿ.ÿÿ */
	$"00FD FF00 00F9 FF00 00FC FF00 00FD FF02"            /* .ýÿ..ùÿ..üÿ..ýÿ. */
	$"00FF 00FE FF00 00FE FF03 00FF FF00 FDFF"            /* .ÿ.þÿ..þÿ..ÿÿ.ýÿ */
	$"0800 FF00 FFFF 00FF FF00 FEFF 0300 FFFF"            /* ..ÿ.ÿÿ.ÿÿ.þÿ..ÿÿ */
	$"00FD FF05 00FF 00FF FF00 E9FF 02DD BB77"            /* .ýÿ..ÿ.ÿÿ.éÿ.Ý»w */
	$"E1FF 0300 FFFF 00FC FF00 00FE FF09 00FF"            /* áÿ..ÿÿ.üÿ..þÿÆ.ÿ */
	$"FF00 FFFF 00FF FF00 FDFF 0500 FF00 FFFF"            /* ÿ.ÿÿ.ÿÿ.ýÿ..ÿ.ÿÿ */
	$"00FE FF02 00FF 00FE FF09 00FF FF00 FFFF"            /* .þÿ..ÿ.þÿÆ.ÿÿ.ÿÿ */
	$"00FF FF00 FDFF 0000 F9FF 0000 FCFF 0000"            /* .ÿÿ.ýÿ..ùÿ..üÿ.. */
	$"FDFF 0200 FF00 FEFF 0000 FEFF 0300 FFFF"            /* ýÿ..ÿ.þÿ..þÿ..ÿÿ */
	$"00FD FF08 00FF 00FF FF00 FFFF 00FE FF03"            /* .ýÿ..ÿ.ÿÿ.ÿÿ.þÿ. */
	$"00FF FF00 FDFF 0500 FF00 FFFF 00E9 FF03"            /* .ÿÿ.ýÿ..ÿ.ÿÿ.éÿ. */
	$"DDBB 77FF 016C E2FF 0300 FF00 00FC FF00"            /* Ý»wÿ.lâÿ..ÿ..üÿ. */
	$"00FB FF06 00FF FF00 FFFF 00FE FF06 0000"            /* .ûÿ..ÿÿ.ÿÿ.þÿ... */
	$"FF00 FFFF 00FE FF02 00FF 00FE FF09 00FF"            /* ÿ.ÿÿ.þÿ..ÿ.þÿÆ.ÿ */
	$"FF00 FF00 00FF FF00 FDFF 0000 FEFF 0000"            /* ÿ.ÿ..ÿÿ.ýÿ..þÿ.. */
	$"FDFF 0000 FCFF 0000 FDFF 0200 FF00 FEFF"            /* ýÿ..üÿ..ýÿ..ÿ.þÿ */
	$"0000 FEFF 0300 FFFF 00FD FF08 00FF 00FF"            /* ..þÿ..ÿÿ.ýÿ..ÿ.ÿ */
	$"0000 FFFF 00FE FF03 00FF FF00 FDFF 0500"            /* ..ÿÿ.þÿ..ÿÿ.ýÿ.. */
	$"FF00 FFFF 00FE FF00 00ED FF02 DDBB 77E1"            /* ÿ.ÿÿ.þÿ..íÿ.Ý»wá */
	$"FF03 00FF 0000 FCFF 0000 FBFF 0600 FFFF"            /* ÿ..ÿ..üÿ..ûÿ..ÿÿ */
	$"00FF FF00 FEFF 0600 00FF 00FF FF00 FEFF"            /* .ÿÿ.þÿ...ÿ.ÿÿ.þÿ */
	$"0200 FF00 FEFF 0900 FFFF 00FF 0000 FFFF"            /* ..ÿ.þÿÆ.ÿÿ.ÿ..ÿÿ */
	$"00FD FF00 00FE FF00 00FD FF00 00FC FF00"            /* .ýÿ..þÿ..ýÿ..üÿ. */
	$"00FD FF02 00FF 00FE FF00 00FE FF03 00FF"            /* .ýÿ..ÿ.þÿ..þÿ..ÿ */
	$"FF00 FDFF 0800 FF00 FF00 00FF FF00 FEFF"            /* ÿ.ýÿ..ÿ.ÿ..ÿÿ.þÿ */
	$"0300 FFFF 00FD FF05 00FF 00FF FF00 FEFF"            /* ..ÿÿ.ýÿ..ÿ.ÿÿ.þÿ */
	$"0000 EDFF 02DD BB77 E1FF 0300 FF00 00FC"            /* ..íÿ.Ý»wáÿ..ÿ..ü */
	$"FF00 00FB FF06 00FF FF00 FFFF 00FE FF06"            /* ÿ..ûÿ..ÿÿ.ÿÿ.þÿ. */
	$"0000 FF00 FFFF 00FE FF02 00FF 00FE FF09"            /* ..ÿ.ÿÿ.þÿ..ÿ.þÿÆ */
	$"00FF FF00 FF00 00FF FF00 FDFF 0000 FEFF"            /* .ÿÿ.ÿ..ÿÿ.ýÿ..þÿ */
	$"0000 FDFF 0000 FCFF 0000 FDFF 0200 FF00"            /* ..ýÿ..üÿ..ýÿ..ÿ. */
	$"FEFF 0000 FEFF 0300 FFFF 00FD FF08 00FF"            /* þÿ..þÿ..ÿÿ.ýÿ..ÿ */
	$"00FF 0000 FFFF 00FE FF03 00FF FF00 FDFF"            /* .ÿ..ÿÿ.þÿ..ÿÿ.ýÿ */
	$"0500 FF00 FFFF 00FE FF00 00ED FF03 DDBB"            /* ..ÿ.ÿÿ.þÿ..íÿ.Ý» */
	$"77FF 0136 E2FF 0400 00FF 0000 FEFF FE00"            /* wÿ.6âÿ...ÿ..þÿþ. */
	$"FDFF FB00 01FF FFFE 0000 FFFD 0000 FFFD"            /* ýÿû..ÿÿþ..ÿý..ÿý */
	$"0000 FFFE 0000 FFFE 0003 FF00 00FF FC00"            /* ..ÿþ..ÿþ..ÿ..ÿü. */
	$"FDFF FE00 FBFF FD00 01FF FFFD 0000 FFFE"            /* ýÿþ.ûÿý..ÿÿý..ÿþ */
	$"0000 FFFE 0000 FFFE 0000 FFFC 0007 FFFF"            /* ..ÿþ..ÿþ..ÿü..ÿÿ */
	$"0000 FF00 00FF FB00 00FF FC00 00FF FE00"            /* ..ÿ..ÿû..ÿü..ÿþ. */
	$"01FF FFFE 00EC FF02 DDBB 77E1 FF04 0000"            /* .ÿÿþ.ìÿ.Ý»wáÿ... */
	$"FF00 00FE FFFE 00FD FFFB 0001 FFFF FE00"            /* ÿ..þÿþ.ýÿû..ÿÿþ. */
	$"00FF FD00 00FF FD00 00FF FE00 00FF FE00"            /* .ÿý..ÿý..ÿþ..ÿþ. */
	$"03FF 0000 FFFC 00FD FFFE 00FB FFFD 0001"            /* .ÿ..ÿü.ýÿþ.ûÿý.. */
	$"FFFF FD00 00FF FE00 00FF FE00 00FF FE00"            /* ÿÿý..ÿþ..ÿþ..ÿþ. */
	$"00FF FC00 07FF FF00 00FF 0000 FFFB 0000"            /* .ÿü..ÿÿ..ÿ..ÿû.. */
	$"FFFC 0000 FFFE 0001 FFFF FE00 ECFF 02DD"            /* ÿü..ÿþ..ÿÿþ.ìÿ.Ý */
	$"BB77 E1FF 0400 00FF 0000 FEFF FE00 FDFF"            /* »wáÿ...ÿ..þÿþ.ýÿ */
	$"FB00 01FF FFFE 0000 FFFD 0000 FFFD 0000"            /* û..ÿÿþ..ÿý..ÿý.. */
	$"FFFE 0000 FFFE 0003 FF00 00FF FC00 FDFF"            /* ÿþ..ÿþ..ÿ..ÿü.ýÿ */
	$"FE00 FBFF FD00 01FF FFFD 0000 FFFE 0000"            /* þ.ûÿý..ÿÿý..ÿþ.. */
	$"FFFE 0000 FFFE 0000 FFFC 0007 FFFF 0000"            /* ÿþ..ÿþ..ÿü..ÿÿ.. */
	$"FF00 00FF FB00 00FF FC00 00FF FE00 01FF"            /* ÿ..ÿû..ÿü..ÿþ..ÿ */
	$"FFFE 00EC FF03 DDBB 77FF 001F 89FF 0000"            /* ÿþ.ìÿ.Ý»wÿ..‰ÿ.. */
	$"CBFF 02DD BB77 88FF 0000 CBFF 02DD BB77"            /* Ëÿ.Ý»wˆÿ..Ëÿ.Ý»w */
	$"88FF 0000 CBFF 03DD BB77 FF00 1F89 FF00"            /* ˆÿ..Ëÿ.Ý»wÿ..‰ÿ. */
	$"00CB FF02 DDBB 7788 FF00 00CB FF02 DDBB"            /* .Ëÿ.Ý»wˆÿ..Ëÿ.Ý» */
	$"7788 FF00 00CB FF03 DDBB 77FF 001F 89FF"            /* wˆÿ..Ëÿ.Ý»wÿ..‰ÿ */
	$"0000 CBFF 02DD BB77 88FF 0000 CBFF 02DD"            /* ..Ëÿ.Ý»wˆÿ..Ëÿ.Ý */
	$"BB77 88FF 0000 CBFF 03DD BB77 FF00 1F8A"            /* »wˆÿ..Ëÿ.Ý»wÿ..Š */
	$"FFFE 00CC FF02 DDBB 7789 FFFE 00CC FF02"            /* ÿþ.Ìÿ.Ý»w‰ÿþ.Ìÿ. */
	$"DDBB 7789 FFFE 00CC FF03 DDBB 77FF 0019"            /* Ý»w‰ÿþ.Ìÿ.Ý»wÿ.. */
	$"81FF D2FF 02DD BB77 81FF D1FF 02DD BB77"            /* ÿÒÿ.Ý»wÿÑÿ.Ý»w */
	$"81FF D1FF 03DD BB77 FF00 1981 FFD2 FF02"            /* ÿÑÿ.Ý»wÿ..ÿÒÿ. */
	$"DDBB 7781 FFD1 FF02 DDBB 7781 FFD1 FF03"            /* Ý»wÿÑÿ.Ý»wÿÑÿ. */
	$"DDBB 77FF 0019 81FF D2FF 02DD BB77 81FF"            /* Ý»wÿ..ÿÒÿ.Ý»wÿ */
	$"D1FF 02DD BB77 81FF D1FF 03DD BB77 FF00"            /* Ñÿ.Ý»wÿÑÿ.Ý»wÿ. */
	$"1FBA FFFE 009C FF02 DDBB 77B9 FFFE 009C"            /* .ºÿþ.œÿ.Ý»w¹ÿþ.œ */
	$"FF02 DDBB 77B9 FFFE 009C FF03 DDBB 77FF"            /* ÿ.Ý»w¹ÿþ.œÿ.Ý»wÿ */
	$"001F BBFF 0000 99FF 02DD BB77 BAFF 0000"            /* ..»ÿ..™ÿ.Ý»wºÿ.. */
	$"99FF 02DD BB77 BAFF 0000 99FF 03DD BB77"            /* ™ÿ.Ý»wºÿ..™ÿ.Ý»w */
	$"FF00 1FBB FF00 0099 FF02 DDBB 77BA FF00"            /* ÿ..»ÿ..™ÿ.Ý»wºÿ. */
	$"0099 FF02 DDBB 77BA FF00 0099 FF03 DDBB"            /* .™ÿ.Ý»wºÿ..™ÿ.Ý» */
	$"77FF 001F BBFF 0000 99FF 02DD BB77 BAFF"            /* wÿ..»ÿ..™ÿ.Ý»wºÿ */
	$"0000 99FF 02DD BB77 BAFF 0000 99FF 03DD"            /* ..™ÿ.Ý»wºÿ..™ÿ.Ý */
	$"BB77 FF00 85BC FFFB 0004 FF00 00FF FFFE"            /* »wÿ.…¼ÿû..ÿ..ÿÿþ */
	$"00FE FFFE 0000 FFFB 0003 FF00 00FF FD00"            /* .þÿþ..ÿû..ÿ..ÿý. */
	$"07FF 0000 FF00 00FF FFFE 00C5 FF02 DDBB"            /* .ÿ..ÿ..ÿÿþ.Åÿ.Ý» */
	$"77BB FFFB 0004 FF00 00FF FFFE 00FE FFFE"            /* w»ÿû..ÿ..ÿÿþ.þÿþ */
	$"0000 FFFB 0003 FF00 00FF FD00 07FF 0000"            /* ..ÿû..ÿ..ÿý..ÿ.. */
	$"FF00 00FF FFFE 00C5 FF02 DDBB 77BB FFFB"            /* ÿ..ÿÿþ.Åÿ.Ý»w»ÿû */
	$"0004 FF00 00FF FFFE 00FE FFFE 0000 FFFB"            /* ..ÿ..ÿÿþ.þÿþ..ÿû */
	$"0003 FF00 00FF FD00 07FF 0000 FF00 00FF"            /* ..ÿ..ÿý..ÿ..ÿ..ÿ */
	$"FFFE 00C5 FF03 DDBB 77FF 00A0 BBFF 0000"            /* ÿþ.Åÿ.Ý»wÿ. »ÿ.. */
	$"FEFF 0500 00FF 00FF 00FE FF02 00FF 00FE"            /* þÿ...ÿ.ÿ.þÿ..ÿ.þ */
	$"FF08 00FF 00FF FF00 FFFF 00FC FF08 00FF"            /* ÿ..ÿ.ÿÿ.ÿÿ.üÿ..ÿ */
	$"FF00 00FF 00FF 00FE FF00 00C6 FF02 DDBB"            /* ÿ..ÿ.ÿ.þÿ..Æÿ.Ý» */
	$"77BA FF00 00FE FF05 0000 FF00 FF00 FEFF"            /* wºÿ..þÿ...ÿ.ÿ.þÿ */
	$"0200 FF00 FEFF 0800 FF00 FFFF 00FF FF00"            /* ..ÿ.þÿ..ÿ.ÿÿ.ÿÿ. */
	$"FCFF 0800 FFFF 0000 FF00 FF00 FEFF 0000"            /* üÿ..ÿÿ..ÿ.ÿ.þÿ.. */
	$"C6FF 02DD BB77 BAFF 0000 FEFF 0500 00FF"            /* Æÿ.Ý»wºÿ..þÿ...ÿ */
	$"00FF 00FE FF02 00FF 00FE FF08 00FF 00FF"            /* .ÿ.þÿ..ÿ.þÿ..ÿ.ÿ */
	$"FF00 FFFF 00FC FF08 00FF FF00 00FF 00FF"            /* ÿ.ÿÿ.üÿ..ÿÿ..ÿ.ÿ */
	$"00FE FF00 00C6 FF03 DDBB 77FF 0082 BBFF"            /* .þÿ..Æÿ.Ý»wÿ.‚»ÿ */
	$"0000 FEFF 0000 FDFF FC00 00FF FC00 07FF"            /* ..þÿ..ýÿü..ÿü..ÿ */
	$"00FF FF00 FFFF 00FD FF04 0000 FFFF 00FD"            /* .ÿÿ.ÿÿ.ýÿ...ÿÿ.ý */
	$"FFFC 00C6 FF02 DDBB 77BA FF00 00FE FF00"            /* ÿü.Æÿ.Ý»wºÿ..þÿ. */
	$"00FD FFFC 0000 FFFC 0007 FF00 FFFF 00FF"            /* .ýÿü..ÿü..ÿ.ÿÿ.ÿ */
	$"FF00 FDFF 0400 00FF FF00 FDFF FC00 C6FF"            /* ÿ.ýÿ...ÿÿ.ýÿü.Æÿ */
	$"02DD BB77 BAFF 0000 FEFF 0000 FDFF FC00"            /* .Ý»wºÿ..þÿ..ýÿü. */
	$"00FF FC00 07FF 00FF FF00 FFFF 00FD FF04"            /* .ÿü..ÿ.ÿÿ.ÿÿ.ýÿ. */
	$"0000 FFFF 00FD FFFC 00C6 FF03 DDBB 77FF"            /* ..ÿÿ.ýÿü.Æÿ.Ý»wÿ */
	$"0088 BBFF 0000 FEFF 0000 FDFF 0000 FCFF"            /* .ˆ»ÿ..þÿ..ýÿ..üÿ */
	$"0000 FCFF 0600 FF00 FF00 FF00 FEFF 0500"            /* ..üÿ..ÿ.ÿ.ÿ.þÿ.. */
	$"FF00 FFFF 00FD FF00 00C2 FF02 DDBB 77BA"            /* ÿ.ÿÿ.ýÿ..Âÿ.Ý»wº */
	$"FF00 00FE FF00 00FD FF00 00FC FF00 00FC"            /* ÿ..þÿ..ýÿ..üÿ..ü */
	$"FF06 00FF 00FF 00FF 00FE FF05 00FF 00FF"            /* ÿ..ÿ.ÿ.ÿ.þÿ..ÿ.ÿ */
	$"FF00 FDFF 0000 C2FF 02DD BB77 BAFF 0000"            /* ÿ.ýÿ..Âÿ.Ý»wºÿ.. */
	$"FEFF 0000 FDFF 0000 FCFF 0000 FCFF 0600"            /* þÿ..ýÿ..üÿ..üÿ.. */
	$"FF00 FF00 FF00 FEFF 0500 FF00 FFFF 00FD"            /* ÿ.ÿ.ÿ.þÿ..ÿ.ÿÿ.ý */
	$"FF00 00C2 FF03 DDBB 77FF 0085 BBFF 0000"            /* ÿ..Âÿ.Ý»wÿ.…»ÿ.. */
	$"FEFF 0000 FDFF 0000 FCFF 0000 FBFF 0400"            /* þÿ..ýÿ..üÿ..ûÿ.. */
	$"00FF 0000 FEFF 0600 FFFF 00FF FF00 FDFF"            /* .ÿ..þÿ..ÿÿ.ÿÿ.ýÿ */
	$"0000 C2FF 02DD BB77 BAFF 0000 FEFF 0000"            /* ..Âÿ.Ý»wºÿ..þÿ.. */
	$"FDFF 0000 FCFF 0000 FBFF 0400 00FF 0000"            /* ýÿ..üÿ..ûÿ...ÿ.. */
	$"FEFF 0600 FFFF 00FF FF00 FDFF 0000 C2FF"            /* þÿ..ÿÿ.ÿÿ.ýÿ..Âÿ */
	$"02DD BB77 BAFF 0000 FEFF 0000 FDFF 0000"            /* .Ý»wºÿ..þÿ..ýÿ.. */
	$"FCFF 0000 FBFF 0400 00FF 0000 FEFF 0600"            /* üÿ..ûÿ...ÿ..þÿ.. */
	$"FFFF 00FF FF00 FDFF 0000 C2FF 03DD BB77"            /* ÿÿ.ÿÿ.ýÿ..Âÿ.Ý»w */
	$"FF00 A0BB FF00 00FE FF00 00FD FF00 00FE"            /* ÿ. »ÿ..þÿ..ýÿ..þ */
	$"FF02 00FF 00FE FF03 00FF FF00 FEFF 0000"            /* ÿ..ÿ.þÿ..ÿÿ.þÿ.. */
	$"FEFF 0600 FF00 00FF FF00 FDFF 0000 FEFF"            /* þÿ..ÿ..ÿÿ.ýÿ..þÿ */
	$"0000 C6FF 02DD BB77 BAFF 0000 FEFF 0000"            /* ..Æÿ.Ý»wºÿ..þÿ.. */
	$"FDFF 0000 FEFF 0200 FF00 FEFF 0300 FFFF"            /* ýÿ..þÿ..ÿ.þÿ..ÿÿ */
	$"00FE FF00 00FE FF06 00FF 0000 FFFF 00FD"            /* .þÿ..þÿ..ÿ..ÿÿ.ý */
	$"FF00 00FE FF00 00C6 FF02 DDBB 77BA FF00"            /* ÿ..þÿ..Æÿ.Ý»wºÿ. */
	$"00FE FF00 00FD FF00 00FE FF02 00FF 00FE"            /* .þÿ..ýÿ..þÿ..ÿ.þ */
	$"FF03 00FF FF00 FEFF 0000 FEFF 0600 FF00"            /* ÿ..ÿÿ.þÿ..þÿ..ÿ. */
	$"00FF FF00 FDFF 0000 FEFF 0000 C6FF 03DD"            /* .ÿÿ.ýÿ..þÿ..Æÿ.Ý */
	$"BB77 FF00 7FBC FFFE 0000 FFFE 00FD FFFE"            /* »wÿ..¼ÿþ..ÿþ.ýÿþ */
	$"00FE FFFE 00FE FF00 00FE FF00 00FE FF02"            /* .þÿþ.þÿ..þÿ..þÿ. */
	$"0000 FFFC 00FD FFFE 00C5 FF02 DDBB 77BB"            /* ..ÿü.ýÿþ.Åÿ.Ý»w» */
	$"FFFE 0000 FFFE 00FD FFFE 00FE FFFE 00FE"            /* ÿþ..ÿþ.ýÿþ.þÿþ.þ */
	$"FF00 00FE FF00 00FE FF02 0000 FFFC 00FD"            /* ÿ..þÿ..þÿ...ÿü.ý */
	$"FFFE 00C5 FF02 DDBB 77BB FFFE 0000 FFFE"            /* ÿþ.Åÿ.Ý»w»ÿþ..ÿþ */
	$"00FD FFFE 00FE FFFE 00FE FF00 00FE FF00"            /* .ýÿþ.þÿþ.þÿ..þÿ. */
	$"00FE FF02 0000 FFFC 00FD FFFE 00C5 FF03"            /* .þÿ...ÿü.ýÿþ.Åÿ. */
	$"DDBB 77FF 0019 81FF D2FF 02DD BB77 81FF"            /* Ý»wÿ..ÿÒÿ.Ý»wÿ */
	$"D1FF 02DD BB77 81FF D1FF 03DD BB77 FF00"            /* Ñÿ.Ý»wÿÑÿ.Ý»wÿ. */
	$"1981 FFD2 FF02 DDBB 7781 FFD1 FF02 DDBB"            /* .ÿÒÿ.Ý»wÿÑÿ.Ý» */
	$"7781 FFD1 FF03 DDBB 77FF 0019 81FF D2FF"            /* wÿÑÿ.Ý»wÿ..ÿÒÿ */
	$"02DD BB77 81FF D1FF 02DD BB77 81FF D1FF"            /* .Ý»wÿÑÿ.Ý»wÿÑÿ */
	$"03DD BB77 FF00 1981 FFD2 FF02 DDBB 7781"            /* .Ý»wÿ..ÿÒÿ.Ý»w */
	$"FFD1 FF02 DDBB 7781 FFD1 FF03 DDBB 77FF"            /* ÿÑÿ.Ý»wÿÑÿ.Ý»wÿ */
	$"001F B6FF E122 BDFF 02DD BB77 B5FF E122"            /* ..¶ÿá"½ÿ.Ý»wµÿá" */
	$"BDFF 02DD BB77 B5FF E122 BDFF 03DD BB77"            /* ½ÿ.Ý»wµÿá"½ÿ.Ý»w */
	$"FF00 31B6 FF00 22FE 55E6 CC00 22BD FF02"            /* ÿ.1¶ÿ."þUæÌ."½ÿ. */
	$"DDBB 77B5 FF00 22FE 55E6 CC00 22BD FF02"            /* Ý»wµÿ."þUæÌ."½ÿ. */
	$"DDBB 77B5 FF00 22FE 55E6 CC00 22BD FF03"            /* Ý»wµÿ."þUæÌ."½ÿ. */
	$"DDBB 77FF 0031 B6FF 0022 FE55 E6CC 0022"            /* Ý»wÿ.1¶ÿ."þUæÌ." */
	$"BDFF 02DD BB77 B5FF 0022 FE55 E6CC 0022"            /* ½ÿ.Ý»wµÿ."þUæÌ." */
	$"BDFF 02DD BB77 B5FF 0022 FE55 E6CC 0022"            /* ½ÿ.Ý»wµÿ."þUæÌ." */
	$"BDFF 03DD BB77 FF00 1FB6 FFE1 22BD FF02"            /* ½ÿ.Ý»wÿ..¶ÿá"½ÿ. */
	$"DDBB 77B5 FFE1 22BD FF02 DDBB 77B5 FFE1"            /* Ý»wµÿá"½ÿ.Ý»wµÿá */
	$"22BD FF03 DDBB 77FF 002B B6FF 0022 E366"            /* "½ÿ.Ý»wÿ.+¶ÿ."ãf */
	$"0022 BDFF 02DD BB77 B5FF 0022 E366 0022"            /* ."½ÿ.Ý»wµÿ."ãf." */
	$"BDFF 02DD BB77 B5FF 0022 E3CC 0022 BDFF"            /* ½ÿ.Ý»wµÿ."ãÌ."½ÿ */
	$"03DD BB77 FF00 32B6 FF01 2266 E699 02FF"            /* .Ý»wÿ.2¶ÿ."fæ™.ÿ */
	$"CC22 BDFF 02DD BB77 B5FF 0122 66E6 9902"            /* Ì"½ÿ.Ý»wµÿ."fæ™. */
	$"FFCC 22BD FF02 DDBB 77B5 FF01 22CC E4FF"            /* ÿÌ"½ÿ.Ý»wµÿ."Ìäÿ */
	$"0022 BDFF 03DD BB77 FF00 37B6 FF02 2266"            /* ."½ÿ.Ý»wÿ.7¶ÿ."f */
	$"99E7 CC02 FFCC 22BD FF02 DDBB 77B5 FF02"            /* ™çÌ.ÿÌ"½ÿ.Ý»wµÿ. */
	$"2266 99E7 CC02 FFCC 22BD FF02 DDBB 77B5"            /* "f™çÌ.ÿÌ"½ÿ.Ý»wµ */
	$"FF02 22CC FFE7 CC02 FFFF 22BD FF03 DDBB"            /* ÿ."ÌÿçÌ.ÿÿ"½ÿ.Ý» */
	$"77FF 0067 B6FF 0622 6699 CCCC 00CC FD00"            /* wÿ.g¶ÿ."f™ÌÌ.Ìý. */
	$"01CC CCFC 0001 CCCC FA00 04CC CCFF CC22"            /* .ÌÌü..ÌÌú..ÌÌÿÌ" */
	$"BDFF 02DD BB77 B5FF 0622 6699 CCCC 00CC"            /* ½ÿ.Ý»wµÿ."f™ÌÌ.Ì */
	$"FD00 01CC CCFC 0001 CCCC FA00 04CC CCFF"            /* ý..ÌÌü..ÌÌú..ÌÌÿ */
	$"CC22 BDFF 02DD BB77 B5FF 0622 CCFF CCCC"            /* Ì"½ÿ.Ý»wµÿ."ÌÿÌÌ */
	$"00CC FD00 01CC CCFC 0001 CCCC FA00 04CC"            /* .Ìý..ÌÌü..ÌÌú..Ì */
	$"CCFF FF22 BDFF 03DD BB77 FF00 37B6 FF02"            /* Ìÿÿ"½ÿ.Ý»wÿ.7¶ÿ. */
	$"2266 99E7 CC02 FFCC 22BD FF02 DDBB 77B5"            /* "f™çÌ.ÿÌ"½ÿ.Ý»wµ */
	$"FF02 2266 99E7 CC02 FFCC 22BD FF02 DDBB"            /* ÿ."f™çÌ.ÿÌ"½ÿ.Ý» */
	$"77B5 FF02 22CC FFE7 CC02 FFFF 22BD FF03"            /* wµÿ."ÌÿçÌ.ÿÿ"½ÿ. */
	$"DDBB 77FF 0076 B6FF 0722 6699 CCCC 0000"            /* Ý»wÿ.v¶ÿ."f™ÌÌ.. */
	$"CCFE 0008 CCCC 0000 CCCC 00CC CCFC 0006"            /* Ìþ..ÌÌ..ÌÌ.ÌÌü.. */
	$"CC00 CCCC FFCC 22BD FF02 DDBB 77B5 FF07"            /* Ì.ÌÌÿÌ"½ÿ.Ý»wµÿ. */
	$"2266 99CC CC00 00CC FE00 08CC CC00 00CC"            /* "f™ÌÌ..Ìþ..ÌÌ..Ì */
	$"CC00 CCCC FC00 06CC 00CC CCFF CC22 BDFF"            /* Ì.ÌÌü..Ì.ÌÌÿÌ"½ÿ */
	$"02DD BB77 B5FF 0722 CCFF CCCC 0000 CCFE"            /* .Ý»wµÿ."ÌÿÌÌ..Ìþ */
	$"0008 CCCC 0000 CCCC 00CC CCFC 0006 CC00"            /* ..ÌÌ..ÌÌ.ÌÌü..Ì. */
	$"CCCC FFFF 22BD FF03 DDBB 77FF 0037 B6FF"            /* ÌÌÿÿ"½ÿ.Ý»wÿ.7¶ÿ */
	$"0222 6699 E7CC 02FF CC22 BDFF 02DD BB77"            /* ."f™çÌ.ÿÌ"½ÿ.Ý»w */
	$"B5FF 0222 6699 E7CC 02FF CC22 BDFF 02DD"            /* µÿ."f™çÌ.ÿÌ"½ÿ.Ý */
	$"BB77 B5FF 0222 CCFF E7CC 02FF FF22 BDFF"            /* »wµÿ."ÌÿçÌ.ÿÿ"½ÿ */
	$"03DD BB77 FF00 79B6 FF04 2266 99CC CCFE"            /* .Ý»wÿ.y¶ÿ."f™ÌÌþ */
	$"0006 CC00 00CC CC00 CCFE 0004 CCCC 0000"            /* ..Ì..ÌÌ.Ìþ..ÌÌ.. */
	$"CCFD 0004 CCCC FFCC 22BD FF02 DDBB 77B5"            /* Ìý..ÌÌÿÌ"½ÿ.Ý»wµ */
	$"FF04 2266 99CC CCFE 0006 CC00 00CC CC00"            /* ÿ."f™ÌÌþ..Ì..ÌÌ. */
	$"CCFE 0004 CCCC 0000 CCFD 0004 CCCC FFCC"            /* Ìþ..ÌÌ..Ìý..ÌÌÿÌ */
	$"22BD FF02 DDBB 77B5 FF04 22CC FFCC CCFE"            /* "½ÿ.Ý»wµÿ."ÌÿÌÌþ */
	$"0006 CC00 00CC CC00 CCFE 0004 CCCC 0000"            /* ..Ì..ÌÌ.Ìþ..ÌÌ.. */
	$"CCFD 0004 CCCC FFFF 22BD FF03 DDBB 77FF"            /* Ìý..ÌÌÿÿ"½ÿ.Ý»wÿ */
	$"0037 B6FF 0222 6699 E7CC 02FF CC22 BDFF"            /* .7¶ÿ."f™çÌ.ÿÌ"½ÿ */
	$"02DD BB77 B5FF 0222 6699 E7CC 02FF CC22"            /* .Ý»wµÿ."f™çÌ.ÿÌ" */
	$"BDFF 02DD BB77 B5FF 0222 CCFF E7CC 02FF"            /* ½ÿ.Ý»wµÿ."ÌÿçÌ.ÿ */
	$"FF22 BDFF 03DD BB77 FF00 76B6 FF07 2266"            /* ÿ"½ÿ.Ý»wÿ.v¶ÿ."f */
	$"99CC CC00 00CC FE00 03CC CC00 CCFE 0001"            /* ™ÌÌ..Ìþ..ÌÌ.Ìþ.. */
	$"CCCC FC00 06CC 00CC CCFF CC22 BDFF 02DD"            /* ÌÌü..Ì.ÌÌÿÌ"½ÿ.Ý */
	$"BB77 B5FF 0722 6699 CCCC 0000 CCFE 0003"            /* »wµÿ."f™ÌÌ..Ìþ.. */
	$"CCCC 00CC FE00 01CC CCFC 0006 CC00 CCCC"            /* ÌÌ.Ìþ..ÌÌü..Ì.ÌÌ */
	$"FFCC 22BD FF02 DDBB 77B5 FF07 22CC FFCC"            /* ÿÌ"½ÿ.Ý»wµÿ."ÌÿÌ */
	$"CC00 00CC FE00 03CC CC00 CCFE 0001 CCCC"            /* Ì..Ìþ..ÌÌ.Ìþ..ÌÌ */
	$"FC00 06CC 00CC CCFF FF22 BDFF 03DD BB77"            /* ü..Ì.ÌÌÿÿ"½ÿ.Ý»w */
	$"FF00 37B6 FF02 2266 99E7 CC02 FFCC 22BD"            /* ÿ.7¶ÿ."f™çÌ.ÿÌ"½ */
	$"FF02 DDBB 77B5 FF02 2266 99E7 CC02 FFCC"            /* ÿ.Ý»wµÿ."f™çÌ.ÿÌ */
	$"22BD FF02 DDBB 77B5 FF02 22CC FFE7 CC02"            /* "½ÿ.Ý»wµÿ."ÌÿçÌ. */
	$"FFFF 22BD FF03 DDBB 77FF 0070 B6FF 0422"            /* ÿÿ"½ÿ.Ý»wÿ.p¶ÿ." */
	$"6699 CCCC FB00 0BCC CC00 CC00 CC00 CCCC"            /* f™ÌÌû..ÌÌ.Ì.Ì.ÌÌ */
	$"0000 CCFD 0004 CCCC FFCC 22BD FF02 DDBB"            /* ..Ìý..ÌÌÿÌ"½ÿ.Ý» */
	$"77B5 FF04 2266 99CC CCFB 000B CCCC 00CC"            /* wµÿ."f™ÌÌû..ÌÌ.Ì */
	$"00CC 00CC CC00 00CC FD00 04CC CCFF CC22"            /* .Ì.ÌÌ..Ìý..ÌÌÿÌ" */
	$"BDFF 02DD BB77 B5FF 0422 CCFF CCCC FB00"            /* ½ÿ.Ý»wµÿ."ÌÿÌÌû. */
	$"0BCC CC00 CC00 CC00 CCCC 0000 CCFD 0004"            /* .ÌÌ.Ì.Ì.ÌÌ..Ìý.. */
	$"CCCC FFFF 22BD FF03 DDBB 77FF 0037 B6FF"            /* ÌÌÿÿ"½ÿ.Ý»wÿ.7¶ÿ */
	$"0222 6699 E7CC 02FF CC22 BDFF 02DD BB77"            /* ."f™çÌ.ÿÌ"½ÿ.Ý»w */
	$"B5FF 0222 6699 E7CC 02FF CC22 BDFF 02DD"            /* µÿ."f™çÌ.ÿÌ"½ÿ.Ý */
	$"BB77 B5FF 0222 CCFF E7CC 02FF FF22 BDFF"            /* »wµÿ."ÌÿçÌ.ÿÿ"½ÿ */
	$"03DD BB77 FF00 79B6 FF04 2266 99CC CCFD"            /* .Ý»wÿ.y¶ÿ."f™ÌÌý */
	$"000A CC00 CCCC 0000 CC00 00CC CCFE 0000"            /* ..Ì.ÌÌ..Ì..ÌÌþ.. */
	$"CCFE 0004 CCCC FFCC 22BD FF02 DDBB 77B5"            /* Ìþ..ÌÌÿÌ"½ÿ.Ý»wµ */
	$"FF04 2266 99CC CCFD 000A CC00 CCCC 0000"            /* ÿ."f™ÌÌý..Ì.ÌÌ.. */
	$"CC00 00CC CCFE 0000 CCFE 0004 CCCC FFCC"            /* Ì..ÌÌþ..Ìþ..ÌÌÿÌ */
	$"22BD FF02 DDBB 77B5 FF04 22CC FFCC CCFD"            /* "½ÿ.Ý»wµÿ."ÌÿÌÌý */
	$"000A CC00 CCCC 0000 CC00 00CC CCFE 0000"            /* ..Ì.ÌÌ..Ì..ÌÌþ.. */
	$"CCFE 0004 CCCC FFFF 22BD FF03 DDBB 77FF"            /* Ìþ..ÌÌÿÿ"½ÿ.Ý»wÿ */
	$"0037 B6FF 0222 6699 E7CC 02FF CC22 BDFF"            /* .7¶ÿ."f™çÌ.ÿÌ"½ÿ */
	$"02DD BB77 B5FF 0222 6699 E7CC 02FF CC22"            /* .Ý»wµÿ."f™çÌ.ÿÌ" */
	$"BDFF 02DD BB77 B5FF 0222 CCFF E7CC 02FF"            /* ½ÿ.Ý»wµÿ."ÌÿçÌ.ÿ */
	$"FF22 BDFF 03DD BB77 FF00 73B6 FF07 2266"            /* ÿ"½ÿ.Ý»wÿ.s¶ÿ."f */
	$"99CC CC00 00CC FE00 01CC CCFC 0001 CCCC"            /* ™ÌÌ..Ìþ..ÌÌü..ÌÌ */
	$"FD00 07CC 0000 CCCC FFCC 22BD FF02 DDBB"            /* ý..Ì..ÌÌÿÌ"½ÿ.Ý» */
	$"77B5 FF07 2266 99CC CC00 00CC FE00 01CC"            /* wµÿ."f™ÌÌ..Ìþ..Ì */
	$"CCFC 0001 CCCC FD00 07CC 0000 CCCC FFCC"            /* Ìü..ÌÌý..Ì..ÌÌÿÌ */
	$"22BD FF02 DDBB 77B5 FF07 22CC FFCC CC00"            /* "½ÿ.Ý»wµÿ."ÌÿÌÌ. */
	$"00CC FE00 01CC CCFC 0001 CCCC FD00 07CC"            /* .Ìþ..ÌÌü..ÌÌý..Ì */
	$"0000 CCCC FFFF 22BD FF03 DDBB 77FF 0037"            /* ..ÌÌÿÿ"½ÿ.Ý»wÿ.7 */
	$"B6FF 0222 6699 E7CC 02FF CC22 BDFF 02DD"            /* ¶ÿ."f™çÌ.ÿÌ"½ÿ.Ý */
	$"BB77 B5FF 0222 6699 E7CC 02FF CC22 BDFF"            /* »wµÿ."f™çÌ.ÿÌ"½ÿ */
	$"02DD BB77 B5FF 0222 CCFF E7CC 02FF FF22"            /* .Ý»wµÿ."ÌÿçÌ.ÿÿ" */
	$"BDFF 03DD BB77 FF00 37B6 FF02 2266 99E7"            /* ½ÿ.Ý»wÿ.7¶ÿ."f™ç */
	$"CC02 FFCC 22BD FF02 DDBB 77B5 FF02 2266"            /* Ì.ÿÌ"½ÿ.Ý»wµÿ."f */
	$"99E7 CC02 FFCC 22BD FF02 DDBB 77B5 FF02"            /* ™çÌ.ÿÌ"½ÿ.Ý»wµÿ. */
	$"22CC FFE7 CC02 FFFF 22BD FF03 DDBB 77FF"            /* "ÌÿçÌ.ÿÿ"½ÿ.Ý»wÿ */
	$"0037 B6FF 0222 6699 E7CC 02FF CC22 BDFF"            /* .7¶ÿ."f™çÌ.ÿÌ"½ÿ */
	$"02DD BB77 B5FF 0222 6699 E7CC 02FF CC22"            /* .Ý»wµÿ."f™çÌ.ÿÌ" */
	$"BDFF 02DD BB77 B5FF 0222 CCFF E7CC 02FF"            /* ½ÿ.Ý»wµÿ."ÌÿçÌ.ÿ */
	$"FF22 BDFF 03DD BB77 FF00 37B6 FF02 2266"            /* ÿ"½ÿ.Ý»wÿ.7¶ÿ."f */
	$"99E7 CC02 FFCC 22BD FF02 DDBB 77B5 FF02"            /* ™çÌ.ÿÌ"½ÿ.Ý»wµÿ. */
	$"2266 99E7 CC02 FFCC 22BD FF02 DDBB 77B5"            /* "f™çÌ.ÿÌ"½ÿ.Ý»wµ */
	$"FF02 22CC FFE7 CC02 FFFF 22BD FF03 DDBB"            /* ÿ."ÌÿçÌ.ÿÿ"½ÿ.Ý» */
	$"77FF 0037 B6FF 0222 6699 E7CC 02FF CC22"            /* wÿ.7¶ÿ."f™çÌ.ÿÌ" */
	$"BDFF 02DD BB77 B5FF 0222 6699 E7CC 02FF"            /* ½ÿ.Ý»wµÿ."f™çÌ.ÿ */
	$"CC22 BDFF 02DD BB77 B5FF 0222 CCFF E7CC"            /* Ì"½ÿ.Ý»wµÿ."ÌÿçÌ */
	$"02FF FF22 BDFF 03DD BB77 FF00 37B6 FF02"            /* .ÿÿ"½ÿ.Ý»wÿ.7¶ÿ. */
	$"2266 99E7 CC02 FFCC 22BD FF02 DDBB 77B5"            /* "f™çÌ.ÿÌ"½ÿ.Ý»wµ */
	$"FF02 2266 99E7 CC02 FFCC 22BD FF02 DDBB"            /* ÿ."f™çÌ.ÿÌ"½ÿ.Ý» */
	$"77B5 FF02 22CC FFE7 CC02 FFFF 22BD FF03"            /* wµÿ."ÌÿçÌ.ÿÿ"½ÿ. */
	$"DDBB 77FF 0037 B6FF 0222 6699 E7CC 02FF"            /* Ý»wÿ.7¶ÿ."f™çÌ.ÿ */
	$"CC22 BDFF 02DD BB77 B5FF 0222 6699 E7CC"            /* Ì"½ÿ.Ý»wµÿ."f™çÌ */
	$"02FF CC22 BDFF 02DD BB77 B5FF 0222 CCFF"            /* .ÿÌ"½ÿ.Ý»wµÿ."Ìÿ */
	$"E7CC 02FF FF22 BDFF 03DD BB77 FF00 37B6"            /* çÌ.ÿÿ"½ÿ.Ý»wÿ.7¶ */
	$"FF02 2266 99E7 CC02 FFCC 22BD FF02 DDBB"            /* ÿ."f™çÌ.ÿÌ"½ÿ.Ý» */
	$"77B5 FF02 2266 99E7 CC02 FFCC 22BD FF02"            /* wµÿ."f™çÌ.ÿÌ"½ÿ. */
	$"DDBB 77B5 FF02 22CC FFE7 CC02 FFFF 22BD"            /* Ý»wµÿ."ÌÿçÌ.ÿÿ"½ */
	$"FF03 DDBB 77FF 0037 B6FF 0222 6699 E7CC"            /* ÿ.Ý»wÿ.7¶ÿ."f™çÌ */
	$"02FF CC22 BDFF 02DD BB77 B5FF 0222 6699"            /* .ÿÌ"½ÿ.Ý»wµÿ."f™ */
	$"E7CC 02FF CC22 BDFF 02DD BB77 B5FF 0222"            /* çÌ.ÿÌ"½ÿ.Ý»wµÿ." */
	$"CCFF E7CC 02FF FF22 BDFF 03DD BB77 FF00"            /* ÌÿçÌ.ÿÿ"½ÿ.Ý»wÿ. */
	$"32B6 FF02 2266 99E6 FF01 CC22 BDFF 02DD"            /* 2¶ÿ."f™æÿ.Ì"½ÿ.Ý */
	$"BB77 B5FF 0222 6699 E6FF 01CC 22BD FF02"            /* »wµÿ."f™æÿ.Ì"½ÿ. */
	$"DDBB 77B5 FF01 22CC E4FF 0022 BDFF 03DD"            /* Ý»wµÿ."Ìäÿ."½ÿ.Ý */
	$"BB77 FF00 2EB6 FF01 2266 E4CC 0022 BDFF"            /* »wÿ..¶ÿ."fäÌ."½ÿ */
	$"02DD BB77 B5FF 0122 66E4 CC00 22BD FF02"            /* .Ý»wµÿ."fäÌ."½ÿ. */
	$"DDBB 77B5 FF01 22CC E4FF 0022 BDFF 03DD"            /* Ý»wµÿ."Ìäÿ."½ÿ.Ý */
	$"BB77 FF00 1FB6 FFE1 22BD FF02 DDBB 77B5"            /* »wÿ..¶ÿá"½ÿ.Ý»wµ */
	$"FFE1 22BD FF02 DDBB 77B5 FFE1 22BD FF03"            /* ÿá"½ÿ.Ý»wµÿá"½ÿ. */
	$"DDBB 77FF 0019 81FF D2FF 02DD BB77 81FF"            /* Ý»wÿ..ÿÒÿ.Ý»wÿ */
	$"D1FF 02DD BB77 81FF D1FF 03DD BB77 FF00"            /* Ñÿ.Ý»wÿÑÿ.Ý»wÿ. */
	$"1981 FFD2 FF02 DDBB 7781 FFD1 FF02 DDBB"            /* .ÿÒÿ.Ý»wÿÑÿ.Ý» */
	$"7781 FFD1 FF03 DDBB 77FF 0019 81FF D2FF"            /* wÿÑÿ.Ý»wÿ..ÿÒÿ */
	$"02DD BB77 81FF D1FF 02DD BB77 81FF D1FF"            /* .Ý»wÿÑÿ.Ý»wÿÑÿ */
	$"03DD BB77 FF00 1981 FFD2 FF02 DDBB 7781"            /* .Ý»wÿ..ÿÒÿ.Ý»w */
	$"FFD1 FF02 DDBB 7781 FFD1 FF03 DDBB 77FF"            /* ÿÑÿ.Ý»wÿÑÿ.Ý»wÿ */
	$"0019 81FF D2FF 02DD BB77 81FF D1FF 02DD"            /* ..ÿÒÿ.Ý»wÿÑÿ.Ý */
	$"BB77 81FF D1FF 03DD BB77 FF00 1981 FFD2"            /* »wÿÑÿ.Ý»wÿ..ÿÒ */
	$"FF02 DDBB 7781 FFD1 FF02 DDBB 7781 FFD1"            /* ÿ.Ý»wÿÑÿ.Ý»wÿÑ */
	$"FF03 DDBB 77FF 00B5 F3FF 0000 FCFF 0000"            /* ÿ.Ý»wÿ.µóÿ..üÿ.. */
	$"FEFF 0000 F2FF 0000 FCFF 0000 C2FF 0100"            /* þÿ..òÿ..üÿ..Âÿ.. */
	$"00F7 FF03 00FF FF00 FCFF 0300 FFFF 00FC"            /* .÷ÿ..ÿÿ.üÿ..ÿÿ.ü */
	$"FF03 00FF FF00 FEFF FE00 FEFF 0000 ECFF"            /* ÿ..ÿÿ.þÿþ.þÿ..ìÿ */
	$"02DD BB77 F2FF 0000 FCFF 0000 FEFF 0000"            /* .Ý»wòÿ..üÿ..þÿ.. */
	$"F2FF 0000 FCFF 0000 C2FF 0100 00F7 FF03"            /* òÿ..üÿ..Âÿ...÷ÿ. */
	$"00FF FF00 FCFF 0300 FFFF 00FC FF03 00FF"            /* .ÿÿ.üÿ..ÿÿ.üÿ..ÿ */
	$"FF00 FEFF FE00 FEFF 0000 ECFF 02DD BB77"            /* ÿ.þÿþ.þÿ..ìÿ.Ý»w */
	$"F2FF 0000 FCFF 0000 FEFF 0000 F2FF 0000"            /* òÿ..üÿ..þÿ..òÿ.. */
	$"FCFF 0000 C2FF 0100 00F7 FF03 00FF FF00"            /* üÿ..Âÿ...÷ÿ..ÿÿ. */
	$"FCFF 0300 FFFF 00FC FF03 00FF FF00 FEFF"            /* üÿ..ÿÿ.üÿ..ÿÿ.þÿ */
	$"FE00 FEFF 0000 ECFF 03DD BB77 FF00 AFF3"            /* þ.þÿ..ìÿ.Ý»wÿ.¯ó */
	$"FF00 00FC FF00 00FE FF00 00F2 FF00 00FC"            /* ÿ..üÿ..þÿ..òÿ..ü */
	$"FF00 00C3 FF00 00F5 FF04 00FF FF00 00FE"            /* ÿ..Ãÿ..õÿ..ÿÿ..þ */
	$"FF01 0000 F9FF 0000 FCFF 0000 FEFF 0300"            /* ÿ...ùÿ..üÿ..þÿ.. */
	$"FFFF 00EC FF02 DDBB 77F2 FF00 00FC FF00"            /* ÿÿ.ìÿ.Ý»wòÿ..üÿ. */
	$"00FE FF00 00F2 FF00 00FC FF00 00C3 FF00"            /* .þÿ..òÿ..üÿ..Ãÿ. */
	$"00F5 FF04 00FF FF00 00FE FF01 0000 F9FF"            /* .õÿ..ÿÿ..þÿ...ùÿ */
	$"0000 FCFF 0000 FEFF 0300 FFFF 00EC FF02"            /* ..üÿ..þÿ..ÿÿ.ìÿ. */
	$"DDBB 77F2 FF00 00FC FF00 00FE FF00 00F2"            /* Ý»wòÿ..üÿ..þÿ..ò */
	$"FF00 00FC FF00 00C3 FF00 00F5 FF04 00FF"            /* ÿ..üÿ..Ãÿ..õÿ..ÿ */
	$"FF00 00FE FF01 0000 F9FF 0000 FCFF 0000"            /* ÿ..þÿ...ùÿ..üÿ.. */
	$"FEFF 0300 FFFF 00EC FF03 DDBB 77FF 019F"            /* þÿ..ÿÿ.ìÿ.Ý»wÿ.Ÿ */
	$"F3FF FE00 01FF FFFE 0000 FFFE 0001 FFFF"            /* óÿþ..ÿÿþ..ÿþ..ÿÿ */
	$"FE00 F9FF 0000 FCFF 0300 FFFF 00FC FF02"            /* þ.ùÿ..üÿ..ÿÿ.üÿ. */
	$"00FF 00FC FF02 00FF 00FC FF00 00FC FFFE"            /* .ÿ.üÿ..ÿ.üÿ..üÿþ */
	$"0009 FFFF 00FF 0000 FFFF 0000 FEFF FE00"            /* .Æÿÿ.ÿ..ÿÿ..þÿþ. */
	$"02FF 0000 FEFF 0100 00FC FFFE 0005 FFFF"            /* .ÿ..þÿ...üÿþ..ÿÿ */
	$"00FF 0000 FDFF 0000 FEFF 0900 FF00 FF00"            /* .ÿ..ýÿ..þÿÆ.ÿ.ÿ. */
	$"FF00 FFFF 00FE FFFE 0005 FFFF 00FF FF00"            /* ÿ.ÿÿ.þÿþ..ÿÿ.ÿÿ. */
	$"FBFF FE00 FEFF 0100 00FE FF07 00FF 0000"            /* ûÿþ.þÿ...þÿ..ÿ.. */
	$"FFFF 0000 FEFF 02DD BB77 F2FF FE00 01FF"            /* ÿÿ..þÿ.Ý»wòÿþ..ÿ */
	$"FFFE 0000 FFFE 0001 FFFF FE00 F9FF 0000"            /* ÿþ..ÿþ..ÿÿþ.ùÿ.. */
	$"FCFF 0300 FFFF 00FC FF02 00FF 00FC FF02"            /* üÿ..ÿÿ.üÿ..ÿ.üÿ. */
	$"00FF 00FC FF00 00FC FFFE 0009 FFFF 00FF"            /* .ÿ.üÿ..üÿþ.Æÿÿ.ÿ */
	$"0000 FFFF 0000 FEFF FE00 02FF 0000 FEFF"            /* ..ÿÿ..þÿþ..ÿ..þÿ */
	$"0100 00FC FFFE 0005 FFFF 00FF 0000 FDFF"            /* ...üÿþ..ÿÿ.ÿ..ýÿ */
	$"0000 FEFF 0900 FF00 FF00 FF00 FFFF 00FE"            /* ..þÿÆ.ÿ.ÿ.ÿ.ÿÿ.þ */
	$"FFFE 0005 FFFF 00FF FF00 FBFF FE00 FEFF"            /* ÿþ..ÿÿ.ÿÿ.ûÿþ.þÿ */
	$"0100 00FE FF07 00FF 0000 FFFF 0000 FEFF"            /* ...þÿ..ÿ..ÿÿ..þÿ */
	$"02DD BB77 F2FF FE00 01FF FFFE 0000 FFFE"            /* .Ý»wòÿþ..ÿÿþ..ÿþ */
	$"0001 FFFF FE00 F9FF 0000 FCFF 0300 FFFF"            /* ..ÿÿþ.ùÿ..üÿ..ÿÿ */
	$"00FC FF02 00FF 00FC FF02 00FF 00FC FF00"            /* .üÿ..ÿ.üÿ..ÿ.üÿ. */
	$"00FC FFFE 0009 FFFF 00FF 0000 FFFF 0000"            /* .üÿþ.Æÿÿ.ÿ..ÿÿ.. */
	$"FEFF FE00 02FF 0000 FEFF 0100 00FC FFFE"            /* þÿþ..ÿ..þÿ...üÿþ */
	$"0005 FFFF 00FF 0000 FDFF 0000 FEFF 0900"            /* ..ÿÿ.ÿ..ýÿ..þÿÆ. */
	$"FF00 FF00 FF00 FFFF 00FE FFFE 0005 FFFF"            /* ÿ.ÿ.ÿ.ÿÿ.þÿþ..ÿÿ */
	$"00FF FF00 FBFF FE00 FEFF 0100 00FE FF07"            /* .ÿÿ.ûÿþ.þÿ...þÿ. */
	$"00FF 0000 FFFF 0000 FEFF 03DD BB77 FF01"            /* .ÿ..ÿÿ..þÿ.Ý»wÿ. */
	$"C3F3 FF06 00FF FF00 FFFF 00FE FF00 00FE"            /* Ãóÿ..ÿÿ.ÿÿ.þÿ..þ */
	$"FF06 00FF FF00 FFFF 00FD FF00 00FC FF03"            /* ÿ..ÿÿ.ÿÿ.ýÿ..üÿ. */
	$"00FF FF00 FCFF 0200 FF00 FCFF 0200 FF00"            /* .ÿÿ.üÿ..ÿ.üÿ..ÿ. */
	$"FCFF 0000 FDFF 0700 FFFF 00FF FF00 00FE"            /* üÿ..ýÿ..ÿÿ.ÿÿ..þ */
	$"FF11 00FF FF00 FFFF 00FF FF00 FFFF 00FF"            /* ÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"00FF FF00 FCFF 0000 FEFF 0100 00FB FF00"            /* .ÿÿ.üÿ..þÿ...ûÿ. */
	$"00FE FF12 00FF FF00 FFFF 00FF FF00 FFFF"            /* .þÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 FFFF 00FE FFFE 00FE FF0C 00FF"            /* .ÿÿ.ÿÿ.þÿþ.þÿ..ÿ */
	$"FF00 FF00 FFFF 00FF FF00 00FE FF08 00FF"            /* ÿ.ÿ.ÿÿ.ÿÿ..þÿ..ÿ */
	$"FF00 FFFF DDBB 77F2 FF06 00FF FF00 FFFF"            /* ÿ.ÿÿÝ»wòÿ..ÿÿ.ÿÿ */
	$"00FE FF00 00FE FF06 00FF FF00 FFFF 00FD"            /* .þÿ..þÿ..ÿÿ.ÿÿ.ý */
	$"FF00 00FC FF03 00FF FF00 FCFF 0200 FF00"            /* ÿ..üÿ..ÿÿ.üÿ..ÿ. */
	$"FCFF 0200 FF00 FCFF 0000 FDFF 0700 FFFF"            /* üÿ..ÿ.üÿ..ýÿ..ÿÿ */
	$"00FF FF00 00FE FF11 00FF FF00 FFFF 00FF"            /* .ÿÿ..þÿ..ÿÿ.ÿÿ.ÿ */
	$"FF00 FFFF 00FF 00FF FF00 FCFF 0000 FEFF"            /* ÿ.ÿÿ.ÿ.ÿÿ.üÿ..þÿ */
	$"0100 00FB FF00 00FE FF12 00FF FF00 FFFF"            /* ...ûÿ..þÿ..ÿÿ.ÿÿ */
	$"00FF FF00 FFFF 00FF FF00 FFFF 00FE FFFE"            /* .ÿÿ.ÿÿ.ÿÿ.ÿÿ.þÿþ */
	$"00FE FF0C 00FF FF00 FF00 FFFF 00FF FF00"            /* .þÿ..ÿÿ.ÿ.ÿÿ.ÿÿ. */
	$"00FE FF08 00FF FF00 FFFF DDBB 77F2 FF06"            /* .þÿ..ÿÿ.ÿÿÝ»wòÿ. */
	$"00FF FF00 FFFF 00FE FF00 00FE FF06 00FF"            /* .ÿÿ.ÿÿ.þÿ..þÿ..ÿ */
	$"FF00 FFFF 00FD FF00 00FC FF03 00FF FF00"            /* ÿ.ÿÿ.ýÿ..üÿ..ÿÿ. */
	$"FCFF 0200 FF00 FCFF 0200 FF00 FCFF 0000"            /* üÿ..ÿ.üÿ..ÿ.üÿ.. */
	$"FDFF 0700 FFFF 00FF FF00 00FE FF11 00FF"            /* ýÿ..ÿÿ.ÿÿ..þÿ..ÿ */
	$"FF00 FFFF 00FF FF00 FFFF 00FF 00FF FF00"            /* ÿ.ÿÿ.ÿÿ.ÿÿ.ÿ.ÿÿ. */
	$"FCFF 0000 FEFF 0100 00FB FF00 00FE FF12"            /* üÿ..þÿ...ûÿ..þÿ. */
	$"00FF FF00 FFFF 00FF FF00 FFFF 00FF FF00"            /* .ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FFFF 00FE FFFE 00FE FF0C 00FF FF00 FF00"            /* ÿÿ.þÿþ.þÿ..ÿÿ.ÿ. */
	$"FFFF 00FF FF00 00FE FF09 00FF FF00 FFFF"            /* ÿÿ.ÿÿ..þÿÆ.ÿÿ.ÿÿ */
	$"DDBB 77FF 01A5 F3FF 0600 FFFF 00FF FF00"            /* Ý»wÿ.¥óÿ..ÿÿ.ÿÿ. */
	$"FEFF 0000 FEFF 0300 FFFF 00FB FF00 00FC"            /* þÿ..þÿ..ÿÿ.ûÿ..ü */
	$"FF00 00FD FF04 00FF 00FF 00FE FF04 00FF"            /* ÿ..ýÿ..ÿ.ÿ.þÿ..ÿ */
	$"00FF 00FE FF04 00FF 00FF 00FC FF06 00FF"            /* .ÿ.þÿ..ÿ.ÿ.üÿ..ÿ */
	$"FF00 FFFF 00FC FFFE 0009 FFFF 00FF FF00"            /* ÿ.ÿÿ.üÿþ.Æÿÿ.ÿÿ. */
	$"FFFF 00FF FD00 FCFF 0000 FEFF 0000 FBFF"            /* ÿÿ.ÿý.üÿ..þÿ..ûÿ */
	$"0000 FDFF 0000 FCFF 0C00 FFFF 00FF FF00"            /* ..ýÿ..üÿ..ÿÿ.ÿÿ. */
	$"FFFF 00FF FF00 FBFF 0800 FFFF 00FF FF00"            /* ÿÿ.ÿÿ.ûÿ..ÿÿ.ÿÿ. */
	$"FFFF FE00 02FF FF00 FDFF FD00 04FF FFDD"            /* ÿÿþ..ÿÿ.ýÿý..ÿÿÝ */
	$"BB77 F2FF 0600 FFFF 00FF FF00 FEFF 0000"            /* »wòÿ..ÿÿ.ÿÿ.þÿ.. */
	$"FEFF 0300 FFFF 00FB FF00 00FC FF00 00FD"            /* þÿ..ÿÿ.ûÿ..üÿ..ý */
	$"FF04 00FF 00FF 00FE FF04 00FF 00FF 00FE"            /* ÿ..ÿ.ÿ.þÿ..ÿ.ÿ.þ */
	$"FF04 00FF 00FF 00FC FF06 00FF FF00 FFFF"            /* ÿ..ÿ.ÿ.üÿ..ÿÿ.ÿÿ */
	$"00FC FFFE 0009 FFFF 00FF FF00 FFFF 00FF"            /* .üÿþ.Æÿÿ.ÿÿ.ÿÿ.ÿ */
	$"FD00 FCFF 0000 FEFF 0000 FBFF 0000 FDFF"            /* ý.üÿ..þÿ..ûÿ..ýÿ */
	$"0000 FCFF 0C00 FFFF 00FF FF00 FFFF 00FF"            /* ..üÿ..ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"FF00 FBFF 0800 FFFF 00FF FF00 FFFF FE00"            /* ÿ.ûÿ..ÿÿ.ÿÿ.ÿÿþ. */
	$"02FF FF00 FDFF FD00 04FF FFDD BB77 F2FF"            /* .ÿÿ.ýÿý..ÿÿÝ»wòÿ */
	$"0600 FFFF 00FF FF00 FEFF 0000 FEFF 0300"            /* ..ÿÿ.ÿÿ.þÿ..þÿ.. */
	$"FFFF 00FB FF00 00FC FF00 00FD FF04 00FF"            /* ÿÿ.ûÿ..üÿ..ýÿ..ÿ */
	$"00FF 00FE FF04 00FF 00FF 00FE FF04 00FF"            /* .ÿ.þÿ..ÿ.ÿ.þÿ..ÿ */
	$"00FF 00FC FF06 00FF FF00 FFFF 00FC FFFE"            /* .ÿ.üÿ..ÿÿ.ÿÿ.üÿþ */
	$"0009 FFFF 00FF FF00 FFFF 00FF FD00 FCFF"            /* .Æÿÿ.ÿÿ.ÿÿ.ÿý.üÿ */
	$"0000 FEFF 0000 FBFF 0000 FDFF 0000 FCFF"            /* ..þÿ..ûÿ..ýÿ..üÿ */
	$"0C00 FFFF 00FF FF00 FFFF 00FF FF00 FBFF"            /* ..ÿÿ.ÿÿ.ÿÿ.ÿÿ.ûÿ */
	$"0800 FFFF 00FF FF00 FFFF FE00 02FF FF00"            /* ..ÿÿ.ÿÿ.ÿÿþ..ÿÿ. */
	$"FDFF FD00 05FF FFDD BB77 FF01 A8F3 FF06"            /* ýÿý..ÿÿÝ»wÿ.¨óÿ. */
	$"00FF FF00 FFFF 00FE FF00 00FE FF03 00FF"            /* .ÿÿ.ÿÿ.þÿ..þÿ..ÿ */
	$"FF00 FBFF 0000 FCFF 0000 FDFF 0400 FF00"            /* ÿ.ûÿ..üÿ..ýÿ..ÿ. */
	$"FF00 FEFF 0400 FF00 FF00 FEFF 0400 FF00"            /* ÿ.þÿ..ÿ.ÿ.þÿ..ÿ. */
	$"FF00 FCFF 0600 FFFF 00FF FF00 FDFF 0E00"            /* ÿ.üÿ..ÿÿ.ÿÿ.ýÿ.. */
	$"FFFF 00FF FF00 FFFF 00FF FF00 FF00 F9FF"            /* ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ.ùÿ */
	$"0000 FEFF 0000 FBFF 0000 FDFF 0000 FCFF"            /* ..þÿ..ûÿ..ýÿ..üÿ */
	$"0C00 FFFF 00FF FF00 FFFF 00FF FF00 FBFF"            /* ..ÿÿ.ÿÿ.ÿÿ.ÿÿ.ûÿ */
	$"0E00 FFFF 00FF FF00 FF00 FFFF 00FF FF00"            /* ..ÿÿ.ÿÿ.ÿ.ÿÿ.ÿÿ. */
	$"FDFF 0000 FCFF 02DD BB77 F2FF 0600 FFFF"            /* ýÿ..üÿ.Ý»wòÿ..ÿÿ */
	$"00FF FF00 FEFF 0000 FEFF 0300 FFFF 00FB"            /* .ÿÿ.þÿ..þÿ..ÿÿ.û */
	$"FF00 00FC FF00 00FD FF04 00FF 00FF 00FE"            /* ÿ..üÿ..ýÿ..ÿ.ÿ.þ */
	$"FF04 00FF 00FF 00FE FF04 00FF 00FF 00FC"            /* ÿ..ÿ.ÿ.þÿ..ÿ.ÿ.ü */
	$"FF06 00FF FF00 FFFF 00FD FF0E 00FF FF00"            /* ÿ..ÿÿ.ÿÿ.ýÿ..ÿÿ. */
	$"FFFF 00FF FF00 FFFF 00FF 00F9 FF00 00FE"            /* ÿÿ.ÿÿ.ÿÿ.ÿ.ùÿ..þ */
	$"FF00 00FB FF00 00FD FF00 00FC FF0C 00FF"            /* ÿ..ûÿ..ýÿ..üÿ..ÿ */
	$"FF00 FFFF 00FF FF00 FFFF 00FB FF0E 00FF"            /* ÿ.ÿÿ.ÿÿ.ÿÿ.ûÿ..ÿ */
	$"FF00 FFFF 00FF 00FF FF00 FFFF 00FD FF00"            /* ÿ.ÿÿ.ÿ.ÿÿ.ÿÿ.ýÿ. */
	$"00FC FF02 DDBB 77F2 FF06 00FF FF00 FFFF"            /* .üÿ.Ý»wòÿ..ÿÿ.ÿÿ */
	$"00FE FF00 00FE FF03 00FF FF00 FBFF 0000"            /* .þÿ..þÿ..ÿÿ.ûÿ.. */
	$"FCFF 0000 FDFF 0400 FF00 FF00 FEFF 0400"            /* üÿ..ýÿ..ÿ.ÿ.þÿ.. */
	$"FF00 FF00 FEFF 0400 FF00 FF00 FCFF 0600"            /* ÿ.ÿ.þÿ..ÿ.ÿ.üÿ.. */
	$"FFFF 00FF FF00 FDFF 0E00 FFFF 00FF FF00"            /* ÿÿ.ÿÿ.ýÿ..ÿÿ.ÿÿ. */
	$"FFFF 00FF FF00 FF00 F9FF 0000 FEFF 0000"            /* ÿÿ.ÿÿ.ÿ.ùÿ..þÿ.. */
	$"FBFF 0000 FDFF 0000 FCFF 0C00 FFFF 00FF"            /* ûÿ..ýÿ..üÿ..ÿÿ.ÿ */
	$"FF00 FFFF 00FF FF00 FBFF 0E00 FFFF 00FF"            /* ÿ.ÿÿ.ÿÿ.ûÿ..ÿÿ.ÿ */
	$"FF00 FF00 FFFF 00FF FF00 FDFF 0000 FCFF"            /* ÿ.ÿ.ÿÿ.ÿÿ.ýÿ..üÿ */
	$"03DD BB77 FF01 AEF3 FF06 00FF FF00 FFFF"            /* .Ý»wÿ.®óÿ..ÿÿ.ÿÿ */
	$"00FE FF00 00FE FF03 00FF FF00 FCFF 0000"            /* .þÿ..þÿ..ÿÿ.üÿ.. */
	$"FCFF 0000 FBFF 0200 FF00 FCFF 0200 FF00"            /* üÿ..ûÿ..ÿ.üÿ..ÿ. */
	$"FCFF 0200 FF00 FBFF 0600 FFFF 00FF FF00"            /* üÿ..ÿ.ûÿ..ÿÿ.ÿÿ. */
	$"FDFF 1100 FFFF 00FF FF00 FFFF 00FF FF00"            /* ýÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FF00 FFFF 00FC FF00 00FE FF00 00FC FF00"            /* ÿ.ÿÿ.üÿ..þÿ..üÿ. */
	$"00FC FF00 00FC FF0F 00FF FF00 FFFF 00FF"            /* .üÿ..üÿ..ÿÿ.ÿÿ.ÿ */
	$"FF00 FFFF 00FF FF00 FEFF 0E00 FFFF 00FF"            /* ÿ.ÿÿ.ÿÿ.þÿ..ÿÿ.ÿ */
	$"FF00 FF00 FFFF 00FF FF00 FDFF 0800 FFFF"            /* ÿ.ÿ.ÿÿ.ÿÿ.ýÿ..ÿÿ */
	$"00FF FFDD BB77 F2FF 0600 FFFF 00FF FF00"            /* .ÿÿÝ»wòÿ..ÿÿ.ÿÿ. */
	$"FEFF 0000 FEFF 0300 FFFF 00FC FF00 00FC"            /* þÿ..þÿ..ÿÿ.üÿ..ü */
	$"FF00 00FB FF02 00FF 00FC FF02 00FF 00FC"            /* ÿ..ûÿ..ÿ.üÿ..ÿ.ü */
	$"FF02 00FF 00FB FF06 00FF FF00 FFFF 00FD"            /* ÿ..ÿ.ûÿ..ÿÿ.ÿÿ.ý */
	$"FF11 00FF FF00 FFFF 00FF FF00 FFFF 00FF"            /* ÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"00FF FF00 FCFF 0000 FEFF 0000 FCFF 0000"            /* .ÿÿ.üÿ..þÿ..üÿ.. */
	$"FCFF 0000 FCFF 0F00 FFFF 00FF FF00 FFFF"            /* üÿ..üÿ..ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 FFFF 00FE FF0E 00FF FF00 FFFF"            /* .ÿÿ.ÿÿ.þÿ..ÿÿ.ÿÿ */
	$"00FF 00FF FF00 FFFF 00FD FF08 00FF FF00"            /* .ÿ.ÿÿ.ÿÿ.ýÿ..ÿÿ. */
	$"FFFF DDBB 77F2 FF06 00FF FF00 FFFF 00FE"            /* ÿÿÝ»wòÿ..ÿÿ.ÿÿ.þ */
	$"FF00 00FE FF03 00FF FF00 FCFF 0000 FCFF"            /* ÿ..þÿ..ÿÿ.üÿ..üÿ */
	$"0000 FBFF 0200 FF00 FCFF 0200 FF00 FCFF"            /* ..ûÿ..ÿ.üÿ..ÿ.üÿ */
	$"0200 FF00 FBFF 0600 FFFF 00FF FF00 FDFF"            /* ..ÿ.ûÿ..ÿÿ.ÿÿ.ýÿ */
	$"1100 FFFF 00FF FF00 FFFF 00FF FF00 FF00"            /* ..ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ. */
	$"FFFF 00FC FF00 00FE FF00 00FC FF00 00FC"            /* ÿÿ.üÿ..þÿ..üÿ..ü */
	$"FF00 00FC FF0F 00FF FF00 FFFF 00FF FF00"            /* ÿ..üÿ..ÿÿ.ÿÿ.ÿÿ. */
	$"FFFF 00FF FF00 FEFF 0E00 FFFF 00FF FF00"            /* ÿÿ.ÿÿ.þÿ..ÿÿ.ÿÿ. */
	$"FF00 FFFF 00FF FF00 FDFF 0900 FFFF 00FF"            /* ÿ.ÿÿ.ÿÿ.ýÿÆ.ÿÿ.ÿ */
	$"FFDD BB77 FF01 ABF3 FF03 00FF FF00 FEFF"            /* ÿÝ»wÿ.«óÿ..ÿÿ.þÿ */
	$"0000 FEFF 0200 FFFF FE00 FEFF 0300 FFFF"            /* ..þÿ..ÿÿþ.þÿ..ÿÿ */
	$"00FC FF00 00FB FF02 00FF 00FC FF02 00FF"            /* .üÿ..ûÿ..ÿ.üÿ..ÿ */
	$"00FC FF02 00FF 00FD FF02 00FF FFFE 0002"            /* .üÿ..ÿ.ýÿ..ÿÿþ.. */
	$"FFFF 00FC FFFE 000C FFFF 00FF FF00 FFFF"            /* ÿÿ.üÿþ..ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 00FE FF03 00FF FF00 FEFF 0000"            /* .ÿÿ..þÿ..ÿÿ.þÿ.. */
	$"FCFF 0000 FCFF 0000 FCFF 0300 FFFF 00FE"            /* üÿ..üÿ..üÿ..ÿÿ.þ */
	$"FFFE 0002 FFFF 00FE FFFE 00FE FF05 00FF"            /* ÿþ..ÿÿ.þÿþ.þÿ..ÿ */
	$"FF00 FFFF FE00 02FF FF00 FCFF 0100 00FE"            /* ÿ.ÿÿþ..ÿÿ.üÿ...þ */
	$"FF02 DDBB 77F2 FF03 00FF FF00 FEFF 0000"            /* ÿ.Ý»wòÿ..ÿÿ.þÿ.. */
	$"FEFF 0200 FFFF FE00 FEFF 0300 FFFF 00FC"            /* þÿ..ÿÿþ.þÿ..ÿÿ.ü */
	$"FF00 00FB FF02 00FF 00FC FF02 00FF 00FC"            /* ÿ..ûÿ..ÿ.üÿ..ÿ.ü */
	$"FF02 00FF 00FD FF02 00FF FFFE 0002 FFFF"            /* ÿ..ÿ.ýÿ..ÿÿþ..ÿÿ */
	$"00FC FFFE 000C FFFF 00FF FF00 FFFF 00FF"            /* .üÿþ..ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"FF00 00FE FF03 00FF FF00 FEFF 0000 FCFF"            /* ÿ..þÿ..ÿÿ.þÿ..üÿ */
	$"0000 FCFF 0000 FCFF 0300 FFFF 00FE FFFE"            /* ..üÿ..üÿ..ÿÿ.þÿþ */
	$"0002 FFFF 00FE FFFE 00FE FF05 00FF FF00"            /* ..ÿÿ.þÿþ.þÿ..ÿÿ. */
	$"FFFF FE00 02FF FF00 FCFF 0100 00FE FF02"            /* ÿÿþ..ÿÿ.üÿ...þÿ. */
	$"DDBB 77F2 FF03 00FF FF00 FEFF 0000 FEFF"            /* Ý»wòÿ..ÿÿ.þÿ..þÿ */
	$"0200 FFFF FE00 FEFF 0300 FFFF 00FC FF00"            /* ..ÿÿþ.þÿ..ÿÿ.üÿ. */
	$"00FB FF02 00FF 00FC FF02 00FF 00FC FF02"            /* .ûÿ..ÿ.üÿ..ÿ.üÿ. */
	$"00FF 00FD FF02 00FF FFFE 0002 FFFF 00FC"            /* .ÿ.ýÿ..ÿÿþ..ÿÿ.ü */
	$"FFFE 000C FFFF 00FF FF00 FFFF 00FF FF00"            /* ÿþ..ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"00FE FF03 00FF FF00 FEFF 0000 FCFF 0000"            /* .þÿ..ÿÿ.þÿ..üÿ.. */
	$"FCFF 0000 FCFF 0300 FFFF 00FE FFFE 0002"            /* üÿ..üÿ..ÿÿ.þÿþ.. */
	$"FFFF 00FE FFFE 00FE FF05 00FF FF00 FFFF"            /* ÿÿ.þÿþ.þÿ..ÿÿ.ÿÿ */
	$"FE00 02FF FF00 FCFF 0100 00FE FF03 DDBB"            /* þ..ÿÿ.üÿ...þÿ.Ý» */
	$"77FF 002B E5FF 0000 D0FF 0000 A1FF 02DD"            /* wÿ.+åÿ..Ðÿ..¡ÿ.Ý */
	$"BB77 E4FF 0000 D0FF 0000 A1FF 02DD BB77"            /* »wäÿ..Ðÿ..¡ÿ.Ý»w */
	$"E4FF 0000 D0FF 0000 A1FF 03DD BB77 FF00"            /* äÿ..Ðÿ..¡ÿ.Ý»wÿ. */
	$"2EE5 FF00 00D2 FF01 0000 A0FF 02DD BB77"            /* .åÿ..Òÿ... ÿ.Ý»w */
	$"E4FF 0000 D2FF 0100 00A0 FF02 DDBB 77E4"            /* äÿ..Òÿ... ÿ.Ý»wä */
	$"FF00 00D2 FF01 0000 A0FF 03DD BB77 FF00"            /* ÿ..Òÿ... ÿ.Ý»wÿ. */
	$"1981 FFD2 FF02 DDBB 7781 FFD1 FF02 DDBB"            /* .ÿÒÿ.Ý»wÿÑÿ.Ý» */
	$"7781 FFD1 FF03 DDBB 77FF 0019 81FF D2FF"            /* wÿÑÿ.Ý»wÿ..ÿÒÿ */
	$"02DD BB77 81FF D1FF 02DD BB77 81FF D1FF"            /* .Ý»wÿÑÿ.Ý»wÿÑÿ */
	$"03DD BB77 FF00 1981 FFD2 FF02 DDBB 7781"            /* .Ý»wÿ..ÿÒÿ.Ý»w */
	$"FFD1 FF02 DDBB 7781 FFD1 FF03 DDBB 77FF"            /* ÿÑÿ.Ý»wÿÑÿ.Ý»wÿ */
	$"0067 C5FF 0000 FEFF 0000 EFFF 0000 FCFF"            /* .gÅÿ..þÿ..ïÿ..üÿ */
	$"0300 FFFF 00F9 FF00 00C1 FF01 0000 FAFF"            /* ..ÿÿ.ùÿ..Áÿ...úÿ */
	$"02DD BB77 C4FF 0000 FEFF 0000 EFFF 0000"            /* .Ý»wÄÿ..þÿ..ïÿ.. */
	$"FCFF 0300 FFFF 00F9 FF00 00C1 FF01 0000"            /* üÿ..ÿÿ.ùÿ..Áÿ... */
	$"FAFF 02DD BB77 C4FF 0000 FEFF 0000 EFFF"            /* úÿ.Ý»wÄÿ..þÿ..ïÿ */
	$"0000 FCFF 0300 FFFF 00F9 FF00 00C1 FF01"            /* ..üÿ..ÿÿ.ùÿ..Áÿ. */
	$"0000 FAFF 03DD BB77 FF00 4FC1 FF00 00E9"            /* ..úÿ.Ý»wÿ.OÁÿ..é */
	$"FF00 00F6 FF00 00EA FFFE 00DC FF00 00F8"            /* ÿ..öÿ..êÿþ.Üÿ..ø */
	$"FF02 DDBB 77C0 FF00 00E9 FF00 00F6 FF00"            /* ÿ.Ý»wÀÿ..éÿ..öÿ. */
	$"00EA FFFE 00DC FF00 00F8 FF02 DDBB 77C0"            /* .êÿþ.Üÿ..øÿ.Ý»wÀ */
	$"FF00 00E9 FF00 00F6 FF00 00EA FFFE 00DC"            /* ÿ..éÿ..öÿ..êÿþ.Ü */
	$"FF00 00F8 FF03 DDBB 77FF 0172 D8FF 0100"            /* ÿ..øÿ.Ý»wÿ.rØÿ.. */
	$"00FE FFFE 0002 FF00 00FE FF01 0000 FEFF"            /* .þÿþ..ÿ..þÿ...þÿ */
	$"0000 FEFF 0000 F8FF FE00 02FF 0000 FEFF"            /* ..þÿ..øÿþ..ÿ..þÿ */
	$"0000 FEFF FE00 02FF FF00 FEFF 0100 00FE"            /* ..þÿþ..ÿÿ.þÿ...þ */
	$"FFFE 00FE FF01 0000 FEFF 0700 FF00 00FF"            /* ÿþ.þÿ...þÿ..ÿ..ÿ */
	$"FF00 00FD FF00 00FE FF00 00FE FFFE 0009"            /* ÿ..ýÿ..þÿ..þÿþ.Æ */
	$"FFFF 00FF 0000 FFFF 0000 FEFF FE00 02FF"            /* ÿÿ.ÿ..ÿÿ..þÿþ..ÿ */
	$"0000 FEFF 0100 00FC FFFE 000A FFFF 00FF"            /* ..þÿ...üÿþ..ÿÿ.ÿ */
	$"0000 FFFF DDBB 77D7 FF01 0000 FEFF FE00"            /* ..ÿÿÝ»w×ÿ...þÿþ. */
	$"02FF 0000 FEFF 0100 00FE FF00 00FE FF00"            /* .ÿ..þÿ...þÿ..þÿ. */
	$"00F8 FFFE 0002 FF00 00FE FF00 00FE FFFE"            /* .øÿþ..ÿ..þÿ..þÿþ */
	$"0002 FFFF 00FE FF01 0000 FEFF FE00 FEFF"            /* ..ÿÿ.þÿ...þÿþ.þÿ */
	$"0100 00FE FF07 00FF 0000 FFFF 0000 FDFF"            /* ...þÿ..ÿ..ÿÿ..ýÿ */
	$"0000 FEFF 0000 FEFF FE00 09FF FF00 FF00"            /* ..þÿ..þÿþ.Æÿÿ.ÿ. */
	$"00FF FF00 00FE FFFE 0002 FF00 00FE FF01"            /* .ÿÿ..þÿþ..ÿ..þÿ. */
	$"0000 FCFF FE00 0AFF FF00 FF00 00FF FFDD"            /* ..üÿþ..ÿÿ.ÿ..ÿÿÝ */
	$"BB77 D7FF 0100 00FE FFFE 0002 FF00 00FE"            /* »w×ÿ...þÿþ..ÿ..þ */
	$"FF01 0000 FEFF 0000 FEFF 0000 F8FF FE00"            /* ÿ...þÿ..þÿ..øÿþ. */
	$"02FF 0000 FEFF 0000 FEFF FE00 02FF FF00"            /* .ÿ..þÿ..þÿþ..ÿÿ. */
	$"FEFF 0100 00FE FFFE 00FE FF01 0000 FEFF"            /* þÿ...þÿþ.þÿ...þÿ */
	$"0700 FF00 00FF FF00 00FD FF00 00FE FF00"            /* ..ÿ..ÿÿ..ýÿ..þÿ. */
	$"00FE FFFE 0009 FFFF 00FF 0000 FFFF 0000"            /* .þÿþ.Æÿÿ.ÿ..ÿÿ.. */
	$"FEFF FE00 02FF 0000 FEFF 0100 00FC FFFE"            /* þÿþ..ÿ..þÿ...üÿþ */
	$"000B FFFF 00FF 0000 FFFF DDBB 77FF 0199"            /* ..ÿÿ.ÿ..ÿÿÝ»wÿ.™ */
	$"D9FF 1400 FFFF 00FF FF00 FFFF 00FF FF00"            /* Ùÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FF00 FFFF 00FF FF00 FEFF 0000 FEFF 0000"            /* ÿ.ÿÿ.ÿÿ.þÿ..þÿ.. */
	$"FCFF 2700 FFFF 00FF FF00 FFFF 00FF FF00"            /* üÿ'.ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FFFF 00FF FF00 FFFF 00FF FF00 FFFF 00FF"            /* ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"FF00 FF00 FFFF 00FF FF00 00FE FF07 00FF"            /* ÿ.ÿ.ÿÿ.ÿÿ..þÿ..ÿ */
	$"FF00 FFFF 00FF FE00 0AFF 00FF 00FF FF00"            /* ÿ.ÿÿ.ÿþ..ÿ.ÿ.ÿÿ. */
	$"FFFF 0000 FEFF 1100 FFFF 00FF FF00 FFFF"            /* ÿÿ..þÿ..ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 FF00 FFFF 00FC FF00 00FE FF01"            /* .ÿÿ.ÿ.ÿÿ.üÿ..þÿ. */
	$"0000 FDFF 02DD BB77 D8FF 1400 FFFF 00FF"            /* ..ýÿ.Ý»wØÿ..ÿÿ.ÿ */
	$"FF00 FFFF 00FF FF00 FF00 FFFF 00FF FF00"            /* ÿ.ÿÿ.ÿÿ.ÿ.ÿÿ.ÿÿ. */
	$"FEFF 0000 FEFF 0000 FCFF 2700 FFFF 00FF"            /* þÿ..þÿ..üÿ'.ÿÿ.ÿ */
	$"FF00 FFFF 00FF FF00 FFFF 00FF FF00 FFFF"            /* ÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 FFFF 00FF FF00 FF00 FFFF 00FF"            /* .ÿÿ.ÿÿ.ÿÿ.ÿ.ÿÿ.ÿ */
	$"FF00 00FE FF07 00FF FF00 FFFF 00FF FE00"            /* ÿ..þÿ..ÿÿ.ÿÿ.ÿþ. */
	$"0AFF 00FF 00FF FF00 FFFF 0000 FEFF 1100"            /* .ÿ.ÿ.ÿÿ.ÿÿ..þÿ.. */
	$"FFFF 00FF FF00 FFFF 00FF FF00 FF00 FFFF"            /* ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ.ÿÿ */
	$"00FC FF00 00FE FF01 0000 FDFF 02DD BB77"            /* .üÿ..þÿ...ýÿ.Ý»w */
	$"D8FF 1400 FFFF 00FF FF00 FFFF 00FF FF00"            /* Øÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FF00 FFFF 00FF FF00 FEFF 0000 FEFF 0000"            /* ÿ.ÿÿ.ÿÿ.þÿ..þÿ.. */
	$"FCFF 2700 FFFF 00FF FF00 FFFF 00FF FF00"            /* üÿ'.ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FFFF 00FF FF00 FFFF 00FF FF00 FFFF 00FF"            /* ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"FF00 FF00 FFFF 00FF FF00 00FE FF07 00FF"            /* ÿ.ÿ.ÿÿ.ÿÿ..þÿ..ÿ */
	$"FF00 FFFF 00FF FE00 0AFF 00FF 00FF FF00"            /* ÿ.ÿÿ.ÿþ..ÿ.ÿ.ÿÿ. */
	$"FFFF 0000 FEFF 1100 FFFF 00FF FF00 FFFF"            /* ÿÿ..þÿ..ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 FF00 FFFF 00FC FF00 00FE FF01"            /* .ÿÿ.ÿ.ÿÿ.üÿ..þÿ. */
	$"0000 FDFF 03DD BB77 FF01 6CD9 FFFD 000A"            /* ..ýÿ.Ý»wÿ.lÙÿý.. */
	$"FFFF 00FF FF00 FFFF 00FF FFFE 0002 FFFF"            /* ÿÿ.ÿÿ.ÿÿ.ÿÿþ..ÿÿ */
	$"00FE FF00 00F8 FF12 00FF FF00 FFFF 00FF"            /* .þÿ..øÿ..ÿÿ.ÿÿ.ÿ */
	$"FF00 FFFF 00FF FF00 FFFF 00FE FF01 0000"            /* ÿ.ÿÿ.ÿÿ.ÿÿ.þÿ... */
	$"FEFF 0500 FFFF 00FF FFFE 0002 FFFF 00FD"            /* þÿ..ÿÿ.ÿÿþ..ÿÿ.ý */
	$"FFFD 0010 FFFF 00FF 00FF 00FF 00FF 00FF"            /* ÿý..ÿÿ.ÿ.ÿ.ÿ.ÿ.ÿ */
	$"FF00 FFFF 00FC FFFE 0009 FFFF 00FF FF00"            /* ÿ.ÿÿ.üÿþ.Æÿÿ.ÿÿ. */
	$"FFFF 00FF FD00 FCFF 0000 FEFF 0000 FCFF"            /* ÿÿ.ÿý.üÿ..þÿ..üÿ */
	$"02DD BB77 D8FF FD00 0AFF FF00 FFFF 00FF"            /* .Ý»wØÿý..ÿÿ.ÿÿ.ÿ */
	$"FF00 FFFF FE00 02FF FF00 FEFF 0000 F8FF"            /* ÿ.ÿÿþ..ÿÿ.þÿ..øÿ */
	$"1200 FFFF 00FF FF00 FFFF 00FF FF00 FFFF"            /* ..ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 FEFF 0100 00FE FF05 00FF FF00"            /* .ÿÿ.þÿ...þÿ..ÿÿ. */
	$"FFFF FE00 02FF FF00 FDFF FD00 10FF FF00"            /* ÿÿþ..ÿÿ.ýÿý..ÿÿ. */
	$"FF00 FF00 FF00 FF00 FFFF 00FF FF00 FCFF"            /* ÿ.ÿ.ÿ.ÿ.ÿÿ.ÿÿ.üÿ */
	$"FE00 09FF FF00 FFFF 00FF FF00 FFFD 00FC"            /* þ.Æÿÿ.ÿÿ.ÿÿ.ÿý.ü */
	$"FF00 00FE FF00 00FC FF02 DDBB 77D8 FFFD"            /* ÿ..þÿ..üÿ.Ý»wØÿý */
	$"000A FFFF 00FF FF00 FFFF 00FF FFFE 0002"            /* ..ÿÿ.ÿÿ.ÿÿ.ÿÿþ.. */
	$"FFFF 00FE FF00 00F8 FF12 00FF FF00 FFFF"            /* ÿÿ.þÿ..øÿ..ÿÿ.ÿÿ */
	$"00FF FF00 FFFF 00FF FF00 FFFF 00FE FF01"            /* .ÿÿ.ÿÿ.ÿÿ.ÿÿ.þÿ. */
	$"0000 FEFF 0500 FFFF 00FF FFFE 0002 FFFF"            /* ..þÿ..ÿÿ.ÿÿþ..ÿÿ */
	$"00FD FFFD 0010 FFFF 00FF 00FF 00FF 00FF"            /* .ýÿý..ÿÿ.ÿ.ÿ.ÿ.ÿ */
	$"00FF FF00 FFFF 00FC FFFE 0009 FFFF 00FF"            /* .ÿÿ.ÿÿ.üÿþ.Æÿÿ.ÿ */
	$"FF00 FFFF 00FF FD00 FCFF 0000 FEFF 0000"            /* ÿ.ÿÿ.ÿý.üÿ..þÿ.. */
	$"FCFF 03DD BB77 FF01 66D9 FF00 00FC FF0E"            /* üÿ.Ý»wÿ.fÙÿ..üÿ. */
	$"00FF FF00 FFFF 00FF 00FF FF00 FFFF 00FE"            /* .ÿÿ.ÿÿ.ÿ.ÿÿ.ÿÿ.þ */
	$"FF00 00F8 FF12 00FF FF00 FFFF 00FF FF00"            /* ÿ..øÿ..ÿÿ.ÿÿ.ÿÿ. */
	$"FFFF 00FF FF00 FFFF 00FC FF0E 00FF FF00"            /* ÿÿ.ÿÿ.ÿÿ.üÿ..ÿÿ. */
	$"FFFF 00FF 00FF FF00 FFFF 00FD FF00 00FC"            /* ÿÿ.ÿ.ÿÿ.ÿÿ.ýÿ..ü */
	$"FF01 00FF FD00 08FF FF00 FFFF 00FF FF00"            /* ÿ..ÿý..ÿÿ.ÿÿ.ÿÿ. */
	$"FDFF 0E00 FFFF 00FF FF00 FFFF 00FF FF00"            /* ýÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FF00 F9FF 0000 FEFF 0000 FCFF 02DD BB77"            /* ÿ.ùÿ..þÿ..üÿ.Ý»w */
	$"D8FF 0000 FCFF 0E00 FFFF 00FF FF00 FF00"            /* Øÿ..üÿ..ÿÿ.ÿÿ.ÿ. */
	$"FFFF 00FF FF00 FEFF 0000 F8FF 1200 FFFF"            /* ÿÿ.ÿÿ.þÿ..øÿ..ÿÿ */
	$"00FF FF00 FFFF 00FF FF00 FFFF 00FF FF00"            /* .ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FCFF 0E00 FFFF 00FF FF00 FF00 FFFF 00FF"            /* üÿ..ÿÿ.ÿÿ.ÿ.ÿÿ.ÿ */
	$"FF00 FDFF 0000 FCFF 0100 FFFD 0008 FFFF"            /* ÿ.ýÿ..üÿ..ÿý..ÿÿ */
	$"00FF FF00 FFFF 00FD FF0E 00FF FF00 FFFF"            /* .ÿÿ.ÿÿ.ýÿ..ÿÿ.ÿÿ */
	$"00FF FF00 FFFF 00FF 00F9 FF00 00FE FF00"            /* .ÿÿ.ÿÿ.ÿ.ùÿ..þÿ. */
	$"00FC FF02 DDBB 77D8 FF00 00FC FF0E 00FF"            /* .üÿ.Ý»wØÿ..üÿ..ÿ */
	$"FF00 FFFF 00FF 00FF FF00 FFFF 00FE FF00"            /* ÿ.ÿÿ.ÿ.ÿÿ.ÿÿ.þÿ. */
	$"00F8 FF12 00FF FF00 FFFF 00FF FF00 FFFF"            /* .øÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 FFFF 00FC FF0E 00FF FF00 FFFF"            /* .ÿÿ.ÿÿ.üÿ..ÿÿ.ÿÿ */
	$"00FF 00FF FF00 FFFF 00FD FF00 00FC FF01"            /* .ÿ.ÿÿ.ÿÿ.ýÿ..üÿ. */
	$"00FF FD00 08FF FF00 FFFF 00FF FF00 FDFF"            /* .ÿý..ÿÿ.ÿÿ.ÿÿ.ýÿ */
	$"0E00 FFFF 00FF FF00 FFFF 00FF FF00 FF00"            /* ..ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ. */
	$"F9FF 0000 FEFF 0000 FCFF 03DD BB77 FF01"            /* ùÿ..þÿ..üÿ.Ý»wÿ. */
	$"7BD9 FF14 00FF FF00 FFFF 00FF FF00 FFFF"            /* {Ùÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ */
	$"00FF 00FF FF00 FFFF 00FE FF00 00F8 FF26"            /* .ÿ.ÿÿ.ÿÿ.þÿ..øÿ& */
	$"00FF FF00 FFFF 00FF FF00 FFFF 00FF FF00"            /* .ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ. */
	$"FFFF 00FF FF00 FFFF 00FF FF00 FFFF 00FF"            /* ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"00FF FF00 FFFF 00FD FF03 00FF FF00 FEFF"            /* .ÿÿ.ÿÿ.ýÿ..ÿÿ.þÿ */
	$"0000 FBFF 0600 FFFF 00FF FF00 FDFF 1100"            /* ..ûÿ..ÿÿ.ÿÿ.ýÿ.. */
	$"FFFF 00FF FF00 FFFF 00FF FF00 FF00 FFFF"            /* ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ.ÿÿ */
	$"00FC FF00 00FE FF00 00FC FF02 DDBB 77D8"            /* .üÿ..þÿ..üÿ.Ý»wØ */
	$"FF14 00FF FF00 FFFF 00FF FF00 FFFF 00FF"            /* ÿ..ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"00FF FF00 FFFF 00FE FF00 00F8 FF26 00FF"            /* .ÿÿ.ÿÿ.þÿ..øÿ&.ÿ */
	$"FF00 FFFF 00FF FF00 FFFF 00FF FF00 FFFF"            /* ÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ */
	$"00FF FF00 FFFF 00FF FF00 FFFF 00FF 00FF"            /* .ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ.ÿ */
	$"FF00 FFFF 00FD FF03 00FF FF00 FEFF 0000"            /* ÿ.ÿÿ.ýÿ..ÿÿ.þÿ.. */
	$"FBFF 0600 FFFF 00FF FF00 FDFF 1100 FFFF"            /* ûÿ..ÿÿ.ÿÿ.ýÿ..ÿÿ */
	$"00FF FF00 FFFF 00FF FF00 FF00 FFFF 00FC"            /* .ÿÿ.ÿÿ.ÿÿ.ÿ.ÿÿ.ü */
	$"FF00 00FE FF00 00FC FF02 DDBB 77D8 FF14"            /* ÿ..þÿ..üÿ.Ý»wØÿ. */
	$"00FF FF00 FFFF 00FF FF00 FFFF 00FF 00FF"            /* .ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ.ÿ */
	$"FF00 FFFF 00FE FF00 00F8 FF26 00FF FF00"            /* ÿ.ÿÿ.þÿ..øÿ&.ÿÿ. */
	$"FFFF 00FF FF00 FFFF 00FF FF00 FFFF 00FF"            /* ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿÿ.ÿ */
	$"FF00 FFFF 00FF FF00 FFFF 00FF 00FF FF00"            /* ÿ.ÿÿ.ÿÿ.ÿÿ.ÿ.ÿÿ. */
	$"FFFF 00FD FF03 00FF FF00 FEFF 0000 FBFF"            /* ÿÿ.ýÿ..ÿÿ.þÿ..ûÿ */
	$"0600 FFFF 00FF FF00 FDFF 1100 FFFF 00FF"            /* ..ÿÿ.ÿÿ.ýÿ..ÿÿ.ÿ */
	$"FF00 FFFF 00FF FF00 FF00 FFFF 00FC FF00"            /* ÿ.ÿÿ.ÿÿ.ÿ.ÿÿ.üÿ. */
	$"00FE FF00 00FC FF03 DDBB 77FF 0178 D8FF"            /* .þÿ..üÿ.Ý»wÿ.xØÿ */
	$"0100 00FE FF08 00FF FF00 FFFF 00FF FFFE"            /* ...þÿ..ÿÿ.ÿÿ.ÿÿþ */
	$"0002 FFFF 00FE FF00 00FE FF00 00FC FF09"            /* ..ÿÿ.þÿ..þÿ..üÿÆ */
	$"00FF FF00 FFFF 00FF FF00 FEFF FE00 02FF"            /* .ÿÿ.ÿÿ.ÿÿ.þÿþ..ÿ */
	$"FF00 FEFF 0100 00FE FF05 00FF FF00 FFFF"            /* ÿ.þÿ...þÿ..ÿÿ.ÿÿ */
	$"FE00 02FF FF00 FCFF 0100 00FC FF01 0000"            /* þ..ÿÿ.üÿ...üÿ... */
	$"FCFF FE00 02FF FF00 FCFF FE00 0CFF FF00"            /* üÿþ..ÿÿ.üÿþ..ÿÿ. */
	$"FFFF 00FF FF00 FFFF 0000 FEFF 0300 FFFF"            /* ÿÿ.ÿÿ.ÿÿ..þÿ..ÿÿ */
	$"00FE FF00 00FC FF02 DDBB 77D7 FF01 0000"            /* .þÿ..üÿ.Ý»w×ÿ... */
	$"FEFF 0800 FFFF 00FF FF00 FFFF FE00 02FF"            /* þÿ..ÿÿ.ÿÿ.ÿÿþ..ÿ */
	$"FF00 FEFF 0000 FEFF 0000 FCFF 0900 FFFF"            /* ÿ.þÿ..þÿ..üÿÆ.ÿÿ */
	$"00FF FF00 FFFF 00FE FFFE 0002 FFFF 00FE"            /* .ÿÿ.ÿÿ.þÿþ..ÿÿ.þ */
	$"FF01 0000 FEFF 0500 FFFF 00FF FFFE 0002"            /* ÿ...þÿ..ÿÿ.ÿÿþ.. */
	$"FFFF 00FC FF01 0000 FCFF 0100 00FC FFFE"            /* ÿÿ.üÿ...üÿ...üÿþ */
	$"0002 FFFF 00FC FFFE 000C FFFF 00FF FF00"            /* ..ÿÿ.üÿþ..ÿÿ.ÿÿ. */
	$"FFFF 00FF FF00 00FE FF03 00FF FF00 FEFF"            /* ÿÿ.ÿÿ..þÿ..ÿÿ.þÿ */
	$"0000 FCFF 02DD BB77 D7FF 0100 00FE FF08"            /* ..üÿ.Ý»w×ÿ...þÿ. */
	$"00FF FF00 FFFF 00FF FFFE 0002 FFFF 00FE"            /* .ÿÿ.ÿÿ.ÿÿþ..ÿÿ.þ */
	$"FF00 00FE FF00 00FC FF09 00FF FF00 FFFF"            /* ÿ..þÿ..üÿÆ.ÿÿ.ÿÿ */
	$"00FF FF00 FEFF FE00 02FF FF00 FEFF 0100"            /* .ÿÿ.þÿþ..ÿÿ.þÿ.. */
	$"00FE FF05 00FF FF00 FFFF FE00 02FF FF00"            /* .þÿ..ÿÿ.ÿÿþ..ÿÿ. */
	$"FCFF 0100 00FC FF01 0000 FCFF FE00 02FF"            /* üÿ...üÿ...üÿþ..ÿ */
	$"FF00 FCFF FE00 0CFF FF00 FFFF 00FF FF00"            /* ÿ.üÿþ..ÿÿ.ÿÿ.ÿÿ. */
	$"FFFF 0000 FEFF 0300 FFFF 00FE FF00 00FC"            /* ÿÿ..þÿ..ÿÿ.þÿ..ü */
	$"FF03 DDBB 77FF 0025 81FF FBFF 0000 D9FF"            /* ÿ.Ý»wÿ.%ÿûÿ..Ùÿ */
	$"02DD BB77 81FF FAFF 0000 D9FF 02DD BB77"            /* .Ý»wÿúÿ..Ùÿ.Ý»w */
	$"81FF FAFF 0000 D9FF 03DD BB77 FF00 2881"            /* ÿúÿ..Ùÿ.Ý»wÿ.( */
	$"FFFD FF01 0000 D8FF 02DD BB77 81FF FCFF"            /* ÿýÿ...Øÿ.Ý»wÿüÿ */
	$"0100 00D8 FF02 DDBB 7781 FFFC FF01 0000"            /* ...Øÿ.Ý»wÿüÿ... */
	$"D8FF 03DD BB77 FF00 1981 FFD2 FF02 DDBB"            /* Øÿ.Ý»wÿ..ÿÒÿ.Ý» */
	$"7781 FFD1 FF02 DDBB 7781 FFD1 FF03 DDBB"            /* wÿÑÿ.Ý»wÿÑÿ.Ý» */
	$"77FF 0019 81FF D2FF 02DD BB77 81FF D1FF"            /* wÿ..ÿÒÿ.Ý»wÿÑÿ */
	$"02DD BB77 81FF D1FF 03DD BB77 FF00 1981"            /* .Ý»wÿÑÿ.Ý»wÿ.. */
	$"FFD2 FF02 DDBB 7781 FFD1 FF02 DDBB 7781"            /* ÿÒÿ.Ý»wÿÑÿ.Ý»w */
	$"FFD1 FF03 DDBB 77FF 0019 81FF D2FF 02DD"            /* ÿÑÿ.Ý»wÿ..ÿÒÿ.Ý */
	$"BB77 81FF D1FF 02DD BB77 81FF D1FF 03DD"            /* »wÿÑÿ.Ý»wÿÑÿ.Ý */
	$"BB77 FF00 1981 FFD2 FF02 DDBB 7781 FFD1"            /* »wÿ..ÿÒÿ.Ý»wÿÑ */
	$"FF02 DDBB 7781 FFD1 FF03 DDBB 77FF 0019"            /* ÿ.Ý»wÿÑÿ.Ý»wÿ.. */
	$"81FF D2FF 02DD BB77 81FF D1FF 02DD BB77"            /* ÿÒÿ.Ý»wÿÑÿ.Ý»w */
	$"81FF D1FF 03DD BB77 FF00 1981 FFD2 FF02"            /* ÿÑÿ.Ý»wÿ..ÿÒÿ. */
	$"DDBB 7781 FFD1 FF02 DDBB 7781 FFD1 FF03"            /* Ý»wÿÑÿ.Ý»wÿÑÿ. */
	$"DDBB 77FF 0019 81FF D2FF 02DD BB77 81FF"            /* Ý»wÿ..ÿÒÿ.Ý»wÿ */
	$"D1FF 02DD BB77 81FF D1FF 03DD BB77 FF00"            /* Ñÿ.Ý»wÿÑÿ.Ý»wÿ. */
	$"1981 FFD2 FF02 DDBB 7781 FFD1 FF02 DDBB"            /* .ÿÒÿ.Ý»wÿÑÿ.Ý» */
	$"7781 FFD1 FF03 DDBB 77FF 0019 81FF D2FF"            /* wÿÑÿ.Ý»wÿ..ÿÒÿ */
	$"02DD BB77 81FF D1FF 02DD BB77 81FF D1FF"            /* .Ý»wÿÑÿ.Ý»wÿÑÿ */
	$"03DD BB77 FF00 C1BA FFFC 00F7 FFFD 0000"            /* .Ý»wÿ.Áºÿü.÷ÿý.. */
	$"FFFD 00FC FF02 00FF FFFE 00FD FFF9 00FB"            /* ÿý.üÿ..ÿÿþ.ýÿù.û */
	$"FF00 00FD FFFE 00FE FFFE 0001 FFFF FC00"            /* ÿ..ýÿþ.þÿþ..ÿÿü. */
	$"FCFF 0100 00FD FFFE 00FE FFFE 00FE FFFE"            /* üÿ...ýÿþ.þÿþ.þÿþ */
	$"00FD FF02 DDBB 77B9 FFFC 00F7 FFFD 0000"            /* .ýÿ.Ý»w¹ÿü.÷ÿý.. */
	$"FFFD 00FC FF02 00FF FFFE 00FD FFF9 00FB"            /* ÿý.üÿ..ÿÿþ.ýÿù.û */
	$"FF00 00FD FFFE 00FE FFFE 0001 FFFF FC00"            /* ÿ..ýÿþ.þÿþ..ÿÿü. */
	$"FCFF 0100 00FD FFFE 00FE FFFE 00FE FFFE"            /* üÿ...ýÿþ.þÿþ.þÿþ */
	$"00FD FF02 DDBB 77B9 FFFC 00F7 FFFD 0000"            /* .ýÿ.Ý»w¹ÿü.÷ÿý.. */
	$"FFFD 00FC FF02 00FF FFFE 00FD FFF9 00FB"            /* ÿý.üÿ..ÿÿþ.ýÿù.û */
	$"FF00 00FD FFFE 00FE FFFE 0001 FFFF FC00"            /* ÿ..ýÿþ.þÿþ..ÿÿü. */
	$"FCFF 0100 00FD FFFE 00FE FFFE 00FE FFFE"            /* üÿ...ýÿþ.þÿþ.þÿþ */
	$"00FD FF03 DDBB 77FF 012A BBFF 0100 00FE"            /* .ýÿ.Ý»wÿ.*»ÿ...þ */
	$"FF01 0000 F9FF 0000 FEFF 0600 FFFF 00FF"            /* ÿ...ùÿ..þÿ..ÿÿ.ÿ */
	$"FF00 FDFF 0000 FEFF 0100 00FE FF04 0000"            /* ÿ.ýÿ..þÿ...þÿ... */
	$"FFFF 00FE FF00 00FD FFFE 00FE FF00 00FE"            /* ÿÿ.þÿ..ýÿþ.þÿ..þ */
	$"FF02 00FF 00FE FF00 00FC FF00 00FD FF06"            /* ÿ..ÿ.þÿ..üÿ..ýÿ. */
	$"00FF FF00 FFFF 00FE FF02 00FF 00FE FF02"            /* .ÿÿ.ÿÿ.þÿ..ÿ.þÿ. */
	$"00FF 00FE FF00 00FE FF02 DDBB 77BA FF01"            /* .ÿ.þÿ..þÿ.Ý»wºÿ. */
	$"0000 FEFF 0100 00F9 FF00 00FE FF06 00FF"            /* ..þÿ...ùÿ..þÿ..ÿ */
	$"FF00 FFFF 00FD FF00 00FE FF01 0000 FEFF"            /* ÿ.ÿÿ.ýÿ..þÿ...þÿ */
	$"0400 00FF FF00 FEFF 0000 FDFF FE00 FEFF"            /* ...ÿÿ.þÿ..ýÿþ.þÿ */
	$"0000 FEFF 0200 FF00 FEFF 0000 FCFF 0000"            /* ..þÿ..ÿ.þÿ..üÿ.. */
	$"FDFF 0600 FFFF 00FF FF00 FEFF 0200 FF00"            /* ýÿ..ÿÿ.ÿÿ.þÿ..ÿ. */
	$"FEFF 0200 FF00 FEFF 0000 FEFF 02DD BB77"            /* þÿ..ÿ.þÿ..þÿ.Ý»w */
	$"BAFF 0100 00FE FF01 0000 F9FF 0000 FEFF"            /* ºÿ...þÿ...ùÿ..þÿ */
	$"0600 FFFF 00FF FF00 FDFF 0000 FEFF 0100"            /* ..ÿÿ.ÿÿ.ýÿ..þÿ.. */
	$"00FE FF04 0000 FFFF 00FE FF00 00FD FFFE"            /* .þÿ...ÿÿ.þÿ..ýÿþ */
	$"00FE FF00 00FE FF02 00FF 00FE FF00 00FC"            /* .þÿ..þÿ..ÿ.þÿ..ü */
	$"FF00 00FD FF06 00FF FF00 FFFF 00FE FF02"            /* ÿ..ýÿ..ÿÿ.ÿÿ.þÿ. */
	$"00FF 00FE FF02 00FF 00FE FF00 00FE FF03"            /* .ÿ.þÿ..ÿ.þÿ..þÿ. */
	$"DDBB 77FF 011B BCFF 0200 00FF FE00 02FF"            /* Ý»wÿ..¼ÿ...ÿþ..ÿ */
	$"0000 FBFF 0000 FDFF 0600 FFFF 00FF FF00"            /* ..ûÿ..ýÿ..ÿÿ.ÿÿ. */
	$"FDFF 0000 FEFF 0900 FF00 FFFF 0000 FFFF"            /* ýÿ..þÿÆ.ÿ.ÿÿ..ÿÿ */
	$"00F7 FF00 00FE FF00 00FE FF02 00FF 00FE"            /* .÷ÿ..þÿ..þÿ..ÿ.þ */
	$"FF00 00FD FF00 00F9 FF03 00FF FF00 FEFF"            /* ÿ..ýÿ..ùÿ..ÿÿ.þÿ */
	$"0200 FF00 FEFF 0200 FF00 FEFF 0000 FEFF"            /* ..ÿ.þÿ..ÿ.þÿ..þÿ */
	$"02DD BB77 BBFF 0200 00FF FE00 02FF 0000"            /* .Ý»w»ÿ...ÿþ..ÿ.. */
	$"FBFF 0000 FDFF 0600 FFFF 00FF FF00 FDFF"            /* ûÿ..ýÿ..ÿÿ.ÿÿ.ýÿ */
	$"0000 FEFF 0900 FF00 FFFF 0000 FFFF 00F7"            /* ..þÿÆ.ÿ.ÿÿ..ÿÿ.÷ */
	$"FF00 00FE FF00 00FE FF02 00FF 00FE FF00"            /* ÿ..þÿ..þÿ..ÿ.þÿ. */
	$"00FD FF00 00F9 FF03 00FF FF00 FEFF 0200"            /* .ýÿ..ùÿ..ÿÿ.þÿ.. */
	$"FF00 FEFF 0200 FF00 FEFF 0000 FEFF 02DD"            /* ÿ.þÿ..ÿ.þÿ..þÿ.Ý */
	$"BB77 BBFF 0200 00FF FE00 02FF 0000 FBFF"            /* »w»ÿ...ÿþ..ÿ..ûÿ */
	$"0000 FDFF 0600 FFFF 00FF FF00 FDFF 0000"            /* ..ýÿ..ÿÿ.ÿÿ.ýÿ.. */
	$"FEFF 0900 FF00 FFFF 0000 FFFF 00F7 FF00"            /* þÿÆ.ÿ.ÿÿ..ÿÿ.÷ÿ. */
	$"00FE FF00 00FE FF02 00FF 00FE FF00 00FD"            /* .þÿ..þÿ..ÿ.þÿ..ý */
	$"FF00 00F9 FF03 00FF FF00 FEFF 0200 FF00"            /* ÿ..ùÿ..ÿÿ.þÿ..ÿ. */
	$"FEFF 0200 FF00 FEFF 0000 FEFF 03DD BB77"            /* þÿ..ÿ.þÿ..þÿ.Ý»w */
	$"FF01 12BC FF08 00FF 00FF FF00 FFFF 00FB"            /* ÿ..¼ÿ..ÿ.ÿÿ.ÿÿ.û */
	$"FF00 00FA FFFE 00FD FF11 00FF 00FF FF00"            /* ÿ..úÿþ.ýÿ..ÿ.ÿÿ. */
	$"FF00 FF00 FF00 FFFF 00FF FF00 FAFF 0000"            /* ÿ.ÿ.ÿ.ÿÿ.ÿÿ.úÿ.. */
	$"FEFF 0000 FEFF 0500 FF00 00FF 00FC FF00"            /* þÿ..þÿ..ÿ..ÿ.üÿ. */
	$"00F9 FF03 00FF FF00 FEFF 0200 FF00 FEFF"            /* .ùÿ..ÿÿ.þÿ..ÿ.þÿ */
	$"0200 FF00 FEFF 0000 FEFF 02DD BB77 BBFF"            /* ..ÿ.þÿ..þÿ.Ý»w»ÿ */
	$"0800 FF00 FFFF 00FF FF00 FBFF 0000 FAFF"            /* ..ÿ.ÿÿ.ÿÿ.ûÿ..úÿ */
	$"FE00 FDFF 1100 FF00 FFFF 00FF 00FF 00FF"            /* þ.ýÿ..ÿ.ÿÿ.ÿ.ÿ.ÿ */
	$"00FF FF00 FFFF 00FA FF00 00FE FF00 00FE"            /* .ÿÿ.ÿÿ.úÿ..þÿ..þ */
	$"FF05 00FF 0000 FF00 FCFF 0000 F9FF 0300"            /* ÿ..ÿ..ÿ.üÿ..ùÿ.. */
	$"FFFF 00FE FF02 00FF 00FE FF02 00FF 00FE"            /* ÿÿ.þÿ..ÿ.þÿ..ÿ.þ */
	$"FF00 00FE FF02 DDBB 77BB FF08 00FF 00FF"            /* ÿ..þÿ.Ý»w»ÿ..ÿ.ÿ */
	$"FF00 FFFF 00FB FF00 00FA FFFE 00FD FF11"            /* ÿ.ÿÿ.ûÿ..úÿþ.ýÿ. */
	$"00FF 00FF FF00 FF00 FF00 FF00 FFFF 00FF"            /* .ÿ.ÿÿ.ÿ.ÿ.ÿ.ÿÿ.ÿ */
	$"FF00 FAFF 0000 FEFF 0000 FEFF 0500 FF00"            /* ÿ.úÿ..þÿ..þÿ..ÿ. */
	$"00FF 00FC FF00 00F9 FF03 00FF FF00 FEFF"            /* .ÿ.üÿ..ùÿ..ÿÿ.þÿ */
	$"0200 FF00 FEFF 0200 FF00 FEFF 0000 FEFF"            /* ..ÿ.þÿ..ÿ.þÿ..þÿ */
	$"03DD BB77 FF01 0FBC FF02 00FF 00FC FF00"            /* .Ý»wÿ..¼ÿ..ÿ.üÿ. */
	$"00FB FF00 00FA FF02 00FF 00FD FF0D 00FF"            /* .ûÿ..úÿ..ÿ.ýÿÂ.ÿ */
	$"00FF FF00 FF00 FF00 FF00 FFFF FD00 FAFF"            /* .ÿÿ.ÿ.ÿ.ÿ.ÿÿý.úÿ */
	$"0000 FEFF 0000 FEFF 0000 FEFF 0100 00FC"            /* ..þÿ..þÿ..þÿ...ü */
	$"FF00 00FA FF00 00FE FF00 00FE FF02 00FF"            /* ÿ..úÿ..þÿ..þÿ..ÿ */
	$"00FE FF02 00FF 00FE FF00 00FE FF02 DDBB"            /* .þÿ..ÿ.þÿ..þÿ.Ý» */
	$"77BB FF02 00FF 00FC FF00 00FB FF00 00FA"            /* w»ÿ..ÿ.üÿ..ûÿ..ú */
	$"FF02 00FF 00FD FF0D 00FF 00FF FF00 FF00"            /* ÿ..ÿ.ýÿÂ.ÿ.ÿÿ.ÿ. */
	$"FF00 FF00 FFFF FD00 FAFF 0000 FEFF 0000"            /* ÿ.ÿ.ÿÿý.úÿ..þÿ.. */
	$"FEFF 0000 FEFF 0100 00FC FF00 00FA FF00"            /* þÿ..þÿ...üÿ..úÿ. */
	$"00FE FF00 00FE FF02 00FF 00FE FF02 00FF"            /* .þÿ..þÿ..ÿ.þÿ..ÿ */
	$"00FE FF00 00FE FF02 DDBB 77BB FF02 00FF"            /* .þÿ..þÿ.Ý»w»ÿ..ÿ */
	$"00FC FF00 00FB FF00 00FA FF02 00FF 00FD"            /* .üÿ..ûÿ..úÿ..ÿ.ý */
	$"FF0D 00FF 00FF FF00 FF00 FF00 FF00 FFFF"            /* ÿÂ.ÿ.ÿÿ.ÿ.ÿ.ÿ.ÿÿ */
	$"FD00 FAFF 0000 FEFF 0000 FEFF 0000 FEFF"            /* ý.úÿ..þÿ..þÿ..þÿ */
	$"0100 00FC FF00 00FA FF00 00FE FF00 00FE"            /* ...üÿ..úÿ..þÿ..þ */
	$"FF02 00FF 00FE FF02 00FF 00FE FF00 00FE"            /* ÿ..ÿ.þÿ..ÿ.þÿ..þ */
	$"FF03 DDBB 77FF 012D BCFF 0800 FF00 FFFF"            /* ÿ.Ý»wÿ.-¼ÿ..ÿ.ÿÿ */
	$"00FF FF00 FBFF 0000 FEFF FE00 03FF 00FF"            /* .ÿÿ.ûÿ..þÿþ..ÿ.ÿ */
	$"00FD FFFE 000E FFFF 00FF 00FF 00FF 00FF"            /* .ýÿþ..ÿÿ.ÿ.ÿ.ÿ.ÿ */
	$"FF00 FFFF 00FA FF00 00FD FF09 0000 FF00"            /* ÿ.ÿÿ.úÿ..ýÿÆ..ÿ. */
	$"FF00 00FF FF00 FEFF 0200 FFFF FE00 02FF"            /* ÿ..ÿÿ.þÿ..ÿÿþ..ÿ */
	$"FF00 FDFF 0000 FEFF 0200 FF00 FEFF 0200"            /* ÿ.ýÿ..þÿ..ÿ.þÿ.. */
	$"FF00 FEFF 0000 FEFF 02DD BB77 BBFF 0800"            /* ÿ.þÿ..þÿ.Ý»w»ÿ.. */
	$"FF00 FFFF 00FF FF00 FBFF 0000 FEFF FE00"            /* ÿ.ÿÿ.ÿÿ.ûÿ..þÿþ. */
	$"03FF 00FF 00FD FFFE 000E FFFF 00FF 00FF"            /* .ÿ.ÿ.ýÿþ..ÿÿ.ÿ.ÿ */
	$"00FF 00FF FF00 FFFF 00FA FF00 00FD FF09"            /* .ÿ.ÿÿ.ÿÿ.úÿ..ýÿÆ */
	$"0000 FF00 FF00 00FF FF00 FEFF 0200 FFFF"            /* ..ÿ.ÿ..ÿÿ.þÿ..ÿÿ */
	$"FE00 02FF FF00 FDFF 0000 FEFF 0200 FF00"            /* þ..ÿÿ.ýÿ..þÿ..ÿ. */
	$"FEFF 0200 FF00 FEFF 0000 FEFF 02DD BB77"            /* þÿ..ÿ.þÿ..þÿ.Ý»w */
	$"BBFF 0800 FF00 FFFF 00FF FF00 FBFF 0000"            /* »ÿ..ÿ.ÿÿ.ÿÿ.ûÿ.. */
	$"FEFF FE00 03FF 00FF 00FD FFFE 000E FFFF"            /* þÿþ..ÿ.ÿ.ýÿþ..ÿÿ */
	$"00FF 00FF 00FF 00FF FF00 FFFF 00FA FF00"            /* .ÿ.ÿ.ÿ.ÿÿ.ÿÿ.úÿ. */
	$"00FD FF09 0000 FF00 FF00 00FF FF00 FEFF"            /* .ýÿÆ..ÿ.ÿ..ÿÿ.þÿ */
	$"0200 FFFF FE00 02FF FF00 FDFF 0000 FEFF"            /* ..ÿÿþ..ÿÿ.ýÿ..þÿ */
	$"0200 FF00 FEFF 0200 FF00 FEFF 0000 FEFF"            /* ..ÿ.þÿ..ÿ.þÿ..þÿ */
	$"03DD BB77 FF01 15BB FF07 00FF 0000 FFFF"            /* .Ý»wÿ..»ÿ..ÿ..ÿÿ */
	$"0000 FBFF 0000 FDFF 0900 FFFF 00FF FF00"            /* ..ûÿ..ýÿÆ.ÿÿ.ÿÿ. */
	$"FFFF 00FE FF0B 00FF 00FF FF00 FFFF 00FF"            /* ÿÿ.þÿ..ÿ.ÿÿ.ÿÿ.ÿ */
	$"FF00 F7FF 0000 FBFF 0300 FFFF 00FE FF00"            /* ÿ.÷ÿ..ûÿ..ÿÿ.þÿ. */
	$"00FE FF00 00FA FF00 00FD FF00 00FE FF02"            /* .þÿ..úÿ..ýÿ..þÿ. */
	$"00FF 00FE FF02 00FF 00FE FF00 00FE FF02"            /* .ÿ.þÿ..ÿ.þÿ..þÿ. */
	$"DDBB 77BA FF07 00FF 0000 FFFF 0000 FBFF"            /* Ý»wºÿ..ÿ..ÿÿ..ûÿ */
	$"0000 FDFF 0900 FFFF 00FF FF00 FFFF 00FE"            /* ..ýÿÆ.ÿÿ.ÿÿ.ÿÿ.þ */
	$"FF0B 00FF 00FF FF00 FFFF 00FF FF00 F7FF"            /* ÿ..ÿ.ÿÿ.ÿÿ.ÿÿ.÷ÿ */
	$"0000 FBFF 0300 FFFF 00FE FF00 00FE FF00"            /* ..ûÿ..ÿÿ.þÿ..þÿ. */
	$"00FA FF00 00FD FF00 00FE FF02 00FF 00FE"            /* .úÿ..ýÿ..þÿ..ÿ.þ */
	$"FF02 00FF 00FE FF00 00FE FF02 DDBB 77BA"            /* ÿ..ÿ.þÿ..þÿ.Ý»wº */
	$"FF07 00FF 0000 FFFF 0000 FBFF 0000 FDFF"            /* ÿ..ÿ..ÿÿ..ûÿ..ýÿ */
	$"0900 FFFF 00FF FF00 FFFF 00FE FF0B 00FF"            /* Æ.ÿÿ.ÿÿ.ÿÿ.þÿ..ÿ */
	$"00FF FF00 FFFF 00FF FF00 F7FF 0000 FBFF"            /* .ÿÿ.ÿÿ.ÿÿ.÷ÿ..ûÿ */
	$"0300 FFFF 00FE FF00 00FE FF00 00FA FF00"            /* ..ÿÿ.þÿ..þÿ..úÿ. */
	$"00FD FF00 00FE FF02 00FF 00FE FF02 00FF"            /* .ýÿ..þÿ..ÿ.þÿ..ÿ */
	$"00FE FF00 00FE FF03 DDBB 77FF 0124 BBFF"            /* .þÿ..þÿ.Ý»wÿ.$»ÿ */
	$"0100 00FE FF01 0000 F9FF 0000 FEFF 0900"            /* ...þÿ...ùÿ..þÿÆ. */
	$"FFFF 00FF FF00 FFFF 00FE FF0B 00FF 00FF"            /* ÿÿ.ÿÿ.ÿÿ.þÿ..ÿ.ÿ */
	$"FF00 FFFF 00FF FF00 FEFF 0000 FBFF 0000"            /* ÿ.ÿÿ.ÿÿ.þÿ..ûÿ.. */
	$"FBFF 0300 FFFF 00FE FF00 00FE FF00 00FB"            /* ûÿ..ÿÿ.þÿ..þÿ..û */
	$"FF06 00FF FF00 FFFF 00FE FF02 00FF 00FE"            /* ÿ..ÿÿ.ÿÿ.þÿ..ÿ.þ */
	$"FF02 00FF 00FE FF00 00FE FF02 DDBB 77BA"            /* ÿ..ÿ.þÿ..þÿ.Ý»wº */
	$"FF01 0000 FEFF 0100 00F9 FF00 00FE FF09"            /* ÿ...þÿ...ùÿ..þÿÆ */
	$"00FF FF00 FFFF 00FF FF00 FEFF 0B00 FF00"            /* .ÿÿ.ÿÿ.ÿÿ.þÿ..ÿ. */
	$"FFFF 00FF FF00 FFFF 00FE FF00 00FB FF00"            /* ÿÿ.ÿÿ.ÿÿ.þÿ..ûÿ. */
	$"00FB FF03 00FF FF00 FEFF 0000 FEFF 0000"            /* .ûÿ..ÿÿ.þÿ..þÿ.. */
	$"FBFF 0600 FFFF 00FF FF00 FEFF 0200 FF00"            /* ûÿ..ÿÿ.ÿÿ.þÿ..ÿ. */
	$"FEFF 0200 FF00 FEFF 0000 FEFF 02DD BB77"            /* þÿ..ÿ.þÿ..þÿ.Ý»w */
	$"BAFF 0100 00FE FF01 0000 F9FF 0000 FEFF"            /* ºÿ...þÿ...ùÿ..þÿ */
	$"0900 FFFF 00FF FF00 FFFF 00FE FF0B 00FF"            /* Æ.ÿÿ.ÿÿ.ÿÿ.þÿ..ÿ */
	$"00FF FF00 FFFF 00FF FF00 FEFF 0000 FBFF"            /* .ÿÿ.ÿÿ.ÿÿ.þÿ..ûÿ */
	$"0000 FBFF 0300 FFFF 00FE FF00 00FE FF00"            /* ..ûÿ..ÿÿ.þÿ..þÿ. */
	$"00FB FF06 00FF FF00 FFFF 00FE FF02 00FF"            /* .ûÿ..ÿÿ.ÿÿ.þÿ..ÿ */
	$"00FE FF02 00FF 00FE FF00 00FE FF03 DDBB"            /* .þÿ..ÿ.þÿ..þÿ.Ý» */
	$"77FF 00C4 B9FF FD00 F7FF FE00 01FF FFFE"            /* wÿ.Ä¹ÿý.÷ÿþ..ÿÿþ */
	$"0001 FFFF FE00 01FF FFFC 00FE FFF8 00FC"            /* ..ÿÿþ..ÿÿü.þÿø.ü */
	$"FFFE 00FD FF00 00FD FFFE 00FE FF00 00FA"            /* ÿþ.ýÿ..ýÿþ.þÿ..ú */
	$"FFFD 00FE FFFE 00FE FFFE 00FE FFFE 00FD"            /* ÿý.þÿþ.þÿþ.þÿþ.ý */
	$"FF02 DDBB 77B8 FFFD 00F7 FFFE 0001 FFFF"            /* ÿ.Ý»w¸ÿý.÷ÿþ..ÿÿ */
	$"FE00 01FF FFFE 0001 FFFF FC00 FEFF F800"            /* þ..ÿÿþ..ÿÿü.þÿø. */
	$"FCFF FE00 FDFF 0000 FDFF FE00 FEFF 0000"            /* üÿþ.ýÿ..ýÿþ.þÿ.. */
	$"FAFF FD00 FEFF FE00 FEFF FE00 FEFF FE00"            /* úÿý.þÿþ.þÿþ.þÿþ. */
	$"FDFF 02DD BB77 B8FF FD00 F7FF FE00 01FF"            /* ýÿ.Ý»w¸ÿý.÷ÿþ..ÿ */
	$"FFFE 0001 FFFF FE00 01FF FFFC 00FE FFF8"            /* ÿþ..ÿÿþ..ÿÿü.þÿø */
	$"00FC FFFE 00FD FF00 00FD FFFE 00FE FF00"            /* .üÿþ.ýÿ..ýÿþ.þÿ. */
	$"00FA FFFD 00FE FFFE 00FE FFFE 00FE FFFE"            /* .úÿý.þÿþ.þÿþ.þÿþ */
	$"00FD FF03 DDBB 77FF 0030 81FF 0100 00F6"            /* .ýÿ.Ý»wÿ.0ÿ...ö */
	$"FF00 00E0 FF02 DDBB 7781 FF02 FF00 00F6"            /* ÿ..àÿ.Ý»wÿ.ÿ..ö */
	$"FF00 00E0 FF02 DDBB 7781 FF02 FF00 00F6"            /* ÿ..àÿ.Ý»wÿ.ÿ..ö */
	$"FF00 00E0 FF03 DDBB 77FF 0019 81FF D2FF"            /* ÿ..àÿ.Ý»wÿ..ÿÒÿ */
	$"02DD BB77 81FF D1FF 02DD BB77 81FF D1FF"            /* .Ý»wÿÑÿ.Ý»wÿÑÿ */
	$"03DD BB77 FF00 1981 FFD2 FF02 DDBB 7781"            /* .Ý»wÿ..ÿÒÿ.Ý»w */
	$"FFD1 FF02 DDBB 7781 FFD1 FF03 DDBB 77FF"            /* ÿÑÿ.Ý»wÿÑÿ.Ý»wÿ */
	$"0019 81FF D2FF 02DD BB77 81FF D1FF 02DD"            /* ..ÿÒÿ.Ý»wÿÑÿ.Ý */
	$"BB77 81FF D1FF 03DD BB77 FF00 1981 FFD2"            /* »wÿÑÿ.Ý»wÿ..ÿÒ */
	$"FF02 DDBB 7781 FFD1 FF02 DDBB 7781 FFD1"            /* ÿ.Ý»wÿÑÿ.Ý»wÿÑ */
	$"FF03 DDBB 77FF 0019 81FF D2FF 02DD BB77"            /* ÿ.Ý»wÿ..ÿÒÿ.Ý»w */
	$"81FF D1FF 02DD BB77 81FF D1FF 03DD BB77"            /* ÿÑÿ.Ý»wÿÑÿ.Ý»w */
	$"FF00 1981 FFD2 FF02 DDBB 7781 FFD1 FF02"            /* ÿ..ÿÒÿ.Ý»wÿÑÿ. */
	$"DDBB 7781 FFD1 FF03 DDBB 77FF 0019 81FF"            /* Ý»wÿÑÿ.Ý»wÿ..ÿ */
	$"D2FF 02DD BB77 81FF D1FF 02DD BB77 81FF"            /* Òÿ.Ý»wÿÑÿ.Ý»wÿ */
	$"D1FF 03DD BB77 FF00 1981 FFD2 FF02 DDBB"            /* Ñÿ.Ý»wÿ..ÿÒÿ.Ý» */
	$"7781 FFD1 FF02 DDBB 7781 FFD1 FF03 DDBB"            /* wÿÑÿ.Ý»wÿÑÿ.Ý» */
	$"77FF 001C F6FF 81DD DCDD 01BB 77F5 FF81"            /* wÿ..öÿÝÜÝ.»wõÿ */
	$"DDDC DD01 BB77 F5FF 81DD DCDD 02BB 77FF"            /* ÝÜÝ.»wõÿÝÜÝ.»wÿ */
	$"0019 F6FF 81BB DBBB 0077 F5FF 81BB DBBB"            /* ..öÿ»Û».wõÿ»Û» */
	$"0077 F5FF 81BB DBBB 0177 FF00 14F7 FF81"            /* .wõÿ»Û».wÿ..÷ÿ */
	$"77D9 77F6 FF81 77D9 77F6 FF81 77D9 7700"            /* wÙwöÿwÙwöÿwÙw. */
	$"FF00 00FF"                                          /* ÿ..ÿ */
};

data 'cicn' (128) {
	$"0000 0000 8010 0000 0000 0020 0020 0000"            /* ....€...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0004 0001 0004 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ..ÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF 9000 0001 9000 0001 FFFF"            /* ÿÿÿÿÿÿ......ÿÿ */
	$"FFFF 8000 0001 BFFF FFFD A000 0005 A5E7"            /* ÿÿ€...¿ÿÿý ...¥ç */
	$"CFE5 A000 0005 A6E6 4FA5 A000 0005 A765"            /* Ïå ...¦æO¥ ...§e */
	$"CDE5 A000 0005 A6E5 CFA5 A000 0005 A7E5"            /* Íå ...¦åÏ¥ ...§å */
	$"4DE5 A000 0005 A7A6 CEE5 A000 0005 A6E7"            /* Må ...§¦Îå ...¦ç */
	$"CF65 A000 0005 A000 0005 A000 0005 A000"            /* Ïe ... ... ... . */
	$"0005 A000 0005 A000 0005 A000 0005 A000"            /* .. ... ... ... . */
	$"0005 A000 0005 BFFF FFFD 8000 0001 FFFF"            /* .. ...¿ÿÿý€...ÿÿ */
	$"FFFF 0000 0000 0000 0007 0000 FFFF FFFF"            /* ÿÿ..........ÿÿÿÿ */
	$"FFFF 0001 CCCC CCCC FFFF 0002 9999 9999"            /* ÿÿ..ÌÌÌÌÿÿ..™™™™ */
	$"FFFF 0003 6666 6666 CCCC 0004 CCCC CCCC"            /* ÿÿ..ffffÌÌ..ÌÌÌÌ */
	$"CCCC 0005 5555 5555 5555 0006 2222 2222"            /* ÌÌ..UUUUUU.."""" */
	$"2222 000F 0000 0000 0000 6666 6666 6666"            /* ""........ffffff */
	$"6666 6666 6666 6666 6666 6555 4444 4444"            /* ffffffffffeUDDDD */
	$"4444 4444 4444 4444 4446 6555 4444 4444"            /* DDDDDDDDDFeUDDDD */
	$"4444 4444 4444 4444 4446 6666 6666 6666"            /* DDDDDDDDDFffffff */
	$"6666 6666 6666 6666 6666 6333 3333 3333"            /* ffffffffffc33333 */
	$"3333 3333 3333 3333 3336 6322 2222 2222"            /* 3333333336c""""" */
	$"2222 2222 2222 2222 2016 6324 4444 4444"            /* """""""" .c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4F4F FFF4"            /* DDDDDDDD@.c$OOÿô */
	$"4FFF FF44 FFFF FFF4 4016 6324 4444 4444"            /* OÿÿDÿÿÿô@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4FF4 FFF4"            /* DDDDDDDD@.c$Oôÿô */
	$"4FF4 4F44 FFFF F4F4 4016 6324 4444 4444"            /* OôODÿÿôô@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4FFF 4FF4"            /* DDDDDDDD@.c$OÿOô */
	$"4F4F FF44 FF4F FFF4 4016 6324 4444 4444"            /* OOÿDÿOÿô@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4FF4 FFF4"            /* DDDDDDDD@.c$Oôÿô */
	$"4F4F FF44 FFFF F4F4 4016 6324 4444 4444"            /* OOÿDÿÿôô@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4FFF FFF4"            /* DDDDDDDD@.c$Oÿÿô */
	$"4F4F 4F44 FF4F FFF4 4016 6324 4444 4444"            /* OOODÿOÿô@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4FFF F4F4"            /* DDDDDDDD@.c$Oÿôô */
	$"4FF4 FF44 FFF4 FFF4 4016 6324 4444 4444"            /* OôÿDÿôÿô@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4FF4 FFF4"            /* DDDDDDDD@.c$Oôÿô */
	$"4FFF FF44 FFFF 4FF4 4016 6324 4444 4444"            /* OÿÿDÿÿOô@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4444 4444"            /* DDDDDDDD@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4444 4444"            /* DDDDDDDD@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4444 4444"            /* DDDDDDDD@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4444 4444"            /* DDDDDDDD@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4444 4444"            /* DDDDDDDD@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4444 4444"            /* DDDDDDDD@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4444 4444"            /* DDDDDDDD@.c$DDDD */
	$"4444 4444 4444 4444 4016 6324 4444 4444"            /* DDDDDDDD@.c$DDDD */
	$"4444 4444 4444 4444 4016 6320 0000 0000"            /* DDDDDDDD@.c .... */
	$"0000 0000 0000 0000 0016 6311 1111 1111"            /* ..........c..... */
	$"1111 1111 1111 1111 1116 6666 6666 6666"            /* ..........ffffff */
	$"6666 6666 6666 6666 6666"                           /* ffffffffff */
};

data 'ckid' (128, "Projector Data") {
	$"4D5C 8AE3 3FE5 3230 0004 0000 0000 0000"            /* M\Šã?å20........ */
	$"0000 B7F1 FB74 B7F1 FB75 0000 0000 0000"            /* ..·ñût·ñûu...... */
	$"0000 0000 0000 0000 0000 0566 6F62 6572"            /* ...........fober */
	$"0003 312E 3200 0E6D 7344 6973 706C 6179"            /* ..1.2..msDisplay */
	$"2E72 7372 6300 0000 0000 00"                        /* .rsrc...... */
};

data 'icl4' (128) {
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FEEE CCCC CCCC CCCC CCCC CCCC CCCC CCCF"            /* þîÌÌÌÌÌÌÌÌÌÌÌÌÌÏ */
	$"FEEE CCCC CCCC CCCC CCCC CCCC CCCC CCCF"            /* þîÌÌÌÌÌÌÌÌÌÌÌÌÌÏ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FDDD DDDD DDDD DDDD DDDD DDDD DDDD DDDF"            /* ýÝÝÝÝÝÝÝÝÝÝÝÝÝÝß */
	$"FDDD DDDD DDDD DDDD DDDD DDDD DDDD DDCF"            /* ýÝÝÝÝÝÝÝÝÝÝÝÝÝÝÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CFCF FFFC CFFF FFCC FFFF FFFC C0CF"            /* ýÜÏÏÿüÏÿÿÌÿÿÿüÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CFFC FFFC CFFC CFCC FFFF FCFC C0CF"            /* ýÜÏüÿüÏüÏÌÿÿüüÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CFFF CFFC CFCF FFCC FFCF FFFC C0CF"            /* ýÜÏÿÏüÏÏÿÌÿÏÿüÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CFFC FFFC CFCF FFCC FFFF FCFC C0CF"            /* ýÜÏüÿüÏÏÿÌÿÿüüÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CFFF FFFC CFCF CFCC FFCF FFFC C0CF"            /* ýÜÏÿÿüÏÏÏÌÿÏÿüÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CFFF FCFC CFFC FFCC FFFC FFFC C0CF"            /* ýÜÏÿüüÏüÿÌÿüÿüÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CFFC FFFC CFFF FFCC FFFF CFFC C0CF"            /* ýÜÏüÿüÏÿÿÌÿÿÏüÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDDC CCCC CCCC CCCC CCCC CCCC CCCC C0CF"            /* ýÜÌÌÌÌÌÌÌÌÌÌÌÌÀÏ */
	$"FDD0 0000 0000 0000 0000 0000 0000 00CF"            /* ýÐ.............Ï */
	$"FDCC CCCC CCCC CCCC CCCC CCCC CCCC CCCF"            /* ýÌÌÌÌÌÌÌÌÌÌÌÌÌÌÏ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
};

data 'icl8' (128) {
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
	$"FDFB FBFB 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ýûûû++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFD"            /* +++++++++++++++ý */
	$"FDFB FBFB 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ýûûû++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFD"            /* +++++++++++++++ý */
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
	$"FD7F 7F7F 7F7F 7F7F 7F7F 7F7F 7F7F 7F7F"            /* ý............... */
	$"7F7F 7F7F 7F7F 7F7F 7F7F 7F7F 7F7F 7FFD"            /* ...............ý */
	$"FD7F 5454 5454 5454 5454 5454 5454 5454"            /* ý.TTTTTTTTTTTTTT */
	$"5454 5454 5454 5454 5454 5454 5400 2AFD"            /* TTTTTTTTTTTTT.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2BFF 2BFF FFFF FF2B 2BFF FFFF"            /* ý.T++ÿ+ÿÿÿÿ++ÿÿÿ */
	$"FFFF 2B2B FFFF FFFF FFFF FF2B 2B00 2AFD"            /* ÿÿ++ÿÿÿÿÿÿÿ++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2BFF FF2B FFFF FF2B 2BFF FF2B"            /* ý.T++ÿÿ+ÿÿÿ++ÿÿ+ */
	$"2BFF 2B2B FFFF FFFF FF2B FF2B 2B00 2AFD"            /* +ÿ++ÿÿÿÿÿ+ÿ++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2BFF FFFF 2BFF FF2B 2BFF 2BFF"            /* ý.T++ÿÿÿ+ÿÿ++ÿ+ÿ */
	$"FFFF 2B2B FFFF 2BFF FFFF FF2B 2B00 2AFD"            /* ÿÿ++ÿÿ+ÿÿÿÿ++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2BFF FF2B FFFF FF2B 2BFF 2BFF"            /* ý.T++ÿÿ+ÿÿÿ++ÿ+ÿ */
	$"FFFF 2B2B FFFF FFFF FF2B FF2B 2B00 2AFD"            /* ÿÿ++ÿÿÿÿÿ+ÿ++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2BFF FFFF FFFF FF2B 2BFF 2BFF"            /* ý.T++ÿÿÿÿÿÿ++ÿ+ÿ */
	$"2BFF 2B2B FFFF 2BFF FFFF FF2B 2B00 2AFD"            /* +ÿ++ÿÿ+ÿÿÿÿ++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2BFF FFFF FF2B FF2B 2BFF FF2B"            /* ý.T++ÿÿÿÿ+ÿ++ÿÿ+ */
	$"FFFF 2B2B FFFF FF2B FFFF FF2B 2B00 2AFD"            /* ÿÿ++ÿÿÿ+ÿÿÿ++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2BFF FF2B FFFF FF2B 2BFF FFFF"            /* ý.T++ÿÿ+ÿÿÿ++ÿÿÿ */
	$"FFFF 2B2B FFFF FFFF 2BFF FF2B 2B00 2AFD"            /* ÿÿ++ÿÿÿÿ+ÿÿ++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"            /* ý.T+++++++++++++ */
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* +++++++++++++.*ý */
	$"FD7F 5400 0000 0000 0000 0000 0000 0000"            /* ý.T............. */
	$"0000 0000 0000 0000 0000 0000 0000 2AFD"            /* ..............*ý */
	$"FD7F 2A2A 2A2A 2A2A 2A2A 2A2A 2A2A 2A2A"            /* ý.************** */
	$"2A2A 2A2A 2A2A 2A2A 2A2A 2A2A 2A2A 2AFD"            /* ***************ý */
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
};

data 'ics#' (128) {
	$"FFFF 8001 FFFF 8001 BFFD A005 AF75 A005"            /* ÿÿ€.ÿÿ€.¿ý .¯u . */
	$"AED5 A005 AF75 A005 A005 BFFD 8001 FFFF"            /* ®Õ .¯u . .¿ý€.ÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿÿ */
};

data 'ics4' (128) {
	$"FFFF FFFF FFFF FFFF FCCC CCCC CCCC CCCF"            /* ÿÿÿÿÿÿÿÿüÌÌÌÌÌÌÏ */
	$"FFFF FFFF FFFF FFFF FDDD DDDD DDDD DDDF"            /* ÿÿÿÿÿÿÿÿýÝÝÝÝÝÝß */
	$"FDCC CCCC CCCC CCCF FDCC CCCC CCCC C0CF"            /* ýÌÌÌÌÌÌÏýÌÌÌÌÌÀÏ */
	$"FDCC FFFF CFFF C0CF FDCC CCCC CCCC C0CF"            /* ýÌÿÿÏÿÀÏýÌÌÌÌÌÀÏ */
	$"FDCC FFFC FFCF C0CF FDCC CCCC CCCC C0CF"            /* ýÌÿüÿÏÀÏýÌÌÌÌÌÀÏ */
	$"FDCC FFFF CFFF C0CF FDCC CCCC CCCC C0CF"            /* ýÌÿÿÏÿÀÏýÌÌÌÌÌÀÏ */
	$"FDCC CCCC CCCC C0CF FDC0 0000 0000 00CF"            /* ýÌÌÌÌÌÀÏýÀ.....Ï */
	$"FDCC CCCC CCCC CCCF FFFF FFFF FFFF FFFF"            /* ýÌÌÌÌÌÌÏÿÿÿÿÿÿÿÿ */
};

data 'ics8' (128) {
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
	$"FDF8 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 F8FD"            /* ýøøøøøøøøøøøøøøý */
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
	$"FD7F 7F7F 7F7F 7F7F 7F7F 7F7F 7F7F 7FFD"            /* ý..............ý */
	$"FD7F 5454 5454 5454 5454 5454 5454 2AFD"            /* ý.TTTTTTTTTTTT*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* ý.T++++++++++.*ý */
	$"FD7F 542B FFFF FFFF 2BFF FFFF 2B00 2AFD"            /* ý.T+ÿÿÿÿ+ÿÿÿ+.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* ý.T++++++++++.*ý */
	$"FD7F 542B FFFF FF2B FFFF 2BFF 2B00 2AFD"            /* ý.T+ÿÿÿ+ÿÿ+ÿ+.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* ý.T++++++++++.*ý */
	$"FD7F 542B FFFF FFFF 2BFF FFFF 2B00 2AFD"            /* ý.T+ÿÿÿÿ+ÿÿÿ+.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* ý.T++++++++++.*ý */
	$"FD7F 542B 2B2B 2B2B 2B2B 2B2B 2B00 2AFD"            /* ý.T++++++++++.*ý */
	$"FD7F 5400 0000 0000 0000 0000 0000 2AFD"            /* ý.T...........*ý */
	$"FD7F 2A2A 2A2A 2A2A 2A2A 2A2A 2A2A 2AFD"            /* ý.*************ý */
	$"FDFD FDFD FDFD FDFD FDFD FDFD FDFD FDFD"            /* ýýýýýýýýýýýýýýýý */
};

data 'mcvs' (128, "MacCVS Version Resource") {
	$"FABB 0003 0000 0001 0000 0000 0E6D 7344"            /* ú»...........msD */
	$"6973 706C 6179 2E72 7372 6300 0331 2E32"            /* isplay.rsrc..1.2 */
	$"0000 0003 2D6B 6200 0454 6465 7600 0000"            /* ....-kb..Tdev... */
	$"0000 0000 0000 0017 7372 632F 6D61 632F"            /* ........src/mac/ */
	$"4170 706C 732F 6D73 4469 7370 6C61 79"              /* Appls/msDisplay */
};

data 'vers' (1) {
	$"0161 8000 0001 0431 2E36 3104 312E 3631"            /* .a€....1.61.1.61 */
};

data 'vers' (2) {
	$"0161 8000 0001 0431 2E36 3111 A920 3139"            /* .a€....1.61.© 19 */
	$"3837 2D32 3030 3120 4752 414D 45"                   /* 87-2001 GRAME */
};

