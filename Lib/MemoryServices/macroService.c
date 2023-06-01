#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure for a line in the macro
struct Line {
    char code;
    int opcode;
    int dstRegister;
    int srcRegister;
    struct Line* next;
};

struct Macro {
    char macroName[50];
    struct Line* lines;
    struct Macro* next;
};

static struct Macro* macroHead = NULL;

void addMacro(char* macroName, char* fileName, struct Line* linesHead){
    
}

getMacro(char* macroName){
    struct Macro* macro = macroHead;
    while (macro != NULL){
        if (strcmp(macro->macroName, macroName) == 0)
            return macro;
        macro = macro->next;
    }
    return NULL;
}

bool isMacroName(char* macroName){
    return getMacro(macroName) != NULL;
}
