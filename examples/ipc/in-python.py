#!/usr/bin/env python
from socket import *
import os, os.path, sys

if len(sys.argv) != 2:
    print 'usage:', sys.argv[0], '<IPC_COMMAND>'
    sys.exit(1)

sckt = socket(AF_UNIX, SOCK_DGRAM)
sckt.connect('/home/nick/.xy/ipc')
sckt.send(sys.argv[1])
