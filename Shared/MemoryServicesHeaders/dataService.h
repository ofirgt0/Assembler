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
extern Label *externalLabelList;
extern Label *entryLabelList;
extern Label *normalCommandLabelList;
extern DataLabel *dataLabelList;
extern StringLabel *stringLabelList;

/* Representing the types of labels. */
typedef enum
{
    External,     /* Represents an external label. */
    Entry,        /* Represents an entry label. */
    normalCommand /* Represents a normal command label. */
} LabelType;

/* Define the label structure */
typedef struct label
{
    char name[MAX_LABEL_NAME_LENGTH]; /* Holds the label's name. */
    int address;                      /* Holds the label's address. */
    struct label *next;               /* Points to the next label in the linked list. */
} Label;

/* Representing the data label structure. */
typedef struct dataLabel
{
    Label label;
    int *data;
} DataLabel;

/* Representing the string label structure. */
typedef struct stringLabel
{
    Label label;
    char *string;
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
