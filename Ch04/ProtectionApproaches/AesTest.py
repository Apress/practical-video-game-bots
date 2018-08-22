#!/usr/bin/env python3

from Crypto.Cipher import AES
from Crypto import Random

def main():
  key = b"The secret key a"
  iv = Random.new().read(AES.block_size)

  # Encryption
  encryption_suite = AES.new(key, AES.MODE_CBC, iv)
  cipher_text = encryption_suite.encrypt(b"Hello world!    ")
  print(cipher_text)

  # Decryption
  decryption_suite = AES.new(key, AES.MODE_CBC, iv)
  plain_text = decryption_suite.decrypt(cipher_text)
  print(plain_text)

if __name__ == '__main__':
  main()
