#include <stdio.h>
#include <stdlib.h>
#include "errorsHandler.h"

/* Variable to store the count of errors */
int errorsCounter = 0;

/* Function to print error log prefix */
void printLogPrefix(char *fileName, int address)
{
    fprintf(stderr, "Error in file %s at address %d: ", fileName, address);
}

/* Function to get the count of errors */
int getErrorsCounter()
{
    return errorsCounter;
}