#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesReader.h"
#include "errorsHandler.h"

char *removeFileNameExtension(const char *filename);
void prepareSecondRun(const char *fileName);
void commandParser(char *command, const char *fileName, int lineNumber);
void printLabels(const char *);
void startFirstRun(char *, int, const char *);

void logNewLine(const char *line, int lineNumber)
{
    printf("\n\n\n ----------------------------------------------------\n");

    printf("Processing line: %s\n", line);
}

char *getFileNameWithExtension(const char *fileName, char *extension)
{
    char *fileNameWithExtension = (char *)malloc(strlen(fileName) + strlen(extension) + 1);
    if (!fileNameWithExtension)
    {
        return NULL;
    }

    strcpy(fileNameWithExtension, fileName);
    strcat(fileNameWithExtension, extension);

    return fileNameWithExtension;
}

/**
 *  This function reads and processes a given file.
 *  In the first run, it saves labels, macros, entries, externs, data, and strings.
 *  In the second run, it processes each line and parses the commands.
 */
void fileReader(const char *fileName)
{
    char line[256];
    char *asmFileName;
    char *macroFileName;
    int i;
    int notEmptyLinesCounter = 0;
    int errorCount;
    FILE *file;
    FILE *macroFile;

    asmFileName = getFileNameWithExtension(fileName, ASM_FILE_NAME_EXTENSION);

    if (!asmFileName)
    {
        return;
    }

    file = fopen(asmFileName, "r");

    if (file == NULL)
    {
        OPENING_FILE_ERROR(fileName, -1);
        free(asmFileName);
        return;
    }

    for (i = 0; fgets(line, sizeof(line), file) != NULL; i++)
    {
        if (line[0] == '\0' || line[0] == '\n' || strlen(line) == 0)
            continue;

        removePrefixSpaces(line);
        logNewLine(line, notEmptyLinesCounter);
        startFirstRun(line, notEmptyLinesCounter, fileName);
        notEmptyLinesCounter++;
    }
    notEmptyLinesCounter = 0;
    if (getErrorsCounter() > 0)
    {
        printf("\nErrors found in file. program stopped.\n");
        fprintf(stderr, "\nErrors found in file. program stopped.\n");
        fprintf(stderr, "Total Errors in file %s: %d\n", fileName, errorCount);
        fclose(file);
        free(asmFileName);
        return;
    }

    fseek(file, 0, SEEK_SET);
    prepareSecondRun(fileName);
    printf("\n################################ S--E--C--O--N--D--R--U--N ################################\n");

    macroFileName = getFileNameWithExtension(fileName, MACRO_FILE_NAME_EXTENSION);
    if (!macroFileName)
    {
        return;
    }

    macroFile = fopen(macroFileName, "r");
    free(macroFileName);

    if (macroFile == NULL)
    {
        OPENING_FILE_ERROR(fileName, -1);
        return;
    }

    for (i = 0; fgets(line, sizeof(line), macroFile) != NULL; i++)
    {
        removePrefixSpaces(line);
        if (line[0] == '\0' || line[0] == '\n' || strlen(line) == 0)
            continue;

        notEmptyLinesCounter++;
        logNewLine(line, notEmptyLinesCounter);
        commandParser(line, fileName, notEmptyLinesCounter);
    }

    printLabels(fileName);
    fclose(macroFile);
}

void layoutBulkOfLines(int lineNumber, int linesNumber, char *fileName, int macroLineInFile)
{
    char line[256];
    int currentLine;
    int i = 0;
    char *asmFileName;
    char *macroFileName;
    FILE *file;

    fileName = removeFileNameExtension(fileName);
    printf("########################### layoutBulkOfLines - after remove file extension: %s\n", fileName);

    asmFileName = getFileNameWithExtension(fileName, ASM_FILE_NAME_EXTENSION);
    if (!asmFileName)
    {
        return;
    }
    printf("########################### layoutBulkOfLines - after remove asmFileName extension: %s\n", asmFileName);
    file = fopen(asmFileName, "r");

    if (file == NULL)
    {
        OPENING_FILE_ERROR(asmFileName, -1);
        free(asmFileName);
        return;
    }
    free(asmFileName);
    currentLine = 1;

    while (currentLine < lineNumber && fgets(line, sizeof(line), file))
        currentLine++;

    macroFileName = getFileNameWithExtension(fileName, MACRO_FILE_NAME_EXTENSION);
    if (!macroFileName)
    {
        fclose(file);
        return;
    }

    while (currentLine <= lineNumber + linesNumber - 1 && fgets(line, sizeof(line), file))
    {
        removePrefixSpaces(line);
        logNewLine(line, 1);
        printf("line: %s\n", line);
        printf("macroFileName %s \n", macroFileName);
        startFirstRun(line, i + macroLineInFile, fileName);
        currentLine++;
        i++;
    }

    free(macroFileName);
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
