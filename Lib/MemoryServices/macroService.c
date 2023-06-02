#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure for a line in the macro
struct Line
{ // TODO: MOVE THIS STRUCT TO DATA STRUCTURE
    int commandIndex;
    char code;
    int opcode;
    int dstRegister;
    int srcRegister;
    int address;
    char *label;
    char *originalCommand;
    struct Line *next;
};

struct Macro
{
    char macroName[50];
    struct Line *lines;
    struct Macro *next;
};

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
    }
}