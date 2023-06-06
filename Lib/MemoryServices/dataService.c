#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "errorsHandler.h"
#define MAX_LABEL_NAME_LENGTH 100

/*counter of the instructions addresses.*/
extern int IC;

/*counter of the data addresses.*/
extern int DC;

/*dynamic array of .data instructions.*/
extern int **data;

/*dynamic array of .string instructions.*/
extern char **strings;

/*dynamic array of labels structures (externals, entries, commands).*/
extern Label *labels;
extern int labelsPhySize;     /*labels array physical size (memory allocated).*/
extern int labelsLogicalSize; /*labels array logical size (that amount of labels that exists in the input file).*/

// TODO: add structure for binary words
/*
* addNewLabel:
  @input: newLabel
  - add the label to the array based on the the label type (externals, entries, commands).
* getLabel:
  @input: labelName
  - the function return the address of the label. if not exist - return default 0.

- note: consider the option to use generic function that will do the functionallty and few func that will call this one with rellevant file name and list.

 /* Define an enumeration to represent the type of a label */
typedef enum
{
    Ext,
    Entry,
    Command
} LabelType;

// this is the definitions of a label.
typedef struct label
{
    char name[MAX_LABEL_NAME_LENGTH];
    LabelType type;
    int address;
    // pIntNode extAddrs;

} Label;

Label *getLabel(char *labelName)
{
    for (int i = 0; i < labelsLogicalSize; i++)
    {
        if (strcmp(labels[i].name, labelName) == 0)
        {
            return &labels[i];
        }
    }
    return NULL;
}

bool addNewLabel(LabelType type, int address, char name[MAX_LABEL_NAME_LENGTH])
{
    for (int i = 0; i < labelsLogicalSize; i++)
    {
        if (strcmp(labels[i].name, name) == 0)
        {
            EXISTING_LABEL(name); /* Handle error: Label already exists */
        }
    }
    /* If the label does not already exist,
       a new label is created, and added to the labels array */

    // if the labels array is full: create an array twice as big and copy old array to new array.
    if (labelsLogicalSize >= labelsPhySize)
    {
        labelsPhySize *= 2;
        labels = realloc(labels, labelsPhySize * sizeof(Label));
        if (labels == NULL)
        {
            /* Error: Memory allocation failed */
            printf("Error: allocation has been failed\n");
            return false;
        }
    }
    /*update the labels array with the new label.*/
    labels[labelsLogicalSize].address = address;
    labels[labelsLogicalSize].type = type;
    strcpy(labels[labelsLogicalSize].name, name);
    labelsLogicalSize++;
    return true;
}
