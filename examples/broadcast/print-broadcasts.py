#!/usr/bin/env python
from socket import *

IP = "224.0.0.3" 
PORT = 47002

sock = socket(AF_INET, SOCK_DGRAM)
sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
sock.setsockopt(SOL_IP, IP_MULTICAST_LOOP, 1)
sock.bind(('', PORT))

intf = gethostbyname(gethostname())
sock.setsockopt(SOL_IP, IP_MULTICAST_IF, inet_aton(intf))
sock.setsockopt(SOL_IP, IP_ADD_MEMBERSHIP, inet_aton(IP) + inet_aton(intf))

print 'Running, interrupt to exit.'
while True:
    data, addr = sock.recvfrom(256)
    print data

