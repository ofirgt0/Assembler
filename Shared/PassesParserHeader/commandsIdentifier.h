#ifndef COMMANDS_IDENTIFIER_H
#define COMMANDS_IDENTIFIER_H

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

/* Forward declaration of the opaque struct. */
typedef struct commandsIdentifier commandsIdentifier;
typedef struct Line Line; /*Added this line*/

/**
 * Replace multiple consecutive spaces in a string with a single space.
 * @param str the input string.
 */
void replaceMultipleSpaces(char *str);

/**
 * This function removes leading white space characters from a given string.
 * The function iterates over the string, identifies the leading spaces, and then
 * shifts the remaining characters to the start of the string, effectively removing the spaces.
 * The string is then null-terminated at the end to ensure it's still a valid C-string.
 * @param command The input string from which leading spaces will be removed. This string is modified in-place.
 */
void removePrefixSpaces(char *command);

/**
 * Get the index of a command in the commandsNames array.
 * @param command the command to search for.
 * @param list the list to be searched.
 * @return The index of the command if found, otherwise -1.
 */
int getCommandIndexByList(char command[], char *list[]);

/**
 * Check if a character represents a valid register name.
 * @param registerName the character to check.
 * @return true if the character is a valid register name, false otherwise.
 */
bool isRegisterName(char registerName[]);

/**
 * Get the index of a character in a string by a separator.
 * @param str the string to be searched.
 * @param seperator the separator character.
 * @return the index of the character if found, 0 otherwise.
 */
int getCharIndexBySeparator(char *str, char seperator);

/**
 * Get a label from a command string.
 * @param command the command string.
 * @return a string of the label if found, NULL otherwise.
 */
char *tryGetLabel(char *command);

/**
 * Parse a string into a number.
 * @param str the string to be parsed.
 * @return the parsed number if successful, NaN (Not a Number) otherwise.
 */
double tryGetNumber(char *str);

/**
 * Get a substring from a string up to a separator.
 * @param str the input string.
 * @param separator the separator character.
 * @return a substring up to the separator if found, NULL otherwise.
 */
char *getSubstringBySeparator(char *str, char separator);

/**
 * Start the first run of the command parsing process.
 * @param command the command string.
 * @param lineNumber the line number of the command in the input file.
 * @param fileName the name of the input file.
 */
void startFirstRun(char command[], int lineNumber, char *fileName);

/**
 * Parse a string into an array of integers.
 * @param input the input string.
 * @param length a pointer to an integer where the length of the parsed array will be stored.
 * @return a pointer to the first element of the array if successful, NULL otherwise.
 */
int *parseIntArray(char *input, size_t *length);

/**
 * Determine the number of lines a command will take in the compiled output.
 * @param command the command string.
 * @return the number of lines the command will take.
 */
int determineLinesNumber(char *command);

/**
 * Parse a command and perform the necessary actions based on the command type.
 * @param command the command string.
 * @param fileName the name of the input file.
 */
void commandParser(char *command, char *fileName);

#endif /* COMMANDSIDENTIFIER_H */
