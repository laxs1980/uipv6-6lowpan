#!/bin/sh

APP_1_PID=""
APP_2_PID=""

exit_hook()
{
	echo "SIGTERM"
	echo "Kill $APP_1_PID, $APP_2_PID"
	kill $APP_1_PID $APP_2_PID
	exit 1
}

kldstat -n if_tap > /dev/null
if [ $? ]
then
	echo "Tap module loaded, unload it."
	kldunload if_tap
fi

echo "Load tap module."
kldload if_tap

echo "Run app."
export UDPDEV_S_IP="127.0.0.1"
export UDPDEV_C_IP="127.0.0.1"
export UDPDEV_S_PORT="40001"
export UDPDEV_C_PORT="40002"
export UDPDEV_LL_ADDR="1"
./main > app_1.log&
APP_1_PID=$!

export UDPDEV_S_IP="127.0.0.1"
export UDPDEV_C_IP="127.0.0.1"
export UDPDEV_S_PORT="40002"
export UDPDEV_C_PORT="40001"
export UDPDEV_LL_ADDR="2"
./main > app_2.log&
APP_2_PID=$!

trap exit_hook SIGHUP SIGINT SIGTERM

while true
do
done
