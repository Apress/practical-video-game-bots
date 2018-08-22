#!/usr/bin/env python3

import socket

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
  s.bind(("127.0.0.1", 24000))
  s.listen(1)
  conn, addr = s.accept()
  data = conn.recv(1024, socket.MSG_WAITALL)
  print(data)
  s.close()

if __name__ == '__main__':
  main()
