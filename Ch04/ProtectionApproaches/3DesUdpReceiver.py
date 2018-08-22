#!/usr/bin/env python3

import socket
from Crypto.Cipher import DES3

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  s.bind(("127.0.0.1", 24000))
  data, addr = s.recvfrom(1024, socket.MSG_WAITALL)

  key = b"The secret key a"
  decryption_suite = DES3.new(key, DES3.MODE_CBC, data[0:DES3.block_size])
  plain_text = decryption_suite.decrypt(data[DES3.block_size:])
  print(plain_text)

  s.close()

if __name__ == '__main__':
  main()
