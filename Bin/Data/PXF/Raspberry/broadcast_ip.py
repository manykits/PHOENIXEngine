import socket
from time import sleep

s =socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('',0))
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST,1)

while True:
    s.sendto(b'camera',('<broadcast>',9909))
    sleep(2)
