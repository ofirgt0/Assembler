#ifndef DATASERVICE_H
#define DATASERVICE_H
#include <stdbool.h>
#include "errorsHandler.h"

/* Extern declarations for global variables */
extern int IC;
extern int DC;
extern int **data;
extern char **strings;
extern int dataCount;
extern int stringsCount;

/* Define an enumeration to represent the type of a label */
typedef enum
{
    Ext,
    Entry,
    Command,
    Data,
    Strings
} LabelType;

/* Define the label structure */
typedef struct label
{
    char name[MAX_LABEL_NAME_LENGTH];
    LabelType type;
    int address;
    struct label *next;
} Label;

/**
 * Classifies a label by type and retrieves the pointer to its list.
 * @param type The type of the label.
 * @return A pointer to the list of labels for the given type.
 */
Label **classifyLabelByType(LabelType type);

/**
 * Retrieves a label by its name from the hash table.
 * @param labelName The label name to search for.
 * @param type The type of the label.
 * @return A pointer to the found label, or NULL if not found.
 */
Label *getLabel(const char *labelName, LabelType type);

/**
 * Adds a new label to the hash table.
 * @param type The type of the label (Ext, Entry, Command).
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewLabel(LabelType type, int address, const char *name);

/**
 * Adds a new data or strings label to the list.
 * @param type The type of the label (DATA or STRINGS).
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewDataOrStringsLabel(LabelType type, int address, const char *name);

/**
 * Adds a new label to the hash table.
 * @param Command The type of the label.
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewCommandLabel(int address, const char *name);

/**
 * Adds a new external label to the list.
 * @param type The type of the label (Ext, Entry, Command).
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewExternalLabel(LabelType type, int address, const char *name);

/**
 * Adds a new entry label to the list.
 * @param type The type of the label (Ext, Entry, Command).
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewEntryLabel(LabelType type, int address, const char *name);

#endif /* DATASERVICE_H */
