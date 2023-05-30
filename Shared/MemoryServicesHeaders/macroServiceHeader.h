#include <stdio.h>
#include <stdlib.h>
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
