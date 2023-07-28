#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "writeToFile.h"
#include "errorsHandler.h"

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
} // in the encoder: (ARE, dstAddressing, opcode, srcAddressing)

void encodLabelOperand(char *fileName, char AREcode, int address)
{
    int code[12] = {0};
    setBinaryCodeInRange(0, 9, address, code);
    setARE(AREcode, code);
    // printCommandToFile(fileName, code); // Implement printCommandToFile function
}

void encodImmidiate(char *fileName, int immediate)
{
    int code[12] = {0};
    setBinaryArray(code, immediate);
    setARE('A', code);
    // printCommandToFile(fileName, code); // Implement printCommandToFile function
}

void encodeRegister(char *fileName, int register1, int register2)
{
    int code[12] = {0};
    setBinaryCodeInRange(0, 4, register1, code);
    setBinaryCodeInRange(5, 9, register2, code);
    setARE('A', code);
    // printCommandToFile(fileName, code); // Implement printCommandToFile function
}

void setBinaryCodeInRange(int startCell, int endCell, int number, int arr[])
{
    for (int i = endCell; i >= startCell; i--)
    { /// TODO: make sure its work correctly
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

void setBinaryArray(int arr[], int decimalNumber) // ?
{
    int i;

    for (i = 0; decimalNumber > 0; i++)
    {
        arr[i] = decimalNumber % 2;
        decimalNumber = decimalNumber / 2;
    }
}
