#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LABEL_NAME_LENGTH 100

/*
* addNewLabel:
  @input: newLabel
  - add the label to the array.
  - write the label in the rellevant file.
* addNewExternal:
  @input: newLabel
  - add the label to the array.
  - write the label in the rellevant file.
* addNewEntry:
  @input: newLabel
  - add the label to the array.
  - write the label in the rellevant file.
* getLabel:
  @input: labelName
  - the function return the address of the label. if not exist - return default 0.

- note: consider the option to use generic function that will do the functionallty and few func that will call this one with rellevant file name and list.

*/

/**
 * labelName: An array of characters to store the label name
 * address: An integer to store the address associated with the label.
 * data: A dynamic 2D array of integers to store the data associated with the label.
 * strings: A dynamic 2D array of integers to store the ASCII values of strings associated
 * externals: A pointer to another struct Label, representing the list of external labels associated with the label.
 * entries: A pointer to another struct Label, representing the list of entry labels associated with the label.
 * labels: A pointer to another struct Label, representing the list of labels associated with the label.
 * DC: An integer that store the counter, for the data addresses.
 * IC: An integer that store the counter, for the instruction addresses.
 */

struct Label
{
    char labelName[MAX_LABEL_NAME_LENGTH];
    int address;
};

struct Label *getLabel(char *labelName)
{
    struct Label *label = labelHead;
    while (label != NULL)
    {
        if (strcmp(label->labelName, labelName) == 0)
            return label;
        label = label->labels;
    }
    return NULL;
}

/**
 *Checking if a a label with the same name already exists by calling the getExistingLabel function.
 * להוסיף
 * להוסיף
 * להוסיף
 * להוסיף
 *
 *
 */

void addNewLabel(const char *labelType, const char *fileName, struct Label newLabel)
{
    /* If the label does not already exist, a new label is created, and added to the list of the labels */
    struct Label *newLabel = (struct Label *)malloc(sizeof(struct Label));
    if (newLabel == NULL)
    {
        /* Error: Memory allocation failed */
        printf("Error: allocation has been failed\n");
        return;
    }
}

/**
 *A boolean function Checking if a a label with the same name already exists.
 * @return true if the name already exists.
 * @return false if the name does not exists.
 */
bool isExistingLabel(const char *labelName, struct Label *labels, int numLabels)
{
    for (int i = 0; i < numLabels; i++)
    {
        if (strcmp(labels[i].labelName, labelName) == 0)
        {
            return true;
        }
    }

    return false;
}