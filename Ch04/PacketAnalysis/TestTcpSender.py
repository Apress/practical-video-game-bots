#!/usr/bin/env python3

import socket

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
  s.settimeout(2)
  s.connect(("127.0.0.1", 24000))
  s.send(bytes([44, 55, 66]))
  s.close()

if __name__ == '__main__':
  main()
