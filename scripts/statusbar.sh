#!/usr/bin/sh

#makes sure there are not more than one instances of status bar
if [ $(pidof -o %PPID -x $0 | wc -w) -gt 1 ]

then 

    exit 0

else

    memory () {

        free --mega --total | grep 'Mem' | awk '{var="Memory usage: "$3"" "/" $2 " MiB"; printf var }'
    }

    is_charging () {

        bat_num=$1 

        awk '{if (/Charging/) {print "*"} else {}}' /sys/class/power_supply/BAT$bat_num/status
    }

    battery () {

        echo "Battery 1: $(is_charging 0)$(cat /sys/class/power_supply/BAT0/capacity)% Battery 2: $(is_charging 1)$(cat /sys/class/power_supply/BAT1/capacity)% | $(date '+%a %d %b %Y %I:%M %P %Z')"

    }

    status () {

        echo " $(memory) | $(battery) "

    }

    while true; do
        xsetroot -name "$(status)"
        sleep 5
    done

fi
