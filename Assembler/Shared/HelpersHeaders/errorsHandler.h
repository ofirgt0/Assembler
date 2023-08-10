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

/* Retrieves the total count of errors.
 * @return The count of errors.
 */
int getErrorsCounter();

void logNewError(const char *fileName, int lineNumber);

/* Macro for reporting an error when there is extraneous text after the end of a command.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define EXTRANEOUS_TEXT(fileName, lineNumber)                      \
    do                                                             \
    {                                                              \
        errorsCounter++;                                           \
        printLogPrefix(fileName, lineNumber);                      \
        fprintf(stderr, "Extraneous text after end of command\n"); \
        return;                                                    \
    } while (0)

/* Macro for reporting an error when a parameter is missing.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define MISSING_PARAMETER(fileName, lineNumber) \
    do                                          \
    {                                           \
        errorsCounter++;                        \
        printLogPrefix(fileName, lineNumber);   \
        fprintf(stderr, "Missing parameter\n"); \
        return;                                 \
    } while (0)

/* Macro for reporting an error when a comma is missing.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define MISSING_COMMA(fileName, lineNumber)   \
    do                                        \
    {                                         \
        errorsCounter++;                      \
        printLogPrefix(fileName, lineNumber); \
        fprintf(stderr, "Missing comma\n");   \
        return;                               \
    } while (0)

/* Macro for reporting an error when there are multiple consecutive commas.*/
#define TRAILING_COMMA_ERROR(fileName, lineNumber)                            \
    do                                                                        \
    {                                                                         \
        logNewError(fileName, lineNumber);                                    \
        fprintf(stderr, "Error in %s at line %d: Trailing comma detected.\n", \
                fileName, lineNumber);                                        \
    } while (0)

#define INVALID_FLOAT_ERROR(fileName, lineNumber, token)                                 \
    do                                                                                   \
    {                                                                                    \
        logNewError(fileName, lineNumber);                                               \
        fprintf(stderr, "Error in %s at line %d: Invalid float value encountered: %s\n", \
                fileName, lineNumber, token);                                            \
    } while (0)

#define INVALID_CHAR_ERROR(fileName, lineNumber, token)                                   \
    do                                                                                    \
    {                                                                                     \
        logNewError(fileName, lineNumber);                                                \
        fprintf(stderr, "Error in %s at line %d: Invalid character encountered in: %s\n", \
                fileName, lineNumber, token);                                             \
    } while (0)

/**
 * Logs an error when an invalid option for a command is found.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_OPTION_FOR_COMMAND(fileName, lineNumber)                                                                              \
    do                                                                                                                                \
    {                                                                                                                                 \
        logNewError(fileName, lineNumber);                                                                                            \
        fprintf(stderr, "This line has been failed according to: The command does not match the type of parameters that entered.\n"); \
    } while (0)

/**
 * Logs an error when an invalid file format is detected.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_FILE_FORMAT(fileName, lineNumber)                                                                   \
    do                                                                                                              \
    {                                                                                                               \
        logNewError(fileName, lineNumber);                                                                          \
        fprintf(stderr, "This line has been failed according to: Invalid file format, cant open/create a file.\n"); \
    } while (0)

/**
 * Logs an error when an invalid instruction is detected.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_INSTRUCTION(fileName, lineNumber)                                                      \
    do                                                                                                 \
    {                                                                                                  \
        logNewError(fileName, lineNumber);                                                             \
        fprintf(stderr, "This line has been failed according to: There is an invalid instruction.\n"); \
    } while (0)

/**
 * Logs an error when memory allocation fails.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define MEMORY_ALLOCATION_FAILED(fileName, lineNumber)                                                   \
    do                                                                                                   \
    {                                                                                                    \
        logNewError(fileName, lineNumber);                                                               \
        fprintf(stderr, "This line has been failed according to: Memory allocation has been failed.\n"); \
    } while (0)

/**
 * Logs an error when the number of operands is incorrect.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INCORRECT_OPERANDS_NUMBER_ERROR(fileName, lineNumber)                                                                                      \
    do                                                                                                                                             \
    {                                                                                                                                              \
        logNewError(fileName, lineNumber);                                                                                                         \
        fprintf(stderr, "This line has been failed according to: The number of operands that in this line, corresponds to the type of operand\n"); \
    } while (0)

/**
 * Logs an error when an invalid register name is used.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_REGISTER_NAME(fileName, lineNumber)                                                                  \
    do                                                                                                               \
    {                                                                                                                \
        logNewError(fileName, lineNumber);                                                                           \
        fprintf(stderr, "This line has been failed according to: The register that you tried to use is invalid.\n"); \
    } while (0)

/**
 * Logs an error when there's an issue opening or creating a file.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define OPENING_FILE_ERROR(fileName, lineNumber)                                                                    \
    do                                                                                                              \
    {                                                                                                               \
        logNewError(fileName, lineNumber);                                                                          \
        fprintf(stderr, "This line has been failed according to: Invalid file format, cant open/create a file.\n"); \
    } while (0)

/**
 * Logs an error when there's a conflict in macro names.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define MACRO_NAME_CONFLICT(fileName, lineNumber)                                                                                \
    do                                                                                                                           \
    {                                                                                                                            \
        logNewError(fileName, lineNumber);                                                                                       \
        fprintf(stderr, "This line has been failed according to: There is a conflict in the macro name in the file you ran.\n"); \
    } while (0)

/**
 * Logs an error when an invalid 'ARE' code is detected.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_ARE_CODE(fileName, lineNumber)                                                                                      \
    do                                                                                                                              \
    {                                                                                                                               \
        logNewError(fileName, lineNumber);                                                                                          \
        fprintf(stderr, "This line has been failed according to: There is an invalid 'ARE' code  in the line you tried to ran.\n"); \
    } while (0)

/**
 * Logs an error when an invalid command is detected.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 */
#define INVALID_COMMAND_ERROR(fileName, lineNumber)                                                             \
    do                                                                                                          \
    {                                                                                                           \
        logNewError(fileName, lineNumber);                                                                      \
        fprintf(stderr, "This line has been failed according to: The command that in this line is invalid.\n"); \
    } while (0)

/**
 * Logs an error when a label was not found.
 * @param fileName The name of the file where the error occurred.
 * @param address The address or line number where the error was found.
 * @param labelName The name of the missing label.
 */
#define LABEL_WAS_NOT_FOUND_ERROR(fileName, lineNumber, labelName)                                                                        \
    do                                                                                                                                    \
    {                                                                                                                                     \
        logNewError(fileName, lineNumber);                                                                                                \
        fprintf(stderr, "This line has been failed according to: In the current line no matching label was found for: %s.\n", labelName); \
    } while (0)

#define INVALID_LABEL_FORMAT(fileName, lineNumber)                                                                                                                         \
    do                                                                                                                                                                     \
    {                                                                                                                                                                      \
        logNewError(fileName, lineNumber);                                                                                                                                 \
        fprintf(stderr, "This line has been failed according to: Invalid label format. Labels must start with capital letters and be immediately followed by a colon.\n"); \
    } while (0)

#define INVALID_INTEGER_VALUE(fileName, lineNumber, token)                                                                  \
    do                                                                                                                      \
    {                                                                                                                       \
        logNewError(fileName, lineNumber);                                                                                  \
        fprintf(stderr, "This line has been failed according to: Invalid number type. Only integer values are allowed.\n"); \
    } while (0)

#define LABEL_ALREADY_EXISTS_ERROR(fileName, lineNumber, label)                                                          \
    do                                                                                                                   \
    {                                                                                                                    \
        logNewError(fileName, lineNumber);                                                                               \
        fprintf(stderr, "This line has been failed according to: The label '%s' in this line already exists.\n", label); \
    } while (0)

#define INVALID_REGISTER_FOUND(fileName, lineNumber, registerName)           \
    do                                                                       \
    {                                                                        \
        logNewError(fileName, lineNumber);                                   \
        fprintf(stderr, "Invalid register encountered: %s\n", registerName); \
    } while (0)

#define INVALID_PREFIX_ERROR(fileName, lineNumber)                                                           \
    do                                                                                                       \
    {                                                                                                        \
        logNewError(fileName, lineNumber);                                                                   \
        fprintf(stderr, "In file %s, at line %d: Invalid command prefix detected.\n", fileName, lineNumber); \
    } while (0)

#define INVALID_OPERAND_COUNT(fileName, lineNumber)                                                                                   \
    do                                                                                                                                \
    {                                                                                                                                 \
        fprintf(stderr, "In the file %s there is an error at line number %d: Incorrect number of operands.\n", fileName, lineNumber); \
    } while (0)

#define UNKNOWN_COMMAND_ERROR(fileName, lineNumber)                                                   \
    do                                                                                                \
    {                                                                                                 \
        logNewError(fileName, lineNumber);                                                            \
        fprintf(stderr, "In file %s, at line %d: Unknown command detected.\n", fileName, lineNumber); \
    } while (0)

#endif /* ERRORSHANDLER_H */