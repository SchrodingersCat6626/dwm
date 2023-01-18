#!/usr/bin/sh

screencapture(){

flameshot full --clipboard --path $HOME/Pictures/screenshots/

}

xsetroot -name " $(screencapture) "
sleep 0.5

