#!/usr/bin/sh

mastervolume() {

    #tests if master volume is muted

    if [ -n  "$(echo "$amixeroutput" | grep -oP -i '\[off\]')" ];

    then

        echo "[muted]"

    else

        echo "$(echo "$amixeroutput" | grep -oP -i -m 1 '[0-9]*%')"
    
    
    fi

}

#pauses status bar
kill -STOP $(pgrep statusbar.sh)

amixeroutput="$(amixer get Master)"

xsetroot -name " Master volume: $(mastervolume) "

#This checks if there are more than one instances of the current script running. If there are it simply exits the current script. If this is the only instance, it will allow statusbar.sh to resume execution. This helps deal with glitchiness when the the script is invokes in rapid succession. (ex. I click the same keybinding multiple times quickly).

if [ $(pidof -o %PPID -x $0 | wc -w) -eq 1 ]

     then

         kill -CONT $(pgrep statusbar.sh)

         exit 1

     else

         exit 1
 fi

