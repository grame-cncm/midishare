/total size/ {
  payloads++;
  totPayloadSize += $4;
  sqTotPayloadSize += $4^2;
  if ( $4 > maxPayloadSize ) maxPayloadSize = $4;
}

/command section size/ {
  commandSections++;
  totCommandSectionSize += $5;
  sqTotCommandSectionSize += $5^2;
  if ( $5 > maxCommandSectionSize ) maxCommandSectionSize = $5;
}

/number of commands/ {
  numberOfCommands++;
  totNumberOfCommands += $5;
  sqTotNumberOfCommands += $5^2;
  if ( $5 > maxNumberOfCommands ) maxNumberOfCommands = $5;
}

/journal section size/ {
  journalSections++;
  totJournalSectionSize += $5;
  sqTotJournalSectionSize += $5^2;
  if ( $5 > maxJournalSectionSize ) maxJournalSectionSize = $5;
}

/channel . journal size/ {
  channelJournals[$3]++;
  totChannelJournalSize[$3] += $6;
  sqTotChannelJournalSize[$3] += $6^2;
  if ( $6 > maxChannelJournalSize[$3] ) maxChannelJournalSize[$3] = $6;
}

/number of channels/ {
  if ( $5 > channels ) channels = $5;
}

/total duration/ {
  totalTime = $4;
}

END {
  printf ( "track duration :\t\t\t\t%d\n", totalTime );
  printf ( "number of payloads :\t\t\t\t%d\n", payloads );
  printf ( "number of channels :\t\t\t\t%d\n", channels );
  printf ( "\n" );
  printf ( "payload size average :\t\t\t\t%d\n", totPayloadSize / payloads );
  printf ( "payload size s.d :\t\t\t\t%d\n", sqrt ( sqTotPayloadSize / payloads - ( totPayloadSize / payloads )^2 ) );
  printf ( "payload size max :\t\t\t\t%d\n", maxPayloadSize );
  printf ( "payload bandwidth :\t\t\t\t%d\n", totPayloadSize / totalTime * 8 );
  printf ( "\n" );
  printf ( "command section size average :\t\t\t%d\n", totCommandSectionSize / commandSections );
  printf ( "command section size s.d :\t\t\t%d\n", sqrt ( sqTotCommandSectionSize / commandSections - ( totCommandSectionSize / commandSections )^2 ) );
  printf ( "command section size max :\t\t\t%d\n", maxCommandSectionSize );
  printf ( "command section bandwidth :\t\t\t%d\n", totCommandSectionSize / totalTime * 8 );
  printf ( "command section bandwidth ratio :\t\t%f\n", totCommandSectionSize / totPayloadSize );
  printf ( "\n" );
  printf ( "journal section size average :\t\t\t%d\n", totJournalSectionSize / journalSections );
  printf ( "journal section size s.d :\t\t\t%d\n", sqrt ( sqTotJournalSectionSize / journalSections - ( totJournalSectionSize / journalSections )^2 ) );
  printf ( "journal section size max :\t\t\t%d\n", maxJournalSectionSize );
  printf ( "journal section bandwidth :\t\t\t%d\n", totJournalSectionSize / totalTime * 8 );
  printf ( "journal section bandwidth ratio :\t\t%f\n", totJournalSectionSize / totPayloadSize );
  for ( i = 0 ; i < 16 ; i++ ) {
    if ( channelJournals[i] != 0 ) {
      printf ( "\n" );
      printf ( "channel %d journal section size average :\t%d\n", i, totChannelJournalSize[i] / channelJournals[i] );
      printf ( "channel %d journal section size s.d :\t\t%d\n", i, sqrt ( sqTotChannelJournalSize[i] / channelJournals[i] - ( totChannelJournalSize[i] / channelJournals[i] )^2 ) );
      printf ( "channel %d journal section size max :\t\t%d\n", i, maxChannelJournalSize[i] );
      printf ( "channel %d journal section bandwidth :\t\t%d\n", i, totChannelJournalSize[i] / totalTime * 8 );
    }
  }
}
