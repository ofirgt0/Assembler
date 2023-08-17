#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "errorsHandler.h"
#include "writeToFile.h"

/**
 *  The main function of the project.
 *  This function reads and processes multiple files provided as command-line arguments.
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

/**
 * Parses a given assembly command.
 * @param command The assembly command to parse.
 * @param fileName The name of the source file.
 * @param lineNumber The line number in the source file.
 */
void commandParser(char *command, const char *fileName, int lineNumber);

/**
 * Starts the first run of the file reader.
 * @param line The current line from the source file.
 * @param lineNumber The line number in the source file.
 * @param fileName The name of the source file.
 */
void startFirstRun(char *, int, const char *);

/* Logs the currently processed line */
void logNewLine(const char *line, int lineNumber)
{
    printf("\n\n\n ----------------------------------------------------\n");

    printf("Processing line: %s\n", line);
}

/* Constructs a file name by appending a given extension to the base file name. */
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
 * @param fileName The name of the source file.
 */
void fileReader(const char *fileName)
{
    char line[81];
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
        if (strlen(line) > 80)
        {
            LINE_LENGTH_ERROR(fileName, i + 1);
            continue;
        }

        if (line[0] == '\0' || line[0] == '\n' || strlen(line) == 0)
            continue;

        removePrefixSpaces(line);
        logNewLine(line, notEmptyLinesCounter);
        startFirstRun(line, notEmptyLinesCounter, fileName);
        notEmptyLinesCounter++;
    }
    notEmptyLinesCounter = 0;

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
        if (strlen(line) > 80)
        {
            LINE_LENGTH_ERROR(fileName, i + 1);
            continue;
        }

        removePrefixSpaces(line);
        if (line[0] == '\0' || line[0] == '\n' || strlen(line) == 0)
            continue;

        notEmptyLinesCounter++;
        logNewLine(line, notEmptyLinesCounter);
        commandParser(line, fileName, notEmptyLinesCounter);
    }

    errorCount = getErrorsCounter();
    if (errorCount > 0)
    {

        char *obFileName = getFileNameWithExtension(fileName, ".ob");
        if (obFileName)
        {
            if (isFileExist(obFileName))
            {
                /* Delete the .ob file if it exists */
                remove(obFileName);
            }
            free(obFileName);
        }

        fprintf(stderr, "\nErrors found in file. program stopped.\n");
        fprintf(stderr, "Total Errors in file %s: %d\n", fileName, errorCount);
        fclose(file);
        free(asmFileName);
        return;
    }
    free(asmFileName);

    printLabels(fileName);
    fclose(macroFile);
}

/**
 * Processes multiple lines of a macro, laying out their content.
 * @param lineNumber The starting line number in the source file.
 * @param linesNumber The total number of lines to process.
 * @param fileName The name of the source file.
 * @param macroLineInFile The line number of the macro in the source file.
 */
void layoutBulkOfLines(int lineNumber, int linesNumber, char *fileName, int macroLineInFile)
{
    char line[81];
    int currentLine;
    int i = 0;
    char *asmFileName;
    char *macroFileName;
    FILE *file;

    fileName = removeFileNameExtension(fileName);

    asmFileName = getFileNameWithExtension(fileName, ASM_FILE_NAME_EXTENSION);
    if (!asmFileName)
    {
        return;
    }

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
        startFirstRun(line, i + macroLineInFile, fileName);
        currentLine++;
        i++;
    }

    /*setUpStaticVariables();*/

    free(macroFileName);
    fclose(file);
}

/*void setUpStaticVariables(){
    initCommandsIdentifierStaticVariable();
    initErrorsCounter();
    initStaticVariable();
    initMacroStaticVariables();
}*/
