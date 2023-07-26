#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "errorsHandler.h"
#include "dataService.h"
#include "writeToFile.h"

//TODO: validate if we need to move the define to the header file or should it stay here
#define EXTERN_LABEL_TYPE "extern"
#define ENTRY_LABEL_TYPE "entry"
#define NORMAL_LABEL_TYPE ""

#define MAX_LABEL_NAME_LENGTH 31

/* Initialize the label lists. */
static LabelNode *externalLabelList = NULL;
static LabelNode *entryLabelList = NULL;
static LabelNode *normalCommandLabelList = NULL;
static DataLabel *dataLabelList = NULL;
static StringLabel *stringLabelList = NULL;

/* Initialize the global counters. */
static int IC = 0; /* Instruction counter. */
static int DC = 0; /* Data counter. */

/*
 * Function to search for a label in all linked lists.
 * @param labelName: The name of the label to search for.
 * @return The found label or NULL if the label was not found.
 */
LabelNode *tryGetLabel(const char *labelName)
{
    LabelNode *label = NULL;
    LabelNode *labelLists[] = {externalLabelList, normalCommandLabelList};
    for (int i = 0; i < sizeof(labelLists) / sizeof(LabelNode *); i++)
    {
        label = searchLabelInList(labelLists[i], labelName);
        if (label != NULL)
            break;
    }

    return label;
}

DataLabel *tryGetDataLabel(char *labelName)
{
    DataLabel *currentDataLabel = dataLabelList;
    while (currentDataLabel != NULL)
    {
        if (strcmp(currentDataLabel->label->name, labelName) == 0)
        {
            break;
        }
        currentDataLabel = currentDataLabel->next;
    }
    return currentDataLabel;
}

//NOTE: we defenetly could use generics here but it isnt in the course scope :)
StringLabel *tryGetStringLabel(char *labelName)
{
    StringLabel *currentDataLabel = dataLabelList;
    while (currentDataLabel != NULL)
    {
        if (strcmp(currentDataLabel->label->name, labelName) == 0)
            break;
        
        currentDataLabel = currentDataLabel->next;
    }
    return currentDataLabel;
}

bool isLabelExist(char *labelName)
{
    return (tryGetDataLabel(labelName) || tryGetLabel(labelName) || tryGetStringLabel(labelName)); 
}

LabelType determineLabelType(char *labelType){
    return strcmp(EXTERN_LABEL_TYPE,labelType) ? External : 
        (strcmp(ENTRY_LABEL_TYPE,labelType) ? Entry : NormalCommand);  
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
bool tryAddNewLabel(char *type, char *labelName)
{
    /* Check if the label already exists */
    //Label *existingLabel = tryGetLabel(name);
    if (isLabelExist(labelName))
    {
        /* If the label already exists, report an error - By reference from the "errorsHandler" file. */
        EXISTING_LABEL(name, __FILE__, __LINE__);
        return false;
    }

    LabelNode *newLabel = NULL;
    newLabel = (LabelNode *)malloc(sizeof(LabelNode));

    LabelNode *relevantList =  strcmp(EXTERN_LABEL_TYPE, type) ? externalLabelList : 
        (strcmp(ENTRY_LABEL_TYPE, type) ? entryLabelList : normalCommandLabelList);

    newLabel->next = relevantList;
    relevantList = newLabel;
    DC += sizeof(int);

    switch (determineLabelType(type))
    {
        case External:
            codeImage[IC] = address; /* Update the Code Image. */
            break;
        case Entry:
            newLabel->next = entryLabelList;
            entryLabelList = newLabel;
            IC += sizeof(int);
            codeImage[IC] = address; /* Update the Code Image. */
            break;
        case NormalCommand:
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

    return true;
}


/// TODO: (liron u can do it) create insert label for each type

void addNewExtern(char* externName);
void addNewEntry(char* externName);
void addData(int data[], char* labelName);
void addString(char* string, char* labelName);
void addNewLabel();