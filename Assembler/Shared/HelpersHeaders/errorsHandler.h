#ifndef ERRORSHANDLER_H
#define ERRORSHANDLER_H

#define ERROR_MISSING_COMMA "Illegal comma\n"
#define ERROR_MISSING_PARAMETER "Missing parameter\n"
#define ERROR_INVALID_COMMAND "Undefined command name\n"
#define ERROR_UNDEFINED_COMPLEX_VAR "Undefined complex variable\n"
#define ERROR_EXTRANEOUS_TEXT "Extraneous text after end of command\n"
#define ERROR_MULTIPLE_CONSECUTIVE_COMMAS "Multiple consecutive commas\n"
#define ERROR_THIRD_PARAMETER_ISNT_NUMBER "Third parameter is not a number\n"
#define ERROR_SECOND_PARAMETER_ISNT_NUMBER "Second parameter is not a number\n"

extern int errorsCounter;

/* Prints the error log prefix.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
void printLogPrefix(const char *fileName, int address);

/* Retrieves the total count of errors.
 * @return The count of errors.
 */
int getErrorsCounter();

/* Macro for reporting an error when there is extraneous text after the end of a command.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define EXTRANEOUS_TEXT(fileName, address)                         \
    do                                                             \
    {                                                              \
        errorsCounter++;                                           \
        printLogPrefix(fileName, address);                         \
        fprintf(stderr, "Extraneous text after end of command\n"); \
        return;                                                    \
    } while (0)

/* Macro for reporting an error when a parameter is missing.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define MISSING_PARAMETER(fileName, address)    \
    do                                          \
    {                                           \
        errorsCounter++;                        \
        printLogPrefix(fileName, address);      \
        fprintf(stderr, "Missing parameter\n"); \
        return;                                 \
    } while (0)

/* Macro for reporting an error when a comma is missing.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define MISSING_COMMA(fileName, address)    \
    do                                      \
    {                                       \
        errorsCounter++;                    \
        printLogPrefix(fileName, address);  \
        fprintf(stderr, "Missing comma\n"); \
        return;                             \
    } while (0)

/* Macro for reporting an error when there are multiple consecutive commas.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define MULTIPLE_CONSECUTIVE_COMMAS(fileName, address)    \
    do                                                    \
    {                                                     \
        errorsCounter++;                                  \
        printLogPrefix(fileName, address);                \
        fprintf(stderr, "Multiple consecutive commas\n"); \
        return;                                           \
    } while (0)

/* Macro for reporting an error when a label already exists.
 * @param labelName The name of the existing label.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define EXISTING_LABEL(labelName, fileName, address)               \
    do                                                             \
    {                                                              \
        errorsCounter++;                                           \
        printLogPrefix(fileName, address);                         \
        fprintf(stderr, "Label '%s' already exists\n", labelName); \
        return;                                                    \
    } while (0)

/* Macro for reporting an error when a label is not found.
 * @param fileName The name of the file where the error occurred.
 * @param address The address (usually a line number) at which the error occurred.
 * @param labelName The name of the label that was not found.
 */
#define LABEL_NOT_FOUND_ERROR(fileName, address, labelName)   \
    do                                                        \
    {                                                         \
        errorsCounter++;                                      \
        printLogPrefix(fileName, address);                    \
        fprintf(stderr, "Label '%s' not found\n", labelName); \
        return;                                               \
    } while (0)

/* Macro for reporting a memory allocation failure.
 * @param fileName The name of the file where the error occurred.
 * @param address The address (usually a line number) at which the error occurred.
 * @param returnValue The value that the function should return in the event of a failure.
 */
#define MEMORY_ALLOCATION_FAILED(fileName, address, returnValue) \
    do                                                           \
    {                                                            \
        errorsCounter++;                                         \
        printLogPrefix(fileName, address);                       \
        fprintf(stderr, "Memory allocation failed\n");           \
        return returnValue;                                      \
    } while (0)

/* Macro for reporting a memory allocation failure for void functions.
 * @param fileName The name of the file where the error occurred.
 * @param address The address (usually a line number) at which the error occurred.
 */
#define MEMORY_ALLOCATION_FAILED_FOR_VOID(fileName, address) \
    do                                                       \
    {                                                        \
        errorsCounter++;                                     \
        printLogPrefix(fileName, address);                   \
        fprintf(stderr, "Memory allocation failed\n");       \
        return;                                              \
    } while (0)

/**
 * This macro increments the global error counter, prints an error log prefix
 * with the provided file name and address, and then reports the error message, to the standard error stream.
 * @param fileName The name of the source file where the error occurred.
 * @param address The line number in the source file where the error occurred.
 */
#define FILE_OPEN_ERROR(fileName, address)                   \
    do                                                       \
    {                                                        \
        errorsCounter++;                                     \
        printLogPrefix(fileName, address);                   \
        fprintf(stderr, "Failed to open/create the file\n"); \
        return;                                              \
    } while (0)

/**
 * This macro increments the global error counter, prints an error log prefix
 * with the provided file name and address, and then reports the error message, to the standard error stream.
 * @param fileName The name of the source file where the error occurred.
 * @param address The line number in the source file where the error occurred.
 * @return false if can not open/create the file.
 */
#define FILE_OPEN_ERROR_BOOL(fileName, address)              \
    do                                                       \
    {                                                        \
        errorsCounter++;                                     \
        printLogPrefix(fileName, address);                   \
        fprintf(stderr, "Failed to open/create the file\n"); \
        return false;                                        \
    } while (0)

/**
 * This macro increments the global error counter, prints an error log prefix
 * with the provided file name and address, and then reports the error message, to the standard error stream.
 * @param fileName The name of the source file where the error occurred.
 * @param address The line number in the source file where the error occurred.
 */
#define INVALID_FILE_FORMAT(fileName, address)    \
    do                                            \
    {                                             \
        errorsCounter++;                          \
        printLogPrefix(fileName, address);        \
        fprintf(stderr, "Invalid file format\n"); \
        return;                                   \
    } while (0)

/**
 * This macro increments the global error counter, prints an error log prefix
 * with the provided file name and address, and then reports the error message, to the standard error stream.
 * @param fileName The name of the source file where the error occurred.
 * @param address The line number in the source file where the error occurred.
 */
#define MACRO_NAME_CONFLICT(fileName, address)    \
    do                                            \
    {                                             \
        errorsCounter++;                          \
        printLogPrefix(fileName, address);        \
        fprintf(stderr, "Macro name conflict\n"); \
        return;                                   \
    } while (0)

/**
 * This macro increments the global error counter, prints an error log prefix
 * with the provided file name and address, and then reports the error message, to the standard error stream.
 * @param fileName The name of the source file where the error occurred.
 * @param address The line number in the source file where the error occurred.
 */
#define INVALID_COMMAND_ERROR(fileName, address)    \
    do                                              \
    {                                               \
        errorsCounter++;                            \
        printLogPrefix(fileName, address);          \
        fprintf(stderr, "Invalid command found\n"); \
        return;                                     \
    } while (0)

/**
 * This macro reports an error when an invalid ARE code is encountered.
 * @param fileName The name of the file where the error occurred.
 * @param address The address (usually a line number) at which the error occurred.
 */
#define INVALID_ARE_CODE(fileName, address)            \
    do                                                 \
    {                                                  \
        errorsCounter++;                               \
        printLogPrefix(fileName, address);             \
        fprintf(stderr, "Error: Invalid ARE code.\n"); \
        return;                                        \
    } while (0)

/**
 * This macro reports an error when an invalid register name is encountered.
 * @param fileName The name of the file where the error occurred.
 * @param address The address (usually a line number) at which the error occurred.
 * @param returnValue The value that the function should return in the event of a failure.
 */
#define INVALID_REGISTER_NAME(fileName, address, returnValue) \
    do                                                        \
    {                                                         \
        printLogPrefix(fileName, address);                    \
        fprintf(stderr, "Invalid register name\n");           \
        return returnValue;                                   \
    } while (0)

/**
 * This macro reports an error when an invalid instruction is provided.
 * @param fileName The name of the file where the error occurred.
 * @param address The address (usually a line number) at which the error occurred.
 */
#define INVALID_INSTRUCTION(fileName, address)             \
    do                                                     \
    {                                                      \
        printLogPrefix(fileName, address);                 \
        fprintf(stderr, "Invalid instruction provided\n"); \
        return;                                            \
    } while (0)

/**
 * This macro reports an error when an invalid option is provided for a command.
 * @param fileName The name of the file where the error occurred.
 * @param address The address (usually a line number) at which the error occurred.
 */
#define INVALID_OPTION_FOR_COMMAND(fileName, address)    \
    do                                                   \
    {                                                    \
        printLogPrefix(fileName, address);               \
        fprintf(stderr, "Invalid option for command\n"); \
        return;                                          \
    } while (0)

/**
 * This macro increments the global error counter, prints an error log prefix
 * with the provided file name and address, and then reports the error message, to the standard error stream.
 * @param fileName The name of the source file where the error occurred.
 * @param address The line number in the source file where the error occurred.
 */
#define INCORRECT_OPERANDS_ERROR(fileName, address)                 \
    do                                                              \
    {                                                               \
        errorsCounter++;                                            \
        printLogPrefix(fileName, address);                          \
        fprintf(stderr, "Incorrect number of operands provided\n"); \
        return;                                                     \
    } while (0)

#endif /* ERRORSHANDLER_H */