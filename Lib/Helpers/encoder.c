#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandsIdentifier.h"
#include "writeToFile.h"
#include "errorHandler.h"
#include "commandsIdentifier.h"
#include <math.h>

#define WORD_SIZE 12
#define MEMORY_SIZE 1024

// Define addressing method codes
#define IMMEDIATE_ADDRESSING 1
#define DIRECT_ADDRESSING 3
#define DIRECT_REGISTER_ADDRESSING 5

// Define the A, R, E values
#define ABSOLUTE_ENCODING 0
#define EXTERNAL_ENCODING 1
#define RELOCATION_ENCODING 2

// Define OperandCount enum
typedef enum
{
    NoOperand,
    SingleOperand,
    TwoOperands
} OperandCount;

// Each memory cell is an array of 12 bits
static int memory[MEMORY_SIZE][WORD_SIZE]; // static ?

/**
 * Function: convertOpCodeToBinary
 * --------------------------------
 * Converts an integer opcode to its binary representation.
 * @param opcode
 *      The opcode to be converted to binary.
 * @return
 *      A string that represents the binary form of the opcode.
 */
const char *convertOpCodeToBinary(int opcode)
{
    static char binaryString[5];
    int i;

    binaryString[4] = '\0';

    for (i = 3; i >= 0; i--)
    {
        binaryString[i] = (opcode % 2) + '0';
        opcode /= 2;
    }

    return binaryString;
}

/**
 * Function: wordToBase64
 * ---------------------
 * Converts a 12-bit word to its base64 representation.
 * @param word The 12-bit word to convert.
 * @param str The string to store the base64 representation (must be at least 3 characters long).
 */
void wordToBase64(unsigned short word, char *str)
{
    const char *base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    str[0] = base64Chars[(word >> 6) & 0x3F];
    str[1] = base64Chars[word & 0x3F];
    str[2] = '\0';
}

//
/**
 * * Function: encodeInstructionByDirectAddressing
 * -------------------------------------------------
 * This function encodes an instruction, using the Direct Addressing method.
 * @param command: The command to be encoded.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 * @param filename The name of the file to write to.
 */
void encodeInstructionByDirectAddressing(char *command, int ARE, int srcRegister, int dstRegister, OperandCount operandCount, char *filename)
{
    int opcode;
    const char *binaryOpcode;
    int instructionBits[WORD_SIZE] = {0}; // Initialize an array to store the instruction bits

    // if there is an error, return
    if (getErrorsCounter() > 0)
    {
        return;
    }

    // Convert the command name to opcode
    opcode = getCommandIndexByList(command, commandsNames);

    // Convert opcode to binary string
    binaryOpcode = convertOpCodeToBinary(opcode);

    // Encode the ARE and opcode into the instruction bits
    instructionBits[0] = (ARE >> 1) & 1;
    instructionBits[1] = ARE & 1;
    int i;
    for (i = 0; i < 4; i++)
    {
        instructionBits[i + 5] = binaryOpcode[i] - '0'; // Convert char to int and store in instructionBits.
    }

    // Based on the number of operands, encode the source and/or destination register and store the instruction in the corresponding memory cell(s).
    switch (operandCount)
    {
    case NoOperand:
        // noOperands case: simply store the instruction in all memory cells.
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                memory[i][j] = instructionBits[j];
            }
        }
        break;
    case SingleOperand:
        // singleOperand case: validate the destination register, encode it and store the instruction in the corresponding memory cell.
        if (dstRegister >= 0 && dstRegister < MEMORY_SIZE)
        {
            for (int i = 0; i < 3; i++)
            {
                instructionBits[i + 2] = (dstRegister >> (2 - i)) & 1;
            }
            for (int i = 0; i < WORD_SIZE; i++)
            {
                memory[dstRegister][i] = instructionBits[i];
            }
        }
        else
        {
            printf("Error: Invalid memory address\n");
            exit(1);
        }
        break;
    case TwoOperands:
        // twoOperands case: validate the source register, encode the source and destination register and store the instruction in the corresponding memory cell.
        if (srcRegister >= 0 && srcRegister < MEMORY_SIZE)
        {
            for (int i = 0; i < 3; i++)
            {
                instructionBits[i + 2] = (dstRegister >> (2 - i)) & 1;
                instructionBits[i + 9] = (srcRegister >> (2 - i)) & 1;
            }
            for (int i = 0; i < WORD_SIZE; i++)
            {
                memory[srcRegister][i] = instructionBits[i];
            }
        }
        else
        {
            printf("Error: Invalid memory address\n");
            exit(1);
        }
        break;
    }
    // Convert the instructionBits to base64 and write to the file
    char str[3];
    wordToBase64(instructionBits[0] << 6 | instructionBits[1] << 5 | instructionBits[2] << 4 | instructionBits[3] << 3 |
                     instructionBits[4] << 2 | instructionBits[5] << 1 | instructionBits[6] | instructionBits[7] << 11 |
                     instructionBits[8] << 10 | instructionBits[9] << 9 | instructionBits[10] << 8 | instructionBits[11] << 7,
                 str);
    char fullFilename[256];
    sprintf(fullFilename, "%s.ob", filename);
    writeToFile(fullFilename, str);
}

/**
 * * Function: encodeInstructionByImmediateAddressing
 * -------------------------------------------------
 * This function encodes an instruction using the Immediate Addressing method.
 * @param command The name of the command.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 * @param fileName - The name of the file to write to.
 */
* /
    void encodeInstructionByImmediateAddressing(char *command, int ARE, int srcRegister, int dstRegister, OperandCount operandCount, char *filename)
{
    int opcode;
    const char *binaryOpcode;
    int instructionBits[WORD_SIZE] = {0}; // Initialize an array to store the instruction bits

    // if there is an error, return
    if (getErrorsCounter() > 0)
    {
        return;
    }

    // Convert the command name to opcode
    opcode = getCommandIndexByList(command, commandsNames);

    // Convert opcode to binary string
    binaryOpcode = convertOpCodeToBinary(opcode);

    // Encode the ARE and opcode into the instruction bits
    instructionBits[0] = (ARE >> 1) & 1;
    instructionBits[1] = ARE & 1;
    for (int i = 0; i < 4; i++)
    {
        instructionBits[i + 5] = binaryOpcode[i] - '0'; // Convert char to int and store in instructionBits.
    }

    // Based on the number of operands, encode the source and/or destination register and store the instruction in all memory cells.
    switch (operandCount)
    {
    case NoOperand:
        // noOperands case: simply store the instruction in all memory cells.
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                memory[i][j] = instructionBits[j];
            }
        }
        break;
    case SingleOperand:
        // singleOperand case: encode the destination register and store the instruction in all memory cells.
        for (int i = 0; i < 3; i++)
        {
            instructionBits[i + 2] = (dstRegister >> (2 - i)) & 1;
        }
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                memory[i][j] = instructionBits[j];
            }
        }
        break;
    case TwoOperands:
        // twoOperands case: encode the source and destination register and store the instruction in all memory cells.
        for (int i = 0; i < 3; i++)
        {
            instructionBits[i + 2] = (dstRegister >> (2 - i)) & 1;
            instructionBits[i + 9] = (srcRegister >> (2 - i)) & 1;
        }
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                memory[i][j] = instructionBits[j];
            }
        }
        break;
    }

    char str[3];
    wordToBase64(instructionBits[0] << 6 | instructionBits[1] << 5 | instructionBits[2] << 4 | instructionBits[3] << 3 |
                     instructionBits[4] << 2 | instructionBits[5] << 1 | instructionBits[6] | instructionBits[7] << 11 |
                     instructionBits[8] << 10 | instructionBits[9] << 9 | instructionBits[10] << 8 | instructionBits[11] << 7,
                 str);
    char fullFilename[256];
    sprintf(fullFilename, "%s.ob", fileName);
    writeToFile(fullFilename, str);
}

/**
 * * Function: encodeInstructionByDirectRegisterAddressing
 * --------------------------------------------------------
 * This function encodes an instruction using Direct Register Addressing method.
 * @param command The name of the command.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 * @param filename The name of the file to write to.
 */
void encodeInstructionByDirectRegisterAddressing(char *command, int ARE, int srcRegister, int dstRegister, OperandCount operandCount, char *filename)
{
    int opcode;
    const char *binaryOpcode;
    int instructionBits[WORD_SIZE] = {0}; // Initialize an array to store the instruction bits

    // if there is an error, return
    if (getErrorsCounter() > 0)
    {
        return;
    }

    // Convert the command name to opcode
    opcode = getCommandIndexByList(command, commandsNames);

    // Convert opcode to binary string
    binaryOpcode = convertOpCodeToBinary(opcode);

    // Initialize an array to store the instruction bits
    int instructionBits[WORD_SIZE] = {0};

    // Encode the ARE and opcode into the instruction bits
    instructionBits[0] = (ARE >> 1) & 1;
    instructionBits[1] = ARE & 1;
    for (int i = 0; i < 4; i++)
    {
        instructionBits[i + 5] = binaryOpcode[i] - '0'; // Convert char to int and store in instructionBits.
    }

    // Based on the number of operands, encode the source and/or destination register and store the instruction in memory.
    switch (operandCount)
    {
    case NoOperand:
        // noOperands case: simply store the instruction in all memory cells.
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                memory[i][j] = instructionBits[j];
            }
        }
        break;
    case SingleOperand:
        // singleOperand case: encode the destination register and store the instruction in all memory cells.
        for (int i = 0; i < 5; i++)
        {
            instructionBits[i + 2] = (dstRegister >> (4 - i)) & 1;
        }
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                memory[i][j] = instructionBits[j];
            }
        }
        break;
    case TwoOperands:
        // twoOperands case: encode the source and destination register and store the instruction in all memory cells.
        for (int i = 0; i < 5; i++)
        {
            instructionBits[i + 2] = (dstRegister >> (4 - i)) & 1;
            instructionBits[i + 7] = (srcRegister >> (4 - i)) & 1;
        }
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                memory[i][j] = instructionBits[j];
            }
        }
        break;
    }

    char str[3];
    wordToBase64(instructionBits[0] << 6 | instructionBits[1] << 5 | instructionBits[2] << 4 | instructionBits[3] << 3 |
                     instructionBits[4] << 2 | instructionBits[5] << 1 | instructionBits[6] | instructionBits[7] << 11 |
                     instructionBits[8] << 10 | instructionBits[9] << 9 | instructionBits[10] << 8 | instructionBits[11] << 7,
                 str);
    char fullFilename[256];
    sprintf(fullFilename, "%s.ob", fileName);
    writeToFile(fullFilename, str);
}
