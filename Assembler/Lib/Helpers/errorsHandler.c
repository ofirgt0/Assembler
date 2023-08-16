#include <stdio.h>
#include <stdlib.h>
#include "errorsHandler.h"

/* Counter for tracking the number of logged errors. */
static int errorsCounter = 0;

/* Returns the current count of logged errors.
 * @return The total count of errors so far.
 */
int getErrorsCounter()
{
    return errorsCounter;
}

void initErrorsCounter(){
    errorsCounter = 0;
}

/* Logs a new error for a given file and line number, and increments the error counter.
 * @param fileName Name of the assembly file where the error occurred.
 * @param lineNumber Line number in the file where the error was found.
 */
void logNewError(const char *fileName, int lineNumber)
{
    lineNumber++;
    if (lineNumber != -1)
    {
        fprintf(stderr, "In the file %s there is an error at line number %d: \n", fileName, lineNumber);
    }
    else
    {
        fprintf(stderr, "Error in file %s: \n", fileName);
    }

    errorsCounter++;
}
