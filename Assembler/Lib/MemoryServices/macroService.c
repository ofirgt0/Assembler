#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "macroService.h"

void remove_spaces(char *str);
void getBulkOfLines(int lineNumber, int linesNumber, char *fileName);
char *my_strdup(const char *s);

static struct macroDataNode *head = NULL;

/**
 * Checks if a given macro name exists.
 * @param macroName The name of the macro.
 * @return true if the macro name exists, false otherwise.
 */
bool isMacroName(char *macroName)
{
    printf("inside isMacroName\n");
    return getMacro(macroName) != NULL;
}

/**
 * Retrieves a macro based on its name.
 * @param macroName The name of the macro.
 * @return The macro structure if found, or NULL if not found.
 */
struct macroDataNode *getMacro(char *macroName)
{
    struct macroDataNode *macro;

    char *macroNameCopy = (char *)malloc(strlen(macroName) + 1);
    if (macroNameCopy == NULL)
    {
        perror("Memory allocation failed");
        return NULL;
    }
    strcpy(macroNameCopy, macroName);
    remove_spaces(macroNameCopy);

    macro = head;

    while (macro != NULL)
    {
        printf("%s strcmp %s \n", macro->macroName, macroNameCopy);
        if (strcmp(macro->macroName, macroNameCopy) == 0)
        {
            free(macroNameCopy);
            return macro;
        }
        macro = macro->next;
    }
    free(macroNameCopy);
    printf("after getMacro\n");
    return NULL;
}

void sendMacro(char *macroName, char *fileName)
{
    struct macroDataNode *macro = getMacro(macroName);
    printf("send macro: %s %d %d\n", macro->macroName, macro->lineNumber, macro->linesCount);
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
    newNode->macroName = my_strdup(macroName); /*Note: Remember to free this memory later*/
    newNode->lineNumber = lineNumber + 2;
    newNode->linesCount = 0;

    /*Insert the new node at the beginning of the list*/
    newNode->next = head;
    head = newNode;
}

void updateLinesCount(const char *macroName, int newLinesCount)
{
    struct macroDataNode *current;

    printf("update macro Lines Count: %s lines: %d\n", macroName, newLinesCount);

    current = head;

    while (current != NULL)
    {
        if (strcmp(current->macroName, macroName) == 0)
        {
            printf("line number %d to line %d\n", current->lineNumber, current->lineNumber + newLinesCount);
            current->linesCount = newLinesCount; /*Update the linesCount property*/
            return;
        }
        current = current->next;
    }
    printf("Error: Macro '%s' not found in the list.\n", macroName);
}

/* This function is used to free all the allocated memory when the program terminates. */
void freeMacroList()
{
    struct macroDataNode *current = head, *temp;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp->macroName);
        free(temp);
    }
}
