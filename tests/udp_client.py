from socket import *
import sys
import select

address = ('localhost', 40001)
client_socket = socket(AF_INET, SOCK_DGRAM)

client_socket.sendto("Hello message", address)

