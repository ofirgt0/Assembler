#ifndef MACRO_SERVICE_H
#define MACRO_SERVICE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Adds a new macro to the list of macros.
 * @param macroName The name of the macro.
 * @param fileName The name of the file associated with the macro.
 * @param line The line structure representing the macro line to be added.
 */
void addNewMacro(char *macroName, char *fileName, struct Line line);

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

#endif // MACRO_SERVICE_H

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

    /*void addNode(struct linkedList *list, const char *macroName, int lineNumber, int linesCount);*/

    /**
     * Get a macro by its name.
     * @param macroName The name of the macro to retrieve.
     * @return Pointer to the Macro struct if found, NULL otherwise.
     */
    struct Macro *getMacro(char *macroName);

/**
 * Sends the bulk of lines corresponding to a macro to a specified file.
 *
 * @param macroName The name of the macro to send the lines of.
 * @param fileName The name of the file to write the lines to.
 */
void sendMacro(char *macroName, char *fileName);

/**
 * Adds a new macro node to the linked list.
 *
 * @param macroName The name of the macro to be added.
 * @param lineNumber The line number where the macro is defined.
 */
void addMacro(const char *macroName, int lineNumber);

/**
 * Searches for a macro node with the given macroName.
 * @param macroName The name of the macro to search for.
 * @return A pointer to the macro structure if found, or NULL if not found.
 */
struct Macro *searchNode(const char *macroName);

/**
 * Updates the linesCount property of the specified macro.
 * @param macroName The name of the macro whose linesCount to update.
 * @param newLinesCount The new value of the linesCount property.
 */
void updateLinesCount(const char *macroName, int newLinesCount);

#endif /* LINKEDLIST_H */
