#!/usr/bin/env python
from socket import *
import os, os.path

sckt = socket(AF_UNIX, SOCK_DGRAM)
sckt.connect('/home/nick/.xy/ipc')

sckt.send("babye")
