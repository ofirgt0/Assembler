#include <stdio.h>
#include "filesReader.h"

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
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", fileName);
        return;
    }

    fileName = removeFileNameExtension(fileName);
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
    for (i = 0; fgets(line, sizeof(line), file) != NULL; i++)
    {
        removePrefixSpaces(line);
        if (line == '\0')
            continue;

        logNewLine(line, 0);
        commandParser(line, fileName, i);
    }
    printLabels(fileName);
    fclose(file);
}

/**
 *  This function retrieves a block of lines from a file and processes them.
 *  It starts reading from a specified line number and reads a specified number of lines.
 */
void getBulkOfLines(int lineNumber, int linesNumber, char *fileName)
{
    char line[256];
    int currentLine;
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", fileName);
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