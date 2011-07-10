#!/usr/bin/env ruby
require 'socket'
require 'ipaddr'

MCAST_GROUP = '224.0.0.3'
PORT = 47002
MESSAGE = 'From %s, xy says: %s'
IP = '127.0.0.1'

optval = IPAddr.new(MCAST_GROUP).hton + IPAddr.new(IP).hton

sock = UDPSocket.new
sock.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, true)
sock.setsockopt(Socket::IPPROTO_IP, Socket::IP_MULTICAST_LOOP, true)
sock.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, optval)
sock.bind('', PORT)

puts 'Running, interrupt to exit.'
loop do
    msg, info = sock.recvfrom(256)
    puts format('speaking "%s"', msg)
    system("/usr/bin/festival -b \"(SayText \\\"#{msg}\\\")\"")
end
