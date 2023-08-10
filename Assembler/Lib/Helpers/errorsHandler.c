#include <stdio.h>
#include <stdlib.h>
#include "errorsHandler.h"

/* Variable to store the count of errors */
int errorsCounter = 0;

/* Function to get the count of errors */
int getErrorsCounter()
{
    return errorsCounter;
}

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