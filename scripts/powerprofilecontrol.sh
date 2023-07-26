#!/usr/bin/sh


powerstatus (){


   echo "$(tuned-adm active)" # prints current powerprofile to stdout

}

getPowerprofile (){

  # Returns the currently active powerprofile.
  #
  # Example stdout from tuned-adm active:
  #     Current active profile: laptop-ac-powersave
  #
  # Awk isolated the 4th element (sep=spaces); the "laptop-ac-powersave" in this case.

  echo "$(tuned-adm active | awk '{print $4}')"

}



toggleprofile (){

    if [ $(getPowerprofile) = "balanced" ]; then 

        tuned-adm profile latency-performance

    elif [ $(getPowerprofile) = "latency-performance" ]; then

        tuned-adm profile laptop-battery-powersave

    elif [ $(getPowerprofile) = "laptop-battery-powersave" ]; then

        tuned-adm profile laptop-ac-powersave

    elif [ $(getPowerprofile) = "laptop-ac-powersave" ]; then

        tuned-adm profile balanced

    else # this condition could be met if tuned is set to off or some other profile
      
      tuned-adm auto_profile # will set to the balanced profile automatically.

fi


}

toggleprofile
#pauses status bar
kill -STOP $(pgrep statusbar.sh)
xsetroot -name " $(powerstatus) "
sleep 1

#This checks if there are more than one instances of the current script running. If there are it simply exits the current script. If this is the only instance, it will allow statusbar.sh to resume execution. This helps deal with glitchiness when the the script is invokes in rapid succession. (ex. I click the same keybinding multiple times quickly).

if [ $(pidof -o %PPID -x $0 | wc -w) -eq 1 ]

     then

         kill -CONT $(pgrep statusbar.sh)

         exit 1

     else

         exit 1
 fi

