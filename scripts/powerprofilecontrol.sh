#!/usr/bin/sh


powerstatus (){


   echo "$(powerprofilesctl get)"

}



toggleprofile (){

    if [ "$(powerprofilesctl get)" = "balanced" ]; then 

        powerprofilesctl set performance

    elif [ "$(powerprofilesctl get)" = "performance" ]; then

        powerprofilesctl set power-saver


    elif [ "$(powerprofilesctl get)" = "power-saver" ]; then

        powerprofilesctl set balanced

    else

        xsetroot -name " unknown profiles "

        exit 1
fi


}

toggleprofile
#pauses status bar
kill -STOP $(pgrep statusbar.sh)
sleep 0.5
xsetroot -name " Setting power profile to: $(powerstatus) "
sleep 0.5

#This checks if there are more than one instances of the current script running. If there are it simply exits the current script. If this is the only instance, it will allow statusbar.sh to resume execution. This helps deal with glitchiness when the the script is invokes in rapid succession. (ex. I click the same keybinding multiple times quickly).

if [ $(pidof -o %PPID -x $0 | wc -w) -eq 1 ]

     then

         kill -CONT $(pgrep statusbar.sh)

         exit 1

     else

         exit 1
 fi

