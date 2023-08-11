#include <stdbool.h>

#ifndef WRITE_TO_FILE_H
#define WRITE_TO_FILE_H

#define WORD_LENGTH 12

/*
 * Writes an integer array to a file.
 * @param filename: The name of the file to write to.
 * @param arr: The integer array to write.
 * @return true if the array was written successfully, false otherwise.
 */
bool writeIntArrayToFile(const char *filename, const int arr[]);

/*
 * Checks if a file exists.
 * @param fileName: The name of the file to check.
 * @return true if the file exists, false otherwise.
 */
bool isFileExist(const char *fileName);

/*
 * Writes a label and its address to a file.
 * @param filename: The name of the file to write to.
 * @param labelName: The label's name to write.
 * @param address: The address associated with the label.
 * @return true if the label and its address were written successfully, false otherwise.
 */
bool writeLabelToFile(const char *filename, char *labelName, int address);

/*
 * Appends a string to a file.
 * @param filename: The name of the file to append to.
 * @param text: The text string to append.
 */
void appendStringToFile(const char *filename, const char *text);

#endif /* WRITE_TO_FILE_H */