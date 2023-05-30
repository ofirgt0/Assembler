#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to convert a string to binary representation
void stringToBinary(const char* input) {
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        unsigned char currentChar = (unsigned char)input[i];
        for (int j = 7; j >= 0; j--) {
            printf("%d", (currentChar >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

// Function to convert a string to base64 representation
void stringToBase64(const char* input) {
    const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int len = strlen(input);
    int outputLen = (int)ceil((double)len * 8 / 6); // Calculate the output length

    char* output = (char*)malloc(outputLen + 1);
    if (output == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    memset(output, '=', outputLen); // Fill the output with '=' as padding characters
    output[outputLen] = '\0';

    for (int i = 0, j = 0; i < len; i += 3, j += 4) {
        unsigned char byte1 = (unsigned char)input[i];
        unsigned char byte2 = (i + 1 < len) ? (unsigned char)input[i + 1] : 0;
        unsigned char byte3 = (i + 2 < len) ? (unsigned char)input[i + 2] : 0;

        unsigned char index1 = byte1 >> 2;
        unsigned char index2 = ((byte1 & 0x03) << 4) | (byte2 >> 4);
        unsigned char index3 = ((byte2 & 0x0F) << 2) | (byte3 >> 6);
        unsigned char index4 = byte3 & 0x3F;

        output[j] = base64Chars[index1];
        output[j + 1] = base64Chars[index2];
        output[j + 2] = base64Chars[index3];
        output[j + 3] = base64Chars[index4];
    }

    printf("%s\n", output);
    free(output);
}