#include "writeToFile.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * Write a string to a file.
 * If the file already exists, the string is written on a new line at the end of the file.
 * If the file doesn't exist, a new file is created with the given name and the string is written to it.
 * @param filename The name of the file to write to.
 * @param string   The string to be written to the file.
 */
bool writeToFile(const char *filename, const char *string)
{

    /* Open the file in "append" mode */
    FILE *file = fopen(filename, "a");

    if (file != NULL)
    {
        /* Write the string to a new line at the end of the file */
        fprintf(file, "\n%s", string);
        /* Close the file */
        fclose(file);
        return true;
    }
    else
    {
        /* Open the file in "write" mode (create a new file) */
        file = fopen(filename, "w");

        if (file != NULL)
        {
            /* Write the string to the file */
            fprintf(file, "%s", string);
            /* Close the file */
            fclose(file);
            return true;
        }
        else
        {
            printf("Failed to open/create the file.\n");
            return false;
        }
    }
}

/**
 * Writes a label to a file.
 * If the label is of type Ext or Entry and the corresponding file does not exist,
 * the function returns true without creating the file.
 * Otherwise, it writes the label to the file.
 * @param filename The name of the file to write to.
 * @param label    The label to be written to the file.
 * @return true if the operation is successful or if the file does not need to be created, otherwise - false.
 */
bool writeLabelToFile(const char *filename, Label *label)
{
    if ((label->type == Ext || label->type == Entry) && !isFileExist(filename))
    {
        return true;
    }

    char labelText[256];
    sprintf(labelText, "%s %d %d", label->name, label->address, label->type);
    return writeToFile(filename, labelText);
}

/**
 * Check if a file exists.
 * @param fileName The name of the file to check.
 * @return true if the file exists, otherwise - false.
 */
bool isFileExist(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file != NULL)
    {
        fclose(file);
        return true;
    }
    return false;
}