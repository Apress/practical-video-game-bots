#!/usr/bin/env python3

import socket
from Crypto.Cipher import XOR

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  s.bind(("127.0.0.1", 24000))
  data, addr = s.recvfrom(1024, socket.MSG_WAITALL)

  key = b"The secret key"
  decryption_suite = XOR.new(key)
  plain_text = decryption_suite.decrypt(data)
  print(plain_text)

  s.close()

if __name__ == '__main__':
  main()
