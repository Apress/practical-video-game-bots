#!/usr/bin/env python3

import socket
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  s.bind(("127.0.0.1", 24001))

  public_key, addr = s.recvfrom(1024, socket.MSG_WAITALL)

  key = RSA.importKey(public_key)
  cipher = PKCS1_OAEP.new(key)

  cipher_text = cipher.encrypt(b"Hello world!")

  s.sendto(cipher_text, ("127.0.0.1", 24000))
  s.close()

if __name__ == '__main__':
  main()
