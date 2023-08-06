#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "writeToFile.h"
#include "errorsHandler.h"
#include "encoder.h"

#define fileSuffix_commands ".ob"
#define fileSuffix_ENTRY ".ent"
#define fileSuffix_EXTERN ".ext"
#define fileSuffix_EXTERN ".am"

void encodLabelOperand(char *fileName, char AREcode, int address);
void encodImmidiate(char *fileName, int immidiate);
void setARE(char AREcode, int *commandCode);
void setBinaryArray(int arr[], int decimalNumber, int borderCell);
;
char *concatenateStrings(const char *str1, const char *str2);
char *removeFileNameExtension(const char *filename);

void encodExternLabel(char *fileName)
{
    int code[12] = {0};
    setARE('E', code);
}

void printIntArray(const int array[])
{
    printf("Int Array: ");
    int i;
    for (i = 0; i < 12; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void encodInstructionCode(char *fileName, char AREcode, int srcAddressing, int opcode, int dstAddressing)
{
    int code[12] = {0};
    setBinaryCodeInRange(0, 2, srcAddressing, code);
    setBinaryCodeInRange(3, 6, opcode, code);
    setBinaryCodeInRange(7, 9, dstAddressing, code);
    setARE(AREcode, code);
    fileName = removeFileNameExtension(fileName);
    appendStringToFile(concatenateStrings(fileName, fileSuffix_commands), binaryArrayToBase64(code, 12));
} /*in the encoder: (ARE, dstAddressing, opcode, srcAddressing)*/

void encodLabelOperand(char *fileName, char AREcode, int address)
{
    printf("encod Label Operand in address: %d\n", address);
    int code[12] = {0};
    if (AREcode == 'E')
        code[11] = 1;
    else
    {
        setBinaryCodeInRange(0, 9, address, code);
        setARE(AREcode, code);
    }

    fileName = removeFileNameExtension(fileName);
    printf("add new command in base64 to file: %s\n", binaryArrayToBase64(code, 12));
    appendStringToFile(concatenateStrings(fileName, fileSuffix_commands), binaryArrayToBase64(code, 12));
    /*printCommandToFile(fileName, code); // Implement printCommandToFile function*/
}

void encodImmidiate(char *fileName, int immediate)
{

    int code[12] = {0};
    (immediate < 0) ? setNegativeBinaryArray(code, immediate, 10) : setBinaryArray(code, immediate, 10);
    setARE('A', code);
    fileName = removeFileNameExtension(fileName);
    printIntArray(code);
    appendStringToFile(concatenateStrings(fileName, fileSuffix_commands), binaryArrayToBase64(code, 12));
}

void encodValue(char *fileName, int value)
{
    int code[12] = {0};
    (value < 0) ? setNegativeBinaryArray(code, value, 12) : setBinaryArray(code, value, 12);

    fileName = removeFileNameExtension(fileName);
    printIntArray(code);
    appendStringToFile(concatenateStrings(fileName, fileSuffix_commands), binaryArrayToBase64(code, 12));
}

void encodeRegister(char *fileName, int register1, int register2)
{
    printf("encode Register: %s \n", fileName);
    int code[12] = {0};
    setBinaryCodeInRange(0, 4, register1, code);
    setBinaryCodeInRange(5, 9, register2, code);
    setARE('A', code);

    fileName = removeFileNameExtension(fileName);
    appendStringToFile(concatenateStrings(fileName, fileSuffix_commands), binaryArrayToBase64(code, 12));
}

void setBinaryCodeInRange(int startCell, int endCell, int number, int arr[])
{
    int i;
    for (i = endCell; i >= startCell; i--)
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

char base64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *binaryArrayToBase64(int *inrArray, int length)
{
    char *binaryString = (char *)malloc(length * sizeof(char) * 6);
    char *base64String = (char *)malloc(length * sizeof(char) * 2);
    int i, j = 0;

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

    int mask = 1 << borderCell - 1;
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
    for (i = size - 1; i >= 0; i--)
    {
        if (carry == 0)
        {
            break;
        }

        int sum = binaryArray[i] + carry;
        binaryArray[i] = sum % 2;
        carry = sum / 2;
    }
}

void setNegativeBinaryArray(int arr[], int decimalNumber, int borderCell)
{
    decimalNumber = -decimalNumber;
    int i;
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

    if (result != NULL)
    {
        strcpy(result, str1);
        strcat(result, str2);
    }

    return result;
}

char *removeFileNameExtension(const char *filename)
{
    const char *extension = strrchr(filename, '.');
    if (extension != NULL)
    {
        size_t extensionIndex = extension - filename;
        char *result = (char *)malloc(extensionIndex + 1);
        if (result != NULL)
        {
            strncpy(result, filename, extensionIndex);
            result[extensionIndex] = '\0';
            return result;
        }
    }
    return filename;
}