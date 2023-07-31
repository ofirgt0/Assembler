#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "commandsIdentifier.h"
#include "macroService.h"
#include "dataService.h"
#include "filesReader.h" // to check if getting error while compiline because the commandsIndetifier in the filesRader -

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

/**
 * A thread-safe version of strtok.
 * This function works like the standard 'strtok' function but is reentrant and doesn't modify the delimiters string.
 * It returns a pointer to the next token in 'str' that is delimited by a character from 'delim'.
 * If there are no more tokens, it returns NULL.
 */
char *my_strtok_r(char *str, const char *delim, char **saveptr)
{
    char *token;
    if (str == NULL)
    {
        str = *saveptr;
    }
    str += strspn(str, delim);
    if (*str == '\0')
    {
        *saveptr = str;
        return NULL;
    }
    token = str;
    str = strpbrk(token, delim);
    if (str == NULL)
    {
        *saveptr = (char *)strchr(token, '\0');
    }
    else
    {
        *str = '\0';
        *saveptr = str + 1;
    }
    return token;
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

    for (commandIndex = 0; commandIndex < COMMANDS_NUMBER; commandIndex++) /*TODO: change to real list length*/
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

/**
 * Extracts the label from a command string.
 * If the label is found, it is removed from the command string.
 */
char *tryGetLabel(char *command)
{
    int index;
    char *label;
    index = getCharIndexBySeparator(command, LABEL_SEPERATOR);

    if (index == 0)
        return NULL;

    label = (char *)malloc((index + 1) * sizeof(char));
    strncpy(label, command, index - 1); /*-1 because we dont need the seperator inside the label name*/
    label[index + 1] = '\0';            /*Null-terminate the new string*/
    command += index;                   /*We dont need the label anymore*/

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
    return 0.0 / 0.0;
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
    int prefixIndex; /*TODO: check if we need to remove PREFIX SPACES*/

    if (macroFlag) /*if we in a macro - > count the lines*/
    {
        linesCounter++;
        return;
    }
    label = tryGetLabel(command); /*if label exist - we will get the label name and remove it from the command*/
    prefixIndex = getCommandIndexByList(command, commandsPrefix);

    if (label != NULL && strcmp(label, "") != 0) /*note in page 41*/
    {
        if (prefixIndex < 2)
        {
            /*TODO: warning*/
        }
        else
        {
            addNewLabel(label); /*regular label*/
        }
    }

    if (prefixIndex != -1)
    {
        char *secondVar;
        secondVar = command + strlen(commandsPrefix[prefixIndex]); /*in this part we have the label in the hand*/

        switch (prefixIndex)
        {
        case 0: /*extern*/
        {
            /*TODO: add warning for the option that label != null  as note in page 41*/
            addNewExtern(secondVar);
            return;
        }
        case 1: /*entry*/
        {
            addNewEntry(secondVar);
            return;
        }
        case 2: /*mcro*/
        {
            strcpy(secondVar, currentMacro);
            addMacro(secondVar, lineNumber);
            macroFlag = true;
            return;
        }
        case 3: /*endmcro*/
        {
            updateLinesCount(currentMacro, linesCounter);

            macroFlag = false;
            currentMacro = NULL;
            linesCounter = 0;
            return;
        }
        case 4: /*data*/
        {
            size_t length;
            int *data;
            data = parseIntArray(secondVar, &length);
            addData(data, secondVar);
            return;
        }
        case 5: /*string*/
        {

            return;
        }
        default: /*useless*/
            return;
        }
    }
    else
    {
        int linesNumber;
        linesNumber = determineLinesNumber(command);
        if (label != NULL)
            addNewLabel(label);
        increaseIC(linesNumber);
    }
}

/**
 * Parses an array of integers from a comma-separated string.
 * The function returns a dynamically allocated array of integers and stores the array size in 'length'.
 * If an error occurs during parsing, the function returns NULL and 'length' is undefined.
 */
int *parseIntArray(char *input, size_t *length)
{
    size_t count = 0;
    int *result = NULL;
    int *temp = NULL;

    char *inputCopy = strdup(input);
    char *token = NULL;
    char *endptr = NULL;
    int val;

    char *rest = inputCopy;

    while ((token = strtok_r(rest, ",", &rest)))
    {
        /*Handle consecutive commas*/
        if (strlen(token) == 0)
        {
            printf("Error: Invalid input format - consecutive commas\n");
            free(inputCopy);
            free(result);
            return NULL;
        }

        val = strtol(token, &endptr, 10);

        /*Handle non-integer characters*/
        if (*endptr != '\0')
        {
            printf("Error: Invalid input format - non-integer characters\n");
            free(inputCopy);
            free(result);
            return NULL;
        }

        /*Reallocate memory for the result array*/
        count++;
        temp = (int *)realloc(result, count * sizeof(int));
        if (!temp)
        {
            printf("Error: Memory allocation failed\n");
            free(inputCopy);
            free(result);
            return NULL;
        }
        result = temp;

        /*Store the parsed integer in the array*/
        result[count - 1] = val;
    }

    free(inputCopy);
    *length = count;
    return result;
}

/**
 * Determines the number of lines that a command will occupy in the machine code.
 * The function counts the number of operands and takes into account whether they are registers or not.
 */
int determineLinesNumber(char *command)
{
    char *firstVar;

    int commandIndex;
    commandIndex = getCommandIndexByList(command, commandsNames);

    if (commandIndex > 13) /*0 vars*/
        return 1;

    if (commandIndex > 3 && commandIndex != 6) /*one var*/
        return 2;

    else /* in this part we r in the case of 2 vars*/
    {
        command = command + strlen(commandsNames[commandIndex]);
        replaceMultipleSpaces(command);

        firstVar = getSubstringBySeparator(command, VAR_SEPARATOR);
        command += strlen(firstVar) + 1; /* + 1 for seperator ','*/

        if (isRegisterName(firstVar) && isRegisterName(command))
            return 2;
        return 3;
    }
}

/**
 * Handles the second run of the assembler.
 * The function parses each command and its operands and adds them to the code image.
 */
void commandParser(char *command, char *fileName)
{
    char *label = NULL;
    int prefixIndex;
    int commandIndex;
    double immidiate1 = 0.5, immidiate2 = 0.5;
    char *label1 = NULL, *label2 = NULL;
    int register1 = -1, register2 = -1;
    char *firstVar = NULL;

    label = tryGetLabel(command);
    command += strlen(label);
    /*TODO: check if we need to remove PREFIX SPACES*/
    prefixIndex = getCommandIndexByList(command, commandsPrefix);
    if (prefixIndex != -1)
        return; /*we handle this commands in the first run*/

    if (isMacroName(command))
    {
        sendMacro(command, fileName);
        return;
    }

    commandIndex = getCommandIndexByList(command, commandsNames);
    command = command + strlen(commandsNames[commandIndex]);
    replaceMultipleSpaces(command);

    if (commandIndex > 13) /*0 vars*/
    {
        if (strlen(command) > 0)
        {
            /*TODO: handle error*/
        }
        addNewLine(commandIndex, -1, -1, NULL, NULL, 0, 0); /* Note: -1 means that there is no register in this operand slot*/
    }
    else if (commandIndex < 14 && commandIndex > 3 && commandIndex != 6) /*one var*/
    {
        if (isRegisterName(command))
        {
            if (strlen(command) == 3)
            {
                addNewLine(commandIndex, command[2] - '0', -1, NULL, NULL, 0, 0); /*Note: -1 means that there is no register in this operand slot*/
            }
            else
            {
                /*TODO: handle error for char*/
            }
        }
        else if (isLabelExist(command))
        {
            addNewLine(commandIndex, -1, -1, command, NULL, 0, 0);
        }
        else if (isdigit(command[0]) || command[0] == '-') /*TODO: check if this option exist*/
        {
            addNewLine(commandIndex, -1, -1, NULL, NULL, tryGetNumber(command), 0);
        }
        else
        {
            /*TODO: handle error - there is no option for command*/
        }
    }
    else /* in this part we r in the case of 2 vars*/
    {
        firstVar = getSubstringBySeparator(command, VAR_SEPARATOR);
        command += strlen(firstVar) + 1; /*+ 1 for seperator ','*/

        if (firstVar[0] == '+' || firstVar[0] == '-' || isdigit(firstVar[0])) /* TODO: to check 'isdigit' should be ((unsigned char)firstVar[0])) ? */
            immidiate1 = tryGetNumber(firstVar);

        else if (isLabelExist(firstVar))
            label1 = firstVar;

        else if (isRegisterName(firstVar))
            register1 = firstVar[2] - '0';

        if (command[0] == '-' || isdigit(command[0])) /* TODO: to check 'isdigit' should be ((unsigned char)command[0]])) ? */
            immidiate2 = tryGetNumber(command);

        else if (isLabelExist(command))
            label2 = command;

        else if (isRegisterName(command))
            register2 = command[2] - '0';

        /*TODO: handle error unknown var*/

        addNewLine(commandIndex, register1, register2, label1, label2, immidiate1, immidiate2);
    }
}