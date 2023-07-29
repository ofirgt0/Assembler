#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "commandsIdentifier.h"

#define VAR_SEPARATOR ','
#define LABEL_SEPERATOR ':'
#define COMMANDS_NUMBER 16
#define REGISTER_PREFIX '@'
#define FLOAT_NUMBER_DOT '.'
#define MAX_COMMAND_LENGTH 100
#define COMMANDS_PREFIX_NUMBER 6
#define MACRO_COMMAND "mcro"
#define END_MACRO_COMMAND "endmcro"
#define DEFAULT_ADDRESS 0
#define ABSOLUTE_A_R_E_DECIMAL_CODE 0
#define EXTERNAL_A_R_E_DECIMAL_CODE 1
#define RELOCATABLE_A_R_E_DECIMAL_CODE 2

/// TODO: MOVE TO ERROR HANDLER
#define ERROR_MISSING_COMMA "Illegal comma\n"
#define ERROR_MISSING_PARAMETER "Missing parameter\n"
#define ERROR_INVALID_COMMAND "Undefined command name\n"
#define ERROR_UNDEFINED_COMPLEX_VAR "Undefined complex variable\n"
#define ERROR_EXTRANEOUS_TEXT "Extraneous text after end of command\n"
#define ERROR_MULTIPLE_CONSECUTIVE_COMMAS "Multiple consecutive commas\n"
#define ERROR_THIRD_PARAMETER_ISNT_NUMBER "Third parameter is not a number\n"
#define ERROR_SECOND_PARAMETER_ISNT_NUMBER "Second parameter is not a number\n"

char *commandsNames[COMMANDS_NUMBER] = {
    "mov",  // 0
    "cmp",  // 1
    "add",  // 2
    "sub",  // 3
    "not",  // 4 - one var
    "clr",  // 5 - one var
    "lea",  // 6
    "inc",  // 7 - one var
    "dec",  // 8 - one var
    "jmp",  // 9 - one var
    "bne",  // 10 - one var
    "red",  // 11 - one var
    "prn",  // 12 - one var
    "jsr",  // 13 - one var
    "rts",  // 14 - 0 var
    "stop", // 15 - 0 var
};

char *commandsPrefix[COMMANDS_PREFIX_NUMBER] = {
    ".extern",
    ".entry",
    "mcro",
    "endmcro",
    ".data",
    ".string"};

static bool macroFlag = false;

void replaceMultipleSpaces(char *str)
{
    int i, j;
    int whitespaceCount = 0;
    int len = strlen(str);

    for (i = 0, j = 0; i < len; i++)
    {
        if (str[i] == ' ' || str[i] == '\t')
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

/*
    Gets the index of a command in the commandsNames array.
    Input: command - the command to search for.
    Output: The index of the command if found, otherwise -1.
*/
int getCommandIndexByList(char command[], char *list[])
{
    int commandLength = 0;
    int commandIndex = 0;

    while (command[commandLength] != ' ' && command[commandLength] != '\t' && command[commandLength] != VAR_SEPARATOR && command[commandLength] != '\0')
        commandLength++;

    for (commandIndex = 0; commandIndex < COMMANDS_NUMBER; commandIndex++) // TODO: change to real list length
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

int getCharIndexBySeparator(char *str, char seperator)
{
    int i;

    for (i = 0; str[i] != '\0'; i++)
    {
        if (strchr(seperator, str[i]) != NULL)
            return i;
    }

    return 0;
}

char *tryGetLabel(char *command)
{
    int index = getCharIndexBySeparator(command, LABEL_SEPERATOR);

    if (index == 0)
        return NULL;

    char *label = (char *)malloc((index + 1) * sizeof(char));
    strncpy(label, command, index - 1); // -1 because we dont need the seperator inside the label name
    label[index + 1] = '\0';            // Null-terminate the new string
    command += index;                   // We dont need the label anymore

    return label;
}

double tryGetNumber(char *str)
{
    char *endptr;
    double number = strtod(str, &endptr);

    // Check if conversion was successful and if the entire string was processed
    if (*endptr == '\0')
    {
        return number;
    }

    // If conversion failed or not the entire string was processed, return NaN (Not a Number)
    return 0.0 / 0.0;
}

char *getSubstringBySeparator(char *str, char separator)
{
    if (str == NULL)
        return NULL;

    // find the index of the separator (if present)
    const char *separator_ptr = strchr(str, separator);

    // calculate the length of the new substring
    size_t new_string_length = separator_ptr ? (size_t)(separator_ptr - str) : strlen(str);

    // allocate memory for the new substring
    char *new_string = (char *)malloc((new_string_length + 1) * sizeof(char));
    if (new_string == NULL)
    {
        return NULL;
    }

    // copy characters from the input string to the new substring
    strncpy(new_string, str, new_string_length);
    new_string[new_string_length] = '\0'; // null terminate the new substring

    return new_string;
}

// --------------------------------------------------------------------------------------------------

static int linesCounter = 0;
static char *currentMacro = NULL;

void startFirstRun(char command[], int lineNumber, char *fileName)
{
    if (macroFlag) // if we r in macro, count the lines
    {
        linesCounter++;
        return;
    }
    // liron: LabelName: .entry WOW
    char *label = tryGetLabel(command); // if label exist - we will get the label name and remove it from the command
    // liron    .entry WOW
    /// TODO: check if we need to remove PREFIX SPACES
    int prefixIndex = getCommandIndexByList(command, commandsPrefix);

    if (label != "") // note in page 41
    {
        if (prefixIndex < 2)
        {
            /// TODO: warning
        }
        else
        {
            addNewLabel(label); // regular label
        }
    }

    if (prefixIndex != -1)
    {
        char *secondVar = command + strlen(commandsPrefix[prefixIndex]); // in this part we have the label in the hand
        switch (prefixIndex)
        {
        case 0: // extern
        {
            /// TODO: add warning for the option that label != null  as note in page 41
            addNewExtern(secondVar); // TODO: in dataService
            return;
        }
        case 1: // entry
        {
            addNewEntry(secondVar); // TODO: in dataService
            return;
        }
        case 2: // mcro
        {
            strcpy(secondVar, currentMacro);
            addMacro(secondVar, lineNumber);
            macroFlag = true;
            return;
        }
        case 3: // endmcro
        {
            updateLinesCount(linesCounter);

            macroFlag = false;
            currentMacro = NULL;
            linesCounter = 0;
            return;
        }
        case 4: // data
        {
            size_t length;
            int *data = parseIntArray(secondVar, &length);
            addNewData(data, secondVar); // in dataService
            return;
        }
        case 5: // string
        {

            return;
        }
        default: // useless
            return;
        }
    }
    else
    {
        int linesNumber = determineLinesNumber(command);
        if(label != NULL)
            addNewLabel(label);
        increaseIC(linesNumber);
    }
}

int* parseIntArray(char* input, size_t* length) {
    size_t count = 0;
    int* result = NULL;

    char* inputCopy = strdup(input);
    char* token;
    char* rest = inputCopy;

    while ((token = strtok_r(rest, ",", &rest))) {
        // Handle consecutive commas
        if (strlen(token) == 0) {
            printf("Error: Invalid input format - consecutive commas\n");
            free(inputCopy);
            free(result);
            return NULL;
        }

        char* endptr;
        int val = strtol(token, &endptr, 10);

        // Handle non-integer characters
        if (*endptr != '\0') {
            printf("Error: Invalid input format - non-integer characters\n");
            free(inputCopy);
            free(result);
            return NULL;
        }

        // Reallocate memory for the result array
        count++;
        int* temp = (int*)realloc(result, count * sizeof(int));
        if (!temp) {
            printf("Error: Memory allocation failed\n");
            free(inputCopy);
            free(result);
            return NULL;
        }
        result = temp;

        // Store the parsed integer in the array
        result[count - 1] = val;
    }

    free(inputCopy);
    *length = count;
    return result;
}

int determineLinesNumber(char *command)
{
    int commandIndex = getCommandIndexByList(command, commandsNames);

    if (commandIndex > 13) // 0 vars
        return 1;

    if (commandIndex > 3 && commandIndex != 6) // one var
        return 2;

    else // in this part we r in the case of 2 vars
    {
        command = command + strlen(commandsNames[commandIndex]);
        replaceMultipleSpaces(command);

        char *firstVar = getSubstringBySeparator(command, VAR_SEPARATOR);
        command += strlen(firstVar) + 1; // + 1 for seperator ,

        if (isRegisterName(firstVar) && isRegisterName(command))
            return 2;
        return 3;
    }
}

// second run
void *commandParser(char *command, char *fileName)
{
    char *label = tryGetLabel(command);
    command += strlen(label);
    /// TODO: check if we need to remove PREFIX SPACES
    int prefixIndex = getCommandIndexByList(command, commandsPrefix);
    if (prefixIndex != -1)
        return; // we handle this commands in the first run

    if (isMacroName(command))
    {
        /// TODO: what should we do with label before macro?
        sendMacro(command, fileName);
        return;
    }

    int commandIndex;
    commandIndex = getCommandIndexByList(command, commandsNames);
    command = command + strlen(commandsNames[commandIndex]);
    replaceMultipleSpaces(command);

    if (commandIndex > 13) // 0 vars
    {
        if (strlen(command) > 0)
        {
            /// TODO: handle error
        }
        addNewLine(commandIndex, -1, -1, NULL, NULL, 0, 0); /// Note: -1 means that there is no register in this operand slot
    }
    else if (commandIndex < 14 && commandIndex > 3 && commandIndex != 6) // one var
    {
        if (isRegisterName(command))
        {
            if (strlen(command) == 3)
            {
                addNewLine(commandIndex, command[2] - '0', -1, NULL, NULL, 0, 0); /// Note: -1 means that there is no register in this operand slot
            }
            else
            {
                /// TODO: handle error for char
            }
        }
        else if (isLabelExist(command))
        {
            addNewLine(commandIndex, -1, -1, command, NULL, 0, 0);
        }
        else if (isdigit(command[0]) || command[0] == '-') /// TODO: check if this option exist
        {
            addNewLine(commandIndex, -1, -1, NULL, NULL, tryGetNumber(command), 0);
        }
        else
        {
            /// TODO: handle error - there is no option for command
        }
    }
    else // in this part we r in the case of 2 vars
    {
        double immidiate1 = 0.5, immidiate2 = 0.5;
        char *label1 = NULL, *label2 = NULL;
        int register1 = -1, register2 = -1;

        char *firstVar = getSubstringBySeparator(command, VAR_SEPARATOR);
        command += strlen(firstVar) + 1; // + 1 for seperator ,

        if (firstVar[0] == '+' || firstVar[0] == '-' || isDigit(firstVar[0])) // validate NaN is a known word in c
            immidiate1 = tryGetNumber(firstVar);

        else if (isLabelExist(firstVar))
            label1 = firstVar;

        else if (isRegisterName(firstVar))
            register1 = firstVar[2] - '0';

        if (command[0] == '-' || isDigit(command[0])) // validate NaN is a known word in c
            immidiate2 = tryGetNumber(command);

        else if (isLabelExist(command))
            label2 = command;

        else if (isRegisterName(command))
            register2 = command[2] - '0';

        /// TODO: handle error unknown var

        addNewLine(commandIndex, register1, register2, label1, label2, immidiate1, immidiate2);
    }
}