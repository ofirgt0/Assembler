#ifndef WRITE_TO_FILE_H
#define WRITE_TO_FILE_H
#include <stdio.h>
#include <stdbool.h>
#include "errorsHandler.h"

/**
 * Writes an integer array to a file.
 * --------------------------
 * @param filename - The name of the file to write the integer array to.
 * @param arr - The integer array to be written to the file.
 * @return Returns true if the writing operation is successful, or false otherwise.
 */
bool writeIntArrayToFile(const char *filename, const int arr[])

    /**
     * Writes a label to a file.
     * -------------------------
     * If the label is of type Ext or Entry and the corresponding file does not exist,
     * the function returns true without creating the file.
     * Otherwise, it writes the label to the file.
     * @param filename The name of the file to write to.
     * @param label    The label to be written to the file.
     * @return true if the operation is successful or if the file does not need to be created, otherwise - false.
     */
    bool writeLabelToFile(const char *filename, Label *label);

/**
 * Checks if a file exists.
 * -------------------------
 * @param fileName The name of the file to check.
 * @return true if the file exists, otherwise - false.
 */
bool isFileExist(const char *fileName);

#endif /* WRITE_TO_FILE_H */