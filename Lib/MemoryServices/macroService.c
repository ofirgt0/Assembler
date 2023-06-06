#include "macroService.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static struct Macro *macroHead = NULL;

void addNewMacro(char *macroName, char *fileName, struct Line line)
{
    struct Macro *newMacro = getMacro(macroName);
    if (newMacro == NULL)
    {
        newMacro = (struct Macro *)malloc(sizeof(struct Macro));

        if (newMacro == NULL)
        {
            // Error: Memory allocation failed
            printf("Error: Memory allocation failed\n");
            return;
        }
        strcpy(newMacro->macroName, macroName);
        newMacro->next = NULL;

        struct Macro *currentMacro = macroHead;
        while (currentMacro->next != NULL)
            currentMacro = currentMacro->next;

        currentMacro->next = newMacro;
    }

    struct Line *newLine = (struct Line *)malloc(sizeof(struct Line));

    if (newLine == NULL)
    {
        // Error: Memory allocation failed
        printf("Error: Memory allocation failed\n");
        return;
    }

    newLine->code = line.code;
    newLine->opcode = line.opcode;
    newLine->dstRegister = line.dstRegister;
    newLine->srcRegister = line.srcRegister;
    newLine->address = line.address;
    newLine->label = line.label;
    newLine->originalCommand = line.originalCommand;
    newLine->commandIndex = line.commandIndex;
    newLine->next = NULL;

    struct Line *currentLine = macroHead->lines;
    while (currentLine->next != NULL)
        currentLine = currentLine->next;
    currentLine->next = newLine;
}

struct Macro *getMacro(char *macroName)
{
    struct Macro *macro = macroHead;
    while (macro != NULL)
    {
        if (strcmp(macro->macroName, macroName) == 0)
            return macro;
        macro = macro->next;
    }
    return NULL;
}

bool isMacroName(char *macroName)
{
    return getMacro(macroName) != NULL;
}

bool setExistMacro(char *macroName, char *fileName, struct Line line)
{
    // struct Macro* currentMacro = macroHead;
    // while (currentMacro != NULL){
    //     if (strcmp(currentMacro->macroName, macroName) == 0)
    //         return true;
    //     currentMacro = currentMacro->next;
    // }
    // return false;
    // TODO: Implement the logic for checking if a macro with the given name and line exists.
    //       You can uncomment and modify the provided code according to your requirements.
}

void sendMacro(char *macroName)
{
    struct Macro *macro = getMacro(macroName);
    if (macro == NULL)
    {
        // Error: Macro not found
        printf("Error: Macro not found\n");
        return;
    }
    struct Line *currentLine = macro->lines;
    while (currentLine != NULL)
    {
        // TODO: Process the lines of the macro
        currentLine = currentLine->next;
    }
}