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

typedef struct commandsIdentifier commandsIdentifier;
typedef struct Line Line;

/**
 * Validates a given number to check if it lies within the permissible range.
 * The function checks if the provided number is within the range of -4095 to 4096 (inclusive).
 * @param number The integer value to be validated.
 * @return Returns `true` if the number is within the range, otherwise returns `false`.
 */
bool validateNumber(int number);

/**
 * Gets rid of leading spaces in a command string.
 * This prepares the command string for efficient parsing and processing.
 * @param command The raw command string that might start with spaces.
 */
void removePrefixSpaces(char *command);

/**
 * Given a command or a prefix, this function fetches its index from a predefined list.
 * This is useful to quickly identify what operation or instruction we're dealing with.
 * @param command The command or prefix to search for.
 * @param list The predefined list of commands or prefixes.
 * @param listLength The total number of commands or prefixes in the list.
 * @return The index of the command in the list, or -1 if not found.
 */
int getCommandIndexByList(char *command, char *list[], int listLength);

/**
 * Outputs the label details to the console for debugging.
 * This helps in tracking which labels are being processed or encountered errors.
 * @param filename The name of the assembly file currently being processed.
 */
void printLabel(char *filename);

/**
 * Checks if the given string matches the naming convention for a register in our assembly language.
 * Ensures that the register names used in the commands are valid.
 * @param registerName The string that we suspect to be a register name.
 * @return true if the string is a valid register name, false otherwise.
 */
bool isRegisterName(char registerName[], int lineNumber, char *fileName);

/**
 * Fetches the position of a specific character within a string.
 * This is primarily used to locate separators like ',' which help in parsing operands from a command.
 * @param str The string where we want to locate the separator.
 * @param separator The character we're looking for.
 * @return The position of the separator in the string, or -1 if not present.
 */
int getCharIndexBySeparator(char *str, char seperator);

/**
 * If present, extracts the label part of a command and adjusts the command pointer.
 * Labels are used as markers or destinations for certain commands.
 * @param command Pointer to the command string. This gets adjusted if a label is found.
 * @return The label if present, or NULL if there's no label in the command.
 */
char *tryGetLabel(char **command, char *fileName, int lineNumber);

/**
 * Attempts to interpret a part of the command string as a numeric operand.
 * Some commands have immediate numeric values as operands, and this helps fetch them.
 * @param str The string segment that might represent a number.
 * @return The numeric value if it's a valid number, or 0.5 as a placeholder for invalid numbers.
 */
double tryGetNumber(char *str, char *fileName, int lineNumber);

/**
 * Grabs a segment of the string up to a specified character.
 * This aids in splitting commands into their individual parts or operands.
 * @param str The main string from which we want to extract a segment.
 * @param separator The character marking the end of the segment.
 * @return The extracted segment as a new string.
 */
char *getSubstringBySeparator(char *str, char separator);

/**
 * Takes a raw command string, identifies its type, and does preliminary processing.
 * This is the first step in converting our assembly commands to machine code.
 * @param command The raw command string from the assembly file.
 * @param lineNumber The line number in the assembly file. Useful for error messages.
 * @param fileName The assembly file's name. Again, for error context.
 */
void startFirstRun(char command[], int lineNumber, char *fileName);

/**
 * Converts a string of comma-separated numbers into an array of integers.
 * This is typically used for commands that define a series of data.
 * @param input The raw string containing the numbers.
 * @param length A pointer to a size_t variable where the number of parsed integers will be stored.
 * @return A pointer to the first element of the dynamically allocated integer array.
 */
int *parseIntArray(char *input, size_t *length, char *fileName, int lineNumber);

/**
 * Predicts the number of machine code lines a command will be translated to.
 * Different assembly commands might result in multiple machine code instructions.
 * @param command The assembly command string.
 * @return An estimate of how many lines of machine code this will produce.
 */
int determineLinesNumber(char *command, int lineNumber, char *fileName);

/**
 * The main function that turns our assembly command into its machine code counterpart.
 * It identifies the command type, processes the operands, and adds the resulting code to our output.
 * @param command The assembly command string.
 * @param fileName The name of the assembly file, for error context.
 * @param lineNumber The line number in the assembly file, for error messages.
 */
void commandParser(char *command, char *fileName, int lineNumber);

#endif /* COMMANDSIDENTIFIER_H */
