#include <stdio.h>
#include "filesReader.h"

void logNewLine(const char *line)
{ /* TODO: DELETE THIS FUNCTION AND IMPLEMENT IN THE EXECUTER */
    printf("Processing line: %s\n", line);
}

/* Function to read and process a file */
/* Input: filename - the name of the file to be read and processed */
void fileReader(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", fileName);
        return;
    }

    char line[256];

    /* first run - save label, macro. entry, extern, data and string */
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line == '\0')
            continue;

        removePrefixSpaces(line);

        startFirstRun(line);
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {

        removePrefixSpaces(line);
        if (line == '\0')
            continue;

        logNewLine(line);
        parseCommand(line, fileName);
    }

    fclose(file);
}

void getBulkOfLines(int lineNumber, int linesNumber, char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", fileName);
        return;
    }
    int currentLine = 1;
    char line[256];

    while (currentLine < lineNumber && fgets(line, sizeof(line), file))
        currentLine++;

    while (currentLine <= lineNumber + linesNumber - 1 && fgets(line, sizeof(line), file))
    {
        logNewLine(line);
        parseCommand(line, fileName);
        currentLine++;
    }

    fclose(file);
}

void removePrefixSpaces(char *command)
{
    int i, j;
    for (i = 0, j = 0; command[i]; i++)
    {
        if (command[i] == ' ' || command[i] == '\n' || command[i] == '\t')
        {
            command++;
        }
        else
        {
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    int i;
    if (argc < 2)
    {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    /* Iterate through each command-line argument (file name) */
    for (i = 1; i < argc; i++)
        fileReader(argv[i]);

    return 0;
}