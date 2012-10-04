#!/bin/sh

kldstat -n if_tap > /dev/null
if [ $? ]
then
	echo "Tap module loaded, unload it."
	kldunload if_tap
fi

echo "Load tap module."
kldload if_tap

echo "Run app."
./main
