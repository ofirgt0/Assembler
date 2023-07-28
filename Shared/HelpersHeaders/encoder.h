#ifndef ENCODER_H
#define ENCODER_H

/*
    Function: encodExternLabel
    -------------------------
    Encodes an extern label into its machine code representation.

    fileName: The name of the output file.

    Inputs:
    - fileName: A pointer to a string containing the name of the output file.

    Outputs:
    None.
*/
void encodExternLabel(char *fileName);

/*
    Function: encodInstructionCode
    ------------------------------
    Encodes an instruction command into its machine code representation.

    fileName: The name of the output file.
    AREcode: The ARE (Addressing-Relative-External) code for the command.
    srcAddressing: The addressing mode for the source operand.
    opcode: The opcode representing the instruction.
    dstAddressing: The addressing mode for the destination operand.

    Inputs:
    - fileName: A pointer to a string containing the name of the output file.
    - AREcode: A character representing the ARE (Addressing-Relative-External) code ('A', 'R', or 'E').
    - srcAddressing: An integer representing the addressing mode for the source operand.
    - opcode: An integer representing the opcode for the instruction.
    - dstAddressing: An integer representing the addressing mode for the destination operand.

    Outputs:
    None.
*/
void encodInstructionCode(char *fileName, char AREcode, int srcAddressing, int opcode, int dstAddressing);

/*
    Function: encodLabelOperand
    ---------------------------
    Encodes a label operand into its machine code representation.

    fileName: The name of the output file.
    AREcode: The ARE (Addressing-Relative-External) code for the operand.
    address: The address of the label.

    Inputs:
    - fileName: A pointer to a string containing the name of the output file.
    - AREcode: A character representing the ARE (Addressing-Relative-External) code ('A', 'R', or 'E').
    - address: An integer representing the address of the label.

    Outputs:
    None.
*/
void encodLabelOperand(char *fileName, char AREcode, int address);

/*
    Function: encodImmidiate
    ------------------------
    Encodes an immediate value into its machine code representation.

    fileName: The name of the output file.
    immediate: The immediate value to be encoded.

    Inputs:
    - fileName: A pointer to a string containing the name of the output file.
    - immediate: An integer representing the immediate value.

    Outputs:
    None.
*/
void encodImmidiate(char *fileName, int immediate);

/*
    Function: encodeRegister
    ------------------------
    Encodes two registers into their machine code representation.

    fileName: The name of the output file.
    register1: The first register value to be encoded.
    register2: The second register value to be encoded.

    Inputs:
    - fileName: A pointer to a string containing the name of the output file.
    - register1: An integer representing the first register value.
    - register2: An integer representing the second register value.

    Outputs:
    None.
*/
void encodeRegister(char *fileName, int register1, int register2);

/*
    Function: setBinaryCodeInRange
    ------------------------------
    Sets the binary representation of a number in a specified range of an array.

    startCell: The starting index of the range.
    endCell: The ending index of the range.
    number: The number to be converted to binary.
    arr: The array in which the binary representation will be stored.

    Inputs:
    - startCell: An integer representing the starting index of the range.
    - endCell: An integer representing the ending index of the range.
    - number: An integer representing the number to be converted to binary.
    - arr: A pointer to an integer array in which the binary representation will be stored.

    Outputs:
    None.
*/
void setBinaryCodeInRange(int startCell, int endCell, int number, int arr[]);

/*
    Function: setARE
    ----------------
    Sets the ARE (Addressing-Relative-External) code in the machine code array.

    AREcode: The ARE code to be set.
    commandCode: The machine code array.

    Inputs:
    - AREcode: A character representing the ARE (Addressing-Relative-External) code ('A', 'R', or 'E').
    - commandCode: A pointer to an integer array representing the machine code.

    Outputs:
    None.
*/
void setARE(char AREcode, int *commandCode);

/**
    Function: setBinaryArray
    ------------------------
    Converts a decimal number to binary and stores the result in an array.

    @param arr: The array in which the binary representation will be stored.
    @param decimalNumber: The decimal number to be converted to binary.

    Inputs:
    - arr: A pointer to an integer array in which the binary representation will be stored.
    - decimalNumber: An integer representing the decimal number to be converted to binary.

    Outputs:
    None.
*/
void setBinaryArray(int arr[], int decimalNumber);

#endif // ENCODER_H
