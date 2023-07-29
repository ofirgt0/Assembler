#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "writeToFile.h"
#include "errorHandler.h"

void encodLabelOperand(char *fileName, char AREcode, int address);
void encodImmidiate(char *fileName, int immidiate);
void setARE(char AREcode, int *commandCode);
void setBinaryArray(int arr[], int decimalNumber);

void encodExternLabel(char *fileName)
{
    int code[12] = {0};
    setARE('E', code);
}

void encodInstructionCode(char *fileName, char AREcode, int srcAddressing, int opcode, int dstAddressing)
{
    int code[12] = {0};
    setBinaryCodeInRange(0, 2, srcAddressing, code);
    setBinaryCodeInRange(3, 6, opcode, code);
    setBinaryCodeInRange(7, 9, dstAddressing, code);
    setARE(AREcode, code);
} /*in the encoder: (ARE, dstAddressing, opcode, srcAddressing)*/

void encodLabelOperand(char *fileName, char AREcode, int address)
{
    int code[12] = {0};
    setBinaryCodeInRange(0, 9, address, code);
    setARE(AREcode, code);
    /*printCommandToFile(fileName, code); // Implement printCommandToFile function*/
}

void encodImmidiate(char *fileName, int immediate)
{
    int code[12] = {0};
    setBinaryArray(code, immediate);
    setARE('A', code);
    /*printCommandToFile(fileName, code); // Implement printCommandToFile function*/
}

void encodeRegister(char *fileName, int register1, int register2)
{
    int code[12] = {0};
    setBinaryCodeInRange(0, 4, register1, code);
    setBinaryCodeInRange(5, 9, register2, code);
    setARE('A', code);
    /* printCommandToFile(fileName, code); // Implement printCommandToFile function*/
}

void setBinaryCodeInRange(int startCell, int endCell, int number, int arr[])
{
    for (int i = endCell; i >= startCell; i--)
    { /*TODO: make sure its work correctly*/
        arr[i] = number % 2;
        number /= 2;
    }
}

void setARE(char AREcode, int *commandCode)
{
    switch (AREcode)
    {
    case 'A':
        commandCode[10] = 0;
        commandCode[11] = 0;
        break;
    case 'R':
        commandCode[10] = 1;
        commandCode[11] = 0;
        break;
    case 'E':
        commandCode[10] = 0;
        commandCode[11] = 1;
        break;
    default:
        printf("Error: Invalid ARE code\n");
        exit(1);
    }
}

/*Base64 character table*/
const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*Function to encode a binary array to Base64*/
char *encodeToBase64(const int *binaryArray, size_t length)
{
    size_t base64Len = 4 * ((length + 2) / 3); /*Calculate the length of Base64 string*/

    char *base64String = (char *)malloc(base64Len + 1);
    if (!base64String)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    size_t i = 0, j = 0;
    while (i < length)
    {
        unsigned int buffer = 0;
        int bitsRemaining = 24;

        /*Collect 3 bytes (24 bits) from the binary array*/
        while (bitsRemaining && i < length)
        {
            buffer = (buffer << 8) | binaryArray[i];
            bitsRemaining -= 8;
            i++;
        }

        /*Append the Base64 characters to the result string*/
        int shift = 18;
        while (j < base64Len)
        {
            base64String[j++] = base64Chars[(buffer >> shift) & 0x3F];
            shift -= 6;
        }
    }

    /*Add padding characters if needed*/
    while (j % 4 != 0)
    {
        base64String[j++] = '=';
    }

    base64String[j] = '\0';
    return base64String;
}

void setBinaryArray(int arr[], int decimalNumber) /* TODO: to check with ofir if this was the meaning of the function implementaion ? */
{
    int i;

    for (i = 0; decimalNumber > 0; i++)
    {
        arr[i] = decimalNumber % 2;
        decimalNumber = decimalNumber / 2;
    }
}