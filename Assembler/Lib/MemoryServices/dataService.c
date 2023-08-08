#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "errorsHandler.h"
#include "dataService.h"
#include "encoder.h"
#include "writeToFile.h"

/* Initialize the label lists. */
static struct GenericLabelNode *externalLabelList = NULL;
static struct GenericLabelNode *entryLabelList = NULL;
static struct GenericLabelNode *normalCommandLabelList = NULL;
static struct GenericLabelNode *dataLabelList = NULL;
static struct GenericLabelNode *stringLabelList = NULL;

typedef enum
{
    None = 0,
    Immediate = 1,
    Direct = 3,
    RegisterDirect = 5
} AddressingMethod;

int searchLabel(char *labelName);
void updateEntryLabelAddress(char *entryName, int address);
void encodValue(char *fileName, char character);
char *charToString(char c);
bool handleLastLines(char *fileName, char *label, int register, double immidiate, bool isFirstOperand);

/* Initialize the global counters. */
static int IC = 100; /* Instruction counter. */
static int DC = 0;   /* Data counter. */

void addNewLine(char *fileName, int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2)
{
    AddressingMethod dstAddressing, srcAddressing;
    int address = 0;
    bool commandValidation;

    dstAddressing = register2 != -1 ? RegisterDirect : (label2 != NULL ? Direct : (immidiate2 != 0.5 ? Immediate : None)); /*0.5 means null for int*/
    srcAddressing = register1 != -1 ? RegisterDirect : (label1 != NULL ? Direct : (immidiate1 != 0.5 ? Immediate : None)); /*0.5 means null for int*/

    if (dstAddressing == None)
    {
        dstAddressing = srcAddressing;
        srcAddressing = None;
    }

    IC++; /*for the base command*/
    encodInstructionCode(fileName, ARE_CODE_A, srcAddressing, opcode, dstAddressing);
    printf("IC is %d\n", IC);

    /*if dstAddressing is 0 do it means that its command with 1 opperand,*/
    /*then we need to consider the srcAddressing as dstAddressing*/
    commandValidation = (dstAddressing == None) ? validateOpcodeMatchAddressingMethod(opcode, dstAddressing, srcAddressing) : validateOpcodeMatchAddressingMethod(opcode, srcAddressing, dstAddressing);

    if (!commandValidation)
    {
        INVALID_COMMAND_ERROR(fileName, __LINE__);
    }

    if (opcode > 13) /*only 1 line for stop and rts*/
        return;

    if (handleLastLines(fileName, label1, register1, immidiate1, true))
        handleLastLines(fileName, label2, register2, immidiate2, false);
}

bool handleLastLines(char *fileName, char *label, int reg, double immidiate, bool isFirstOperand)
{
    int address = 0;
    if (label != NULL)
    {
        IC++;
        address = searchExternLabel(label);
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_E, address);
            return false;
        }

        address = searchLabel(label);
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_R, address);
            return false;
        }

        address = searchEntry(label);
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_R, address);
            return false;
        }

        LABEL_NOT_FOUND_ERROR(fileName, __LINE__, label);
    }
    else if (immidiate != 0.5)
    {
        IC++;
        encodImmidiate(fileName, (int)immidiate);
    }
    else if (reg != -1)
    {
        IC++;
        isFirstOperand ? encodeRegister(fileName, reg, 0) : encodeRegister(fileName, 0, reg);
    }
    return true;
}

void printAm(char *fileName, char *command)
{
    printf("printAm %s", command);
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

char *my_strdup(const char *s)
{
    char *new;

    if (s == NULL)
    {
        printf("Error: Null pointer passed to my_strdup.\n");
        return NULL;
    }

    new = (char *)malloc(strlen(s) + 1); /*+1 for the null-terminator*/
    if (new == NULL)
    {
        MEMORY_ALLOCATION_FAILED(__FILE__, __LINE__, NULL);
    }

    strcpy(new, s);
    return new;
}

void increaseIC(int value)
{
    IC += value;
}

char *intToStringWithSpace(int ic, int dc)
{
    /* Calculate length of numbers in string format */
    size_t ic_length = (ic == 0) ? 1 : (int)log10(ic) + 1;
    size_t dc_length = (dc == 0) ? 1 : (int)log10(dc) + 1;

    /* Calculate total length (+2 for space and null terminator) */
    int totalLength = ic_length + dc_length + 2;

    char *result = (char *)malloc(totalLength);
    if (result == NULL)
    {
        return NULL;
    }

    sprintf(result, "%d %d", ic, dc);

    return result;
}

void prepareSecondRun(char *fileName)
{
    appendStringToFile(concatenateStrings(fileName, ".ob"), intToStringWithSpace(IC - 100, DC));
    IC = 100;
}

bool isLabelExist(char *label, int lineNumber, char *fileName)
{
    if (searchExternLabel(label) != -1)
    {
        writeLabelToFile(concatenateStrings(fileName, ".ext"), label, IC + 1);
        return true;
    }

    return searchEntry(label) != -1 || searchLabel(label) != -1 ||
           searchDataLabel(label) != NULL || searchStringLabel(label) != NULL;
}

void updateEntryLabelAddress(char *entryName, int address)
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
            current->label->address = address;
            return;
        }
        current = current->next;
    }
}

char *charToString(char c)
{
    static char str[2];
    str[0] = c;
    str[1] = '\0';
    return str;
}

/*Region: senders*/

void sendStringValue(char *fileName, char *labelName)
{
    struct StringLabel *current;
    int i;
    printf("sendStringValue: %s %s\n", fileName, labelName);
    current = searchStringLabel(labelName);
    printf("sendStringValue: %s\n", current->label->name);

    for (i = 1; current->string[i] != '\0'; i++)
    {
        if (current->string[i] != 34)
            encodValue(fileName, current->string[i]);
    }
    encodValue(fileName, current->string[i]);
}

void sendDataValue(char *fileName, char *labelName)
{
    struct DataLabel *current;
    int i;
    current = searchDataLabel(labelName);

    for (i = 0; i < current->size; i++)
    {
        encodValue(fileName, current->data[i]);
    }
}

void sendLabelsToFile(const char *filename)
{
    struct LabelNode *current_LabelNode;
    current_LabelNode = entryLabelList;
    while (current_LabelNode != NULL)
    {
        writeLabelToFile(concatenateStrings(filename, ".ent"), current_LabelNode->label->name, current_LabelNode->label->address);
        current_LabelNode = current_LabelNode->next;
    }
}

int getStringLenUntilNewLine(char *string)
{
    char *startQuote, *endQuote;

    /* Find the opening and closing quotation marks */
    startQuote = strchr(string, '\"');
    endQuote = strrchr(string, '\"');

    /* Check that both an opening and a closing quotation mark were found */
    if (startQuote == NULL || endQuote == NULL || startQuote >= endQuote)
    {
        printf("Error: String not properly enclosed in quotation marks.\n");
        return false;
    }

    /* Check that there are no unexpected characters after the closing quotation mark */
    if (*(endQuote + 1) != '\0' && *(endQuote + 1) != '\n' && *(endQuote + 1) != ' ')
    {
        printf("Error: Unexpected characters after closing quotation mark.\n");
        return false;
    }

    /* Calculate the actual length of the string between the quotation marks */
    return endQuote - startQuote;
}

/*Region: generic linkList functions*/

struct GenericLabelNode *getListByType(LabelType LabelType)
{
    switch (LabelType)
    {
    case (NormalCommand):
        return normalCommandLabelList;
        break;
    case (External):
        return externalLabelList;
        break;
    case (Entry):
        return entryLabelList;
        break;
    case (Data):
        return dataLabelList;
        break;
    case (String):
        return stringLabelList;
        break;
    default:
        break;
    }
}

bool addNewGenericLabel(char *labelName, LabelType type, int *data, int size, char *string)
{
    struct Label *label = NULL;
    struct GenericLabelNode *newNode = NULL;
    struct GenericLabelNode *head = getListByType(type);
    int address;

    /* Allocating an extra byte because of the "Ensure null termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        /*Memory allocation failed*/
        return genericLabelNode;
    }

    /*Set appropriate label type and name as per your requirement*/
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure null termination */
    label->address = IC + DC;                      /* due to page 30*/

    newNode = (struct GenericLabelNode *)malloc(sizeof(struct GenericLabelNode));
    if (newNode == NULL)
    {
        /*Memory allocation failed*/
        free(label); /*Clean up the previously allocated memory*/
        return genericLabelNode;
    }

    newNode->label = label;
    newNode->next = NULL;

    if (type == Data)
    {
        newNode->data = data;
        newNode->size = size;
        DC += size;
    }
    else if (type == String)
    {
        newNode->string = string;
        DC += getStringLenUntilNewLine(string);
    }
    else if (type == NormalCommand)
    {
        if (searchEntry(labelName) != -1)
        {
            printf("updateEntryLabelAddress to address %d \n", label->address);
            updateEntryLabelAddress(labelName, label->address);
        }
    }
    else if (type == External)
    {
        label->address = 1;
    }
    else if (type == Entry)
    {
        address = searchLabel(entryName);
        label->address = address != -1 ? address : 9;
    }

    newNode->next = head;
    head = newNode;

    return true;
}

struct GenericLabelNode *searchGenericLabel(char *labelName, LabelType type)
{
    struct GenericLabelNode *current;
    current = getListByType(type);
    size_t nameLabelLength = strlen(current->label->name);
    if (current->label->name[nameLabelLength - 1] == '\n')
    {
        current->label->name[nameLabelLength - 1] = '\0';
    }
    while (current != NULL)
    {
        if (strcmp(current->label->name, labelName) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL; /*Label was not found*/
}

/*Region: custom generic functions*/

struct GenericLabelNode *searchDataLabel(char *labelName)
{
    return searchGenericLabel(labelName, Data); /*Label was not found*/
}

struct GenericLabelNode *searchStringLabel(char *labelName)
{
    return searchGenericLabel(labelName, String);
}

int searchLabel(char *labelName)
{
    return searchGenericLabel(labelName, NormalCommand)->label->address;
}

int searchExternLabel(char *externName)
{
    return searchGenericLabel(externName, External)->label->address;
}

int searchEntry(char *entryName)
{
    return searchGenericLabel(entryName, Entry)->label->address;
}

bool addData(int data[], char *labelName, int length)
{
    return addNewGenericLabel(labelName, Data, data, length, NULL);
}

bool addString(char *string, char *labelName)
{
    return addNewGenericLabel(labelName, String, NULL, 0, string);
}

bool addNewLabel(char *labelName)
{
    return addNewGenericLabel(labelName, NormalCommand, NULL, 0, NULL);
}

bool addNewEntry(char *entryName)
{
    return addNewGenericLabel(entryName, Entry, NULL, 0, NULL);
}

bool addNewExtern(char *externName)
{
    return addNewGenericLabel(externName, External, NULL, 0, NULL);
}
