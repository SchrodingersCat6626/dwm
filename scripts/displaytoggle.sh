#!/usr/bin/sh

# toggle between turning of and of integrated display and xrandr-auto










toggleprofile
sleep 0.5
xsetroot -name " Setting power profile to: $(powerstatus) "
sleep 0.5

