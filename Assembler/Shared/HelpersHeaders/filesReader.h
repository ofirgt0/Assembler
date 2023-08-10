#ifndef FILESREADER_H
#define FILESREADER_H

/*
 * Logs a new line of text.
 * @param line The line of text to be logged.
 */
void logNewLine(const char *line, int lineNumber);

/*
 * Reads a file and processes its contents.
 * @param fileName The name of the file to be read and processed.
 */
void fileReader(const char *fileName);

/*
 * Removes leading spaces from a command string.
 * @param command The command string to be processed.
 */
void removePrefixSpaces(char *command);

char *getFileNameWithExtension(const char *fileName, char *extension);

void layoutBulkOfLines(int lineNumber, int linesNumber, char *fileName, int macroLineInFile);

#endif /* FILESREADER_H */