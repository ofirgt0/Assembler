#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "errorsHandler.h"
#include "dataService.h"
#include "encoder.h"
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
void addNewLine(char *fileName, int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2)
{
    AddressingMethod dstAddressing;
    AddressingMethod srcAddressing;
    int address = 0;
    bool commandValidation;

    dstAddressing = register2 != -1 ? RegisterDirect : (label2 != NULL ? Direct : (immidiate2 != 0.5 ? Immediate : None)); /*0.5 means null for int*/
    srcAddressing = register1 != -1 ? RegisterDirect : (label1 != NULL ? Direct : (immidiate1 != 0.5 ? Immediate : None)); /*0.5 means null for int*/

    if (srcAddressing == None)
    {
        srcAddressing = dstAddressing;
    }

    printf("\nsrcAddressing = %d, dstAddressing = %d\n\n\n", srcAddressing, dstAddressing);

    printf("label1: %s , register1: %d , immidiate1: %f", label1, register1, immidiate1);
    printf("\n\n");

    IC++; /*for the base command*/
    encodInstructionCode(fileName, ARE_CODE_A, srcAddressing, opcode, dstAddressing);
    printf("IC is %d\n", IC);

    /*if dstAddressing is 0 do it means that its command with 1 opperand,*/
    /*then we need to consider the srcAddressing as dstAddressing*/
    commandValidation = (dstAddressing == None) ? validateOpcodeMatchAddressingMethod(opcode, dstAddressing, srcAddressing) : validateOpcodeMatchAddressingMethod(opcode, srcAddressing, dstAddressing);

    if (!commandValidation)
    {
        /*TODO: handle error*/
        printf("ERROR: invalid command\n");
    }

    if (opcode > 13) /*only 1 line for stop and rts*/
        return;

    if (register1 != -1)
    {
        IC++;
        printf("IC is %d\n", IC);
        if (register2 != -1)
        {
            encodeRegister(fileName, register1, register2);
            return;
        }
        else
        {
            encodeRegister(fileName, register1, 0);
        }
    }
    else if (label1 != NULL)
    {
        IC++;
        printf("IC is %d\n", IC);
        printf("we r in the case that label1 != NULL\n");

        address = searchExternLabel(label1); /*TODO: we need to check if the label is external or entry to set the ARE code as well*/
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_E, address);
            return;
        }

        address = searchLabel(label1);
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_R, address);
            return;
        }

        address = searchEntry(label1);
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_R, address);
            return;
        }

        /* TODO: handle error label not found*/
    }
    else if (immidiate1 != 0.5)
    {
        IC++;
        printf("IC is %d\n", IC);
        encodImmidiate(fileName, (int)immidiate1);
    }

    if (register2 != -1)
    {
        IC++;
        printf("IC is %d\n", IC);
        encodeRegister(ARE_CODE_A, 0, register2);
        return;
    }
    else if (label2 != NULL)
    {
        IC++;
        printf("IC is %d\n", IC);
        printf("label2 != NULL  %s\n", label2);
        address = searchExternLabel(label2); /*TODO: we need to check if the label is external or entry to set the ARE code as well*/
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_E, address);
            return;
        }

        address = searchLabel(label2);
        printf("searchLabel return address %d\n", address);
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_R, address);
            return;
        }

        address = searchEntry(label2);
        printf("searchEntry return address %d\n", address);
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_R, address);
            return;
        }
    }
    else if (immidiate2 != 0.5)
    {
        IC++;
        printf("IC is %d\n", IC);
        encodImmidiate(fileName, (int)immidiate2);
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

    /* Allocating an extra byte because of the "Ensure null termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    label->type = EXTERN_LABEL_TYPE;
    strncpy(label->name, externName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0';

    label->address = 1; /*Assuming -1 represents an external label address.*/

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

void printLabelList(struct LabelNode *head)
{
    struct LabelNode *current = head;

    printf("Label List:\n");
    while (current != NULL)
    {
        printf("Type: %c, Name: %s, Address: %d\n", current->label->type, current->label->name, current->label->address);
        current = current->next;
    }
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

    /* Allocating an extra byte because of the "Ensure null termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    label->type = ENTRY_LABEL_TYPE;
    strncpy(label->name, entryName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure null termination (because the use of 'strncpy') */

    label->address = 10; /*The address will be set later when the entry is resolved.*/

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
bool addData(int data[], char *labelName, int length)
{
    printf("addData %s\n", labelName);
    struct Label *label = NULL;
    struct DataLabel *newNode = NULL;

    /* Allocating an extra byte because of the "Ensure null termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    label->type = DATA_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure null termination */

    printf("DC before is %d\n", DC);
    DC += length;
    printf("DC after is %d\n", DC);
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

    return true;
}

/**
 * Duplicates a string by creating a new copy in the heap.
 * This function allocates memory for the new string, copies
 * the original string into the new memory, and returns a pointer to it.
 */
char *my_strdup(const char *s)
{
    if (s == NULL)
    {
        printf("Error: Null pointer passed to my_strdup.\n");
        return NULL;
    }

    char *new = (char *)malloc(strlen(s) + 1); /*+1 for the null-terminator*/
    if (new == NULL)
    {
        printf("Error: Memory allocation failed in my_strdup.\n");
        return NULL;
    }

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

    /* Allocating an extra byte because of the "Ensure null termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    label->type = STRING_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure null termination  */

    printf("DC BEFORE is %d\n", DC);

    /* Find the opening and closing quotation marks */
    char *startQuote = strchr(string, '\"');
    char *endQuote = strrchr(string, '\"');

    /* Check that both an opening and a closing quotation mark were found */
    if (startQuote == NULL || endQuote == NULL || startQuote >= endQuote)
    {
        printf("Error: String not properly enclosed in quotation marks.\n");
        return;
    }

    /* Check that there are no unexpected characters after the closing quotation mark */
    if (*(endQuote + 1) != '\0' && *(endQuote + 1) != '\n' && *(endQuote + 1) != ' ')
    {
        printf("Error: Unexpected characters after closing quotation mark.\n");
        return;
    }

    /* Calculate the actual length of the string between the quotation marks */
    int stringLength = endQuote - startQuote;

    DC += stringLength; /*+1 FOR /0*/
    label->address = DC;    /*The address will be set later when the string is linked to the code.*/
    printf("DC AFTER is %d\n", DC);

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

    /* Allocating an extra byte because of the "Ensure null termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return false;
    }

    /*Set appropriate label type and name as per your requirement*/
    label->type = NORMAL_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure null termination */
    label->address = IC + DC;                      /* due to page 30*/

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

    if (searchEntry(labelName) != -1)
    {
        printf("updateEntryLabelAddress to address %d \n", label->address);
        updateEntryLabelAddress(labelName, label->address);
    }
    printf("addNewLabel %s with address %d done\n", labelName, label->address);
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

void initIC()
{
    IC = 100;
}

bool isLabelExist(char *label)
{
    return searchExternLabel(label) != -1 || searchEntry(label) != -1 || searchLabel(label) != -1 || searchDataLabel(label) != -1 || searchStringLabel(label) != -1;
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
            printf("%s found in externalLabelList", externName);
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
    size_t i;
    current = entryLabelList;
    while (current != NULL)
    {

        size_t nameLabelLength = strlen(current->label->name);
        if (current->label->name[nameLabelLength - 1] == '\n')
        {
            current->label->name[nameLabelLength - 1] = '\0';
        }
        printf("current->label->name: %s, entryName: %s\n",
               current->label->name, entryName);

        if (strncmp(current->label->name, entryName, strlen(current->label->name)) == 0)
        {
            printf("%s found in entryLabelList\n", entryName);
            return current->label->address;
        }

        current = current->next;
    }

    return -1; /*Label was not found*/
}

updateEntryLabelAddress(char *entryName, int address)
{
    struct LabelNode *current;
    current = entryLabelList;
    while (current != NULL)
    {
        size_t newlinePos = strcspn(entryName, "\n");
        if (newlinePos != strlen(entryName))
        {
            entryName[newlinePos] = '\0';
        }

        if (strcasecmp(current->label->name, entryName) == 0)
        {
            printf("innnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
            current->label->address = address;
            return;
        }
        current = current->next;
    }
}

int searchLabel(char *labelName)
{
    struct LabelNode *current;
    current = normalCommandLabelList;
    while (current != NULL)
    {
        size_t newlinePos = strcspn(labelName, "\n");
        if (newlinePos != strlen(labelName))
        {
            labelName[newlinePos] = '\0';
        }

        if (strcasecmp(current->label->name, labelName) == 0)
        {
            return current->label->address;
        }
        current = current->next;
    }
    /*    printf("searchLabel return -1 to %s", labelName);
        printLabelList(normalCommandLabelList);*/
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
            return current->label->address;
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
            return current->label->address;
        }
        current = current->next;
    }
    return -1; /*Label was not found*/
}

void printLabels(const char *filename)
{
    struct StringLabel *current_StringLabel;
    current_StringLabel = stringLabelList;
    while (current_StringLabel != NULL)
    {
        writeLabelToFile(concatenateStrings(filename, "_string.txt"), current_StringLabel->label->name, current_StringLabel->label->address);
        current_StringLabel = current_StringLabel->next;
    }

    struct LabelNode *current_LabelNode;
    current_LabelNode = externalLabelList;
    while (current_LabelNode != NULL)
    {
        writeLabelToFile(concatenateStrings(filename, ".ext"), current_LabelNode->label->name, current_LabelNode->label->address);
        current_LabelNode = current_LabelNode->next;
    }

    current_LabelNode = entryLabelList;
    while (current_LabelNode != NULL)
    {
        writeLabelToFile(concatenateStrings(filename, ".ent"), current_LabelNode->label->name, current_LabelNode->label->address);
        current_LabelNode = current_LabelNode->next;
    }
}

/*
static struct LabelNode *externalLabelList = NULL;
static struct LabelNode *entryLabelList = NULL;
static struct LabelNode *normalCommandLabelList = NULL;
static struct DataLabel *dataLabelList = NULL;
static struct StringLabel *stringLabelList = NULL;*/
