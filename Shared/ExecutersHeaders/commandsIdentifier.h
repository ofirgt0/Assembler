#ifndef COMMANDS_IDENTIFIER_H
#define COMMANDS_IDENTIFIER_H

#include <stdbool.h>

/* Forward declaration of Label * /
    struct Label;

/* Forward declaration of Line */
struct Line;

extern char *commandsNames[]; // because the using by the encoder ?

/**
 * Check if the given command is a known macro.
 * @param command The command to check.
 * @return True if the command is a known macro, false otherwise.

bool isMacroName(char command[]);*/

/**
 * Function: getCommand
 * --------------------
 *   Requests a command from the user.
 *   @param command: A pointer to a character buffer where the user command will be stored.
 *   @returns: void
 */
void getCommand(char command[]);

/**
 * Function: removeSpacesAndTabs
 * -----------------------------
 *   Removes spaces and tabs from a string.
 *   @param str: The string to remove spaces and tabs from.
 *   @returns: void
 */
void removeSpacesAndTabs(char *str);

/**
 * Function: replaceMultipleSpaces
 * -------------------------------
 *   Replaces multiple consecutive spaces in a string with a single space.
 *   @param str: The string to process.
 *   @returns: void
 */
void replaceMultipleSpaces(char *str);

/**
 * Function: getCommandIndexByList
 * -------------------------------
 *   Gets the index of a command in the commandsNames array.
 *   @param command: The command to search for.
 *   @param list: A pointer to the list of command names.
 *   @returns: The index of the command if found, otherwise -1.
 */
int getCommandIndexByList(char command[], char *list[]);

/**
 * Function: isRegisterName
 * ------------------------
 *   Checks if a character represents a valid register name.
 *   @param registerName: The character to check.
 *   @returns: true if the character is a valid register name, false otherwise.
 */
bool isRegisterName(char registerName[]);

/**
 * Function: skipNumber
 * --------------------
 *   Skips a number in a command string and returns a pointer to the next character.
 *   @param command: The command string.
 *   @returns: A pointer to the next character after the skipped number.
 */
char *skipNumber(char *command);

/**
 * Function: commandIdentifier
 * ---------------------------
 *   Executes a command based on the input command string.
 *   @param command: The command string to execute.
 *   @param fileName: The name of the file that contains the command.
 *   @returns: void
 */
void commandIdentifier(char command[], char *fileName);

/**
 * Function: getCharIndexBySeparatorIndex
 * --------------------------------------
 *   Returns the index of a character following a given separator in a string.
 *   @param str: The string to search.
 *   @param sepIndex: The separator index to find.
 *   @returns: The character index if the separator index is found, otherwise -1.
 */
int getCharIndexBySeparatorIndex(const char *str, int sepIndex);

/**
 * Function: tryGetLabel
 * ---------------------
 *   Tries to get a label from a command string.
 *   @param command: The command string.
 *   @returns: A pointer to the label if it exists, otherwise NULL.
 */
char *tryGetLabel(char *command);

/**
 * Function: cutString
 * -------------------
 *   Creates a new string that is a substring of an existing string.
 *   @param str: The original string.
 *   @param startIndex: The start index of the substring in the original string.
 *   @param endIndex: The end index of the substring in the original string.
 *   @returns: A pointer to the new string. The caller is responsible for freeing this string.
 */
char *cutString(const char *str, int startIndex, int endIndex);

/**
 * Function: commandParser
 * -----------------------
 *   Parses a line of command.
 *   @param line: The line to parse.
 *   @param commandIndex: The index of the command in the commandsNames array.
 *   @param label: The label associated with the command.
 *   @returns: A pointer to a Line structure representing the parsed line.
 */
struct Line *commandParser(char *line, int commandIndex, char *label);

/**
 * Function: tryGetNumber
 * ----------------------
 *   Tries to convert a string to a number.
 *   @param str: The string to convert.
 *   @returns: The converted number, or NaN if the conversion failed.
 */
double tryGetNumber(char *str);

/**
 * Function: getSubstringBySeparator
 * ---------------------------------
 *   Gets a substring of a string up to a given separator.
 *   @param str: The original string.
 *   @param separator: The separator to find.
 *   @returns: A pointer to the new substring. The caller is responsible for freeing this string.
 */
char *getSubstringBySeparator(char *str, char separator);

/**
 * Function: startFirstRun
 * -----------------------
 *   Begins the first run of a command string.
 *   @param command: The command string to process.
 *   @param lineNumber: The line number of the command in the file.
 *   @param fileName: The name of the file that contains the command.
 *   @returns: void
 */
void startFirstRun(char command[], int lineNumber, char *fileName);

#endif /* COMMANDSIDENTIFIER_H */
