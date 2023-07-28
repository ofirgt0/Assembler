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
#define COMMANDS_PREFIX_NUMBER 4
#define MACRO_COMMAND "mcro"
#define END_MACRO_COMMAND "endmcro"
#define DEFAULT_ADDRESS 0
#define ABSOLUTE_A_R_E_DECIMAL_CODE 0
#define EXTERNAL_A_R_E_DECIMAL_CODE 1
#define RELOCATABLE_A_R_E_DECIMAL_CODE 2

#define ERROR_MISSING_COMMA "Illegal comma\n"
#define ERROR_MISSING_PARAMETER "Missing parameter\n"
#define ERROR_INVALID_COMMAND "Undefined command name\n"
#define ERROR_UNDEFINED_COMPLEX_VAR "Undefined complex variable\n"
#define ERROR_EXTRANEOUS_TEXT "Extraneous text after end of command\n"
#define ERROR_MULTIPLE_CONSECUTIVE_COMMAS "Multiple consecutive commas\n"
#define ERROR_THIRD_PARAMETER_ISNT_NUMBER "Third parameter is not a number\n"
#define ERROR_SECOND_PARAMETER_ISNT_NUMBER "Second parameter is not a number\n"

struct Line // TODO: MOVE THIS STRUCT TO DATA STRUCTURE
{
    // char code; //A R E?
    int opcode;
    int dstRegister; // if exist
    int srcRegister; // if exist
    int address;
    Label *label;
    char *originalCommand; // useless - we dont gonna use this
    struct Line *next;
};

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
    "endmcro"};

static bool macroFlag = false;

isMacroName(char command[])
{
    // TODO: check with macro service if the command is a known macro
}

/* Function to get the command from the user */
void getCommand(char command[])
{
    printf("\nEnter command:\n");
    if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
    {
        printf("\nexit the program\n");
        exit(0);
    }

    printf("your command is:\n");
    printf("%s", command);
}

/*
    Removes spaces and tabs from a string.
    Input: str - the string to remove spaces and tabs from.
    Output: None. The function modifies the input string in-place.
*/
void removeSpacesAndTabs(char *str)
{
    int i, j;
    for (i = 0, j = 0; str[i]; i++)
    {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
} // useless?

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

/*
    Skips a number in a command string and returns a pointer to the next character.
    Input: command - the command string.
    Output: A pointer to the next character after the skipped number.
*/
char *skipNumber(char *command)
{
    int i = 0;
    int dotFlag = 0;
    if (command[0] == '-')
        i++;
    for (; isdigit(command[i]) || command[i] == FLOAT_NUMBER_DOT; i++)
    {
        if (command[i] == FLOAT_NUMBER_DOT)
        {
            if (dotFlag || !isdigit(command[i + 1]))
                return command + i;
            dotFlag = 1;
        }
    }
    return command + i;
}

/*
    Executes a command based on the input command string.
    Input: command - the command string to execute.
    Output: None.
*/
void commandIdentifier(char command[], char *fileName)
{
    if (isMacroName(command)) // TODO: think if we can do it outside of the big if
    {
        sendMacro(command); // TODO: WE CAN STOP HERE
        return;
    }

    int commandPrefix = getCommandIndexByList(command, commandsPrefix);
    if (commandPrefix != -1)
    {
        char *secondVar = command + getCharIndexBySeparatorIndex(command, 1); // TODO: rewrite this function
        switch (commandPrefix)
        {
        case 0: // extern
        {
            addNewExtern(secondVar, fileName); // TODO: in dataService
            return;
        }
        case 1: // entry
        {
            addNewEntry(secondVar, fileName); // TODO: in dataService
            return;
        }
        case 2: // mcro
        {
            bool isSuccess = addNewMacro(secondVar, fileName); // TODO: splite between creating macro and insert new line to the macro
            if (isSuccess)
                macroFlag = true;
            return;
        }
        case 3: // endmcro
        {
            macroFlag = false;
            return;
        }
        default: // useless
            return;
        }
    }

    char *label = tryGetLabel(command);

    int commandIndex;
    commandIndex = getCommandIndexByList(command, commandsNames);
    command = command + strlen(commandsNames[commandIndex]);

    replaceMultipleSpaces(command);
    struct Line *parsedLine = commandParser(command, commandIndex, label);
    macroFlag ? insertMacroNewLine(parsedLine) : insertNewCommand(parsedLine, fileName);

    if (commandIndex == -1)
    {
        printf(ERROR_INVALID_COMMAND);
        return;
    }
}

int getCharIndexBySeparatorIndex(const char *str, int sepIndex)
{
    char delimiters[] = " ,:"; // Delimiters: space, comma, colon
    int charIndex = 0;
    int separatorCount = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (strchr(delimiters, str[i]) != NULL)
        {
            separatorCount++;
            if (separatorCount > sepIndex)
                break;
        }
        else
            charIndex++;
    }

    if (separatorCount < sepIndex)
        charIndex = -1; // Separator index not found

    return charIndex;
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

char *cutString(const char *str, int startIndex, int endIndex)
{
    int strLength = strlen(str);
    int cutLength = endIndex - startIndex + 1;

    if (startIndex >= 0 && endIndex < strLength && startIndex <= endIndex)
    {
        char *result = malloc(cutLength + 1);
        strncpy(result, str + startIndex, cutLength);
        result[cutLength] = '\0';
        return result;
    }
    else
    {
        char *result = malloc(1);
        result[0] = '\0';
        return result;
    }
}

void *commandParser(char *line, int commandIndex, char *label)
{
    struct Line *newLine = (struct Line *)malloc(sizeof(struct Line *));

    if (newLine == NULL)
    {
        // Error: Memory allocation failed
        printf("Error: Memory allocation failed\n");
        return;
    }
    strcpy(newLine->originalCommand, line);
    if (commandIndex > 13) // 0 vars
    {
        if (strlen(line) > 0)
        {
            // TODO: handle error
        }
    }
    else if (commandIndex < 14 && commandIndex > 3 && commandIndex != 6) // one var
    {
        if (isRegisterName(line))
        {
        }
        else if (isLabelExist(line))
        { // from data service
        }
        else
        {
            /// TODO: handle error - there is no option for command
        }
    }
    else // in this part we r in the case of 2 vars
    {
        char *firstVar = getSubstringBySeperator(line, VAR_SEPARATOR);
        int firstRegisterNumber = tryGetNumber(firstVar);
        if (firstVar != NaN) // validate NaN is a known word in c
        {
        }
        else if (isLabelExist(line)) // check if the part after the command is register
        {
        }
        else
        {
            /// TODO: handle error unknown var
        }
    }

    return newLine;
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
    if (macroFlag)
    {
        linesCounter++;
        return;
    }
    char *label = tryGetLabel(command);
    int prefixIndex = getCommandIndexByList(command, commandsPrefix);

    if (prefixIndex > 1 && label != "") // note in page 41
    {
        tryAddNewLabel(label); // regular label
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
                addMacro(secondVar, lineNumber); // TODO: splite between creating macro and insert new line to the macro
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
            default: // useless
                return;
        }
    }
}

void *commandParser(char *command)
{
    char *label = tryGetLabel(command);
    command += strlen(label);

    if (isMacroName(command))
    {
        /// TODO: what should we do with label before macro?
        sendMacro(command);
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
        addNewLine5(label, commandIndex, -1, -1); /// Note: -1 means that there is no register in this operand slot
    }
    else if (commandIndex < 14 && commandIndex > 3 && commandIndex != 6) // one var
    {
        if (isRegisterName(command))
        { // if
            if (strlen(command) == 3)
            {
                addNewLine5(label, commandIndex, command[2], -1);
            }
            else
            {
                /// TODO: handle error for char
            }
        }
        else if (isLabelExist(command))
        { // from data service
            addNewLine3(label, commandIndex, command, -1);
        }
        else
        {
            /// TODO: handle error - there is no option for command
        }
    }
    else // in this part we r in the case of 2 vars
    {
        char *firstVar = getSubstringBySeparator(command, VAR_SEPARATOR);
        command += strlen(firstVar) + 1; // +1 for seperator ,

        if (firstVar[0] == '-' || isDigit(firstVar[0])) // validate NaN is a known word in c
        {
            int firstRegisterNumber = tryGetNumber(firstVar);
            addNewLine1(label, commandIndex, firstRegisterNumber, command[2]);
        }
        else if (isLabelExist(firstVar)) // check if the part after the command is register
        {
            if (isRegisterName(command))
            {
                addNewLine3(label, commandIndex, firstVar, command[2]);
            }
        }
        else if (isRegisterName(firstVar))
        {
            if (isRegisterName(command))
            {
                addNewLine3(label, commandIndex, firstVar, command);
                addNewLine(commandIndex, firstVar, command, NULL, NULL, 0, 0);
            }
        }
        else
        {
            /// TODO: handle error unknown var
        }
    }
}