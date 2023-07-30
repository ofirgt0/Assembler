#include <stdio.h>
#include <stdbool.h>
#include "writeToFile.h"
#include <stdbool.h>

/**
 * Write an integer array as a string to a file on the last line.
 * If the file already exists, the array is written on a new line at the end of the file.
 * If the file doesn't exist, a new file is created with the given name,
 * and the array is written to it as a string on the last line.
 * @param filename The name of the file to write to.
 * @param arr      The integer array to be written to the file.
 * @param size     The size of the array.
 * @return true if the write operation is successful, false otherwise.
 */
bool writeIntArrayToFile(const char *filename, const int arr[])
{
    /* Open the file in "append" mode */
    FILE *file = fopen(filename, "a");
    int i;

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
        printf("Failed to open/create the file.\n");
        return false;
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

bool writeLabelToFile(const char *filename, Label *label)
{
    if ((label->type == Ext || label->type == Entry) && !isFileExist(filename))
    {
        return true;
    }

    char labelText[256];
    sprintf(labelText, "%s %d %d", label->name, label->address, label->type);
    return writeToFile(filename, labelText);
} */

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