import socket

addrinfo = socket.getaddrinfo('fe80:0000:0000:0000:0206:98ff:fe00:0232%tap0', 8080, socket.AF_INET6, socket.SOCK_STREAM)

(family, socktype, proto, canonname, sockaddr) = addrinfo[0]


print addrinfo

s = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)

s.connect(sockaddr)

s.sendall("Hello world\n")

data = s.recv(1024)
if not data: exit(1)

print data
