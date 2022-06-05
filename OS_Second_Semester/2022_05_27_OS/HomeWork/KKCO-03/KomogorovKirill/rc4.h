#include <string.h>
#include <stdlib.h>

void swap(unsigned char *a, unsigned char *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int KSA(char *key, unsigned char *S, int len) {
    int j = 0;

    for(int i = 0; i < 256; i++)
        S[i] = i;

    for(int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % len]) % 256;

        swap(&S[i], &S[j]);
    }

    return 0;
}

int PRGA(unsigned char *S, char *plaintext, unsigned char *ciphertext, size_t length) {

    int i = 0;
    int j = 0;

    for(size_t n = 0; n < length; n++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        swap(&S[i], &S[j]);
        int rnd = S[(S[i] + S[j]) % 256];

        ciphertext[n] = rnd ^ plaintext[n];

    }

    return 0;
}

int RC4(char *key, char *plaintext, unsigned char *ciphertext, size_t length) {

    unsigned char S[256];
    KSA(key, S, strlen(key));

    PRGA(S, plaintext, ciphertext, length);

    return 0;
}