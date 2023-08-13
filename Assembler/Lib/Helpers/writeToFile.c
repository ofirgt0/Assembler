#include <stdio.h>
#include <stdbool.h>
#include "writeToFile.h"
#include <stdbool.h>
#include "errorsHandler.h"

bool writeIntArrayToFile(const char *filename, const int arr[])
{
    int i;
    /* Open the file in "append" mode */
    FILE *file;
    file = fopen(filename, "a");

    if (file != NULL)
    {
        /* Write the array elements as a string on the last line */
        fprintf(file, "\n");
        for (i = 0; i < WORD_LENGTH; i++)
        {
            fprintf(file, "%d", arr[i]);
        }
        /* Close the file */
        fclose(file);
        return true;
    }
    else
    {
        OPENING_FILE_ERROR(filename, -1); /* TODO: to handle -1 issue, need to set the real lineNumber */
        return false;
    }
}

void appendStringToFile(const char *filename, const char *text)
{
    FILE *file;

    printf("debug: write the line %s to file %s\n", text, filename);

    file = fopen(filename, "a");
    if (file == NULL)
    {
        OPENING_FILE_ERROR(filename, -1); /* TODO: to handle -1 issue, need to set the real lineNumber */
        return;
    }

    fprintf(file, "%s\n", text);

    fclose(file);
}

bool writeLabelToFile(const char *filename, char *labelName, int address)
{
    FILE *file;

    printf("writeLabelToFile: %s  %d\n", labelName, address);

    file = fopen(filename, "a");
    if (file == NULL)
    {
        OPENING_FILE_ERROR(filename, address);
    }

    fprintf(file, "%s  %d\n", labelName, address);

    fclose(file);

    return true;
}

bool isFileExist(const char *fileName)
{
    FILE *file;

    file = fopen(fileName, "r");

    if (file != NULL)
    {
        fclose(file);
        return true;
    }

    return false;
}