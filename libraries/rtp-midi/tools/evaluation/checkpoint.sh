#!/bin/bash

echo $1 "no checkpoint refresh"
echo ""
/home/falquet/rtp-midi/examples/sendmidifile -v 2 -d 200 $1 > "$1.chk.no.out"
awk -f /home/falquet/rtp-midi/evaluation/stats.awk "$1.chk.no.out"
echo ""

for c in 1000 5000 25000
do
  echo $1 "checkpoint refresh interval " $c
  echo ""
  /home/falquet/rtp-midi/examples/sendmidifile -v 2 -d 200 -c $c $1 > "$1.chk.$c.out"
  awk -f /home/falquet/rtp-midi/evaluation/stats.awk "$1.chk.$c.out"
  echo ""
done
