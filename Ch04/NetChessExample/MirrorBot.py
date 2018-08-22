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

  # process turns
  while(1):
    # recv - server turn
    s.recv(1024, socket.MSG_WAITALL)
    data = s.recv(1024, socket.MSG_WAITALL)
    print(data)

    start_num = 7 - data[2]
    end_num = 7 - data[4]

    # send - client turn
    s.send(bytes([0x07, 0, 0, 0]))
    s.send(bytes([0, 0, start_num, data[3], end_num, data[5], 0x00]))

  s.close()

if __name__ == '__main__':
  main()
