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

/* Define the label structure */
struct Label
{
    char type;
    char name[MAX_LABEL_NAME_LENGTH];
    int address;
};

struct LabelNode
{
    struct Label *label;
    struct LabelNode *next;
};

/* Representing the data label structure. */
struct DataLabel
{
    struct Label *label;
    int *data;
    struct DataLabel *next;
};

/* Representing the string label structure. */
struct StringLabel
{
    struct Label *label;
    char *string;
    struct StringLabel *next;
};

/**
 * Add a new line with the given parameters.
 * @param opcode the opcode.
 * @param register1 the register1.
 * @param register2 the register2.
 * @param label1 the label1.
 * @param label2 the label2.
 * @param immidiate1 the immidiate1.
 * @param immidiate2 the immidiate2.
 */
void addNewLine(int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2);

/**
 * Validate if opcode matches addressing method.
 * @param opcode the opcode.
 * @param srcAddressing the source addressing.
 * @param dstAddressing the destination addressing.
 * @return true if validation is successful, false otherwise.
 */
bool validateOpcodeMatchAddressingMethod(int opcode, int srcAddressing, int dstAddressing);

/**
 * Add a new external label.
 * @param externName the name of the external label.
 * @return true if adding is successful, false otherwise.
 */
bool addNewExtern(char *externName);

/**
 * Add a new entry label.
 * @param entryName the name of the entry label.
 * @return true if adding is successful, false otherwise.
 */
bool addNewEntry(char *entryName);

/**
 * Add data to a label.
 * @param data the data.
 * @param labelName the name of the label.
 * @return true if adding is successful, false otherwise.
 */
bool addData(int data[], char *labelName);

/**
 * Add a string to a label.
 * @param string the string.
 * @param labelName the name of the label.
 * @return true if adding is successful, false otherwise.
 */
bool addString(char *string, char *labelName);

/**
 * Add a new label.
 * @param labelName the name of the label.
 * @return true if adding is successful, false otherwise.
 */
bool addNewLabel(char *labelName);

/**
 * Increase IC by the given value.
 * @param value the value to increase IC by.
 */
void increaseIC(int value);

/**
 * Search for an external label by name.
 * @param externName the name of the external label.
 * @return the address of the external label if found, -1 otherwise.
 */
int searchExternLabel(char *externName);

/**
 * Search for an entry label by name.
 * @param entryName the name of the entry label.
 * @return the address of the entry label if found, -1 otherwise.
 */
int searchEntry(char *entryName);

/**
 * Search for a data label by name.
 * @param data the data.
 * @param labelName the name of the label.
 * @return the address of the data label if found, -1 otherwise.
 */
int searchDataLabel(char *labelName);

/**
 * Search for a string label by name.
 * @param string the string.
 * @param labelName the name of the label.
 * @return the address of the string label if found, -1 otherwise.
 */
int searchStringLabel(char *labelName);

#endif /* DATASERVICE_H */