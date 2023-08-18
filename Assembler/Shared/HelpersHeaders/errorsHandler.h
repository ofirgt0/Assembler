#ifndef ERRORSHANDLER_H
#define ERRORSHANDLER_H

#define REGISTER_PREFIX '@'

/**
 * Retrieves the current count of errors encountered during assembly file processing.
 * @return The total count of errors so far.
 */
int getErrorsCounter();

/**
 * Logs a new error to the global errorsCounter and prints the error details.
 * @param fileName Name of the assembly file where the error occurred.
 * @param lineNumber Line number in the file where the error was found.
 */
void logNewError(const char *fileName, int lineNumber);

/**
 * Logs an error for extraneous text after the end of a command.
 * @param fileName Name of the assembly file where the error occurred.
 * @param lineNumber Line number in the file where the error was found.
 */
#define EXTRANEOUS_TEXT_ERROR(fileName, lineNumber)       \
    do                                                    \
    {                                                     \
        logNewError(fileName, lineNumber);                \
        fprintf(stderr, "Extraneous text detected.\n\n"); \
    } while (0)

/**
 * Logs an error when having a trailing comma was found.
 * @param fileName Name of the assembly file where the error occurred.
 * @param lineNumber Line number in the file where the error was found.
 */
#define TRAILING_COMMA_ERROR(fileName, lineNumber)                 \
    do                                                             \
    {                                                              \
        logNewError(fileName, lineNumber);                         \
        fprintf(stderr, "Trailing comma has been detected. \n\n"); \
    } while (0)

/**
 * Logs an error when a multipule consecutive commas founded.
 * @param fileName Name of the assembly file where the error occurred.
 * @param lineNumber Line number in the file where the error was found.
 */
#define MULTIPLE_CONSECUTIVE_COMMAS_ERROR(fileName, lineNumber)                  \
    do                                                                           \
    {                                                                            \
        logNewError(fileName, lineNumber);                                       \
        fprintf(stderr, "Multipule consecutive commas has been detected. \n\n"); \
    } while (0)

/**
 * Logs an error when an invalid option for a command is found.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_OPTION_FOR_COMMAND(fileName, lineNumber)                                        \
    do                                                                                          \
    {                                                                                           \
        logNewError(fileName, lineNumber);                                                      \
        fprintf(stderr, "The command does not match the type of parameters that entered.\n\n"); \
    } while (0)

/**
 * Logs an error when an invalid file format is detected.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_FILE_FORMAT(fileName, lineNumber)                             \
    do                                                                        \
    {                                                                         \
        logNewError(fileName, lineNumber);                                    \
        fprintf(stderr, "Invalid file format, cant open/create a file.\n\n"); \
    } while (0)

/**
 * Logs an error when an invalid instruction is detected.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */

#define UNKNOWN_OPERAND(fileName, lineNumber)             \
    do                                                    \
    {                                                     \
        logNewError(fileName, lineNumber);                \
        fprintf(stderr, "Unknown operator founded.\n\n"); \
    } while (0)

/**
 * Logs an error when memory allocation fails.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define MEMORY_ALLOCATION_FAILED(fileName, lineNumber)             \
    do                                                             \
    {                                                              \
        logNewError(fileName, lineNumber);                         \
        fprintf(stderr, "Memory allocation has been failed.\n\n"); \
    } while (0)

/**
 * Logs an error when an invalid register name is used.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_REGISTER_NAME(fileName, lineNumber)                            \
    do                                                                         \
    {                                                                          \
        logNewError(fileName, lineNumber);                                     \
        fprintf(stderr, "The register that you tried to use is invalid.\n\n"); \
    } while (0)

/**
 * Logs an error when there's an issue opening or creating a file.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define OPENING_FILE_ERROR(fileName, lineNumber)                              \
    do                                                                        \
    {                                                                         \
        logNewError(fileName, lineNumber);                                    \
        fprintf(stderr, "Invalid file format, cant open/create a file.\n\n"); \
    } while (0)

/**
 * Logs an error when there's a conflict in macro names.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define MACRO_NAME_CONFLICT(fileName, lineNumber)                                          \
    do                                                                                     \
    {                                                                                      \
        logNewError(fileName, lineNumber);                                                 \
        fprintf(stderr, "There is a conflict in the macro name in the file you ran.\n\n"); \
    } while (0)

/**
 * Logs an error when an invalid command is detected.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_COMMAND_ERROR(fileName, lineNumber)                       \
    do                                                                    \
    {                                                                     \
        logNewError(fileName, lineNumber);                                \
        fprintf(stderr, "The command that in this line is invalid.\n\n"); \
    } while (0)

/**
 * Logs an error when a label was not found.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 * @param labelName The name of the missing label.
 */
#define LABEL_WAS_NOT_FOUND_ERROR(fileName, lineNumber, labelName)                                  \
    do                                                                                              \
    {                                                                                               \
        logNewError(fileName, lineNumber);                                                          \
        fprintf(stderr, "In the current line no matching label was found for: %s.\n\n", labelName); \
    } while (0)

/**
 * Logs an error when an invalid label format is detected. Labels must start with
 * capital letters and be immediately followed by a colon.
 * @param fileName The name of the file where the error occurred.
 * @param lineNumber The line number in the file where the error occurred.
 */
#define INVALID_LABEL_FORMAT(fileName, lineNumber, label)                   \
    do                                                                      \
    {                                                                       \
        logNewError(fileName, lineNumber);                                  \
        fprintf(stderr, "Invalid label format for label '%s'.\n\n", label); \
    } while (0)

/**
 * Logs an error when a label already exists.
 * @param fileName Name of the assembly file where the error occurred.
 * @param lineNumber Line number in the file where the error was found.
 * @param label The duplicate label name.
 */
#define LABEL_ALREADY_EXISTS_ERROR(fileName, lineNumber, label)                    \
    do                                                                             \
    {                                                                              \
        logNewError(fileName, lineNumber);                                         \
        fprintf(stderr, "The label '%s' in this line already exists.\n\n", label); \
    } while (0)

/**
 * Logs an error when an unknown command is detected in the assembly code.
 * @param fileName The name of the assembly file where the error occurred.
 * @param lineNumber The line number in the file where the error occurred.
 */
#define UNKNOWN_COMMAND_ERROR(fileName, lineNumber)                                                     \
    do                                                                                                  \
    {                                                                                                   \
        logNewError(fileName, lineNumber);                                                              \
        fprintf(stderr, "In file %s, at line %d: Unknown command detected.\n\n", fileName, lineNumber); \
    } while (0)

/**
 * Logs an error when an invalid parameter is passed to 'entry' or 'extern'.
 * @param fileName The name of the file where the error occurred.
 * @param lineNumber The line number in the file where the error occurred.
 * @param label The label or parameter that caused the error.
 */
#define INVALID_ENTRY_EXTERN_PARAM(fileName, lineNumber, label)                                           \
    do                                                                                                    \
    {                                                                                                     \
        logNewError(fileName, lineNumber);                                                                \
        fprintf(stderr, "Error: Invalid parameter '%s' for 'entry' or 'extern' instruction.\n\n", label); \
    } while (0)

/**
 * Logs an error when a line in the assembly file exceeds the allowable length.
 * @param fileName The name of the assembly file where the error occurred.
 * @param lineNumber The line number in the file where the error occurred.
 */
#define LINE_LENGTH_ERROR(fileName, lineNumber)                                         \
    do                                                                                  \
    {                                                                                   \
        logNewError(fileName, lineNumber);                                              \
        fprintf(stderr, "ERROR in file %s at line %d: Line exceeds 80 characters!\n\n", \
                fileName, lineNumber);                                                  \
    } while (0)

/**
 * Logs an error when an invalid number is detected in the assembly code.
 * @param fileName The name of the assembly file where the error occurred.
 * @param lineNumber The line number in the file where the error occurred.
 * @param token The string representation of the invalid number.
 */
#define INVALID_NUMBER_VALUE(fileName, lineNumber, token)                                                                           \
    do                                                                                                                              \
    {                                                                                                                               \
        logNewError(fileName, lineNumber);                                                                                          \
        fprintf(stderr, "Invalid number '%s'. This number does not match the numbers that you can use in this course.\n\n", token); \
    } while (0)

#endif /* ERRORSHANDLER_H */