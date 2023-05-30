#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "complex.h"

#define MAX_COMMAND_LENGTH 100
#define COMMANDS_NUMBER 16
#define COMMANDS_PREFIX_NUMBER 4
#define VAR_SEPERATOR ','
#define FLOAT_NUMBER_DOT '.'
#define REGISTER_PREFIX '@'

#define ERROR_EXTRANEOUS_TEXT "Extraneous text after end of command\n"
#define ERROR_MISSING_PARAMETER "Missing parameter\n"
#define ERROR_SECOND_PARAMETER_ISNT_NUMBER "Second parameter is not a number\n"
#define ERROR_THIRD_PARAMETER_ISNT_NUMBER "Third parameter is not a number\n"
#define ERROR_MISSING_COMMA "Illegal comma\n"
#define ERROR_MULTIPLE_CONSECUTIVE_COMMAS "Multiple consecutive commas\n"
#define ERROR_INVALID_COMMAND "Undefined command name\n"
#define ERROR_UNDEFINED_COMPLEX_VAR "Undefined complex variable\n"

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
    "endmcro"};

bool isMacroName(char command[]);

/* Function to get the command from the user */
void getCommand(char command[]);

/*
    Removes spaces and tabs from a string.
    Input: str - the string to remove spaces and tabs from.
    Output: None. The function modifies the input string in-place.
*/
void removeSpacesAndTabs(char *str);

/*
    Gets the index of a command in the commandsNames array.
    Input: command - the command to search for.
    Output: The index of the command if found, otherwise -1.
*/
int getCommandIndexByList(char command[], char *list[]);

/*
    Checks if a character represents a valid register name.
    Input: register - the character to check.
    Output: true if the character is a valid register name, false otherwise.
*/
bool isRegisterName(char registerName[]);

/*
    Skips a number in a command string and returns a pointer to the next character.
    Input: command - the command string.
    Output: A pointer to the next character after the skipped number.
*/
char *skipNumber(char *command);

/*
    Executes a command based on the input command string.
    Input: command - the command string to execute.
    Output: None.
*/
void parseCommand(char command[]);
