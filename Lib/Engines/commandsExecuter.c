#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define commandsFileSuffixType '.as'

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

static struct Line *linesHead = NULL;
struct Line *(*functionsArray[16])(struct Line line) = 
    {mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop};

void insertNewCommand(struct Line line, char *fileName)
{
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

    struct Line *currentLine = linesHead;
    while (currentLine->next != NULL)
        currentLine = currentLine->next;
    currentLine->next = newLine;

    writeToFile(fileName + commandsFileSuffixType, newLine->originalCommand);
}

void executeCommands()
{
    if (getErrorsCounter() > 0)
    {
        printf("Errors found, aborting.\n");
        return;
    }

    struct Line *jumpTo = NULL;
    while (linesHead != NULL) // we run on the head because after the execution of the commands we delete the lines
    {
        if (linesHead->commandIndex >= 0 && linesHead->commandIndex < sizeof(functionsArray) / sizeof(functionsArray[0]))
        {
            jumpTo = functionsArray[linesHead->commandIndex](*linesHead); // TODO: split the commands that return pointer from the void functions

            if (jumpTo != NULL)
                linesHead = jumpTo;
            else
                linesHead = linesHead->next;

            jumpTo = NULL;
        }
        else
        {
            printf("Error: command index is out of bounds.\n");
            return;
        }
    }
}

// TODO: IMPLEMENT THE FOLLOWING FUNCTIONS IN DIFFRENT FILE
// all the functions will return a pointer to the next line to execute or null if its not jump function
struct Line *mov(struct Line line) {}
struct Line *cmp(struct Line line) {}
struct Line *add(struct Line line) {}
struct Line *sub(struct Line line) {}
struct Line *not(struct Line line) {}
struct Line *clr(struct Line line) {}
struct Line *lea(struct Line line) {}
struct Line *inc(struct Line line) {}
struct Line *dec(struct Line line) {}
struct Line *jmp(struct Line line) {}
struct Line *bne(struct Line line) {}
struct Line *red(struct Line line) {}
struct Line *prn(struct Line line) {}
struct Line *jsr(struct Line line) {}
struct Line *rts(struct Line line) {}
struct Line *stop() {}