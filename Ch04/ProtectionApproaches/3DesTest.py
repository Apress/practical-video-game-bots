#!/usr/bin/env python3

from Crypto.Cipher import DES3
from Crypto import Random

def main():
  key = b"The secret key a"
  iv = Random.new().read(DES3.block_size)

  # Encryption
  encryption_suite = DES3.new(key, DES3.MODE_CBC, iv)
  cipher_text = encryption_suite.encrypt(b"Hello world!    ")
  print(cipher_text)

  # Decryption
  decryption_suite = DES3.new(key, DES3.MODE_CBC, iv)
  plain_text = decryption_suite.decrypt(cipher_text)
  print(plain_text)

if __name__ == '__main__':
  main()
