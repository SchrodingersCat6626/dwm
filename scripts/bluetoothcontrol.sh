#!/usr/bin/sh

# Note: This script only works because F10 is the bluetooth button on my thinkpad. The function to disable bluetooth is 
#not handled by this script (+ That would require sudo privileges.
# This script simply displays the bluetooth status.


bltstatus () {

    # if the adapter isn't found then bluetooth is turned off at the hardware level.
    adapter=$(lsusb | grep Bluetooth)

    [ -z "$adapter" ] && echo " Bluetooth [off] " || echo " Bluetooth [on] "

}



sleep 1

kill -STOP $(pgrep statusbar.sh)

xsetroot -name "$(bltstatus)"

sleep 1

#This checks if there are more than one instances of the current script running. If there are it simply exits the current script. If this is the only instance, it will allow statusbar.sh to resume execution. This helps deal with glitchiness when the the script is invokes in rapid succession. (ex. I click the same keybinding multiple times quickly).

if [ $(pidof -o %PPID -x $0 | wc -w) -eq 1 ]

     then

         kill -CONT $(pgrep statusbar.sh)

         exit 1

     else

         exit 1
 fi

