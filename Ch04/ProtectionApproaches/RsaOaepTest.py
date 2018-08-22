#!/usr/bin/env python3

from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto import Random

def main():
  key = RSA.generate(1024, Random.new().read)

  # Encryption
  encryption_suite = PKCS1_OAEP.new(key)
  cipher_text = encryption_suite.encrypt(b"Hello world!")
  print(cipher_text)

  # Decryption
  decryption_suite = PKCS1_OAEP.new(key)
  plain_text = decryption_suite.decrypt(cipher_text)
  print(plain_text)

if __name__ == '__main__':
  main()
