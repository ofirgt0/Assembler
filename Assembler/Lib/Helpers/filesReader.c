#include <stdio.h>
#include "filesReader.h"
#include "errorsHandler.h"

#define ASM_FILE_NAME_EXTENSION ".as"
#define MACRO_FILE_NAME_EXTENSION ".am"

void prepareSecondRun(const char *fileName);
void startFirstRun(char *line, int lineNumber, const char *fileName);
char *removeFileNameExtension(const char *fileName);
void printLabels(const char *fileName);
void commandParser(const char *line, const char *fileName, int lineNumber);

void logNewLine(const char *line, int lineNumber)
{
    printf("\n\n\n ----------------------------------------------------\n");

    printf("Processing line: %s\n", line);
}

/**
 *  This function reads and processes a given file.
 *  In the first run, it saves labels, macros, entries, externs, data, and strings.
 *  In the second run, it processes each line and parses the commands.
 */
void fileReader(const char *fileName)
{
    char line[256];
    int i;

    FILE *file = fopen(getFileNameWithExtension(fileName, ASM_FILE_NAME_EXTENSION), "r");
    if (file == NULL)
    {
        FILE_OPEN_ERROR(__FILE__, __LINE__);
        return;
    }

    /*fileName = removeFileNameExtension(fileName);*/
    /*First run - save the label, macro. entry, extern, data and string*/
    for (i = 0; fgets(line, sizeof(line), file) != NULL; i++)
    {
        if (line == '\0')
            continue;

        removePrefixSpaces(line);
        logNewLine(line, i);
        startFirstRun(line, i, fileName);
    }

    fseek(file, 0, SEEK_SET);
    prepareSecondRun(fileName);
    printf("\n################################ S--E--C--O--N--D--R--U--N ################################\n");
    /* Second run */

    FILE *macroFile = fopen(getFileNameWithExtension(fileName, MACRO_FILE_NAME_EXTENSION), "r");
    if (macroFile == NULL)
    {
        FILE_OPEN_ERROR(__FILE__, __LINE__);
        return;
    }

    for (i = 0; fgets(line, sizeof(line), macroFile) != NULL; i++)
    {
        removePrefixSpaces(line);
        if (line == '\0')
            continue;

        logNewLine(line, 0);
        commandParser(line, fileName, i);
    }

    printLabels(fileName);
    fclose(macroFile);
}

char *getFileNameWithExtension(const char *fileName, char *extension)
{
    char fileNameWithExtension[100];
    strcat(fileNameWithExtension, fileName);
    strcat(fileNameWithExtension, extension);
    return fileNameWithExtension;
}

/**
 *  This function retrieves a block of lines from a file and processes them.
 *  It starts reading from a specified line number and reads a specified number of lines.
 */
void getBulkOfLines(int lineNumber, int linesNumber, char *fileName)
{
    char line[256];
    int currentLine;
    FILE *file = fopen(getFileNameWithExtension(fileName, ASM_FILE_NAME_EXTENSION), "r");
    if (file == NULL)
    {
        FILE_OPEN_ERROR(__FILE__, __LINE__);
        return;
    }
    currentLine = 1;

    while (currentLine < lineNumber && fgets(line, sizeof(line), file))
        currentLine++;

    while (currentLine <= lineNumber + linesNumber - 1 && fgets(line, sizeof(line), file))
    {
        logNewLine(line, 1);
        commandParser(line, fileName, currentLine);
        currentLine++;
    }

    fclose(file);
}

/*TODO: will replace getBulk*/
void layoutBulkOfLines(int lineNumber, int linesNumber, char *fileName, int macroLineInFile)
{
    char line[256];
    int currentLine;
    FILE *file = fopen(getFileNameWithExtension(fileName, ASM_FILE_NAME_EXTENSION), "r");
    if (file == NULL)
    {
        FILE_OPEN_ERROR(__FILE__, __LINE__);
        return;
    }
    currentLine = 1;

    while (currentLine < lineNumber && fgets(line, sizeof(line), file))
        currentLine++;

    int i = 0;
    char fileNameWithExtension[100] = getFileNameWithExtension(fileName, MACRO_FILE_NAME_EXTENSION);
    while (currentLine <= lineNumber + linesNumber - 1 && fgets(line, sizeof(line), file))
    {
        logNewLine(line, 1);
        startFirstRun(line, fileNameWithExtension, i + macroLineInFile);
        currentLine++;
        i++;
    }

    fclose(file);
}

/**
 *  The main function reads and processes multiple files provided as command-line arguments.
 *  It iterates through each command-line argument, treating each as a file name.
 *  It then calls the fileReader function on each file.
 */
int main(int argc, char *argv[])
{
    int i;
    if (argc < 2)
    {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    /*Iterate through each command-line argument (file name)*/
    for (i = 1; i < argc; i++)
        fileReader(argv[i]);

    return 0;
}