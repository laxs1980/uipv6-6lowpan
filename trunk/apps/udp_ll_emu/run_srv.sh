#!/bin/sh

echo "Run server app."
export UDPDEV_S_IP="127.0.0.1"
export UDPDEV_C_IP="127.0.0.1"
export UDPDEV_S_PORT="40002"
export UDPDEV_C_PORT="40001"
export UDPDEV_LL_ADDR="2"
./main
