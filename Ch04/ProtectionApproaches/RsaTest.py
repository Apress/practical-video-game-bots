#!/usr/bin/env python3

from Crypto.PublicKey import RSA
from Crypto import Random

def main():
  key = RSA.generate(1024, Random.new().read)

  # Encryption
  cipher_text = key.encrypt(b"Hello world!", 32)
  print(cipher_text)

  # Decryption
  plain_text = key.decrypt(cipher_text)
  print(plain_text)

if __name__ == '__main__':
  main()
