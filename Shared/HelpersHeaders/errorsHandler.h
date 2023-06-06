#ifndef ERRORSHANDLER_H
#define ERRORSHANDLER_H
#include <stdio.h>
#include <stdlib.h>

/**
 * Prints the error log prefix.
 * @param fileName The name of the file.
 * @param address  The address of the error.
 */
void printLogPrefix(const char *fileName, int address);

/**
 * Gets the count of errors.
 * @return The count of errors.
 */
int getErrorsCounter();

/* Macros for common errors */
/**
 * Macro for reporting an extraneous text error.
 * @param fileName The name of the file.
 * @param address  The address of the error.
 */
#define EXTRANEOUS_TEXT(fileName, address)                \
    do                                                    \
    {                                                     \
        errorsCounter++;                                  \
        printLogPrefix(fileName, address);                \
        printf("Extraneous text after end of command\n"); \
        return;                                           \
    } while (0)

/**
 * Macro for reporting a missing parameter error.
 * @param fileName The name of the file.
 * @param address  The address of the error.
 */
#define MISSING_PARAMETER(fileName, address) \
    do                                       \
    {                                        \
        errorsCounter++;                     \
        printLogPrefix(fileName, address);   \
        printf("Missing parameter\n");       \
        return;                              \
    } while (0)

/**
 * Macro for reporting a missing comma error.
 * @param fileName The name of the file.
 * @param address  The address of the error.
 */
#define MISSING_COMMA(fileName, address)   \
    do                                     \
    {                                      \
        errorsCounter++;                   \
        printLogPrefix(fileName, address); \
        printf("Missing comma\n");         \
        return;                            \
    } while (0)

/**
 * Macro for reporting multiple consecutive commas error.
 * @param fileName The name of the file.
 * @param address  The address of the error.
 */
#define MULTIPLE_CONSECUTIVE_COMMAS(fileName, address) \
    do                                                 \
    {                                                  \
        errorsCounter++;                               \
        printLogPrefix(fileName, address);             \
        printf("Multiple consecutive commas\n");       \
        return;                                        \
    } while (0)

/**
 * Macro for reporting an existing label error.
 * @param labelName The name of the existing label.
 */
#define EXISTING_LABEL(labelName)                         \
    do                                                    \
    {                                                     \
        errorsCounter++;                                  \
        printLogPrefix(fileName, address);                \
        printf("Label '%s' already exists\n", labelName); \
        return;                                           \
    } while (0)

#endif /* ERRORSHANDLER_H */