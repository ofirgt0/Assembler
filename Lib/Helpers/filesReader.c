#include <stdio.h>
#include "filesReader.h"

void logNewLine(const char *line)
{
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

    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", fileName);
        return;
    }

    /*First run - save the label, macro. entry, extern, data and string*/
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line == '\0')
            continue;

        removePrefixSpaces(line);
        logNewLine(line);
        startFirstRun(line);
    }

    fseek(file, 0, SEEK_SET);

    printf("\n-------------S--E--C--O--N--D--R--U--N-------------\n\n");

    /* Second pass on the code */
    while (fgets(line, sizeof(line), file) != NULL)
    {
        removePrefixSpaces(line);
        if (line == '\0')
            continue;

        logNewLine(line);
        /*commandParser(line, fileName);*/
    }

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
        logNewLine(line);
        commandParser(line, fileName);
        currentLine++;
    }

    fclose(file);
}

/**
 *  This function removes spaces at the beginning of a command.
 *  It shifts the command string to the right until it encounters a non-space character.

void removePrefixSpaces(char *command)
{
    int i, j;

    for (i = 0; command[i] == ' ' || command[i] == '\n' || command[i] == '\t'; i++)
    {
    }

    for (j = 0; command[i]; j++, i++)
    {
        command[j] = command[i];
    }

    command[j] = '\0';
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