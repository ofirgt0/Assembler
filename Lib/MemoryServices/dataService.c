#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "errorsHandler.h"
#include "../../Shared/MemoryServicesHeaders/dataService.h"
#include "../../Shared/HelpersHeaders/encoder.h"
#include "writeToFile.h"

/* Initialize the label lists. */
static struct LabelNode *externalLabelList = NULL;
static struct LabelNode *entryLabelList = NULL;
static struct LabelNode *normalCommandLabelList = NULL;
static struct DataLabel *dataLabelList = NULL;
static struct StringLabel *stringLabelList = NULL;

/* Initialize the global counters. */
static int IC = 100; /* Instruction counter. */
static int DC = 0;   /* Data counter. */

/**
 * The addNewLine function is responsible for adding new lines of machine code
 * to the program image. It accepts the opcode, registers, labels, and immediates
 * as arguments and adds them to the binary code. This function also handles encoding
 * of the operands based on the type of addressing and increments the IC (Instruction Counter).
 * Note: -1 is used to indicate an empty register slot.
 */
void addNewLine(int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2)
{

    AddressingMethod dstAddressing;
    AddressingMethod srcAddressing;
    int address = 0;
    bool commandValidation;

    dstAddressing = register2 != -1 ? RegisterDirect : (label2 != NULL ? Direct : (immidiate2 != 0.5 ? Immediate : None)); /*0.5 means null for int*/
    srcAddressing = register1 != -1 ? RegisterDirect : (label1 != NULL ? Direct : (immidiate1 != 0.5 ? Immediate : None)); /*0.5 means null for int*/
    IC++;                                                                                                                  /*for the base command*/
    encodInstructionCode("TODOsetFileName", ARE_CODE_A, srcAddressing, opcode, dstAddressing);

    /*if dstAddressing is 0 do it means that its command with 1 opperand,*/
    /*then we need to consider the srcAddressing as dstAddressing*/
    commandValidation = (dstAddressing == None) ? validateOpcodeMatchAddressingMethod(opcode, dstAddressing, srcAddressing) : validateOpcodeMatchAddressingMethod(opcode, srcAddressing, dstAddressing);

    if (!commandValidation)
    {
        /*TODO: handle error*/
    }

    if (opcode > 13) /*only 1 line for stop and rts*/
        return;

    if (register1 != -1)
    {
        IC++;
        if (register2 != -1)
        {
            encodeRegister(ARE_CODE_A, register1, register2);
            return;
        }
        else
        {
            encodeRegister(ARE_CODE_A, register1, 0);
        }
    }
    else if (label1 != NULL)
    {
        IC++;

        address = searchExternLabel(label1); /*TODO: we need to check if the label is external or entry to set the ARE code as well*/
        if (address != -1)
        {
            encodLabelOperand(ARE_CODE_E, label1, address);
            return;
        }

        address = searchEntry(label1);
        if (address != -1)
        {
            encodLabelOperand(ARE_CODE_R, label1, address);
            return;
        }

        address = searchLabel(label1);
        if (address != -1)
        {
            encodLabelOperand(ARE_CODE_R, label1, address);
            return;
        }
        /* TODO: handle error label not found*/
    }
    else if (immidiate1 != 0.5)
    {
        IC++;
        encodImmidiate(ARE_CODE_R, (int)immidiate1);
    }

    if (register2 != -1)
    {
        IC++;
        encodeRegister(ARE_CODE_A, 0, register2);
        return;
    }
    else if (label2 != NULL)
    {
        IC++;
        address = searchExternLabel(label2); /*TODO: we need to check if the label is external or entry to set the ARE code as well*/
        if (address != -1)
        {
            encodLabelOperand(ARE_CODE_E, label2, address);
            return;
        }

        address = searchEntry(label2);
        if (address != -1)
        {
            encodLabelOperand(ARE_CODE_R, label2, address);
            return;
        }

        address = searchLabel(label2);
        if (address != -1)
        {
            encodLabelOperand(ARE_CODE_R, label2, address);
            return;
        }
    }
    else if (immidiate2 != 0.5)
    {
        IC++;
        encodImmidiate(ARE_CODE_A, (int)immidiate2);
    }
}

/**
 * The validateOpcodeMatchAddressingMethod function checks if the opcode matches
 * the addressing methods. It accepts the opcode, source addressing, and destination
 * addressing as arguments and checks if they are valid according to the assembly
 * language rules. It returns true if the validation is successful and false otherwise.
 */
bool validateOpcodeMatchAddressingMethod(int opcode, int srcAddressing, int dstAddressing) /*validate as page 48*/
{
    bool validateSrcAddressing = (opcode < 4 || (opcode == 6 && srcAddressing == 3)) || srcAddressing == 0;
    bool validateDstAddressing = (opcode > 13 && dstAddressing == 0) ||
                                 (opcode < 14 && (dstAddressing == 3 || dstAddressing == 5 || (dstAddressing == 1 && (opcode == 1 || opcode == 12))));
    return validateSrcAddressing && validateDstAddressing;
}

/**
 * The addNewExtern function is used to add a new external label to the label list.
 * It accepts the name of the external label as an argument. This function creates
 * a new Label instance, sets the label type to External, sets the label name, and
 * appends it to the externalLabelList. It returns true if the operation is successful
 * and false otherwise (e.g., if memory allocation fails).
 */
bool addNewExtern(char *externName)
{
    struct Label *label = NULL;
    struct LabelNode *newNode = NULL;

    label = (struct Label *)malloc(sizeof(struct Label));
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    label->type = EXTERN_LABEL_TYPE;
    strncpy(label->name, externName, MAX_LABEL_NAME_LENGTH);
    label->address = -1; /*Assuming -1 represents an external label address.*/

    newNode = (struct LabelNode *)malloc(sizeof(struct LabelNode));
    if (newNode == NULL)
    {
        /*Memory allocation failed*/
        free(label); /*Clean up the previously allocated memory*/
        return false;
    }

    newNode->label = label;
    newNode->next = externalLabelList;
    externalLabelList = newNode;

    return true;
}

/**
 * The addNewEntry function is used to add a new entry label to the label list.
 * It accepts the name of the entry label as an argument. This function creates
 * a new Label instance, sets the label type to Entry, sets the label name, and
 * appends it to the entryLabelList. It returns true if the operation is successful
 * and false otherwise (e.g., if memory allocation fails).
 */
bool addNewEntry(char *entryName)
{
    struct Label *label = NULL;
    struct LabelNode *newNode = NULL;

    label = (struct Label *)malloc(sizeof(struct Label));
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    label->type = ENTRY_LABEL_TYPE;
    strncpy(label->name, entryName, MAX_LABEL_NAME_LENGTH);
    label->address = -1; /*The address will be set later when the entry is resolved.*/

    newNode = (struct LabelNode *)malloc(sizeof(struct LabelNode));
    if (newNode == NULL)
    {
        /*Memory allocation failed*/
        free(label); /*Clean up the previously allocated memory*/
        return false;
    }

    newNode->label = label;
    newNode->next = entryLabelList;
    entryLabelList = newNode;

    return true;
}

/**
 * The addData function is used to add a new data label to the data label list.
 * It accepts an array of data and the name of the label as arguments. This function
 * creates a new Label and DataLabel instances, sets the label type to Data, sets
 * the label name and data, and appends it to the dataLabelList. It also increments
 * the DC (Data Counter) accordingly. It returns true if the operation is successful
 * and false otherwise (e.g., if memory allocation fails).
 */
bool addData(int data[], char *labelName)
{
    struct Label *label = NULL;
    struct DataLabel *newNode = NULL;

    DC++;
    label = (struct Label *)malloc(sizeof(struct Label));
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    label->type = DATA_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->address = DC; /*The address will be set later when data is linked to the code.*/

    newNode = (struct DataLabel *)malloc(sizeof(struct DataLabel));
    if (newNode == NULL)
    {
        /*Memory allocation failed*/
        free(label); /*Clean up the previously allocated memory*/
        return false;
    }

    newNode->label = label;
    newNode->data = data;
    newNode->next = dataLabelList;
    dataLabelList = newNode;

    DC += sizeof(data) / sizeof(data[0]) - 1;

    return true;
}

/**
 * Duplicates a string by creating a new copy in the heap.
 * This function allocates memory for the new string, copies
 * the original string into the new memory, and returns a pointer to it.
 */
char *my_strdup(const char *s)
{
    char *new = (char *)malloc(strlen(s) + 1); /*+1 for the null-terminator*/
    if (new == NULL)
        return NULL;
    strcpy(new, s);
    return new;
}

/**
 * The addString function is used to add a new string label to the string label list.
 * It accepts a string and the name of the label as arguments. This function creates
 * a new Label and StringLabel instances, sets the label type to String, sets the label
 * name and string, and appends it to the stringLabelList. It also increments the DC (Data Counter)
 * by the length of the string. It returns true if the operation is successful
 * and false otherwise (e.g., if memory allocation fails).
 */
bool addString(char *string, char *labelName)
{
    struct Label *label = NULL;
    struct StringLabel *newNode = NULL;

    DC++;
    label = (struct Label *)malloc(sizeof(struct Label));
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    label->type = STRING_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->address = DC; /*The address will be set later when the string is linked to the code.*/

    newNode = (struct StringLabel *)malloc(sizeof(struct StringLabel));
    if (newNode == NULL)
    {
        /*Memory allocation failed*/
        free(label); /*Clean up the previously allocated memory*/
        return false;
    }

    newNode->label = label;
    newNode->string = my_strdup(string);
    newNode->next = stringLabelList;
    stringLabelList = newNode;

    DC += strlen(string); /*+1 FOR /0*/

    return true;
}

/**
 * The addNewLabel function is used to add a new label to the normal command label list.
 * It accepts the name of the label as an argument. This function creates a new Label
 * instance, sets the label type to NormalCommand, sets the label name and address (which is the current IC),
 * and appends it to the normalCommandLabelList. It returns true if the operation is successful
 * and false otherwise (e.g., if memory allocation fails).
 */
bool addNewLabel(char *labelName)
{
    struct Label *label = NULL;
    struct LabelNode *newNode = NULL;

    label = (struct Label *)malloc(sizeof(struct Label));
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    /*Set appropriate label type and name as per your requirement*/
    label->type = NORMAL_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->address = IC; /* due to page 30*/

    newNode = (struct LabelNode *)malloc(sizeof(struct LabelNode));
    if (newNode == NULL)
    {
        /*Memory allocation failed*/
        free(label); /*Clean up the previously allocated memory*/
        return false;
    }

    newNode->label = label;
    newNode->next = normalCommandLabelList;
    normalCommandLabelList = newNode;

    return true;
}

/**
 * The increaseIC function is used to increment the IC (Instruction Counter) by a given value.
 * This function is typically used after a machine instruction or directive has been processed.
 */
void increaseIC(int value)
{
    IC += value;
}

bool isLabelExist(char *label)
{
    return searchExternLabel(label) != 0 || searchEntry(label) != 0 || searchLabel(label) != 0 || searchDataLabel(label) != 0 || searchStringLabel(label) != 0;
}

/**
 * The searchExternLabel function searches for an external label in the label list by its name.
 * It traverses the externalLabelList and compares each label's name with the given name.
 * If it finds a match, it returns the address of the label. If it doesn't find a match, it returns -1.
 */
int searchExternLabel(char *externName)
{
    struct LabelNode *current;
    current = externalLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, externName) == 0)
        {
            return current->label->address;
        }
        current = current->next;
    }
    return -1; /*Label was not found*/
}

/**
 * The searchEntry function searches for an entry label in the label list by its name.
 * It traverses the entryLabelList and compares each label's name with the given name.
 * If it finds a match, it returns the address of the label. If it doesn't find a match, it returns -1.
 */
int searchEntry(char *entryName)
{
    struct LabelNode *current;
    current = entryLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, entryName) == 0)
        {
            return current->label->address;
        }
        current = current->next;
    }
    return -1; /*Label was not found*/
}

int searchLabel(char *labelName)
{
    struct LabelNode *current;
    current = entryLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, labelName) == 0)
        {
            return current->label->address;
        }
        current = current->next;
    }
    return -1; /*Label was not found*/
}

/**
 * The searchDataLabel function searches for a data label in the data label list by its name.
 * It traverses the dataLabelList and compares each label's name with the given name.
 * If it finds a match, it returns the address of the label (adjusted by the current IC).
 * If it doesn't find a match, it returns -1.
 */
int searchDataLabel(char *labelName)
{
    struct DataLabel *current;
    current = dataLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, labelName) == 0)
        {
            return current->label->address + IC;
        }
        current = current->next;
    }
    return -1; /*Label was not found*/
}

/**
 * The searchStringLabel function searches for a string label in the string label list by its name.
 * It traverses the stringLabelList and compares each label's name and string with the given name and string.
 * If it finds a match, it returns the address of the label (adjusted by the current IC).
 * If it doesn't find a match, it returns -1.
 */
int searchStringLabel(char *labelName)
{
    struct StringLabel *current;
    current = stringLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, labelName) == 0)
        {
            return current->label->address + IC;
        }
        current = current->next;
    }
    return -1; /*Label was not found*/
}