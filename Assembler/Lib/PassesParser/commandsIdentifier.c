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
 * This function checks whether a given null-terminated string consists
 * only of numeric digits (0-9).
 * @param str The input string to be checked for numeric content.
 * @return Returns `true` if the entire string contains only numeric digits,
 * and `false` otherwise.
 */
bool isNumber(const char *str)
{
    int i = 0;
    int length = strlen(str);

    for (i = 0; i < length; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
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
    printf("registerName on : %s\n", registerName);
    printf("return %d\n", registerName[0] == REGISTER_PREFIX && registerName[1] == 'r' && registerName[2] >= '0' && registerName[2] <= '7');
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
 * Validates the format of the provided label. Ensures that the label adheres to
 * the assembly language's naming conventions.
 */
bool validateLabel(char *command)
{

    int i, length = strlen(command);
    if (length > 31)
    {
        printf("LABEL_TOO_LONG_ERROR: %s\n", command);
        return false;
    }
    printf("validateLabel: %s\n", command);
    for (i = 0; i < length; i++)
    {
        printf("validateLabel: %d\n", i);

        if (!(command[i] >= '0' && command[i] <= '9') &&
            !(command[i] >= 'A' && command[i] <= 'Z') &&
            !(command[i] >= 'a' && command[i] <= 'z'))
        {
            printf("Forbidden char: %c\n", command[i]);
            return false;
        }
    }
    return true;
}

/**
 * Extracts a label from a command string, if present. It's one of the crucial
 * functions as labels in assembly can be associated with data or a particular
 * point in the instruction set.
 */
char *tryGetLabel(char **command, char *fileName, int lineNumber)
{
    int index;
    char *label;
    printf("tryGetLabel: %s %d\n", fileName, lineNumber);
    index = getCharIndexBySeparator(*command, LABEL_SEPERATOR);

    if (index <= 0)
        return NULL;

    label = (char *)malloc((index + 1) * sizeof(char));
    strncpy(label, *command, index);
    label[index] = '\0';   /* Null-terminate the new string */
    *command += index + 1; /* Move the command pointer to skip the label and separator */
    printf("before validation\n");
    if (!validateLabel(label))
        INVALID_LABEL_FORMAT(fileName, lineNumber, label);

    return label;
}

/**
 * Tries to parse a number from a string.
 * If the string represents a valid number, the function returns the number.
 * If the string does not represent a valid number, the function returns NaN.
 */
double tryGetNumber(char *str, const char *fileName, int lineNumber)
{
    char *endptr;
    double number;

    /* Check if the string has a decimal point */
    if (strchr(str, '.') != NULL)
    {
        INVALID_NUMBER_VALUE(fileName, lineNumber, str);
        return 0.5;
    }

    number = strtod(str, &endptr);

    /* Check if conversion was successful and if the entire string was processed */
    if (*endptr != '\0')
    {
        INVALID_NUMBER_VALUE(fileName, lineNumber, str);
        return 0.5;
    }

    return number;
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
 * This function is the heart of the program, handling the first run of the assembler.
 * It processes each line of the input assembly code, extracting labels, identifying
 * commands, and updating relevant data structures accordingly.
 */
void startFirstRun(char command[], int lineNumber, char *fileName)
{
    char *label;
    int prefixIndex = 0;
    char *originalCommand = NULL;
    int linesNumber;
    bool isSuccess;

    originalCommand = (char *)malloc(strlen(command) + 1);
    if (originalCommand == NULL)
    {
        MEMORY_ALLOCATION_FAILED(fileName, lineNumber);
        return;
    }

    strcpy(originalCommand, command);
    printf("tryGetLabel: %s\n", fileName);
    label = tryGetLabel(&command, fileName, lineNumber); /*if label exist - we will get the label name and remove it from the command*/

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

    tryGetLabel(&originalCommand, fileName, lineNumber); /* for determining lines number */

    if (label != NULL && strcmp(label, "") != 0 && prefixIndex != -1) /*note in page 41*/
    {
        if (prefixIndex < 2)
        {
            /*INVALID_LABEL_FORMAT(fileName, lineNumber, label);*/ /* TODO: to check if needed - */
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
            if (isNumber(secondVar))
            {
                INVALID_ENTRY_EXTERN_PARAM(fileName, lineNumber, secondVar);
                return;
            }

            if (!isLabelExist(secondVar, lineNumber, fileName, false, 1))
            {
                addNewExtern(secondVar);
            }
            else
            {
                LABEL_ALREADY_EXISTS_ERROR(fileName, lineNumber, secondVar);
            }
            return;
        }
        case 1: /*entry*/
        {
            if (isNumber(secondVar))
            {
                INVALID_ENTRY_EXTERN_PARAM(fileName, lineNumber, secondVar);
                return;
            }

            if (isValidNewEntry(secondVar))
            {
                addNewEntry(secondVar);
            }
            else
            {
                LABEL_ALREADY_EXISTS_ERROR(fileName, lineNumber, secondVar);
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
            int i;
            data = parseIntArray(secondVar, &length, fileName, lineNumber);
            for (i = 0; i < length; i++)
            {
                printf("Parsed data[%d]: %d\n", i, data[i]);
            }
            isSuccess = addData(data, label, length);
            if (!isSuccess)
            {
                printf("LABEL_ALREADY_EXISTS_ERROR: %s\n", label);
                LABEL_ALREADY_EXISTS_ERROR(fileName, lineNumber, label);
            }
            return;
        }
        case 5: /*string*/
        {
            isSuccess = addString(secondVar, label);
            if (!isSuccess)
            {
                printf("LABEL_ALREADY_EXISTS_ERROR: %s\n", label);
                LABEL_ALREADY_EXISTS_ERROR(fileName, lineNumber, label);
            }
            return;
        }
        default: /*useless*/
            return;
        }
    }
    else
    {
        if (label != NULL)
        {
            isSuccess = addNewLabel(label);
            if (!isSuccess)
                LABEL_ALREADY_EXISTS_ERROR(fileName, lineNumber, label);
        }
        printf("determineLinesNumber: %s\n", originalCommand);
        linesNumber = determineLinesNumber(originalCommand, lineNumber, fileName);
        if (linesNumber == 0)
        {
            UNKNOWN_OPERAND(fileName, lineNumber);
            return;
        }

        printf("increaseIC by linesNumber: %d\n", linesNumber);
        increaseIC(linesNumber);
    }
    if (label != NULL)
        free(label);
}

/**
 * Parses an array of integers from a comma-separated string.
 * The function returns a dynamically allocated array of integers and stores the array size in 'length'.
 * If an error occurs during parsing, the function returns NULL and 'length' is undefined.
 */
int *parseIntArray(char *input, size_t *length, const char *fileName, int lineNumber)
{
    int *array = NULL;
    char *token;
    int num;
    int *temp;
    size_t i;
    *length = 0;

    /* Check for extraneous comma or trailing comma */
    if (input[0] == ',')
    {
        EXTRANEOUS_TEXT_ERROR(fileName, lineNumber);
        free(array);
        return NULL;
    }
    else if (input[strlen(input) - 1] == ',')
    {
        TRAILING_COMMA_ERROR(fileName, lineNumber);
        free(array);
        return NULL;
    }

    /* Check for consecutive commas */
    for (i = 0; i < strlen(input) - 1; i++)
    {
        if (input[i] == ',' && input[i + 1] == ',')
        {
            MULTIPLE_CONSECUTIVE_COMMAS_ERROR(fileName, lineNumber);
            free(array);
            return NULL;
        }
    }

    token = strtok(input, ",");
    while (token != NULL)
    {
        /* Check if token has a decimal point */
        if (strchr(token, '.') != NULL)
        {
            INVALID_NUMBER_VALUE(fileName, lineNumber, token);
            free(array);
            return NULL;
        }

        num = atoi(token); /* Using your original atoi function */
        printf("%d \n", num);
        (*length)++;

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
int determineLinesNumber(char *command, int lineNumber, char *fileName)
{
    char *firstVar;
    int commandIndex, linesNumber = 1;
    printf("command: %s\n", command);
    removePrefixSpaces(command);
    commandIndex = getCommandIndexByList(command, commandsNames, COMMANDS_NUMBER);
    printf("commandIndex: %d\n", commandIndex);
    if (commandIndex == -1)
        return 0;

    if (commandIndex > 13) /*0 vars*/
        return 1;

    if (commandIndex > 3 && commandIndex != 6) /*one var*/
        return 2;

    else /* in this part we are in a case of 2 vars*/
    {
        command = command + strlen(commandsNames[commandIndex]);
        remove_spaces(command);
        firstVar = getSubstringBySeparator(command, VAR_SEPARATOR);
        command += strlen(firstVar) + 1; /* + 1 for seperator ','*/

        printf("first if: %s\n", firstVar);
        if (isRegisterName(firstVar) && isRegisterName(command))
            return 2;

        printf("second if: %s\n", firstVar);

        if (isRegisterName(firstVar) || isRegisterName(command))
            linesNumber++;

        printf("third if linesNumber: %d\n", linesNumber);

        if (isLabelExist(firstVar, lineNumber, fileName, false, 1) ||
            isLabelExist(command, lineNumber, fileName, false, 1))
            linesNumber++;

        return linesNumber > 1 ? linesNumber : 0;
    }
}

/* static bool isMacro = false; */

/**
 * The second heart of this program. The commandParser function process each command from the assembly code.
 * It categorizes the command, extracts relevant information, and delegates
 * to other functions for specific processing.
 */
void commandParser(char *command, char *fileName, int lineNumber)
{
    char *label = NULL, *label1 = NULL, *label2 = NULL, *firstVar = NULL, *originalCommand = NULL;
    int prefixIndex, commandIndex, register1 = -1, register2 = -1;
    double immidiate1 = 0.5, immidiate2 = 0.5; /* 0.5 represent the default value for immidiate */
    int determineLinesNumberResult;
    char *secondVar;
    char *commandPrefix;

    originalCommand = (char *)malloc(strlen(command) + 1);
    if (originalCommand == NULL)
    {
        MEMORY_ALLOCATION_FAILED(fileName, lineNumber);
    }

    label = tryGetLabel(&command, fileName, lineNumber);
    strcpy(originalCommand, command);
    removePrefixSpaces(command);

    prefixIndex = getCommandIndexByList(command, commandsPrefix, COMMANDS_PREFIX_NUMBER);

    printf("getCommandIndexByList: %s--------------\n", command);
    if (prefixIndex != -1)
    {
        if (prefixIndex < 3)
        {
            free(originalCommand);
            if (label)
                free(label);
            return;
        }

        commandPrefix = commandsPrefix[prefixIndex];
        secondVar = command + strlen(commandPrefix);

        if (prefixIndex == 5)
        {
            printf("sendStringValue: %s\n", secondVar);
            sendStringValue(fileName, label, secondVar);
        }

        if (prefixIndex == 4)
        {
            int *data;
            size_t length = 0;
            data = parseIntArray(secondVar, &length, fileName, lineNumber);
            sendDataValue(fileName, label, data, length);
            free(data);
        }

        free(originalCommand);
        if (label)
            free(label);
        return;
    }

    commandIndex = getCommandIndexByList(command, commandsNames, COMMANDS_NUMBER);
    command = command + strlen(commandsNames[commandIndex]);
    remove_spaces(command);
    printf("commandIndex: %d\n", commandIndex);
    if (commandIndex == -1)
    {
        UNKNOWN_COMMAND_ERROR(fileName, lineNumber);
        free(originalCommand);
        if (label)
            free(label);
        return;
    }

    if (commandIndex > 13) /*0 vars*/
    {
        if (strlen(command) > 0)
        {
            EXTRANEOUS_TEXT_ERROR(fileName, lineNumber); /* if there is another char that we dont want */
        }
        addNewLine(fileName, commandIndex, -1, -1, NULL, NULL, 0.5, 0.5, lineNumber); /* Note: -1 means that there is no register in this operand slot*/
    }
    else if (commandIndex < 14 && commandIndex > 3 && commandIndex != 6) /*one var*/
    {
        if (isRegisterName(command))
        {
            if (strlen(command) == 3)
            {
                addNewLine(fileName, commandIndex, command[2] - '0', -1, NULL, NULL, 0.5, 0.5, lineNumber); /*Note: -1 means that there is no register in this operand slot*/
            }
            else
            {
                EXTRANEOUS_TEXT_ERROR(fileName, lineNumber); /* if there is another char that we dont want */
            }
        }
        else if (isLabelExist(command, lineNumber, fileName, true, determineLinesNumber(originalCommand, lineNumber, fileName) - 1))
        {
            printf("addNewLine \n");
            addNewLine(fileName, commandIndex, -1, -1, command, NULL, 0.5, 0.5, lineNumber);
        }
        else if (isdigit(command[0]) || command[0] == '-') /*TODO: check if this option exist*/
        {
            addNewLine(fileName, commandIndex, -1, -1, NULL, NULL, tryGetNumber(command, fileName, lineNumber), 0.5, lineNumber);
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
        determineLinesNumberResult = determineLinesNumber(originalCommand, lineNumber, fileName) - 1;
        printf("firstVar %s command %s\n", firstVar, command);

        if (firstVar[0] == '+' || firstVar[0] == '-' || isdigit(firstVar[0]))
            immidiate1 = tryGetNumber(firstVar, fileName, lineNumber);

        else if (isLabelExist(firstVar, lineNumber, fileName, true, determineLinesNumberResult))
        {
            label1 = firstVar;
        }

        else if (isRegisterName(firstVar))
            register1 = firstVar[2] - '0';

        if (command[0] == '-' || isdigit(command[0]))
            immidiate2 = tryGetNumber(command, fileName, lineNumber);

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

        addNewLine(fileName, commandIndex, register1, register2, label1, label2, immidiate1, immidiate2, lineNumber);
    }

    free(originalCommand);
    if (label)
        free(label);
    if (label1 != firstVar && label2 != firstVar)
        free(firstVar);
}
