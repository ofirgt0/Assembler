#ifndef MACRO_SERVICE_H
#define MACRO_SERVICE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Structure for a line in the macro.
 */
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
 * Structure for a macro.
 */
struct Macro
{
    char macroName[50];
    struct Line *lines;
    struct Macro *next;
};

/**
 * Add a new macro to the list of macros.
 * @param macroName The name of the macro to add.
 * @param fileName  The name of the file associated with the macro.
 * @param line      The line to add to the macro.
 */
void addNewMacro(char *macroName, char *fileName, struct Line line);

/**
 * Get a macro by its name.
 * @param macroName The name of the macro to retrieve.
 * @return Pointer to the Macro struct if found, NULL otherwise.
 */
struct Macro *getMacro(char *macroName);

/**
 * Check if a macro with the given name exists.
 * @param macroName The name of the macro to check.
 * @return true if the macro exists, false otherwise.
 */
bool isMacroName(char *macroName);

/**
 * Check if a macro with the given name and line exists.
 * @param macroName The name of the macro to check.
 * @param fileName  The name of the file associated with the macro.
 * @param line      The line to check.
 * @return true if the macro with the line exists, false otherwise.
 */
void setExistMacro(char *macroName, struct Line line)
    /**
     * Send a macro with the given name for processing.
     * @param macroName The name of the macro to send.
     */
    void sendMacro(char *macroName);

#endif // MACRO_SERVICE_H
