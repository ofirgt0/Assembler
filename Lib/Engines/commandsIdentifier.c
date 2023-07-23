#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "commandsIdentifier.h"

#define VAR_SEPERATOR ','
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

struct Line //TODO: MOVE THIS STRUCT TO DATA STRUCTURE
{ 
    char code;
    int opcode;
    int dstRegister;
    int srcRegister;
    int address;
    char *label;
    char *originalCommand;
    struct Line *next;
};


char *commandsNames[COMMANDS_NUMBER] = {
    "mov", // 1
    "cmp", // 2
    "add", // 3
    "sub", // 4
    "not", // 5
    "clr", // 6
    "lea",
    "inc",
    "dec",
    "jmp",
    "bne",
    "red",
    "prn",
    "jsr",
    "rts",
    "stop", // 16
};

char *commandsPrefix[COMMANDS_PREFIX_NUMBER] = {
    ".extern",
    ".entry",
    "mcro",
    "endmcro"
    };

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
}//useless?

void replaceMultipleSpaces(char* str) {
    int i, j;
    int whitespaceCount = 0;
    int len = strlen(str);

    for (i = 0, j = 0; i < len; i++) {
        if (str[i] == ' ' || str[i] == '\t') 
            whitespaceCount++;
        else 
            whitespaceCount = 0;

        if (whitespaceCount <= 1) {
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

    while (command[commandLength] != ' ' && command[commandLength] != '\t' && command[commandLength] != VAR_SEPERATOR && command[commandLength] != '\0')
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
    int commandIndex;
    char* label = tryGetLabel(command);
    commandIndex = getCommandIndexByList(command, commandsNames);
    command = command + strlen(commandsNames[commandIndex]);
    replaceMultipleSpaces(command);

    if (commandIndex == -1 && !macroFlag) // TODO: make sure that there is no option for label or extern inside a macro
    {
        if (isMacroName(command))// TODO: think if we can do it outside of the big if
            sendMacro(command); //TODO: WE CAN STOP HERE

        if (strncmp(END_MACRO_COMMAND, command, strlen(END_MACRO_COMMAND)))
            macroFlag = false;

        int commandPrefix = getCommandIndexByList(command, commandsPrefix);
        if (commandPrefix != -1)
        {
            char *secondVar = command + getCharIndexBySeparatorIndex(command, 1);
            switch (commandPrefix)
            {
            case 0: // extern
            {
                addNewExtern(secondVar, fileName);
                break;
            }
            case 1: // entry
            {
                addNewEntry(secondVar, fileName);
                break;
            }
            case 2: // mcro
            {
                bool isSuccess = addNewMacro(secondVar, fileName); // TODO: splite between creating macro and insert new line to the macro
                if (isSuccess)
                    macroFlag = true;
                break;
            }
            default: // useless
                break;
            }
        }
        else
        {
            printf(ERROR_INVALID_COMMAND);
            return;
        }
    }
    else // its a regular command (without prefix)
    {
        if(commandIndex == -1)
        {
            printf(ERROR_INVALID_COMMAND);
            return;
        }
        //removeSpacesAndTabs(command);
        //command = command + strlen(commandsNames[commandIndex]);
        
        struct Line *parsedLine = commandParser(command, commandIndex, label);
        macroFlag ? insertMacroNewLine(parsedLine) : insertNewCommand(parsedLine, fileName);
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

char* tryGetLabel(char* command){
    int index = getCharIndexBySeparator(command, LABEL_SEPERATOR);

    if(index == 0)
        return NULL;

    char* label = (char*) malloc((index + 1) * sizeof(char));
    strncpy(label, command, index - 1); // -1 because we dont need the seperator inside the label name
    label[index + 1] = '\0'; // Null-terminate the new string
    command += index; // We dont need the label anymore

    return label;
}

char* cutString(const char* str, int startIndex, int endIndex) {
    int strLength = strlen(str);
    int cutLength = endIndex - startIndex + 1;

    if (startIndex >= 0 && endIndex < strLength && startIndex <= endIndex) {
        char* result = malloc(cutLength + 1);
        strncpy(result, str + startIndex, cutLength);
        result[cutLength] = '\0';
        return result;
    } else {
        char* result = malloc(1);
        result[0] = '\0';
        return result;
    }
}


struct Line* commandParser(char *line, int commandIndex, char* label, int AREcode)
{
    struct Line *newLine = (struct Line *)malloc(sizeof(struct Line));

    if (newLine == NULL)
    {
        // Error: Memory allocation failed
        printf("Error: Memory allocation failed\n");
        return;
    }
    strcpy(newLine->originalCommand, line);
    newLine->code = ABSOLUTE_A_R_E_DECIMAL_CODE; // TODO: understand the meaning of the A R E code (!!!)
    newLine->address = DEFAULT_ADDRESS;
    newLine->opcode = commandIndex;

    int startOfRegister1 = strlen(commandsNames[commandIndex]) + 1;
    
    cutString(line, startOfRegister1, startOfRegister1 + 3);
    newLine->dstRegister = line.dstRegister;
    newLine->srcRegister = line.srcRegister;
    newLine->label = label;
    newLine->next = NULL;
    return newLine;
}