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

/* Prints the error log prefix.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
void printLogPrefix(char *fileName, int address);

/* Retrieves the total count of errors.
 * @return The count of errors.
 */
int getErrorsCounter();

/* Macro for reporting an error when there is extraneous text after the end of a command.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define EXTRANEOUS_TEXT(fileName, address)                \
    do                                                    \
    {                                                     \
        errorsCounter++;                                  \
        printLogPrefix(fileName, address);                \
        printf("Extraneous text after end of command\n"); \
        return;                                           \
    } while (0)

/* Macro for reporting an error when a parameter is missing.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define MISSING_PARAMETER(fileName, address) \
    do                                       \
    {                                        \
        errorsCounter++;                     \
        printLogPrefix(fileName, address);   \
        printf("Missing parameter\n");       \
        return;                              \
    } while (0)

/* Macro for reporting an error when a comma is missing.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define MISSING_COMMA(fileName, address)   \
    do                                     \
    {                                      \
        errorsCounter++;                   \
        printLogPrefix(fileName, address); \
        printf("Missing comma\n");         \
        return;                            \
    } while (0)

/* Macro for reporting an error when there are multiple consecutive commas.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define MULTIPLE_CONSECUTIVE_COMMAS(fileName, address) \
    do                                                 \
    {                                                  \
        errorsCounter++;                               \
        printLogPrefix(fileName, address);             \
        printf("Multiple consecutive commas\n");       \
        return;                                        \
    } while (0)

/* Macro for reporting an error when a label already exists.
 * @param labelName The name of the existing label.
 * @param fileName The name of the file where the error occurred.
 * @param address The address at which the error occurred.
 */
#define EXISTING_LABEL(labelName, fileName, address)      \
    do                                                    \
    {                                                     \
        errorsCounter++;                                  \
        printLogPrefix(fileName, address);                \
        printf("Label '%s' already exists\n", labelName); \
        return;                                           \
    } while (0)

#endif /* ERRORSHANDLER_H */