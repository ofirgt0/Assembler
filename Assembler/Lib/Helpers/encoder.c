#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "writeToFile.h"
#include "errorsHandler.h"
#include "encoder.h"
#include "helpfulFunctions.h"

void encodExternLabel(char *fileName)
{
    int code[12] = {0};
    setARE('E', code);
}

void printIntArray(const int array[])
{
    int i;
    printf("Int Array: ");
    for (i = 0; i < 12; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void encodInstructionCode(char *fileName, char AREcode, int srcAddressing, int opcode, int dstAddressing)
{
    int code[12] = {0};
    char *base64Str;
    char *concatenatedStr;
    setBinaryCodeInRange(0, 2, srcAddressing, code);
    setBinaryCodeInRange(3, 6, opcode, code);
    setBinaryCodeInRange(7, 9, dstAddressing, code);

    setARE(AREcode, code);
    base64Str = binaryArrayToBase64(code, 12);
    concatenatedStr = concatenateStrings(fileName, fileSuffix_commands);
    appendStringToFile(concatenatedStr, base64Str);
    free(base64Str);
    free(concatenatedStr);
} /*in the encoder: (ARE, dstAddressing, opcode, srcAddressing)*/

void encodLabelOperand(char *fileName, char AREcode, int address)
{
    int code[12] = {0};
    char *concatenatedStr;

    printf("encod Label Operand in address: %d\n", address);
    if (AREcode == 'E')
        code[11] = 1;
    else
    {
        setBinaryCodeInRange(0, 9, address, code);
        setARE(AREcode, code);
    }

    printf("add new command in base64 to file: %s\n", binaryArrayToBase64(code, 12));
    concatenatedStr = concatenateStrings(fileName, fileSuffix_commands);
    appendStringToFile(concatenatedStr, binaryArrayToBase64(code, 12));
    free(concatenatedStr);
}

void encodImmidiate(char *fileName, int immediate)
{

    int code[12] = {0};
    char *base64Str;
    char *concatenatedStr;
    (immediate < 0) ? setNegativeBinaryArray(code, immediate, 10) : setBinaryArray(code, immediate, 10);
    setARE('A', code);
    base64Str = binaryArrayToBase64(code, 12);
    concatenatedStr = concatenateStrings(fileName, fileSuffix_commands);
    appendStringToFile(concatenatedStr, base64Str);
    free(base64Str);
    free(concatenatedStr);
}

void encodValue(char *fileName, int value)
{
    int code[12] = {0};
    char *base64Str;
    char *concatenatedStr;
    (value < 0) ? setNegativeBinaryArray(code, value, 12) : setBinaryArray(code, value, 12);

    printIntArray(code);
    base64Str = binaryArrayToBase64(code, 12);
    concatenatedStr = concatenateStrings(fileName, fileSuffix_commands);
    appendStringToFile(concatenatedStr, base64Str);
    free(base64Str);
    free(concatenatedStr);
}

void encodeRegister(char *fileName, int register1, int register2)
{
    int code[12] = {0};
    char *base64Str;
    char *concatenatedStr;
    printf("encode Register: %s \n", fileName);
    setBinaryCodeInRange(0, 4, register1, code);
    setBinaryCodeInRange(5, 9, register2, code);
    setARE('A', code);

    base64Str = binaryArrayToBase64(code, 12);
    concatenatedStr = concatenateStrings(fileName, fileSuffix_commands);
    appendStringToFile(concatenatedStr, base64Str);
    free(base64Str);
    free(concatenatedStr);
}

void setBinaryCodeInRange(int startCell, int endCell, int number, int arr[])
{
    int i;
    for (i = endCell; i >= startCell; i--)
    {
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
        INVALID_ARE_CODE(__FILE__, __LINE__);
        exit(1);
    }
}

char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *binaryArrayToBase64(int *inrArray, int length)
{
    char *binaryString = (char *)malloc(length * sizeof(char) * 6);
    char *base64String = (char *)malloc(length * sizeof(char) * 2);
    int i, j = 0;

    if (!binaryString || !base64String)
    {

        if (binaryString)
            free(binaryString);
        if (base64String)
            free(base64String);

        printf("Memory allocation failed");
        return NULL;
    }

    for (i = 0; i < length; i++)
    {
        binaryString[j++] = (inrArray[i] == 0) ? '0' : '1';
    }
    binaryString[j] = '\0';

    j = 0;
    for (i = 0; binaryString[i]; i += 6)
    {
        int chunk = 0;
        int k;
        for (k = 0; k < 6 && binaryString[i + k]; k++)
        {
            chunk = (chunk << 1) | (binaryString[i + k] - '0');
        }
        base64String[j++] = base64Table[chunk];
    }
    base64String[j] = '\0';

    free(binaryString);
    return base64String;
}

void setBinaryArray(int binaryArray[], int decimalNumber, int borderCell)
{

    int mask = 1 << (borderCell - 1);
    int i;

    for (i = 0; i < borderCell; i++)
    {
        binaryArray[i] = (decimalNumber & mask) ? 1 : 0;
        mask >>= 1;
    }
}

void addOneToBinaryArray(int binaryArray[], int size)
{
    int carry = 1;
    int i;
    int sum;
    for (i = size - 1; i >= 0; i--)
    {
        if (carry == 0)
        {
            break;
        }

        sum = binaryArray[i] + carry;
        binaryArray[i] = sum % 2;
        carry = sum / 2;
    }
}

void setNegativeBinaryArray(int arr[], int decimalNumber, int borderCell)
{
    int i;
    decimalNumber = -decimalNumber;
    setBinaryArray(arr, decimalNumber, borderCell);

    for (i = 0; i < borderCell; i++)
    {
        arr[i] = (arr[i] == 1) ? 0 : 1;
    }

    addOneToBinaryArray(arr, borderCell);
}

char *concatenateStrings(const char *str1, const char *str2)
{
    size_t totalLength = strlen(str1) + strlen(str2) + 1;
    char *result = (char *)malloc(totalLength);
    if (!result)
    {
        printf("Memory allocation failed");
        return NULL;
    }

    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

char *removeFileNameExtension(const char *filename)
{
    const char *extension = strrchr(filename, '.');
    if (extension != NULL)
    {
        size_t extensionIndex = extension - filename;
        char *result = (char *)malloc(extensionIndex + 1);
        if (!result)
        {
            printf("Memory allocation failed");
        }

        strncpy(result, filename, extensionIndex);
        result[extensionIndex] = '\0';
        return result;
    }

    return my_strdup(filename);
}