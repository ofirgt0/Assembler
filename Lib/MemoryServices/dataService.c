#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "errorsHandler.h"
#include "dataService.h"
#include "writeToFile.h"

// TODO: validate if we need to move the define to the header file or should it stay here
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
    External,     /* Represents an external label. */
    Entry,        /* Represents an entry label. */
    NormalCommand /* Represents a normal command label. */
} LabelType;

/* Define the label structure */
typedef struct Label
{
    LabelType type;
    char name[MAX_LABEL_NAME_LENGTH]; /* Holds the label's name. */
    int address;                      /* Holds the label's address. */
} Label;

typedef struct LabelNode
{
    Label *label;
    LabelNode *next; /* Points to the next label in the linked list. */
} LabelNode;

/* Representing the data label structure. */
typedef struct DataLabel
{
    Label *label;
    int *data;
    DataLabel *next;
} DataLabel;

/* Representing the string label structure. */
typedef struct StringLabel
{
    Label *label;
    char *string;
    StringLabel *next;
} StringLabel;

/* Initialize the label lists. */
static LabelNode *externalLabelList = NULL;
static LabelNode *entryLabelList = NULL;
static LabelNode *normalCommandLabelList = NULL;
static DataLabel *dataLabelList = NULL;
static StringLabel *stringLabelList = NULL;

/* Initialize the global counters. */
static int IC = 100; /* Instruction counter. */
static int DC = 0; /* Data counter. */

void addNewLine(int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2)
{

    int dstAddressing = register2 != -1 ? 5 : (label2 != NULL ? 3 : (immidiate2 != 0.5 ? 1 : 0));
    int srcAddressing = register1 != -1 ? 5 : (label1 != NULL ? 3 : (immidiate1 != 0.5 ? 1 : 0)); // 0.5 means null for int
    /// TODO: move method addressing code 1 3 5 to enum

    IC++; // for the base command
    encodeInstructionCode(ARE_CODE_A, srcAddressing, opcode, dstAddressing);

    // if dstAddressing is 0 do it means that its command with 1 opperand,
    // then we need to consider the srcAddressing as dstAddressing
    bool commandValidation = (dstAddressing == 0) ? validateOpcodeMatchAddressingMethod(opcode, dstAddressing, srcAddressing) : validateOpcodeMatchAddressingMethod(opcode, srcAddressing, dstAddressing);

    if (!commandValidation)
    {
        /// TODO: handle error
    }

    if (opcode > 13) // only 1 line for stop and rts
        return;

    if (register1 != -1)
    {
        IC++;
        if (register2 != -1)
        {
            encodeRegistersOperands(ARE_CODE_A, register1, register2);
            return;
        }
        else
        {
            encodeRegistersOperands(ARE_CODE_A, register1, 0);
        }
    }
    else if (label1 != NULL)
    {
        IC++;
        int address = tryGetLabel(label1)->label->address; /// TODO: we need to check if the label is external or entry to set the ARE code as well
        encodeLabelOperand(ARE_CODE_R, label1, address);
    }
    else if (immidiate1 != 0.5)
    {
        IC++;
        encodeImmidiateOperands(ARE_CODE_R, (int)immidiate1);
    }

    if (register2 != -1)
    {
        IC++;
        encodeRegistersOperands(ARE_CODE_A, 0, register2);
        return;
    }
    else if (label2 != NULL)
    {
        IC++;
        int address = tryGetLabel(label2)->label->address; /// TODO: we need to check if the label is external or entry to set the ARE code as well
        encodeLabelOperand(ARE_CODE_R, label2, address);
    }
    else if (immidiate2 != 0.5)
    {
        IC++;
        encodeImmidiateOperands(ARE_CODE_A, (int)immidiate2);
    }
}

bool validateOpcodeMatchAddressingMethod(int opcode, int srcAddressing, int dstAddressing) // validate as page 48
{
    bool validateSrcAddressing = (opcode < 4 || (opcode == 6 && srcAddressing == 3)) || srcAddressing == 0;
    bool validateDstAddressing = (opcode > 13 && dstAddressing == 0) ||
                                 (opcode < 14 && (dstAddressing == 3 || dstAddressing == 5 || (dstAddressing == 1 && (opcode == 1 || opcode == 12))));
    return validateSrcAddressing && validateDstAddressing;
}

bool addNewExtern(char *externName)
{
    Label *label = (Label *)malloc(sizeof(Label));
    if (label == NULL)
    {
        // Memory allocation failed
        return false;
    }

    label->type = EXTERN_LABEL_TYPE;
    strncpy(label->name, externName, MAX_LABEL_NAME_LENGTH);
    label->address = -1; // Assuming -1 represents an external label address.

    LabelNode *newNode = (LabelNode *)malloc(sizeof(LabelNode));
    if (newNode == NULL)
    {
        // Memory allocation failed
        free(label); // Clean up the previously allocated memory
        return false;
    }

    newNode->label = label;
    newNode->next = externalLabelList;
    externalLabelList = newNode;

    return true;
}

bool addNewEntry(char *entryName)
{
    Label *label = (Label *)malloc(sizeof(Label));
    if (label == NULL)
    {
        // Memory allocation failed
        return false;
    }

    label->type = ENTRY_LABEL_TYPE;
    strncpy(label->name, entryName, MAX_LABEL_NAME_LENGTH);
    label->address = -1; // The address will be set later when the entry is resolved.

    LabelNode *newNode = (LabelNode *)malloc(sizeof(LabelNode));
    if (newNode == NULL)
    {
        // Memory allocation failed
        free(label); // Clean up the previously allocated memory
        return false;
    }

    newNode->label = label;
    newNode->next = entryLabelList;
    entryLabelList = newNode;

    return true;
}

bool addData(int data[], char *labelName)
{
    DC++;
    Label *label = (Label *)malloc(sizeof(Label));
    if (label == NULL)
    {
        // Memory allocation failed
        return false;
    }

    label->type = DATA_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->address = DC; // The address will be set later when data is linked to the code.

    DataLabel *newNode = (DataLabel *)malloc(sizeof(DataLabel));
    if (newNode == NULL)
    {
        // Memory allocation failed
        free(label); // Clean up the previously allocated memory
        return false;
    }

    newNode->label = label;
    newNode->data = data;
    newNode->next = dataLabelList;
    dataLabelList = newNode;

    DC+= sizeof(data) / sizeof(data[0])-1;

    return true;
}

bool addString(char *string, char *labelName)
{
    DC++;
    Label *label = (Label *)malloc(sizeof(Label));
    if (label == NULL)
    {
        // Memory allocation failed
        return false;
    }

    label->type = STRING_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->address = DC; // The address will be set later when the string is linked to the code.

    StringLabel *newNode = (StringLabel *)malloc(sizeof(StringLabel));
    if (newNode == NULL)
    {
        // Memory allocation failed
        free(label); // Clean up the previously allocated memory
        return false;
    }

    newNode->label = label;
    newNode->string = strdup(string);
    newNode->next = stringLabelList;
    stringLabelList = newNode;

    DC += strlen(string); // +1 FOR /0

    return true;
}

bool addNewLabel(char *labelName)
{
    Label *label = (Label *)malloc(sizeof(Label));
    if (label == NULL)
    {
        // Memory allocation failed
        return false;
    }

    // Set appropriate label type and name as per your requirement
    label->type = NORMAL_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->address = IC;   // due to page 30

    LabelNode *newNode = (LabelNode *)malloc(sizeof(LabelNode));
    if (newNode == NULL)
    {
        // Memory allocation failed
        free(label); // Clean up the previously allocated memory
        return false;
    }

    newNode->label = label;
    newNode->next = normalCommandLabelList;
    normalCommandLabelList = newNode;

    return true;
}

void increaseIC(int value)
{
    IC += value;
}

int searchExternLabel(char *externName)
{
    LabelNode *current = externalLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, externName) == 0)
        {
            return current->label->address;
        }
        current = current->next;
    }
    return -1; // Label not found
}

int searchEntry(char *entryName)
{
    LabelNode *current = entryLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, entryName) == 0)
        {
            return current->label->address;
        }
        current = current->next;
    }
    return -1; // Label not found
}

int searchDataLabel(int data[], char *labelName)
{
    DataLabel *current = dataLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, labelName) == 0)
        {
            return current->label->address + IC;
        }
        current = current->next;
    }
    return -1; // Label not found
}

int searchStringLabel(char *string, char *labelName)
{
    StringLabel *current = stringLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, labelName) == 0)
        {
            if (strcmp(current->string, string) == 0)
            {
                return current->label->address + IC;
            }
        }
        current = current->next;
    }
    return -1; // Label not found
}