#!/usr/bin/env python3

import socket
from Crypto.Cipher import XOR

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  s.bind(("127.0.0.1", 24001))

  key = b"The secret key"
  encryption_suite = XOR.new(key)
  cipher_text = encryption_suite.encrypt(b"Hello world!")

  s.sendto(cipher_text, ("127.0.0.1", 24000))
  s.close()

if __name__ == '__main__':
  main()
