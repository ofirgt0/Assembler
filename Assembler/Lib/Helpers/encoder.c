#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "writeToFile.h"
#include "errorsHandler.h"
#include "encoder.h"
#include "helpfulFunctions.h"

/**
 * Encodes an 'extern' label with the ARE code set to 'E'.
 * This function takes a filename and encodes an 'extern' label
 * in the associated file using the ARE encoding scheme.
 * @param fileName The name of the file where the 'extern' label resides.
 */
void encodExternLabel(char *fileName)
{
    int code[12] = {0};
    setARE('E', code);
}

/**
 * Encodes an instruction code using ARE encoding.
 * This function takes several parameters that specify different
 * parts of an instruction code and encodes them using the ARE scheme.
 * @param fileName The name of the file to which the instruction will be written.
 * @param AREcode The ARE code character ('A', 'R', or 'E').
 * @param srcAddressing The source addressing mode.
 * @param opcode The operation code.
 * @param dstAddressing The destination addressing mode.
 */
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

/**
 * Encodes a label operand using the ARE encoding scheme.
 * @param fileName The name of the file to which the label operand will be written.
 * @param AREcode The ARE code character ('A', 'R', or 'E').
 * @param address The address of the label operand.
 */
void encodLabelOperand(char *fileName, char AREcode, int address)
{
    int code[12] = {0};
    char *concatenatedStr;

    if (AREcode == 'E')
        code[11] = 1;
    else
    {
        setBinaryCodeInRange(0, 9, address, code);
        setARE(AREcode, code);
    }

    concatenatedStr = concatenateStrings(fileName, fileSuffix_commands);
    appendStringToFile(concatenatedStr, binaryArrayToBase64(code, 12));
    free(concatenatedStr);
}

/**
 * Encodes an immediate value using the ARE encoding scheme.
 * @param fileName The name of the file to which the immediate value will be written.
 * @param immediate The immediate value to encode.
 */
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

/**
 * Encodes a given value into its binary representation.
 * @param fileName The name of the file to which the encoded value will be written.
 * @param value The value to encode.
 */
void encodValue(char *fileName, int value)
{
    int code[12] = {0};
    char *base64Str;
    char *concatenatedStr;
    (value < 0) ? setNegativeBinaryArray(code, value, 12) : setBinaryArray(code, value, 12);
    base64Str = binaryArrayToBase64(code, 12);
    concatenatedStr = concatenateStrings(fileName, fileSuffix_commands);
    appendStringToFile(concatenatedStr, base64Str);
    free(base64Str);
    free(concatenatedStr);
}

/* Encodes a register value, setting the appropriate ARE code and register values. */
void encodeRegister(char *fileName, int register1, int register2)
{
    int code[12] = {0};
    char *base64Str;
    char *concatenatedStr;

    setBinaryCodeInRange(0, 4, register1, code);
    setBinaryCodeInRange(5, 9, register2, code);
    setARE('A', code);

    base64Str = binaryArrayToBase64(code, 12);
    concatenatedStr = concatenateStrings(fileName, fileSuffix_commands);
    appendStringToFile(concatenatedStr, base64Str);
    free(base64Str);
    free(concatenatedStr);
}

/* Sets a binary code for a given number within a specific range in the array. */
void setBinaryCodeInRange(int startCell, int endCell, int number, int arr[])
{
    int i;
    for (i = endCell; i >= startCell; i--)
    {
        arr[i] = number % 2;
        number /= 2;
    }
}

/* Sets the ARE code for a given command code.
 * @param AREcode The ARE code character ('A', 'R', or 'E').
 * @param commandCode The command code array in which the ARE code will be set.
 */

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
        fprintf(stderr, "Inavlid 'ARE' code. \n");
        exit(1);
    }
}

/**
 * Converts a binary array to a Base64 encoded string.
 * @param inrArray The binary array to convert.
 * @param length The length of the binary array.
 * @return A string representing the Base64 encoded version of the binary array.
 */
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

        fprintf(stderr, "Memory allocation failed");
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

/* Sets a binary array representation for a given decimal number up to a specified cell. */
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

/* Increments a binary array representation by 1. */
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

/**
 * Sets a binary array representation for a negative decimal number up to a specified cell.
 * @param arr The binary array to set.
 * @param decimalNumber The negative decimal number to convert to binary.
 * @param borderCell The cell up to which the binary representation should be set.
 */
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

/**
 * Concatenates two strings and returns the result.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return A newly allocated string containing the concatenation of str1 and str2.
 */
char *concatenateStrings(const char *str1, const char *str2)
{
    size_t totalLength = strlen(str1) + strlen(str2) + 1;
    char *result = (char *)malloc(totalLength);
    if (!result)
    {
        fprintf(stderr, "Memory allocation failed");
        return NULL;
    }

    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

/**
 * Removes the file extension from a given filename.
 * This function takes a filename and returns a copy of the filename
 * without its extension.
 * @param filename The filename from which to remove the extension.
 * @return A newly allocated string containing the filename without its extension.
 */
char *removeFileNameExtension(const char *filename)
{
    const char *extension = strrchr(filename, '.');
    if (extension != NULL)
    {
        size_t extensionIndex = extension - filename;
        char *result = (char *)malloc(extensionIndex + 1);
        if (!result)
        {
            fprintf(stderr, "Memory allocation failed");
        }

        strncpy(result, filename, extensionIndex);
        result[extensionIndex] = '\0';
        return result;
    }

    return my_strdup(filename);
}