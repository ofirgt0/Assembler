#include "macroService.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static struct Macro *macroHead = NULL;

/**
 * Adds a new macro to the list of macros.
 * @param macroName The name of the macro.
 * @param fileName The name of the file associated with the macro.
 * @param line The line structure representing the macro line to be added.
 */
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
        newMacro->lines = NULL;

        if (macroHead == NULL)
        {
            macroHead = newMacro;
        }
        else
        {
            struct Macro *currentMacro = macroHead;
            while (currentMacro->next != NULL)
                currentMacro = currentMacro->next;

            currentMacro->next = newMacro;
        }
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

    if (macroHead->lines == NULL)
    {
        macroHead->lines = newLine;
    }
    else
    {
        struct Line *currentLine = macroHead->lines;
        while (currentLine->next != NULL)
            currentLine = currentLine->next;
        currentLine->next = newLine;
    }
}



/**
 * Checks if a given macro name exists.
 * @param macroName The name of the macro.
 * @return true if the macro name exists, false otherwise.
 */
bool isMacroName(char *macroName)
{
    return getMacro(macroName) != NULL;
}

/**
 * Sets the last line of a macro with the provided values.
 * @param macroName The name of the macro.
 * @param line The line containing the values to set.
 */
void setExistMacro(char *macroName, struct Line line)
{
    struct Macro *macro = getMacro(macroName);

    if (macro == NULL)
    {
        /*Macro does not exist*/
        printf("Error: Macro '%s' does not exist.\n", macroName);
        return;
    }

    /*Find the last line of the macro*/
    struct Line *lastLine = macro->lines;
    if (lastLine == NULL)
    {
        /*Macro has no lines*/
        printf("Error: Macro '%s' has no lines.\n", macroName);
        return;
    }

    while (lastLine->next != NULL)
        lastLine = lastLine->next;

    /*Set the values of the last line*/
    lastLine->code = line.code;
    lastLine->opcode = line.opcode;
    lastLine->dstRegister = line.dstRegister;
    lastLine->srcRegister = line.srcRegister;
    lastLine->address = line.address;
    lastLine->label = line.label;
    lastLine->originalCommand = line.originalCommand;
    lastLine->commandIndex = line.commandIndex;
    lastLine->next = NULL;

    printf("Last line of macro '%s' has been set.\n", macroName);
}



// -----------------------------------------------------------------------------------------------

typedef struct macroDataNode
{
    char* macroName;
    int lineNumber;
    int linesCount;
    struct macroDataNode* next;
};

/**
 * Retrieves a macro based on its name.
 * @param macroName The name of the macro.
 * @return The macro structure if found, or NULL if not found.
 */
struct macroDataNode *getMacro(char *macroName)
{
    struct macroDataNode *macro = macroHead;
    while (macro != NULL)
    {
        if (strcmp(macro->macroName, macroName) == 0)
            return macro;
        macro = macro->next;
    }
    return NULL;
}

void sendMacro(char *macroName, char *fileName)
{
    struct macroDataNode *macro = getMacro(macroName);
    if (macro == NULL)
    {
        // Error: Macro not found
        printf("Error: Macro not found\n");
        return;
    }
    getBulkOfLines(macro->lineNumber, macro->linesCount, fileName);
    
}

static struct linkedList* head = NULL;

void addMacro(const char* macroName, int lineNumber) {
    // Create a new node
    struct macroDataNode* newNode = (struct macroDataNode*)malloc(sizeof(struct macroDataNode));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    if(searchNode(macroName)!=NULL)
    {
        /// TODO: handle error which macro already exist 
    }

    // Copy the macroName to the new node
    newNode->macroName = strdup(macroName); // Note: Remember to free this memory later
    newNode->lineNumber = lineNumber;
    newNode->linesCount = 0;

    // Insert the new node at the beginning of the list
    newNode->next = head;
    head = newNode;
}

struct macroData* searchNode(const char* macroName) {
    struct macroDataNode* current = head;
    while (current != NULL) {
        if (strcmp(current->macroName, macroName) == 0) {
            return current; // Found the node with the given macroName
        }
        current = current->next;
    }
    return NULL; // Node with the given macroName not found
}

void updateLinesCount(const char* macroName, int newLinesCount) {
    struct macroDataNode* current = head;
    while (current != NULL) {
        if (strcmp(current->macroName, macroName) == 0) {
            current->linesCount = newLinesCount; // Update the linesCount property
            return;
        }
        current = current->next;
    }
    printf("Macro '%s' not found in the list.\n", macroName);
}