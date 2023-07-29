#ifndef MACROSERVICE_H
#define MACROSERVICE_H

#include <stdbool.h>

/**
 * Checks if a given macro name exists.
 * @param macroName The name of the macro.
 * @return true if the macro name exists, false otherwise.
 */
bool isMacroName(char* macroName);

/**
 * Retrieves a macro based on its name.
 * @param macroName The name of the macro.
 * @return The macro structure if found, or NULL if not found.
 */
struct macroDataNode* getMacro(char* macroName);

/**
 * Sends the macro data to a given file.
 * @param macroName The name of the macro.
 * @param fileName The name of the output file.
 */
void sendMacro(char* macroName, char* fileName);

/**
 * Adds a new macro to the list of macros.
 * @param macroName The name of the macro to be added.
 * @param lineNumber The line number where the macro is defined.
 */
void addMacro(const char* macroName, int lineNumber);

/**
 * Searches for a macro node with the given macro name.
 * @param macroName The name of the macro to search for.
 * @return The macro node if found, or NULL if not found.
 */
struct macroDataNode* searchNode(const char* macroName);

/**
 * Updates the line count of a macro in the list of macros.
 * @param macroName The name of the macro to update.
 * @param newLinesCount The new line count for the macro.
 */
void updateLinesCount(const char* macroName, int newLinesCount);

#endif /* MACROSERVICE_H */