import socket 
import time

host = '192.168.254.22'
port = 9998

tcp_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

tcp_client.connect((host, port))

while True:
    tcp_client.sendall("hello".encode())
    time.sleep(1)