#include <stdbool.h>

#ifndef WRITE_TO_FILE_H
#define WRITE_TO_FILE_H

#define WORD_LENGTH 12

/**
 * Writes a string to a file.
 * --------------------------
 * If the file already exists, the string is written on a new line at the end of the file.
 * If the file doesn't exist, a new file is created with the given name and the string is written to it.
 * @param filename The name of the file to write to.
 * @param arr      The integer array to be written to the file.
 * @param size     The size of the array.
 * @return true if the write operation is successful, false otherwise.
 */
bool writeIntArrayToFile(const char *filename, const int arr[]);

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
/*bool writeLabelToFile(const char *filename, Label *label);
 */
/**
 * Checks if a file exists.
 * -------------------------
 * @param fileName The name of the file to check.
 * @return true if the file exists, otherwise - false.
 */
bool isFileExist(const char *fileName);

bool writeLabelToFile(const char *filename, char *labelName, int address);

void appendStringToFile(const char *filename, const char *text);

#endif /* WRITE_TO_FILE_H */
