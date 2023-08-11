#ifndef DATASERVICE_H
#define DATASERVICE_H

#include "errorsHandler.h"

#define EXTERN_LABEL_TYPE "extern"
#define ENTRY_LABEL_TYPE "entry"
#define NORMAL_LABEL_TYPE ""
#define DATA_LABEL_TYPE "data"
#define STRING_LABEL_TYPE "string"

#define ARE_CODE_A 'A'
#define ARE_CODE_R 'R'
#define ARE_CODE_E 'E'

#define MAX_LABEL_NAME_LENGTH 31

/* Representing the types of labels. */
typedef enum
{
    External,
    Entry,
    NormalCommand
} LabelType;

/* AddressingMethod defines types of operand addressing: None (Code 0), ImmediateAddressing method (Code 1), DirectAddressing method (Code 3), and RegisterDirectAddressing method (Code 5). */
typedef enum
{
    None = 0,
    Immediate = 1,
    Direct = 3,
    RegisterDirect = 5
} AddressingMethod;

struct Label
{
    char *type;
    char name[MAX_LABEL_NAME_LENGTH];
    int address;
} Label;

struct LabelNode
{
    struct Label *label;
    struct LabelNode *next;
} LabelNode;

struct DataLabel
{
    struct Label *label;
    int *data;
    int size;
    struct DataLabel *next;
} DataLabel;

struct StringLabel
{
    struct Label *label;
    char *string;
    struct StringLabel *next;
} StringLabel;

/*
 * Function to add a new line to the assembly code.
 * @param fileName: Name of the file being processed.
 * @param opcode: Operation code.
 * @param register1: First register.
 * @param register2: Second register.
 * @param label1: First label.
 * @param label2: Second label.
 * @param immidiate1: First immediate value.
 * @param immidiate2: Second immediate value.
 */
void addNewLine(char *fileName, int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2);

/*
 * Validates if the opcode matches the given addressing methods.
 * @param opcode: The operation code.
 * @param srcAddressing: The addressing method for the source operand.
 * @param dstAddressing: The addressing method for the destination operand.
 * @return true if the opcode matches the addressing methods, false otherwise.
 */
bool validateOpcodeMatchAddressingMethod(int opcode, int srcAddressing, int dstAddressing);

/*
 * Prints the assembly memory (.am) for a given command.
 * @param fileName: Name of the file being processed.
 * @param command: The command to be printed in assembly memory format.
 */
void printAm(char *fileName, char *command);

/*
 * Adds a new external label.
 * @param externName: Name of the external label.
 * @return true if the external label was added successfully, false otherwise.
 */
bool addNewExtern(char *externName);

/*
 * Adds a new entry label.
 * @param entryName: Name of the entry label.
 * @return true if the entry label was added successfully, false otherwise.
 */
bool addNewEntry(char *entryName);

/*
 * Adds a new data label with associated data.
 * @param data: Array of data to be associated with the label.
 * @param labelName: Name of the label.
 * @param length: Length of the data array.
 * @return true if the data label was added successfully, false otherwise.
 */
bool addData(int data[], char *labelName, int length);

/*
 * Validates if a new entry label is valid.
 * @param label: Name of the label to validate.
 * @return true if the label is valid, false otherwise.
 */
bool isValidNewEntry(char *label);

/*
 * Adds a new string label.
 * @param string: The string to be associated with the label.
 * @param labelName: Name of the label.
 * @return true if the string label was added successfully, false otherwise.
 */
bool addString(char *string, char *labelName);

/*
 * Adds a new label.
 * @param labelName: Name of the label to be added.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewLabel(char *labelName);

/*
 * Increases the Instruction Counter (IC) by a given value.
 * @param value: The value to increase the IC by.
 */
void increaseIC(int value);

/*
 * Searches for an external label.
 * @param externName: Name of the external label to search for.
 * @return The address of the external label if found, -1 otherwise.
 */
int searchExternLabel(char *externName);

/*
 * Searches for an entry label.
 * @param entryName: Name of the entry label to search for.
 * @return The address of the entry label if found, -1 otherwise.
 */
int searchEntry(char *entryName);

/*
 * Searches for a data label.
 * @param labelName: Name of the data label to search for.
 * @return Pointer to the DataLabel structure if found, NULL otherwise.
 */
struct DataLabel *searchDataLabel(char *labelName);

/*
 * Searches for a string label.
 * @param labelName: Name of the string label to search for.
 * @return Pointer to the StringLabel structure if found, NULL otherwise.
 */
struct StringLabel *searchStringLabel(char *labelName);

/*
 * Prints the list of labels.
 * @param head: Pointer to the head of the label list.
 */
void printLabelList(struct LabelNode *head);

/*
 * Prints the labels to a given file.
 * @param filename: Name of the file to print the labels to.
 */
void printLabels(const char *filename);

/*
 * Checks if a label exists.
 * @param label: Name of the label to check for.
 * @param lineNumber: Line number in the source code where the label is found.
 * @param fileName: Name of the source file.
 * @param writeToFile: Flag to determine if the result should be written to file.
 * @param linesNumberForCommand: Number of lines associated with the command using the label.
 * @return true if the label exists, false otherwise.
 */
bool isLabelExist(char *label, int lineNumber, char *fileName, bool writeToFile, int linesNumberForCommand);

/*
 * Sends the value of a string label to a file.
 * @param fileName: Name of the file to send the value to.
 * @param labelName: Name of the string label.
 */
void sendStringValue(char *fileName, char *labelName);

/*
 * Sends the value of a data label to a file.
 * @param fileName: Name of the file to send the value to.
 * @param labelName: Name of the data label.
 */
void sendDataValue(char *fileName, char *labelName);

#endif /* DATASERVICE_H */