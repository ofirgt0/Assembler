#ifndef DATASERVICE_H
#define DATASERVICE_H
#include <stdbool.h>
#include "errorsHandler.h"

/* Extern declarations for global variables */
extern int IC;         /* Instruction counter. */
extern int DC;         /* Data counter. */
extern int *codeImage; /* Holds all the instruction words. */
extern int *dataImage; /* Holds all the data words. */

/* Initialize the label lists. */
extern static LabelNode *externalLabelList;
extern static LabelNode *entryLabelList;
extern static LabelNode *normalCommandLabelList;
extern static DataLabel *dataLabelList;
extern static StringLabel *stringLabelList;

/* Representing the types of labels. */
typedef enum
{
    External,     /* Represents an external label. */
    Entry,        /* Represents an entry label. */
    NormalCommand /* Represents a normal command label. */
} LabelType;

/* Define the label structure */
typedef struct Label
{
    LabelType type;
    char name[MAX_LABEL_NAME_LENGTH]; /* Holds the label's name. */
    int address;                      /* Holds the label's address. */
} Label;

typedef struct LabelNode
{
    Label *label;
    LabelNode *next;               /* Points to the next label in the linked list. */
} LabelNode;

/* Representing the data label structure. */
typedef struct DataLabel
{
    Label *label;
    int *data;
    DataLabel *next;
} DataLabel;

/* Representing the string label structure. */
typedef struct StringLabel
{
    Label *label;
    char *string;
    StringLabel *next;  
} StringLabel;

/* Function to search for a label in all linked lists.
 * @param labelName: The name of the label to search for.
 * @return The found label or NULL if the label was not found.
 */
Label *getLabel(const char *labelName);

/**
 * Function to add a new label by its type.
 * @param type: The type of the label.
 * @param address: The address of the label.
 * @param name: The name of the label.
 * @param additionalData: The additional data for the label (if any).
 * @return True if the label was added successfully, false otherwise.
 */
bool addNewLabelByType(LabelType type, int address, const char *name, void *additionalData);

#endif /* DATASERVICE_H */
