#ifndef COMMANDS_EXECUTER_H
#define COMMANDS_EXECUTER_H

#include "commandsIdentifier.h"

#define commandsFileSuffixType ".as"

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

/**
 * Inserts a new command into the command list.
 * @param line The Line structure representing the command.
 * @param fileName The name of the file.
 */
void insertNewCommand(struct Line line, char *fileName);

/**
 * Executes the commands in the command list.
 */
void executeCommands();

struct Line *mov(struct Line line);
struct Line *cmp(struct Line line);
struct Line *add(struct Line line);
struct Line *sub(struct Line line);
struct Line * not(struct Line line);
struct Line *clr(struct Line line);
struct Line *lea(struct Line line);
struct Line *inc(struct Line line);
struct Line *dec(struct Line line);
struct Line *jmp(struct Line line);
struct Line *bne(struct Line line);
struct Line *red(struct Line line);
struct Line *prn(struct Line line);
struct Line *jsr(struct Line line);
struct Line *rts(struct Line line);
struct Line *stop();

#endif /* COMMANDS_EXECUTER_H */
