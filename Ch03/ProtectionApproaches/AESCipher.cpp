#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include <string>

#pragma comment (lib, "advapi32")
#pragma comment (lib, "user32")

using namespace std;

static const uint16_t MAX_LIFE = 20;
static uint16_t gLife = 0;

HCRYPTPROV hProv;
HCRYPTKEY hKey;
HCRYPTKEY hSessionKey;

#define kAesBytes128 16

typedef struct {
    BLOBHEADER  header;
    DWORD       key_length;
    BYTE        key_bytes[kAesBytes128];
} AesBlob128;

static const BYTE gCipherBlockSize = kAesBytes128 * 2;
static BYTE gCipherBlock[gCipherBlockSize] = {0};

void CreateContex()
{
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        printf("CryptAcquireContext() failed - error = 0x%x\n", GetLastError());
    }
}

void CreateKey(string& key)
{
    AesBlob128 aes_blob;
    aes_blob.header.bType = PLAINTEXTKEYBLOB;
    aes_blob.header.bVersion = CUR_BLOB_VERSION;
    aes_blob.header.reserved = 0;
    aes_blob.header.aiKeyAlg = CALG_AES_128;
    aes_blob.key_length = kAesBytes128;
    memcpy(aes_blob.key_bytes, key.c_str(), kAesBytes128);

    if (!CryptImportKey(hProv,
                      reinterpret_cast<BYTE*>(&aes_blob),
                      sizeof(AesBlob128),
                      NULL,
                      0,
                      &hKey))
    {
        printf("CryptImportKey() failed - error = 0x%x\n", GetLastError());
    }
}

void Encrypt()
{
    unsigned long length = kAesBytes128;
    memset(gCipherBlock, 0, gCipherBlockSize);
    memcpy(gCipherBlock, &gLife, sizeof(gLife));

    if (!CryptEncrypt(hKey, 0, TRUE, 0, gCipherBlock, &length, gCipherBlockSize))
    {
        printf("CryptEncrypt() failed - error = 0x%x\n", GetLastError());
        return;
    }
    gLife = 0;
}

void Decrypt()
{
    unsigned long length = gCipherBlockSize;

    if (!CryptDecrypt(hKey, 0, TRUE, 0, gCipherBlock, &length))
    {
        printf("Error CryptDecrypt() failed - error = 0x%x\n", GetLastError());
        return;
    }
    memcpy(&gLife, gCipherBlock, sizeof(gLife));
    memset(gCipherBlock, 0, gCipherBlockSize);
}

int main(int argc, char* argv[])
{
    CreateContex();

    string key("The secret key");

    CreateKey(key);

    gLife = MAX_LIFE;

    Encrypt();

    SHORT result = 0;

    while (true)
    {
        result = GetAsyncKeyState(0x31);

        Decrypt();

        if (result != 0xFFFF8001)
            gLife = gLife - 1;
        else
            gLife = gLife + 1;

        printf("life = %u\n", gLife);

        if (gLife == 0)
            break;

        Encrypt();

        Sleep(1000);
    }
    printf("stop\n");
    return 0;
}
