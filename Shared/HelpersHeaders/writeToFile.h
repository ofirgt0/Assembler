#ifndef WRITE_TO_FILE_H
#define WRITE_TO_FILE_H
#include <stdio.h>
#include <stdbool.h>
#include "errorsHandler.h"

/**
 * Writes a string to a file.
 * --------------------------
 * If the file already exists, the string is written on a new line at the end of the file.
 * If the file doesn't exist, a new file is created with the given name and the string is written to it.
 * @param filename The name of the file to write to.
 * @param string   The string to be written to the file.
 * @return true if the operation is successful, otherwise - false.
 */
bool writeToFile(const char *filename, const char *string);

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