#!/usr/bin/env python3

import socket

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
  s.settimeout(60)
  s.connect(("127.0.0.1", 55555))

  # recv - server confirmation
  s.recv(1024, socket.MSG_WAITALL)
  s.recv(1024, socket.MSG_WAITALL)

  # send - client username
  s.send(bytes([0x09, 0, 0, 0]))
  s.send(bytes([0x0C, 0x63, 0x6C, 0x5F, 0x75, 0x73, 0x65, 0x72, 0x00]))

  # recv - manual edit mode
  s.recv(1024, socket.MSG_WAITALL)
  s.recv(1024, socket.MSG_WAITALL)

  # send - client confirmation for manual edit mode
  s.send(bytes([0x01, 0, 0, 0]))
  s.send(bytes([0x17]))

  # recv - launch the game clock
  s.recv(1024, socket.MSG_WAITALL)
  s.recv(1024, socket.MSG_WAITALL)

  s.close()

if __name__ == '__main__':
  main()
