#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "macroService.h"
#include "filesReader.h"

static struct macroDataNode *head = NULL;

/**
 * Duplicates a string by creating a new copy in the heap.
 * This function allocates memory for the new string, copies
 * the original string into the new memory, and returns a pointer to it.
 */
char *my_strdup(const char *s)
{
    char *new = (char *)malloc(strlen(s) + 1); /*+1 for the null-terminator*/
    if (new == NULL)
        return NULL;
    strcpy(new, s);
    return new;
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
 * Retrieves a macro based on its name.
 * @param macroName The name of the macro.
 * @return The macro structure if found, or NULL if not found.
 */
struct macroDataNode *getMacro(char *macroName)
{
    struct macroDataNode *macro = head;
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
    struct macroDataNode *macro = NULL;

    macro = getMacro(macroName);
    if (macro == NULL)
    {
        /*Error: Macro was not found*/
        printf("Error: Macro not found\n");
        return;
    }
    getBulkOfLines(macro->lineNumber, macro->linesCount, fileName);
}

void addMacro(const char *macroName, int lineNumber)
{
    /*Create a new node*/
    struct macroDataNode *newNode = NULL;
    newNode = (struct macroDataNode *)malloc(sizeof(struct macroDataNode));
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
    newNode->lineNumber = lineNumber;
    newNode->linesCount = 0;

    /*Insert the new node at the beginning of the list*/
    newNode->next = head;
    head = newNode;
}

struct macroData *searchNode(const char *macroName)
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

void updateLinesCount(const char *macroName, int newLinesCount)
{
    struct macroDataNode *current = head;

    while (current != NULL)
    {
        if (strcmp(current->macroName, macroName) == 0)
        {
            current->linesCount = newLinesCount; /*Update the linesCount property*/
            return;
        }
        current = current->next;
    }
    printf("Macro '%s' not found in the list.\n", macroName);
}
