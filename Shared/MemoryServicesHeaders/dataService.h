#ifndef DATASERVICE_H
#define DATASERVICE_H
#include <stdbool.h>
#include "errorsHandler.h"

/**
 * Searches for a label in all linked lists.
 * @param labelName The name of the label to search for.
 * @return The found label or NULL if the label was not found.
 */
LabelNode *tryGetLabel(const char *labelName);

/**
 * Searches for a data label in the list.
 * @param labelName The name of the label to search for.
 * @return The found data label or NULL if the label was not found.
 */
DataLabel *tryGetDataLabel(char *labelName);

/**
 * Searches for a string label in the list.
 * @param labelName The name of the label to search for.
 * @return The found string label or NULL if the label was not found.
 */
StringLabel *tryGetStringLabel(char *labelName);

/**
 * Checks if a label exists in any of the lists.
 * @param labelName The name of the label to check for.
 * @return True if the label exists, false otherwise.
 */
bool isLabelExist(char *labelName);

/**
 * Determines the type of a label.
 * @param labelType The type string of the label.
 * @return The LabelType enum value corresponding to the type string.
 */
LabelType determineLabelType(char *labelType);

/**
 * Tries to add a new label by its type.
 * @param type The type string of the label.
 * @param labelName The name of the label.
 * @return True if the label was added successfully, false otherwise.
 */
bool tryAddNewLabel(char *type, char *labelName);

/**
 *
 */
void addNewLine1(char *prefixLabel, int opcode, int immidiate1, int register2);

/**
 *
 */
void addNewLine3(char *prefixLabel, int opcode, int label1, int register2);

/**
 *
 */
void addNewLine5(char *prefixLabel, int opcode, int register1, int register2);

/**
 *
 */
void addNewLine(int opcode, int register1, int register2, char *label1, char *label2, double immidiate1, double immidiate2);

/**
 * Validates if an opcode matches the addressing method.
 * @param opcode The opcode to validate.
 * @param srcAddressing The source addressing method.
 * @param dstAddressing The destination addressing method.
 * @return True if the opcode matches the addressing method, false otherwise.
 */
bool validateOpcodeMatchAddressingMethod(int opcode, int srcAddressing, int dstAddressing);

/**
 * Adds a new extern label.
 * @param externName The name of the extern label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addNewExtern(char *externName);

/**
 * Adds a new entry label.
 * @param entryName The name of the entry label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addNewEntry(char *entryName);

/**
 * Adds a new data label.
 * @param data The data array.
 * @param labelName The name of the data label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addData(int data[], char *labelName);

/**
 * Adds a new string label.
 * @param string The string.
 * @param labelName The name of the string label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addString(char *string, char *labelName);

/**
 * Adds a new label.
 * @return True if the label was added successfully, false otherwise.
 */
bool addNewLabel();

/**
 * Searches for an extern label.
 * @param externName The name of the extern label to search for.
 * @return The address of the extern label if found, -1 otherwise.
 */
int searchExternLabel(char *externName);

/**
 * Searches for an entry label.
 * @param entryName The name of the entry label to search for.
 * @return The address of the entry label if found, -1 otherwise.
 */
int searchEntry(char *entryName);

/**
 * Searches for a data label.
 * @param data The data array.
 * @param labelName The name of the data label to search for.
 * @return The address of the data label if found, -1 otherwise.
 */
int searchDataLabel(int data[], char *labelName);

/**
 * Searches for a string label.
 * @param string The string.
 * @param labelName The name of the string label to search for.
 * @return The address of the string label if found, -1 otherwise.
 */
int searchStringLabel(char *string, char *labelName);

#endif /* DATA_SERVICE_H */