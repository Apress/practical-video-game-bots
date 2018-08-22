#!/usr/bin/env python3

import socket
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto import Random

def main():
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
  s.bind(("127.0.0.1", 24000))

  key = RSA.generate(1024, Random.new().read)
  public_key = key.publickey().exportKey()

  s.sendto(public_key, ("127.0.0.1", 24001))

  data, addr = s.recvfrom(1024, socket.MSG_WAITALL)

  cipher = PKCS1_OAEP.new(key)
  plain_text = cipher.decrypt(data)
  print(plain_text)

  s.close()

if __name__ == '__main__':
  main()
