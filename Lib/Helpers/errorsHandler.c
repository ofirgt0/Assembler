
/* Macros for common errors: */

#define EXTRANEOUS_TEXT(fileName, address)            \
    errorsCounter++;                                  \
    printLogPrefix(fileName, address);                \
    printf("Extraneous text after end of command\n"); \
    return;

#define MISSING_PARAMETER(fileName, address) \
    errorsCounter++;                         \
    printLogPrefix(fileName, address);       \
    printf("Missing parameter\n");           \
    return;

#define MISSING_COMMA(fileName, address) \
    errorsCounter++;                     \
    printLogPrefix(fileName, address);   \
    printf("Missing comma\n");           \
    return;

#define MULTIPLE_CONSECUTIVE_COMMAS(fileName, address) \
    errorsCounter++;                                   \
    printLogPrefix(fileName, address);                 \
    printf("Multiple consecutive commas\n");           \
    return;

static int errorsCounter = 0;

void printLogPrefix(char *fileName, int address)
{
    printf("Error in file %s at address %d: ", fileName, address);
}
