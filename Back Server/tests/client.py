import socket
import os

HOST = '0.0.0.0'
PORT = 5555
file_name = "client.py"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    name_length = len(file_name)
    print(name_length)
    s.send(name_length.to_bytes(4, 'big'))
    s.send(file_name.encode())
    file_size = os.path.getsize(file_name)
    s.send(file_size.to_bytes(4, 'big'))
    print(file_size)
    with open(file_name, "rb") as f:
        bytes_read = f.read(file_size)
        s.sendall(bytes_read)

