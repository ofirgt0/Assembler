#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

bool setExistMacro(char* macroName){
    struct Macro* currentMacro = macroHead;
    while (currentMacro != NULL){
        if (strcmp(currentMacro->macroName, macroName) == 0)
            return true;
        currentMacro = currentMacro->next;
    }
    return false;
}

void sendMacro(){
    
}