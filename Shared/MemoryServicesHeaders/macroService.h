#ifndef MACROSERVICE_H
#define MACROSERVICE_H

/* Forward declaration of the opaque struct. */
typedef struct macroDataNode
{
    char *macroName;
    int lineNumber;
    int linesCount;
    struct macroDataNode *next;
};

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
void sendMacro(char *macroName, char *fileName);

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

/**
 * Duplicates a string by creating a new copy in the heap.
 * This function allocates memory for the new string, copies
 * the original string into the new memory, and returns a pointer to it.
 * @param s The string to be duplicated.
 * @return A pointer to the newly duplicated string, or NULL if the operation fails.
 */
char *my_strdup(const char *s);

#endif /* MACROSERVICE_H */