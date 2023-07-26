#ifndef COMMANDS_IDENTIFIER_H
#define COMMANDS_IDENTIFIER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_COMMAND_LENGTH 100
#define COMMANDS_NUMBER 16
#define COMMANDS_PREFIX_NUMBER 4
#define VAR_SEPERATOR ','
#define FLOAT_NUMBER_DOT '.'
#define REGISTER_PREFIX '@'

#define ERROR_MISSING_COMMA "Illegal comma\n"
#define ERROR_MISSING_PARAMETER "Missing parameter\n"
#define ERROR_INVALID_COMMAND "Undefined command name\n"
#define ERROR_UNDEFINED_COMPLEX_VAR "Undefined complex variable\n"
#define ERROR_EXTRANEOUS_TEXT "Extraneous text after end of command\n"
#define ERROR_MULTIPLE_CONSECUTIVE_COMMAS "Multiple consecutive commas\n"
#define ERROR_THIRD_PARAMETER_ISNT_NUMBER "Third parameter is not a number\n"
#define ERROR_SECOND_PARAMETER_ISNT_NUMBER "Second parameter is not a number\n"

extern char *commandsNames[];

/**
 * Check if the given command is a known macro.
 * @param command The command to check.
 * @return True if the command is a known macro, false otherwise.
 */
bool isMacroName(char command[]);

/**
 * Get the command from the user.
 * @param command The buffer to store the command.
 */
void getCommand(char command[]);

/**
 * Remove spaces and tabs from a string.
 * @param str The string to remove spaces and tabs from.
 */
void removeSpacesAndTabs(char *str);

/**
 * Get the index of a command in the commandsNames array.
 * @param command The command to search for.
 * @param list The list of commands to search in.
 * @return The index of the command if found, otherwise -1.
 */
int getCommandIndexByList(char command[], char *list[]);

/**
 * Check if a character represents a valid register name.
 * @param registerName The character to check.
 * @return True if the character is a valid register name, false otherwise.
 */
bool isRegisterName(char registerName[]);

/**
 * Skip a number in a command string and return a pointer to the next character.
 * @param command The command string.
 * @return A pointer to the next character after the skipped number.
 */
char *skipNumber(char *command);

/**
 * Identify and execute a command based on the input command string.
 * @param command The command string to execute.
 */
void commandIdentifier(char command[]);

#endif /* COMMANDS_IDENTIFIER_H */
