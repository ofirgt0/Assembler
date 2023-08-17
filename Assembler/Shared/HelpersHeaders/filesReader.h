#ifndef FILESREADER_H
#define FILESREADER_H

#define ASM_FILE_NAME_EXTENSION ".as"
#define MACRO_FILE_NAME_EXTENSION ".am"

/**
 * Logs each new line being processed.
 * @param line The line being logged.
 * @param lineNumber The line number in the file.
 */
void logNewLine(const char *line, int lineNumber);

/**
 * Reads and processes an assembly file.
 * @param fileName The name of the file to be read.
 */
void fileReader(const char *fileName);

/**
 * Removes leading white spaces from a given string.
 * @param command The string from which prefix spaces need to be removed.
 */
void removePrefixSpaces(char *command);

/**
 * Returns the full file name with the desired extension.
 * @param fileName The base name of the file.
 * @param extension The desired extension to append to the file name.
 * @return A new string containing the full file name with the extension.
 */
char *getFileNameWithExtension(const char *fileName, char *extension);

/**
 * Handles the layout of bulk lines during the assembly processing.
 * @param lineNumber The starting line number.
 * @param linesNumber The number of lines to layout.
 * @param fileName The name of the file being processed.
 * @param macroLineInFile The line number of the macro in the file.
 */
void layoutBulkOfLines(int lineNumber, int linesNumber, char *fileName, int macroLineInFile);
/**
 * Removes the file extension from a given filename.
 * @param filename The original filename.
 * @return A new string with the extension removed.
 */
char *removeFileNameExtension(const char *filename);

/**
 * Prepares for the second run of the file reader.
 * @param fileName The name of the source file.
 */
void prepareSecondRun(const char *fileName);

/**
 * Prints the labels from a given source file.
 * @param fileName The name of the source file.
 */
void printLabels(const char *);

#endif /* FILESREADER_H */