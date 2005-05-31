#!/bin/bash

echo "\"no checkpoint\""
awk '/# payload/{printf("%f\t",$4);ok=1}/journal section size/{if(ok)print $5;ok=0}' "$1.chk.no.out"
echo ""

for c in 1000 5000 25000
do
  echo "\"checkpoint $c\""
  awk '/# payload/{printf("%f\t",$4);ok=1}/journal section size/{if(ok)print $5;ok=0}' "$1.chk.$c.out"
  echo ""
done
