#!/bin/bash

for d in 50 100 200 300 400 500 600 700 800 900 1000
do
  echo $1 "grouped by " $d
  echo ""
  /home/falquet/rtp-midi/examples/sendmidifile -v 2 -c 5000 -d $d $1 > "$1.grp.$d.out"
  awk -f /home/falquet/rtp-midi/evaluation/stats.awk "$1.grp.$d.out"
  echo ""
done
