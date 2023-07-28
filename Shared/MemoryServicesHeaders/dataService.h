#ifndef DATASERVICE_H
#define DATASERVICE_H
#include <stdbool.h>
#include "errorsHandler.h"

/**
 * Searches for a label in all linked lists.
 * @param labelName The name of the label to search for.
 * @return The found label or NULL if the label was not found.
 */
LabelNode *tryGetLabel(const char *labelName);

/**
 * Searches for a data label in the list.
 * @param labelName The name of the label to search for.
 * @return The found data label or NULL if the label was not found.
 */
DataLabel *tryGetDataLabel(char *labelName);

/**
 * Searches for a string label in the list.
 * @param labelName The name of the label to search for.
 * @return The found string label or NULL if the label was not found.
 */
StringLabel *tryGetStringLabel(char *labelName);

/**
 * Checks if a label exists in any of the lists.
 * @param labelName The name of the label to check for.
 * @return True if the label exists, false otherwise.
 */
bool isLabelExist(char *labelName);

/**
 * Determines the type of a label.
 * @param labelType The type string of the label.
 * @return The LabelType enum value corresponding to the type string.
 */
LabelType determineLabelType(char *labelType);

/*
    Function: addNewLine1
    -------------------
    Adds a new line of code to the program with a specific opcode, immediate value, and register.

    prefixLabel: The prefix label for the line.
    opcode: The opcode for the instruction.
    immediate1: The immediate value to be used.
    register2: The register to be used, or -1 if not used.

    Inputs:
    - prefixLabel: A pointer to a string representing the prefix label.
    - opcode: An integer representing the opcode for the instruction.
    - immediate1: An integer representing the immediate value.
    - register2: An integer representing the register (or -1 if not used).

    Outputs:
    None.
*/
void addNewLine1(char *prefixLabel, int opcode, int immidiate1, int register2);

/*
    Function: addNewLine3
    -------------------
    Adds a new line of code to the program with a specific opcode, label, and register.

    prefixLabel: The prefix label for the line.
    opcode: The opcode for the instruction.
    label1: The label to be used, or -1 if not used.
    register2: The register to be used, or -1 if not used.

    Inputs:
    - prefixLabel: A pointer to a string representing the prefix label.
    - opcode: An integer representing the opcode for the instruction.
    - label1: An integer representing the label (or -1 if not used).
    - register2: An integer representing the register (or -1 if not used).

    Outputs:
    None.
*/
void addNewLine3(char *prefixLabel, int opcode, int label1, int register2);

/*
    Function: addNewLine5
    -------------------
    Adds a new line of code to the program with a specific opcode and two registers.

    prefixLabel: The prefix label for the line.
    opcode: The opcode for the instruction.
    register1: The first register to be used, or -1 if not used.
    register2: The second register to be used, or -1 if not used.

    Inputs:
    - prefixLabel: A pointer to a string representing the prefix label.
    - opcode: An integer representing the opcode for the instruction.
    - register1: An integer representing the first register (or -1 if not used).
    - register2: An integer representing the second register (or -1 if not used).

    Outputs:
    None.
*/
void addNewLine5(char *prefixLabel, int opcode, int register1, int register2);

/*
    Function: addNewLine
    -------------------
    Adds a new line of code to the program, determining the addressing methods
    for the source and destination, validating the opcode, and encoding the operands.

    opcode: The opcode for the instruction.
    register1: The first register to be used, or -1 if not used.
    register2: The second register to be used, or -1 if not used.
    label1: The first label to be used, or NULL if not used.
    label2: The second label to be used, or NULL if not used.
    immediate1: The first immediate value to be used, or 0.5 if not used.
    immediate2: The second immediate value to be used, or 0.5 if not used.

    Inputs:
    - opcode: An integer representing the opcode for the instruction.
    - register1: An integer representing the first register (or -1 if not used).
    - register2: An integer representing the second register (or -1 if not used).
    - label1: A pointer to a string representing the first label (or NULL if not used).
    - label2: A pointer to a string representing the second label (or NULL if not used).
    - immediate1: A double representing the first immediate value (or 0.5 if not used).
    - immediate2: A double representing the second immediate value (or 0.5 if not used).

    Outputs:
    None.
*/
void addNewLine(int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2);

/**
 * Validates if an opcode matches the addressing method.
 * @param opcode The opcode to validate.
 * @param srcAddressing The source addressing method.
 * @param dstAddressing The destination addressing method.
 * @return True if the opcode matches the addressing method, false otherwise.
 */
bool validateOpcodeMatchAddressingMethod(int opcode, int srcAddressing, int dstAddressing);

/**
 * Adds a new extern label.
 * @param externName The name of the extern label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addNewExtern(char *externName);

/**
 * Adds a new entry label.
 * @param entryName The name of the entry label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addNewEntry(char *entryName);

/**
 * Adds a new data label.
 * @param data The data array.
 * @param labelName The name of the data label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addData(int data[], char *labelName);

/**
 * Adds a new string label.
 * @param string The string.
 * @param labelName The name of the string label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addString(char *string, char *labelName);

/**
 * Adds a new label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addNewLabel();

/**
 * Searches for an extern label.
 * @param externName The name of the extern label to search for.
 * @return The address of the extern label if found, -1 otherwise.
 */
int searchExternLabel(char *externName);

/**
 * Searches for an entry label.
 * @param entryName The name of the entry label to search for.
 * @return The address of the entry label if found, -1 otherwise.
 */
int searchEntry(char *entryName);

/**
 * Searches for a data label.
 * @param data The data array.
 * @param labelName The name of the data label to search for.
 * @return The address of the data label if found, -1 otherwise.
 */
int searchDataLabel(int data[], char *labelName);

/**
 * Searches for a string label.
 * @param string The string.
 * @param labelName The name of the string label to search for.
 * @return The address of the string label if found, -1 otherwise.
 */
int searchStringLabel(char *string, char *labelName);

#endif /* DATA_SERVICE_H */