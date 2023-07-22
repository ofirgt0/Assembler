struct Line
{ 
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

struct Macro {
    char macroName[50];
    struct Line* lines;
    struct Macro* next;
};