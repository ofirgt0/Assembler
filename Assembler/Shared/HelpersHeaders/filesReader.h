#ifndef FILESREADER_H
#define FILESREADER_H

#define ASM_FILE_NAME_EXTENSION ".as"
#define MACRO_FILE_NAME_EXTENSION ".am"

void logNewLine(const char *line, int lineNumber);

void fileReader(const char *fileName);

void removePrefixSpaces(char *command);

char *getFileNameWithExtension(const char *fileName, char *extension);

void layoutBulkOfLines(int lineNumber, int linesNumber, char *fileName, int macroLineInFile);

#endif /* FILESREADER_H */