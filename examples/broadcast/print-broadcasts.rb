#!/usr/bin/env ruby
require 'socket'
require 'ipaddr'

MULTICAST_ADDR = "224.0.0.3" 
PORT = 47002

ip =  IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new("127.0.0.1").hton

sock = UDPSocket.new
sock.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, ip)
sock.bind(Socket::INADDR_ANY, PORT)

puts 'Running, interrupt to exit.'
loop do
    msg, info = sock.recvfrom(256)
    puts "From #{info[3]}, xy says: #{msg}" 
end

