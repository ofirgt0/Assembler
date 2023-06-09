#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define VAR_SEPERATOR ','
#define COMMANDS_NUMBER 16
#define REGISTER_PREFIX '@'
#define FLOAT_NUMBER_DOT '.'
#define MAX_COMMAND_LENGTH 100
#define COMMANDS_PREFIX_NUMBER 4

#define ERROR_MISSING_COMMA "Illegal comma\n"
#define ERROR_MISSING_PARAMETER "Missing parameter\n"
#define ERROR_INVALID_COMMAND "Undefined command name\n"
#define ERROR_UNDEFINED_COMPLEX_VAR "Undefined complex variable\n"
#define ERROR_EXTRANEOUS_TEXT "Extraneous text after end of command\n"
#define ERROR_MULTIPLE_CONSECUTIVE_COMMAS "Multiple consecutive commas\n"
#define ERROR_THIRD_PARAMETER_ISNT_NUMBER "Third parameter is not a number\n"
#define ERROR_SECOND_PARAMETER_ISNT_NUMBER "Second parameter is not a number\n"

char *commandsNames[COMMANDS_NUMBER] = {
    "mov", // 0
    "cmp", // 1
    "add", // 2
    "sub", // 3
    "not", // 4
    "clr", // 5
    "lea", // 6
    "inc", // 7
    "dec", // 8
    "jmp", // 9
    "bne",
    "red",
    "prn",
    "jsr",
    "rts",
    "stop", // 15
};

char *commandsPrefix[COMMANDS_PREFIX_NUMBER] = {
    ".extern",
    ".entry",
    "mcro",
    "endmcro"};

// isMacroName(char command[])
// {
//     // TODO: check with macro service if the command is a known macro
// }

/* Function to get the command from the user */
// void getCommand(char command[])
// {
//     printf("\nEnter command:\n");
//     if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
//     {
//         printf("\nexit the program\n");
//         exit(0);
//     }

//     printf("your command is:\n");
//     printf("%s", command);
// }

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
    {
        commandLength++;
    }
    for (commandIndex = 0; commandIndex < COMMANDS_NUMBER; commandIndex++)
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
void commandIdentifier(char command[])
{
    int commandIndex;
    commandIndex = getCommandIndexByList(command, commandsNames);
    if (commandIndex == -1)
    {
        if (isMacroName(command))
        {
            // TODO: send to the executer the lines of the macro
        }
        int commandPrefix = getCommandIndexByList(command, commandsPrefix);
        if (commandPrefix != -1)
        {
            switch (commandPrefix)
            {
                case 0: // extern
                    /* code */
                    break;
                case 1: // entry
                    /* code */
                    break;
                case 2: // mcro
                    /* code */
                    break;
                case 3: // endmcro
                    /* code */
                    break;
                default: // useless
                    break;
            }
        }
        printf(ERROR_INVALID_COMMAND);
        return;
    }
    else
    {
        // TODO: its a regular command without any prefix - send the command to the executer
    }
    removeSpacesAndTabs(command);
    command = command + strlen(commandsNames[commandIndex]);
}

struct Line commandParser(char *line, ){
    
}