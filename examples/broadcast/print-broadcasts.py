#!/usr/bin/env python
from socket import *

MCAST_GROUP = '224.0.0.3' 
PORT = 47002
MESSAGE = 'From {0}, xy says: {1}'
IP = '127.0.0.1'

optval = inet_aton(MCAST_GROUP) + inet_aton(IP)

sock = socket(AF_INET, SOCK_DGRAM)
sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, True)
sock.setsockopt(SOL_IP, IP_MULTICAST_LOOP, True)
sock.setsockopt(SOL_IP, IP_ADD_MEMBERSHIP, optval)
sock.bind(('', PORT))

print 'Running, interrupt to exit.'
while True:
    data, addr = sock.recvfrom(256)
    print MESSAGE.format(addr[0], data)

