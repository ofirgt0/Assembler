#ifndef FILESREADER_H
#define FILESREADER_H

/*
 * Logs a new line of text.
 * @param line The line of text to be logged.
 *
void logNewLine(const char *line);*/

/*
 * Reads a file and processes its contents.
 * @param fileName The name of the file to be read and processed.
 */
void fileReader(const char *fileName);

/*
 * Retrieves a specified number of lines from a file, starting from a specified line number.
 * @param lineNumber The number of the line from which to start reading.
 * @param linesNumber The number of lines to read.
 * @param fileName The name of the file to read.
 */
void getBulkOfLines(int lineNumber, int linesNumber, char *fileName);

/*
 * Removes leading spaces from a command string.
 * @param command The command string to be processed.
 */
void removePrefixSpaces(char *command);

#endif /* FILESREADER_H */