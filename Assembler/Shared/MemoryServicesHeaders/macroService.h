#ifndef MACROSERVICE_H
#define MACROSERVICE_H

/* Forward declaration of the opaque struct. */
typedef struct macroDataNode
{
    char *macroName;
    int lineNumber;
    int linesCount;
    struct macroDataNode *next;
} macroDataNode;

/**
 * Checks if a given macro name exists.
 * @param macroName The name of the macro.
 * @return true if the macro name exists, false otherwise.
 */
bool isMacroName(char *macroName);

/**
 * Retrieves a macro based on its name.
 * @param macroName The name of the macro.
 * @return The macro structure if found, or NULL if not found.
 */
struct macroDataNode *getMacro(char *macroName);

/**
 * Sends a macro to a file.
 * @param macroName The name of the macro.
 * @param fileName The name of the file.
 */
/*void sendMacro(char *macroName, char *fileName);*/

/**
 * Adds a new macro.
 * @param macroName The name of the macro.
 * @param lineNumber The line number of the macro.
 */
void addMacro(const char *macroName, int lineNumber);

/**
 * Searches a node by macro name.
 * @param macroName The name of the macro.
 * @return The macro data node if found, or NULL if not found.
 */
struct macroDataNode *searchNode(const char *macroName);

/**
 * Updates the lines count of a macro.
 * @param macroName The name of the macro.
 * @param newLinesCount The new lines count.
 */
void updateLinesCount(const char *macroName, int newLinesCount);

void macroLayout(char *macroName, char *fileName, int macroLineInFile);

#endif /* MACROSERVICE_H */