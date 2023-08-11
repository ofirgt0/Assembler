#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "commandsIdentifier.h"
#include "macroService.h"
#include "dataService.h"
#include "filesReader.h"
#include "errorsHandler.h"
#include "writeToFile.h"
#include "helpfulFunctions.h"

/* An Array of our 16 commands names. */
char *commandsNames[COMMANDS_NUMBER] = {
    "mov", /* 0 */
    "cmp", /* 1 */
    "add", /* 2 */
    "sub", /* 3 */
    "not", /* 4 - one var */
    "clr", /* 5 - one var */
    "lea", /* 6 */
    "inc", /* 7 - one var */
    "dec", /* 8 - one var */
    "jmp", /* 9 - one var */
    "bne", /* 10 - one var */
    "red", /* 11 - one var */
    "prn", /* 12 - one var */
    "jsr", /* 13 - one var */
    "rts", /* 14 - 0 var */
    "stop" /* 15 - 0 var */
};

/* An Array of our instructions types. */
char *commandsPrefix[COMMANDS_PREFIX_NUMBER] = {
    ".extern",
    ".entry",
    "mcro",
    "endmcro",
    ".data",
    ".string"};

/* Flag to indicate if we're currently processing a macro. */
static bool macroFlag = false;

void printLabel(const char *filename)
{
    printLabels(filename);
}

/**
 * Replaces multiple consecutive spaces with a single space in a string.
 * The function modifies the input string in-place.
 */
void replaceMultipleSpaces(char *str)
{
    int i, j;
    int whitespaceCount = 0;
    int len = strlen(str);

    for (i = 0, j = 0; i < len; i++)
    {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
            whitespaceCount++;
        else
            whitespaceCount = 0;

        if (whitespaceCount <= 1)
        {
            str[j] = str[i];
            j++;
        }
    }

    str[j] = '\0';
}

/**
 * This function removes leading white space characters from a given string.
 * The function iterates over the string, identifies the leading spaces, and then
 * shifts the remaining characters to the start of the string, effectively removing the spaces.
 */
void removePrefixSpaces(char *command)
{
    int i, j;

    for (i = 0; command[i] == ' ' || command[i] == '\n' || command[i] == '\t'; i++)
    {
    }

    for (j = 0; command[i]; j++, i++)
    {
        command[j] = command[i];
    }

    command[j] = '\0';
}

void remove_spaces(char *str)
{
    int len = strlen(str);
    int j = 0, i;
    for (i = 0; i < len; i++)
    {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t' && str[i] != 0)
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

/*
    Gets the index of a command in the commandsNames array.
    Input: command - the command to search for.
    Output: The index of the command if found, otherwise -1.
*/
int getCommandIndexByList(char *command, char *list[], int listLength)
{
    int commandLength = 0;
    int commandIndex = 0;

    while (command[commandLength] != ' ' && command[commandLength] != '\t' && command[commandLength] != VAR_SEPARATOR &&
           command[commandLength] != '\0' && command[commandLength] != 0 && command[commandLength] != 10)
    {
        commandLength++;
    }

    for (commandIndex = 0; commandIndex < listLength; commandIndex++) /*TODO: change to real list length*/
    {
        /* Two-way inclusion check of the string*/
        if (strlen(list[commandIndex]) == commandLength && strncmp(list[commandIndex], command, commandLength) == 0)
        {
            return commandIndex;
        }
    }
    return -1;
}

/*
    Checks if a character represents a valid register name.
    Input: register - the character to check.
    Output: true if the character is a valid register name, false otherwise.
*/
bool isRegisterName(char registerName[])
{
    if (strlen(registerName) < 2)
        return false;

    return registerName[0] == REGISTER_PREFIX && registerName[1] == 'r' && registerName[2] >= '0' && registerName[2] <= '7';
}

/**
 * Returns the index of the first occurrence of a separator character in a string.
 * If the separator character is not found, the function returns 0.
 */
int getCharIndexBySeparator(char *str, char separator)
{
    int i;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == separator)
            return i;
    }

    return -1;
}

/**
 * Extracts the label from a command string.
 * If the label is found, it is removed from the command string.
 */
char *tryGetLabel(char **command)
{
    int index;
    char *label;

    index = getCharIndexBySeparator(*command, LABEL_SEPERATOR);

    if (index <= 0)
        return NULL;

    label = (char *)malloc((index + 1) * sizeof(char));
    strncpy(label, *command, index);
    label[index] = '\0';   /* Null-terminate the new string */
    *command += index + 1; /* Move the command pointer to skip the label and separator */
    return label;
}

/**
 * Tries to parse a number from a string.
 * If the string represents a valid number, the function returns the number.
 * If the string does not represent a valid number, the function returns NaN.
 */
double tryGetNumber(char *str)
{
    char *endptr;
    double number;
    number = strtod(str, &endptr);

    /*Check if conversion was successful and if the entire string was processed*/
    if (*endptr == '\0')
    {
        return number;
    }

    /*If conversion failed or not the entire string was processed, return NaN (Not a Number)*/
    return 0.5;
}

/**
 * Extracts a substring from a string up to the first occurrence of a separator character.
 * The substring does not include the separator character.
 * The function allocates memory for the new string and it must be freed by the caller.
 */
char *getSubstringBySeparator(char *str, char separator)
{
    /*find the index of the separator (if present)*/
    const char *separator_ptr;
    size_t new_string_length;
    char *new_string;

    if (str == NULL)
        return NULL;

    /* Find separator in string */
    separator_ptr = strchr(str, separator);

    /* Calculate the length of the new string */
    new_string_length = separator_ptr ? (size_t)(separator_ptr - str) : strlen(str);

    new_string = (char *)malloc((new_string_length + 1) * sizeof(char));
    if (new_string == NULL)
    {
        return NULL;
    }

    /* Copy characters into new string */
    strncpy(new_string, str, new_string_length);

    new_string[new_string_length] = '\0'; /*null terminate the new substring*/

    return new_string;
}

static int linesCounter = 0;      /* Counter for the number of lines processed in the  code. */
static char *currentMacro = NULL; /* Holds the name of the macro that is currently being processed. */

/**
 * Handles the first run of the assembler.
 * The function extracts labels and commands from each line and updates the symbol table and the instruction counter.
 */
void startFirstRun(char command[], int lineNumber, char *fileName)
{
    char *label;
    int prefixIndex = 0;
    char *originalCommand = NULL;
    int linesNumber;

    originalCommand = (char *)malloc(strlen(command) + 1);
    if (originalCommand == NULL)
    {
        MEMORY_ALLOCATION_FAILED(fileName, lineNumber);
        return;
    }

    strcpy(originalCommand, command);
    label = tryGetLabel(&command); /*if label exist - we will get the label name and remove it from the command*/

    removePrefixSpaces(command);
    prefixIndex = getCommandIndexByList(command, commandsPrefix, COMMANDS_PREFIX_NUMBER);

    remove_spaces(command);

    if (isMacroName(command))
    {

        macroLayout(command, getFileNameWithExtension(fileName, MACRO_FILE_NAME_EXTENSION), lineNumber);
        free(originalCommand);

        return;
    }

    if (macroFlag && prefixIndex != 3) /*if we in a macro - > count the lines*/
    {
        linesCounter++;
        free(originalCommand);
        return;
    }

    if (prefixIndex != 2 && prefixIndex != 3)
    {

        char *fullFileName = getFileNameWithExtension(fileName, MACRO_FILE_NAME_EXTENSION);

        printf("fullFileNameWithExtension: %s \n", fileName);

        appendStringToFile(fullFileName, originalCommand);
    }

    tryGetLabel(&originalCommand); /* for determining lines number */

    if (label != NULL && strcmp(label, "") != 0 && prefixIndex != -1) /*note in page 41*/
    {
        if (prefixIndex < 2)
        {
            INVALID_OPTION_FOR_COMMAND(fileName, lineNumber);
        }
        else
        {
            addNewLabel(label);
        }
    }
    if (prefixIndex != -1)
    {
        char *secondVar;
        char *commandPrefix = commandsPrefix[prefixIndex];
        secondVar = command + strlen(commandPrefix); /*in this part wer'e holding the label*/

        printf("commandPrefix %s\n", commandPrefix);
        printf("secondVar: %s\n", secondVar);

        switch (prefixIndex)
        {
        case 0: /*extern*/
        {
            if (!isLabelExist(secondVar, lineNumber, fileName, false, 1))
            {
                addNewExtern(secondVar);
            }
            else
            {
                printf("ERROR: label %s not exist in line %d\n", secondVar, lineNumber);
            }
            return;
        }
        case 1: /*entry*/
        {
            if (isValidNewEntry(secondVar))
            {
                addNewEntry(secondVar);
            }
            else
            {
                printf("ERROR: label %s already exist \n", secondVar);
            }
            return;
        }
        case 2: /*mcro*/
        {
            if (currentMacro == NULL)
                currentMacro = (char *)malloc(strlen(secondVar) + 1);

            if (label != NULL && strcmp(label, "") != 0)
                addNewLabel(label);

            strcpy(currentMacro, secondVar);
            addMacro(secondVar, lineNumber);
            macroFlag = true;
            return;
        }
        case 3: /*endmcro*/
        {
            if (label != NULL && strcmp(label, "") != 0)
                addNewLabel(label);

            updateLinesCount(currentMacro, linesCounter);
            macroFlag = false;

            if (currentMacro != NULL)
            {
                free(currentMacro);
                currentMacro = NULL;
            }

            linesCounter = 0;
            return;
        }
        case 4: /*data*/
        {
            size_t length;
            int *data;
            data = parseIntArray(secondVar, &length);
            addData(data, label, length);
            return;
        }
        case 5: /*string*/
        {
            addString(secondVar, label);
            return;
        }
        default: /*useless*/
            return;
        }
    }

    else
    {
        linesNumber = determineLinesNumber(originalCommand);
        increaseIC(linesNumber);
        if (label != NULL)
            addNewLabel(label);
    }
    if (label != NULL)
        free(label);
}

/**
 * Parses an array of integers from a comma-separated string.
 * The function returns a dynamically allocated array of integers and stores the array size in 'length'.
 * If an error occurs during parsing, the function returns NULL and 'length' is undefined.
 */
int *parseIntArray(char *input, size_t *length)
{
    int *array = NULL;
    char *token;
    int num;
    int *temp;
    *length = 0;

    /* Check for trailing comma */
    if (input[strlen(input) - 1] == ',')
    {
        fprintf(stderr, "Error: Trailing comma detected.\n");
    }

    token = strtok(input, ",");
    while (token != NULL)
    {
        (*length)++;
        num = atoi(token);
        printf("%d \n", num);
        temp = realloc(array, (*length) * sizeof(int));
        if (temp == NULL)
        {
            free(array);
            return NULL;
        }
        array = temp;

        array[(*length) - 1] = num;
        token = strtok(NULL, ",");
    }

    return array;
}

/**
 * Determines the number of lines that a command will occupy in the machine code.
 * The function counts the number of operands and takes into account whether they are registers or not.
 */
int determineLinesNumber(char *command)
{
    char *firstVar;
    int commandIndex;
    printf("command: %s\n", command);
    removePrefixSpaces(command);
    commandIndex = getCommandIndexByList(command, commandsNames, COMMANDS_NUMBER);
    printf("command: %s\n", command);
    if (commandIndex > 13) /*0 vars*/
        return 1;

    if (commandIndex > 3 && commandIndex != 6) /*one var*/
        return 2;

    else /* in this part we are in a case of 2 vars*/
    {
        command = command + strlen(commandsNames[commandIndex]);
        remove_spaces(command);
        printf("command: %s\n", command);
        firstVar = getSubstringBySeparator(command, VAR_SEPARATOR);
        command += strlen(firstVar) + 1; /* + 1 for seperator ','*/
        printf("command: %s\n", command);
        printf("firstVar: %s\n", firstVar);
        if (isRegisterName(firstVar) && isRegisterName(command))
            return 2;
        return 3;
    }
}

static bool isMacro = false;

void commandParser(char *command, char *fileName, int lineNumber)
{
    char *label = NULL, *label1 = NULL, *label2 = NULL, *firstVar = NULL, *originalCommand = NULL;
    int prefixIndex, commandIndex, register1 = -1, register2 = -1;
    double immidiate1 = 0.5, immidiate2 = 0.5; /* 0.5 represent the default value for immidiate */
    int determineLinesNumberResult;

    originalCommand = (char *)malloc(strlen(command) + 1);
    if (originalCommand == NULL)
    {
        MEMORY_ALLOCATION_FAILED(fileName, lineNumber);
    }
    strcpy(originalCommand, command);

    label = tryGetLabel(&command);
    removePrefixSpaces(command);
    prefixIndex = getCommandIndexByList(command, commandsPrefix, COMMANDS_PREFIX_NUMBER);

    if (prefixIndex != -1)
    {
        /* if (prefixIndex == 2)
         {
             isMacro = true;
             return;
         }
         if (prefixIndex == 3)
         {
             isMacro = false;
             return;
         }*/
        if (prefixIndex == 4)
            sendDataValue(fileName, label);
        if (prefixIndex == 5)
            sendStringValue(fileName, label);
        return;
    }

    /* if (isMacro)
     {
         printf("command inside macro - continue\n");
         return;
     }

     if (isMacroName(command))
     {
         sendMacro(command, fileName);
         return;
     }
    originalCommand[strlen(originalCommand) - 1] = '\0';
     appendStringToFile(concatenateStrings(fileName, MACRO_SUFFIX), originalCommand);
     if (prefixIndex != -1)
        return; */

    tryGetLabel(&originalCommand); /*we want to remove the label before sending it to determin lines number*/

    printf("originalCommand: %s\n", originalCommand);
    commandIndex = getCommandIndexByList(command, commandsNames, COMMANDS_NUMBER);
    command = command + strlen(commandsNames[commandIndex]);
    remove_spaces(command);
    printf("originalCommand: %s\n", originalCommand);
    printf("commandIndex %d \n", commandIndex);

    if (commandIndex == -1)
    {
        UNKNOWN_COMMAND_ERROR(fileName, lineNumber);
        return;
    }

    if (commandIndex > 13) /*0 vars*/
    {
        if (strlen(command) > 0)
        {
            EXTRANEOUS_TEXT_ERROR(fileName, lineNumber); /* if there is another char that we dont want */
        }
        addNewLine(fileName, commandIndex, -1, -1, NULL, NULL, 0.5, 0.5); /* Note: -1 means that there is no register in this operand slot*/
    }
    else if (commandIndex < 14 && commandIndex > 3 && commandIndex != 6) /*one var*/
    {
        if (isRegisterName(command))
        {
            if (strlen(command) == 3)
            {
                addNewLine(fileName, commandIndex, command[2] - '0', -1, NULL, NULL, 0.5, 0.5); /*Note: -1 means that there is no register in this operand slot*/
            }
            else
            {
                INVALID_OPTION_FOR_COMMAND(fileName, lineNumber); /* if there is another char that we dont want */
            }
        }
        else if (isLabelExist(command, lineNumber, fileName, true, determineLinesNumber(originalCommand) - 1))
        {
            addNewLine(fileName, commandIndex, -1, -1, command, NULL, 0.5, 0.5);
        }
        else if (isdigit(command[0]) || command[0] == '-') /*TODO: check if this option exist*/
        {
            addNewLine(fileName, commandIndex, -1, -1, NULL, NULL, tryGetNumber(command), 0.5);
        }
        else
        {

            INVALID_OPTION_FOR_COMMAND(fileName, lineNumber);
        }
    }
    else /* In this part we are handle a case where there are 2 vars*/
    {
        firstVar = getSubstringBySeparator(command, VAR_SEPARATOR);
        command += strlen(firstVar) + 1; /*+ 1 for seperator ','*/
        determineLinesNumberResult = determineLinesNumber(originalCommand) - 1;
        printf("firstVar %s command %s\n", firstVar, command);

        if (firstVar[0] == '+' || firstVar[0] == '-' || isdigit(firstVar[0]))
            immidiate1 = tryGetNumber(firstVar);

        else if (isLabelExist(firstVar, lineNumber, fileName, true, determineLinesNumberResult))
        {
            label1 = firstVar;
        }

        else if (isRegisterName(firstVar))
            register1 = firstVar[2] - '0';

        if (command[0] == '-' || isdigit(command[0]))
            immidiate2 = tryGetNumber(command);

        else if (isLabelExist(command, lineNumber, fileName, true, determineLinesNumberResult))
        {
            label2 = command;
            printf("label2\n");
        }

        else if (isRegisterName(command))
            register2 = command[2] - '0';

        else
        {
            printf("error\n");
            INVALID_OPTION_FOR_COMMAND(fileName, lineNumber);
        }
        printf("addNewLine: filename %s  commandIndex %d register1 %c register2 %c label1 %s label2 %s immidiate1 %f immidiate2 %f\n", fileName, commandIndex, register1, register2, label1, label2, immidiate1, immidiate2);
        addNewLine(fileName, commandIndex, register1, register2, label1, label2, immidiate1, immidiate2);
    }
}
