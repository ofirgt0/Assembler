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