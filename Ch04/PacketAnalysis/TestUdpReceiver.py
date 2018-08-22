#!/usr/bin/env python3

import socket

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  s.bind(("127.0.0.1", 24000))
  data, addr = s.recvfrom(1024, socket.MSG_WAITALL)
  print(data)
  s.close()

if __name__ == '__main__':
  main()
