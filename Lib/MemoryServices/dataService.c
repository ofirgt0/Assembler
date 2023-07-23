#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "errorsHandler.h"
#include "dataService.h"
#include "writeToFile.h"

#define MAX_LABEL_NAME_LENGTH 31

/* Initialize the label lists. */
static Label *externalLabelList = NULL;
static Label *entryLabelList = NULL;
static Label *normalCommandLabelList = NULL;
static DataLabel *dataLabelList = NULL;
static StringLabel *stringLabelList = NULL;

/* Initialize the global counters. */
int IC = 0; /* Instruction counter. */
int DC = 0; /* Data counter. */

/* Initialize the Code Image and Data Image. */
int *codeImage = NULL; /* Holds all the instruction words that we have promoted so far. */
int *dataImage = NULL; /* Holds all the data words that we have promoted so far. */

/*
 * Private function to search for a label in a given list.
 * @param labelList: The list in which to search.
 * @param labelName: The name of the label to search for.
 * @return The found label or NULL if the label was not found.
 */
Label *searchLabelInList(Label *labelList, const char *labelName)
{
    Label *currentLabel = labelList;
    while (currentLabel != NULL)
    {
        if (strcmp(currentLabel->name, labelName) == 0)
        {
            return currentLabel;
        }
        currentLabel = currentLabel->next;
    }
    return NULL;
}

/*
 * Function to search for a label in all linked lists.
 * @param labelName: The name of the label to search for.
 * @return The found label or NULL if the label was not found.
 */
Label *getLabel(const char *labelName)
{
    Label *label = NULL;
    Label *labelLists[] = {externalLabelList, entryLabelList, normalCommandLabelList,
                           (Label *)dataLabelList, (Label *)stringLabelList};
    for (int i = 0; i < sizeof(labelLists) / sizeof(Label *); i++)
    {
        label = searchLabelInList(labelLists[i], labelName);
        if (label != NULL)
            break;
    }
    return label;
}

/*
 * Function to add a new label by its type.
 * @param type: The type of the label.
 * @param address: The address of the label.
 * @param name: The name of the label.
 * @param additionalData: The additional data for the label (if any).
 * @return True if the label was added successfully, false otherwise.
 * The IC and DC counters are incremented appropriately in this function.
 */
bool addNewLabelByType(LabelType type, int address, const char *name, void *additionalData)
{
    Label *newLabel = NULL;
    /* Check if the label already exists */
    Label *existingLabel = getLabel(name);
    if (existingLabel != NULL)
    {
        /* If the label already exists, report an error - By reference from the "errorsHandler" file. */
        EXISTING_LABEL(name, __FILE__, __LINE__);
        return false;
    }
    switch (type)
    {
    case External:
        newLabel = (Label *)malloc(sizeof(Label));
        newLabel->next = externalLabelList;
        externalLabelList = newLabel;
        IC += sizeof(int);
        codeImage[IC] = address; /* Update the Code Image. */
        break;
    case Entry:
        newLabel = (Label *)malloc(sizeof(Label));
        newLabel->next = entryLabelList;
        entryLabelList = newLabel;
        IC += sizeof(int);
        codeImage[IC] = address; /* Update the Code Image. */
        break;
    case normalCommand:
        newLabel = (Label *)malloc(sizeof(Label));
        newLabel->next = normalCommandLabelList;
        normalCommandLabelList = newLabel;
        IC += sizeof(int);
        codeImage[IC] = address; /* Update the Code Image. */
        break;
    }

    if (newLabel == NULL)
    {
        printf("Error: Allocation has failed.\\n");
        return false;
    }

    newLabel->address = address;
    strcpy(newLabel->name, name);

    return true;
}
