#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "macroService.h"

static struct macroDataNode *head = NULL;

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
 * Retrieves a macro based on its name.
 * @param macroName The name of the macro.
 * @return The macro structure if found, or NULL if not found.
 */
struct macroDataNode *getMacro(char *macroName)
{
    printf("getMacro: start\n");

    struct macroDataNode *macro = head;
    while (macro != NULL)
    {
        if (strcmp(macro->macroName, macroName) == 0)
        {
            printf("getMacro: found macro with macroName: %s\n", macroName);
            return macro;
        }
        macro = macro->next;
    }
    printf("getMacro: macro not found with macroName: %s\n", macroName);

    return NULL;
}

void sendMacro(char *macroName, char *fileName)
{
    printf("sendMacro: start\n");

    struct macroDataNode *macro = getMacro(macroName);
    if (macro == NULL)
    {
        /*Error: Macro was not found*/
        printf("Error: Macro not found\n");
        return;
    }
    getBulkOfLines(macro->lineNumber, macro->linesCount, fileName);
}

struct macroDataNode *searchNode(const char *macroName)
{
    struct macroDataNode *current = head;
    while (current != NULL)
    {
        if (strcmp(current->macroName, macroName) == 0)
        {
            return current; /*Found the node with the given macroName*/
        }
        current = current->next;
    }
    return NULL; /*Node with the given macroName not found*/
}

void addMacro(const char *macroName, int lineNumber)
{
    printf("addMacro: start\n");

    /*Create a new node*/
    struct macroDataNode *newNode = (struct macroDataNode *)malloc(sizeof(struct macroDataNode));
    if (newNode == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    if (searchNode(macroName) != NULL)
    {
        /*TODO: handle error which macro already exist */
    }

    /*Copy the macroName to the new node*/
    newNode->macroName = strdup(macroName); /*Note: Remember to free this memory later*/
    newNode->lineNumber = lineNumber + 2;
    newNode->linesCount = 0;

    /*Insert the new node at the beginning of the list*/
    newNode->next = head;
    head = newNode;

    printf("addMacro: added a macro with macroName: %s, lineNumber: %d\n", macroName, lineNumber);
}

void updateLinesCount(const char *macroName, int newLinesCount)
{
    printf("update macro Lines Count: %s lines: %d\n", macroName, newLinesCount);
    struct macroDataNode *current = head;
    while (current != NULL)
    {
        if (strcmp(current->macroName, macroName) == 0)
        {
            printf("line number %d to line %d\n", current->lineNumber, current->lineNumber + newLinesCount);
            current->linesCount = newLinesCount; /*Update the linesCount property*/
            printf("updateLinesCount: updated lines count for macroName: %s, newLinesCount: %d\n", macroName, newLinesCount);
            return;
        }
        current = current->next;
    }
    printf("Error: Macro '%s' not found in the list.\n", macroName);
}
