#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandsIdentifier.h"
#include "writeToFile.h"
#include "errorHandler.h"
#include "commandsIdentifier.h"
#include "stringConverter.h"

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
int memory[MEMORY_SIZE][WORD_SIZE];

/**
 *  * Function: convertCommandToOpcode
 * -----------------------------------
 * This function takes a command name and finds its corresponding opcode based on the
 * predefined list of command names.
 * The command opcode are from: 0-15 (in decimal base).
 * @param commandName The name of the command to be converted to opcode.
 * @return
 *      1. If the provided command name is recognized, it returns the opcode which is a 0-based index.
 *      2. If the command name is not recognized, the program prints an error message and exits with status code 1.
 */
int convertCommandToOpcode(char *commandName)
{
    for (int i = 0; i < COMMANDS_NUMBER; i++)
    {
        if (strcmp(commandsNames[i], commandName) == 0)
        {
            return i;
        }
    }
    printf("Error: Invalid command name\n");
    exit(1);
}

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

    binaryString[4] = '\0';

    for (int i = 3; i >= 0; i--)
    {
        binaryString[i] = (opcode % 2) + '0';
        opcode /= 2;
    }

    return binaryString;
}

//
/**
 * * Function: encodeInstructionByDirectAddressing
 * -------------------------------------------------
 * This function encodes an instruction, using the Direct Addressing method.
 * @param commandName The name of the command.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 */
void encodeInstructionByDirectAddressing(char *commandName, int ARE, int srcRegister, int dstRegister, OperandCount operandCount)
{
    // if there is an error, return
    if (getErrorsCounter() > 0)
    {
        return;
    }

    // Validate if the command is suitable for the number of operands
    if (!isValidCommandForOperandCount(commandName, operandCount))
    {
        printf("Error: Invalid command for %d operand instruction\n", operandCount);
        exit(1);
    }

    // Convert the command name to opcode
    int opcode = convertCommandToOpcode(commandName);

    // Convert opcode to binary string
    const char *binaryOpcode = convertOpCodeToBinary(opcode);

    // Initialize an array to store the instruction bits
    int instructionBits[WORD_SIZE] = {0};

    // Encode the ARE and opcode into the instruction bits
    instructionBits[0] = (ARE >> 1) & 1;
    instructionBits[1] = ARE & 1;
    for (int i = 0; i < 4; i++)
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
}

/**
 * * Function: encodeInstructionByImmediateAddressing
 * -------------------------------------------------
 * This function encodes an instruction using the Immediate Addressing method.
 * @param commandName The name of the command.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 */
void encodeInstructionByImmediateAddressing(char *commandName, int ARE, int srcRegister, int dstRegister, OperandCount operandCount)
{
    // if there is an error, return
    if (getErrorsCounter() > 0)
    {
        return;
    }

    // Validate if the command is suitable for the number of operands
    if (!isValidCommandForOperandCount(commandName, operandCount))
    {
        printf("Error: Invalid command for %d operand instruction\n", operandCount);
        exit(1);
    }

    // Convert the command name to opcode
    int opcode = convertCommandToOpcode(commandName);

    // Convert opcode to binary string
    const char *binaryOpcode = convertOpCodeToBinary(opcode);

    // Initialize an array to store the instruction bits
    int instructionBits[WORD_SIZE] = {0};

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
}

/**
 * * Function: encodeInstructionByDirectRegisterAddressing
 * --------------------------------------------------------
 * This function encodes an instruction using Direct Register Addressing method.
 * @param commandName The name of the command.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 */
void encodeInstructionByDirectRegisterAddressing(char *commandName, int ARE, int srcRegister, int dstRegister, OperandCount operandCount)
{
    // if there is an error, return
    if (getErrorsCounter() > 0)
    {
        return;
    }

    // Validate if the command is suitable for the number of operands
    if (!isValidCommandForOperandCount(commandName, operandCount))
    {
        printf("Error: Invalid command for %d operand instruction\n", operandCount);
        exit(1);
    }

    // Convert the command name to opcode
    int opcode = convertCommandToOpcode(commandName);

    // Convert opcode to binary string
    const char *binaryOpcode = convertOpCodeToBinary(opcode);

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
}
