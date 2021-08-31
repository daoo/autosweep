#!/usr/bin/env bash

ffmpeg \
  -f x11grab -s 818x540 -i :0.0+53,117 -r 10 \
  -vcodec huffyuv -r 25 \
  /tmp/output.avi

convert -set delay 4 -layers Optimize /tmp/output.avi /tmp/output.gif
