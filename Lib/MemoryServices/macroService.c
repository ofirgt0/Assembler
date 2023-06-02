#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure for a line in the macro
struct Line
{
    char code;
    int opcode;
    int dstRegister;
    int srcRegister;
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
    struct Macro *newMacro = (struct Macro *)malloc(sizeof(struct Macro));

    if (newMacro == NULL)
    {
        // Error: Memory allocation failed
        printf("Error: Memory allocation failed\n");
        return;
    }
    strcpy(newMacro->macroName, macroName);
    newMacro->lines = line; // TODO: FIX THIS - ERROR WHILE POINTER = STRUCT
    newMacro->next = NULL;

    struct Macro *currentMacro = macroHead;
    while (currentMacro->next != NULL)
    {
        currentMacro = currentMacro->next;
    }
    currentMacro->next = newMacro;

    if (isFileExist(fileName))
    {
        writeToFile(fileName, "New row to be added.");
    }
}

getMacro(char *macroName)
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

bool setExistMacro(char *macroName)
{
    // struct Macro* currentMacro = macroHead;
    // while (currentMacro != NULL){
    //     if (strcmp(currentMacro->macroName, macroName) == 0)
    //         return true;
    //     currentMacro = currentMacro->next;
    // }
    // return false;
}

void sendMacro()
{
}

///

//