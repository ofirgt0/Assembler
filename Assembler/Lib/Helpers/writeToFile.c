#include <stdio.h>
#include <stdbool.h>
#include "writeToFile.h"
#include "errorsHandler.h"

/* Writes an integer array to the specified file, returning true on success.
 * @param filename: The name of the file to write to.
 * @param arr: The integer array to write.
 * @return true if the array was written successfully, false otherwise.
 */
bool writeIntArrayToFile(const char *filename, const int arr[])
{
    int i;
    FILE *file;
    file = fopen(filename, "w");

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
        OPENING_FILE_ERROR(filename, -1);
        return false;
    }
}

/* Appends a string to a file.
 * @param filename: The name of the file to append to.
 * @param text: The text string to append.
 */
void appendStringToFile(const char *filename, const char *text)
{
    FILE *file;

    file = fopen(filename, "a");
    if (file == NULL)
    {
        OPENING_FILE_ERROR(filename, -1);
        return;
    }

    fprintf(file, "%s\n", text);

    fclose(file);
}

/* Writes a label and its corresponding address to the specified file, returning true on success.
 * @param filename: The name of the file to write to.
 * @param labelName: The label's name to write.
 * @param address: The address associated with the label.
 * @return true if the label and its address were written successfully, false otherwise.
 */
bool writeLabelToFile(const char *filename, char *labelName, int address)
{
    FILE *file;

    file = fopen(filename, "a");
    if (file == NULL)
    {
        OPENING_FILE_ERROR(filename, address);
    }

    fprintf(file, "%s  %d\n", labelName, address);

    fclose(file);

    return true;
}

/* Checks if a file exists and returns true if it does.
 * @param fileName: The name of the file to check.
 * @return true if the file exists, false otherwise.
 */
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