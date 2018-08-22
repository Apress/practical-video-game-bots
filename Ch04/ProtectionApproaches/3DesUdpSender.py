#!/usr/bin/env python3

import socket
from Crypto.Cipher import DES3
from Crypto import Random

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  s.bind(("127.0.0.1", 24001))

  key = b"The secret key a"
  iv = Random.new().read(DES3.block_size)
  encryption_suite = DES3.new(key, DES3.MODE_CBC, iv)
  cipher_text = iv + encryption_suite.encrypt(b"Hello world!    ")

  s.sendto(cipher_text, ("127.0.0.1", 24000))
  s.close()

if __name__ == '__main__':
  main()
