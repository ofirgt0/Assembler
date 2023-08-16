#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "macroService.h"
#include "errorsHandler.h"
#include "helpfulFunctions.h"

void remove_spaces(char *str);
void layoutBulkOfLines(int lineNumber, int linesNumber, char *fileName, int macroLineInFile);

static struct macroDataNode *head = NULL;

void initMacroStaticVariables(){
    *head=NULL;
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
 * Retrieves a macro node from the list based on its name.
 * @param macroName The name of the macro to retrieve.
 * @return Pointer to the macroDataNode if found, or NULL if not found.
 */
struct macroDataNode *getMacro(char *macroName)
{
    struct macroDataNode *macro;

    char *macroNameCopy = (char *)malloc(strlen(macroName) + 1);
    if (macroNameCopy == NULL)
    {
        MEMORY_ALLOCATION_FAILED(macroName, -1); 
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
    return NULL;
}

/**
 * Processes the layout of a specific macro. This involves checking the macro's 
 * line number and lines count, and laying out its content accordingly.
 * @param macroName: The name of the macro to layout.
 * @param fileName: The name of the file where the macro is located.
 * @param macroLineInFile: The line number where the macro starts in the file.
 */
void macroLayout(char *macroName, char *fileName, int macroLineInFile)
{
    struct macroDataNode *macro = getMacro(macroName);
    if (macro == NULL)
    {
        MACRO_NAME_CONFLICT(fileName, macroLineInFile);
    }
    else
    {
        printf("layout macro: %s %d %d\n", macro->macroName, macro->lineNumber, macro->linesCount);
        layoutBulkOfLines(macro->lineNumber, macro->linesCount, fileName, macroLineInFile);
    }
}

/**
 * Searches for a specific macro node in the list.
 * @param macroName The name of the macro to search for.
 * @return Pointer to the macroDataNode if found, or NULL if not found.
 */
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

/**
 * Adds a new macro to the list. If a macro with the same name already exists, 
 * it flags a name conflict error.
 * @param macroName: The name of the macro to add.
 * @param lineNumber: The line number where the macro starts in the file.
 */
void addMacro(const char *macroName, int lineNumber)
{
    /*Create a new node*/
    struct macroDataNode *newNode = (struct macroDataNode *)malloc(sizeof(struct macroDataNode));
    if (newNode == NULL)
    {
        MEMORY_ALLOCATION_FAILED(macroName, lineNumber);
    }
    if (searchNode(macroName) != NULL)
    {
        free(newNode);
        MACRO_NAME_CONFLICT(macroName, lineNumber);
        return;
    }

    /*Copy the macroName to the new node*/
    newNode->macroName = my_strdup(macroName);
    if (newNode->macroName == NULL)
    {
        free(newNode);
        MEMORY_ALLOCATION_FAILED(macroName, lineNumber);
    }
    newNode->lineNumber = lineNumber + 2;
    newNode->linesCount = 0;

    /*Insert the new node at the beginning of the list*/
    newNode->next = head;
    head = newNode;
}

/**
 * Updates the number of lines associated with a specific macro in the list.
 * @param macroName: The name of the macro to update.
 * @param newLinesCount: The new lines count for the macro.
 */
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
    MACRO_NAME_CONFLICT(macroName, newLinesCount);
}

/**
 * Frees all allocated memory associated with the macro list. 
 * This function is intended to be called when the program terminates.
 */
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

