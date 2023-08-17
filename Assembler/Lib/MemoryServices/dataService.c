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
#include "helpfulFunctions.h"

/* Initialize the label lists. */
static struct LabelNode *externalLabelList = NULL;
static struct LabelNode *entryLabelList = NULL;
static struct LabelNode *normalCommandLabelList = NULL;
static struct DataLabel *dataLabelList = NULL;
static struct StringLabel *stringLabelList = NULL;

/**
 * searchLabel:
 * - Searches for a label in the normal command label list.
 * @param labelName: Name of the label to search for.
 * @return int: Address of the label if found, -1 if not.
 */
int searchLabel(char *labelName);

/**
 * updateEntryLabelAddress:
 * - Updates the address of a specified entry label.
 * @param entryName: Name of the entry label.
 * @param address: New address to set for the entry label.
 */
void updateEntryLabelAddress(char *entryName, int address);

/**
 * encodValue:
 * - Encodes a character to its binary representation and writes to a file.
 * @param fileName: Name of the file to write to.
 * @param character: Character to encode.
 */
void encodValue(char *fileName, char character);

/**
 * charToString:
 * - Converts a character to its string representation.
 * @param c: Character to convert.
 * @return char*: String representation of the character.
 */
char *charToString(char c);

/**
 * getLabelAddressWithoutExtern:
 * - Gets the address of a label excluding external labels.
 * @param label: Name of the label to search for.
 * @return int: Address of the label if found, -1 if not.
 */
int getLabelAddressWithoutExtern(char *label);

/* Initialize the global counters. */
static int IC = 100;              /* Instruction counter. */
static int TotalInstructions = 0; /* Total Instruction counter. */
static int DC = 0;                /* Data counter. */

/**
 * The addNewLine function is responsible for adding new lines of machine code
 * to the program image. It accepts the opcode, registers, labels, and immediates
 * as arguments and adds them to the binary code. This function also handles encoding
 * of the operands based on the type of addressing and increments the IC (Instruction Counter).
 * Note: -1 is used to indicate an empty register slot.
 */
void addNewLine(char *fileName, int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2, int lineNumber)
{
    AddressingMethod dstAddressing;
    AddressingMethod srcAddressing;
    int address = 0;
    bool commandValidation;

    dstAddressing = register2 != -1 ? RegisterDirect : (label2 != NULL ? Direct : (immidiate2 != 0.5 ? Immediate : None)); /*0.5 means NULL for int*/
    srcAddressing = register1 != -1 ? RegisterDirect : (label1 != NULL ? Direct : (immidiate1 != 0.5 ? Immediate : None)); /*0.5 means NULL for int*/

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
        INVALID_OPTION_FOR_COMMAND(fileName, lineNumber);
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

        address = searchExternLabel(label1); /*TODO: we need to check if the label is external or entry to set the ARE code as well*/
        if (address != -1)
        {
            encodLabelOperand(fileName, ARE_CODE_E, address);
            return;
        }

        address = getLabelAddressWithoutExtern(label1);
        if (address != -1)
        {
            printf("address %d\n", address);
            encodLabelOperand(fileName, ARE_CODE_R, address);
            return;
        }

        LABEL_WAS_NOT_FOUND_ERROR(fileName, lineNumber, label1); /* TODO: need to handle -1 issue and to provide the real lineNumber */
    }
    else if (immidiate1 != 0.5)
    {
        IC++;
        encodImmidiate(fileName, (int)immidiate1);
    }

    if (register2 != -1)
    {
        IC++;
        encodeRegister(charToString(ARE_CODE_A), 0, register2);
        return;
    }
    else if (label2 != NULL)
    {
        IC++;

        address = searchExternLabel(label2);
        if (address != -1)
        {
            printf("address %d\n", address);
            encodLabelOperand(fileName, ARE_CODE_E, address);
            return;
        }

        address = getLabelAddressWithoutExtern(label2);
        if (address != -1)
        {
            printf("address %d\n", address);
            encodLabelOperand(fileName, ARE_CODE_R, address);
            return;
        }

        LABEL_WAS_NOT_FOUND_ERROR(fileName, lineNumber, label2); /*TODO: need to handle -1 issue, and to set the real lineNumber */
    }
    else if (immidiate2 != 0.5)
    {
        IC++;
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

    if (isLabelExistWithoutEntries(externName))
    {
        return false;
    }

    /* Allocating an extra byte because of the "Ensure NULL termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return false;
    }

    label->type = EXTERN_LABEL_TYPE;
    strncpy(label->name, externName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0';

    label->address = 1; /*Assuming 1 represents an external label address.*/

    newNode = (struct LabelNode *)malloc(sizeof(struct LabelNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        free(label); /*Clean up the previously allocated memory*/
        return false;
    }

    newNode->label = label;
    newNode->next = externalLabelList;
    externalLabelList = newNode;

    return true;
}

/**
 * printLabelList:
 * - Prints all labels in a given list for debugging purposes.
 * @param head: Head of the label list.
 */
void printLabelList(struct LabelNode *head)
{
    struct LabelNode *current = head;

    printf("Label List:\n");
    while (current != NULL)
    {
        printf("Type: %s, Name: %s, Address: %d\n", current->label->type, current->label->name, current->label->address);

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
    int address;

    /* Allocating an extra byte because of the "Ensure NULL termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return false;
    }

    label->type = ENTRY_LABEL_TYPE;
    strncpy(label->name, entryName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure NULL termination (because the use of 'strncpy') */
    address = searchLabel(entryName);
    label->address = address != -1 ? address : 0; /*The address will be set later when the entry is resolved.*/

    newNode = (struct LabelNode *)malloc(sizeof(struct LabelNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
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
    struct Label *label = NULL;
    struct DataLabel *newNode = NULL;

    if (labelName != NULL && isLabelExistWithoutEntries(labelName))
    {
        return false;
    }

    /* Allocating an extra byte because of the "Ensure NULL termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return false;
    }
    label->type = DATA_LABEL_TYPE;
    if (labelName != NULL)
    {
        strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
        label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure NULL termination */
    }

    label->address = DC; /*The address will be set later when data is linked to the code.*/
    DC += length;

    newNode = (struct DataLabel *)malloc(sizeof(struct DataLabel));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        free(label); /*Clean up the previously allocated memory*/
        return false;
    }

    newNode->label = label;
    newNode->data = data;
    newNode->next = dataLabelList;
    newNode->size = length;
    dataLabelList = newNode;
    printf("-------------------------------NEW DATA LABEL: %s ADDRESS: %d\n", label->name, label->address);
    return true;
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
    int stringLength;
    char *startQuote = NULL;
    char *endQuote = NULL;
    char *ptr = string;

    printf("string is %d\n", string[strlen(string) - 1]);
    if (labelName != NULL && isLabelExistWithoutEntries(labelName))
    {
        return false;
    }

    printf("labelName: %s\n", labelName);

    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    /* Allocating an extra byte because of the "Ensure NULL termination" in the label name, according to the  "strncpy" method */
    if (label == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return false;
    }

    if (labelName != NULL)
    {
        printf("in the if ------------ \n");

        label->type = STRING_LABEL_TYPE;
        strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
        label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure NULL termination  */
    }

    while (*ptr)
    {
        if (*ptr == '\"')
        {
            if (!startQuote)
                startQuote = ptr;
            else
            {
                endQuote = ptr;
                break;
            }
        }
        ptr++;
    }

    if (!startQuote || !endQuote || startQuote >= endQuote)
    {
        printf("Error: String not properly enclosed in quotation marks.\n");
        free(label);
        return false;
    }

    startQuote++;
    *endQuote = '\0';

    stringLength = strlen(startQuote) + 1;

    label->address = DC; /*The address will be set later when the string is linked to the code.*/
    DC += stringLength;
    printf("updated DC is %d\n", DC);

    newNode = (struct StringLabel *)malloc(sizeof(struct StringLabel));
    if (newNode == NULL)
    {
        free(label);
        return false;
    }

    newNode->label = label;
    newNode->string = my_strdup(string);
    /*TODO: check if we can just do: newNode->string = string;*/
    newNode->next = stringLabelList;
    stringLabelList = newNode;
    printf("newNode->string  %s\n", newNode->string);
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

    if (labelName != NULL && isLabelExistWithoutEntries(labelName))
    {
        return false;
    }

    /* Allocating an extra byte because of the "Ensure NULL termination" in the label name, according to the  "strncpy" method */
    label = (struct Label *)malloc(sizeof(struct Label) + 1);
    if (label == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return false;
    }

    /*Set appropriate label type and name as per your requirement*/
    label->type = NORMAL_LABEL_TYPE;
    strncpy(label->name, labelName, MAX_LABEL_NAME_LENGTH);
    label->name[MAX_LABEL_NAME_LENGTH - 1] = '\0'; /* ensure NULL termination */
    label->address = IC + DC;                      /* due to page 30*/

    newNode = (struct LabelNode *)malloc(sizeof(struct LabelNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
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
 * increaseIC:
 * - Increments the global Instruction Counter (IC) by a specified value.
 * @param value: Value to increment the IC by.
 */
void increaseIC(int value)
{
    if (value >= 0)
    {
        IC += value;
        printf("current IC is %d\n", IC);
        printf("current DC is %d\n", DC);
    }
    else
    {
        printf("Error: Invalid value passed to increaseIC. IC can only be incremented by a non-negative value.\n");
    }
}

/**
 * Converts two integer values (ic and dc) to a single string, separated by a space.
 * @param ic: The first integer.
 * @param dc: The second integer.
 * @return A string in the format "ic dc". The caller is responsible for freeing the memory.
 */
char *intToStringWithSpace(int ic, int dc)
{
    /* Calculate length of numbers in string format */
    size_t ic_length = (ic == 0) ? 1 : (int)log10(ic) + 1;
    size_t dc_length = (dc == 0) ? 1 : (int)log10(dc) + 1;

    /* Calculate total length (+2 for space and NULL terminator) */
    int totalLength = ic_length + dc_length + 2;

    char *result = (char *)malloc(totalLength);
    if (result == NULL)
    {
        return NULL;
    }

    sprintf(result, "%d %d", ic, dc);

    return result;
}

/**
 * Prepares for the second run by setting the IC counter and appending a string to a file.
 * @param fileName: Name of the file to prepare.
 */
void prepareSecondRun(char *fileName)
{
    /* If there are errors, we won't proceed. */
    if (getErrorsCounter() > 0)
        return;

    TotalInstructions = IC;
    appendStringToFile(concatenateStrings(fileName, ".ob"), intToStringWithSpace(IC - 100, DC));
    IC = 100;
}

/**
 * Checks if a given label exists in extern labels, entries, or other label lists.
 * Optionally writes the label to a file if it's an extern label.
 * @param label: The label to search for.
 * @param lineNumber: The line number in the source file.
 * @param fileName: The name of the source file.
 * @param writeToFile: Flag to determine if the label should be written to a file.
 * @param linesNumberForCommand: Number of lines for the command.
 * @return true if the label exists, false otherwise.
 */
bool isLabelExist(char *label, int lineNumber, char *fileName, bool writeToFile, int linesNumberForCommand)
{
    printf("isLabelExist: label - %s lineNumber - %d fileName %s writeToFile linesNumberForCommand %d \n", label, lineNumber, fileName, linesNumberForCommand);

    if (searchExternLabel(label) != -1)
    {
        if (writeToFile && getErrorsCounter() == 0)
            writeLabelToFile(concatenateStrings(fileName, ".ext"), label, IC + linesNumberForCommand);
        return true;
    }

    return searchEntry(label) != -1 || searchLabel(label) != -1 ||
           searchDataLabel(label) != NULL || searchStringLabel(label) != NULL;
}

/**
 * Checks if a given label exists in certain label lists, excluding entries.
 * @param label: The label to search for.
 * @return true if the label exists, false otherwise.
 */
bool isLabelExistWithoutEntries(char *label)
{
    return searchExternLabel(label) != -1 || searchLabel(label) != -1 ||
           searchDataLabel(label) != NULL || searchStringLabel(label) != NULL;
}

/**
 * Retrieves the address of a given label, excluding extern labels.
 * @param label: The label whose address is to be retrieved.
 * @return Address of the label or -1 if not found.
 */
int getLabelAddressWithoutExtern(char *label)
{
    struct DataLabel *dataLabel = searchDataLabel(label);
    struct StringLabel *stringLabel = searchStringLabel(label);

    int address = searchLabel(label);
    if (address != -1)
        return address;
    printf("not found in labels\n");

    if (dataLabel != NULL)
        return dataLabel->label->address;
    printf("not found in dataLabels\n");

    if (stringLabel != NULL)
        return stringLabel->label->address;
    printf("not found in stringLabel\n");

    printf("getLabelAddressWithoutExtern\n");
    address = searchEntry(label);
    if (address != -1)
        return address;
    printf("not found in entries\n");
    return -1;
}

/**
 * Validates if a given label can be considered as a new entry.
 * Ensures the label does not already exist in the entries and externs lists.
 * @param label: The label to validate.
 * @return true if the label is a valid new entry, false otherwise.
 */
bool isValidNewEntry(char *label)
{
    return searchEntry(label) == -1 && searchExternLabel(label) == -1;
}

/**
 * The searchExternLabel function searches for an external label in the label list by its name.
 * It traverses the externalLabelList and compares each label's name with the given name.
 * If it finds a match, it returns the address of the label. If it doesn't find a match, it returns -1.
 */
int searchExternLabel(char *externName)
{
    struct LabelNode *current;
    size_t nameLabelLength;
    current = externalLabelList;
    while (current != NULL)
    {
        nameLabelLength = strlen(current->label->name);
        if (current->label->name[nameLabelLength - 1] == '\n')
        {
            current->label->name[nameLabelLength - 1] = '\0';
        }
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
        size_t nameLabelLength = strlen(current->label->name);
        if (current->label->name[nameLabelLength - 1] == '\n')
        {
            current->label->name[nameLabelLength - 1] = '\0';
        }

        if (strncmp(current->label->name, entryName, strlen(current->label->name)) == 0)
        {
            return current->label->address;
        }

        current = current->next;
    }

    return -1; /*Label was not found*/
}

/**
 * Updates the address of an entry label.
 * @param entryName: Name of the entry label.
 * @param address: New address to set.
 */
void updateEntryLabelAddress(char *entryName, int address)
{
    struct LabelNode *current;
    current = entryLabelList;
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ updateEntryLabelAddress: %s %d\n", entryName, address);
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

/**
 * Searches for a regular label by its name.
 * @param labelName: Name of the label.
 * @return Address of the label or -1 if not found.
 */
int searchLabel(char *labelName)
{
    struct LabelNode *current;

    printf("searchLabel: %s\n", labelName);
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
    return -1; /*Label was not found*/
}

void updateAddress(struct Label *labelToUpdate)
{
    printf("TotalInstructions: %d\n", TotalInstructions);
    if (labelToUpdate->address < 100)
        labelToUpdate->address += TotalInstructions;
}

/**
 * The searchDataLabel function searches for a data label in the data label list by its name.
 * It traverses the dataLabelList and compares each label's name with the given name.
 * If it finds a match, it returns the address of the label (adjusted by the current IC).
 * If it doesn't find a match, it returns -1.
 */
struct DataLabel *searchDataLabel(char *labelName)
{
    struct DataLabel *current;
    current = dataLabelList;
    while (current != NULL)
    {
        if (strcmp(current->label->name, labelName) == 0)
        {
            updateAddress(current->label);
            return current;
        }
        current = current->next;
    }
    return NULL; /*Label was not found*/
}

/**
 * The searchStringLabel function searches for a string label in the string label list by its name.
 * It traverses the stringLabelList and compares each label's name and string with the given name and string.
 * If it finds a match, it returns the address of the label (adjusted by the current IC).
 * If it doesn't find a match, it returns -1.
 */
struct StringLabel *searchStringLabel(char *labelName)
{
    struct StringLabel *current;
    printf("searchStringLabel: %s\n", labelName);
    current = stringLabelList;
    while (current != NULL)
    {
        printf("current->label->name: %s\n", current->label->name);
        if (strcmp(current->label->name, labelName) == 0)
        {
            printf("current->label->address: %d\n", current->label->address);
            updateAddress(current->label);
            return current;
        }
        current = current->next;
    }
    return NULL; /*Label was not found*/
}

/**
 * Sends the string value of a label to be encoded.
 * @param fileName: Name of the file.
 * @param labelName: Name of the label.
 * @param string: The string to be encoded.
 */
void sendStringValue(char *fileName, char *labelName, char *string)
{
    int i;
    printf("sendStringValue: %s %s\n", fileName, labelName);

    string++;
    string[strlen(string) - 2] = '\0';

    for (i = 1; string[i] != '\0'; i++)
    {
        if (string[i] != 34)
            encodValue(fileName, string[i]);
    }
    encodValue(fileName, string[i]);
}

/**
 * Sends the data values of a label to be encoded.
 * @param fileName: Name of the file.
 * @param labelName: Name of the label.
 * @param data: Pointer to the array of data values.
 * @param size: Number of elements in the data array.
 */
void sendDataValue(char *fileName, char *labelName, int *data, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("Sending data[%d]: %d to encodValue\n", i, data[i]);
        encodValue(fileName, data[i]);
    }
}

/**
 * Prints the labels associated with a file.
 * @param filename: Name of the file.
 */
void printLabels(const char *filename)
{
    struct LabelNode *current_LabelNode;
    int addressFromDifferentList = 0;

    /* If there are errors, we won't proceed. */
    if (getErrorsCounter() > 0)
        return;

    current_LabelNode = entryLabelList;
    while (current_LabelNode != NULL)
    {
        updateAddress(current_LabelNode->label);

        addressFromDifferentList = searchDataLabel(current_LabelNode->label->name) != NULL ? searchDataLabel(current_LabelNode->label->name)->label->address : (searchStringLabel(current_LabelNode->label->name) != NULL ? searchStringLabel(current_LabelNode->label->name)->label->address : -1);

        if (addressFromDifferentList != -1)
            current_LabelNode->label->address = addressFromDifferentList;

        writeLabelToFile(concatenateStrings(filename, ".ent"), current_LabelNode->label->name, current_LabelNode->label->address);
        current_LabelNode = current_LabelNode->next;
    }
}

/* charToString function converts a single character to a string. */
/* charToString function converts a single character to a dynamically allocated string. */
char *charToString(char c)
{
    char *str = (char *)malloc(2 * sizeof(char)); /* Allocate memory for 2 characters (1 for the char, 1 for '\0') */
    if (!str)
    {
        return NULL;
    }
    str[0] = c;
    str[1] = '\0';

    /* Free the memory before returning */
    free(str);

    return NULL;
}

/*void initStaticVariable(){
    externalLabelList = NULL;
    entryLabelList = NULL;
    normalCommandLabelList = NULL;
    dataLabelList = NULL;
    stringLabelList = NULL;
    IC = 100;
    DC = 0;
    TotalInstructions = 0;
}*/
