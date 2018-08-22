#!/usr/bin/env python3

import socket

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  s.bind(("127.0.0.1", 24001))
  data = bytes("Hello world!", "utf-8")
  s.sendto(data, ("127.0.0.1", 24000))
  s.close()

if __name__ == '__main__':
  main()
