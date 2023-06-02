#include <stdio.h>
#include <stdbool.h>

/**
 * Write a string to a file.
 *
 * If the file already exists, the string is written on a new line at the end of the file.
 * If the file doesn't exist, a new file is created with the given name and the string is written to it.
 *
 * @param filename The name of the file to write to.
 * @param string   The string to be written to the file.
 */
void writeToFile(const char* filename, const char* string) {
    FILE* file = fopen(filename, "a");  // Open the file in "append" mode
    if (file != NULL) {
        fprintf(file, "\n%s", string);  // Write the string to a new line at the end of the file
        fclose(file);  // Close the file
    } else {
        file = fopen(filename, "w");  // Open the file in "write" mode (create a new file)
        if (file != NULL) {
            fprintf(file, "%s", string);  // Write the string to the file
            fclose(file);  // Close the file
        } else {
            printf("Failed to open/create the file.\n");
        }
    }
}

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