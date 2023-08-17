#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define ASM_FILE_NAME_EXTENSION ".as"
#define MACRO_FILE_NAME_EXTENSION ".am"

/**
 * Initializes the commands identifier static variable.
 * This function sets up the static variable used for identifying commands.
 * It should be called before processing any commands.
 */
void initCommandsIdentifierStaticVariable();

/**
 * Initializes the errors counter static variable.
 * This function sets up the static variable used for counting errors encountered
 * during file processing. It should be called before any file processing starts.
 */
void initErrorsCounter();

/**
 * Initializes other necessary static variables.
 * This function sets up other static variables which might be used in the program.
 * Call this function to ensure all static variables are in their initial states.
 */
void initStaticVariable();

/**
 * Initializes the macro static variables.
 * This function sets up static variables specifically used for macro operations.
 * It should be called before any macro-related processing starts.
 */
void initMacroStaticVariables();

/**
 * Reads and processes an assembly file.
 * @param fileName The name of the file to be read.
 */
void fileReader(const char *fileName);

/**
 * Removes leading white spaces from a given string.
 * @param command The string from which prefix spaces need to be removed.
 */
void removePrefixSpaces(char *command);

/**
 * Returns the full file name with the desired extension.
 * @param fileName The base name of the file.
 * @param extension The desired extension to append to the file name.
 * @return A new string containing the full file name with the extension.
 */
char *getFileNameWithExtension(const char *fileName, char *extension);

/**
 * Handles the layout of bulk lines during the assembly processing.
 * @param lineNumber The starting line number.
 * @param linesNumber The number of lines to layout.
 * @param fileName The name of the file being processed.
 * @param macroLineInFile The line number of the macro in the file.
 */
void layoutBulkOfLines(int lineNumber, int linesNumber, char *fileName, int macroLineInFile);
/**
 * Removes the file extension from a given filename.
 * @param filename The original filename.
 * @return A new string with the extension removed.
 */
char *removeFileNameExtension(const char *filename);

/**
 * Prepares for the second run of the file reader.
 * @param fileName The name of the source file.
 */
void prepareSecondRun(const char *fileName);

/**
 * Prints the labels from a given source file.
 * @param fileName The name of the source file.
 */
void printLabels(const char *);

/**
 * Sets up all necessary static variables used in the program.
 * This function initializes various static variables across different modules.
 * It ensures that the program's environment is set up correctly before any
 * significant processing starts. Specifically, it prepares variables for
 * command identification, error counting, general static variables, and
 * macro-related static variables.
 */
void setUpStaticVariables();

#endif /* ASSEMBLER */