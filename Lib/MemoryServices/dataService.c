#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "errorsHandler.h"
#include "dataService.h"
#include "writeToFile.h"

#define MAX_LABEL_NAME_LENGTH 31

/* Define the label lists */
static Label *extLabelList = NULL;
static Label *entryLabelList = NULL;
static Label *commandLabelList = NULL;
static Label *dataLabelList = NULL;
static Label *stringsLabelList = NULL;

/* Define the global variables */
int IC = 0;
int DC = 0;
int **data;
char **strings;
int dataCount = 0;
int stringsCount = 0;

/**
 * Classifies a label by type and retrieves the pointer to its list.
 * @param type The type of the label.
 * @return A pointer to the list of labels for the given type.
 */
Label **classifyLabelByType(LabelType type)
{
    switch (type)
    {
    case Ext:
        return &extLabelList;
    case Entry:
        return &entryLabelList;
    case Command:
        return &commandLabelList;
    case Data:
        return &dataLabelList;
    case Strings:
        return &stringsLabelList;
    default:
        return NULL;
    }
}

/**
 * Retrieves a label by its name from the hash table.
 * @param labelName The label name to search for.
 * @param type The type of the label.
 * @return A pointer to the found label, or NULL if not found.
 */
Label *getLabel(const char *labelName, LabelType type)
{
    Label *currentLabel = *classifyLabelByType(type);

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

/**
 * Adds a new label to the hash table.
 * @param type The type of the label (Ext, Entry, Command).
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewLabel(LabelType type, int address, const char *name)
{
    Label **labelList = classifyLabelByType(type);

    if (getLabel(name, type) != NULL)
    {
        EXISTING_LABEL(name); /* Handle error: Label already exists */
        return false;
    }

    Label *newLabel = (Label *)malloc(sizeof(Label));
    if (newLabel == NULL)
    {
        printf("Error: Allocation has failed.\n");
        return false;
    }

    newLabel->address = address;
    newLabel->type = type;
    strcpy(newLabel->name, name);
    newLabel->next = NULL;

    if (labelList == NULL)
    {
        labelList = newLabel;
    }
    else
    {
        Label *currentLabel = labelList;
        while (currentLabel->next != NULL)
        {
            currentLabel = currentLabel->next;
        }
        currentLabel->next = newLabel;
    }

    return true;
}

/**
 * Adds a new data or strings label to the list.
 * @param type The type of the label (DATA or STRINGS).
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewDataOrStringsLabel(LabelType type, int address, const char *name)
{
    bool result = addNewLabel(type, address, name);
    if (result)
    {
        if (type == Data)
        {
            data[dataCount++] = &address;
            DC++;
            writeLabelToFile("ps.ob", getLabel(name, Data));
        }
        else if (type == Strings)
        {
            strings[stringsCount++] = strdup(name);
            DC += strlen(name) + 1; // +1 for null termination - Marking the end of the string.
            writeLabelToFile("ps.ob", getLabel(name, Strings));
        }
    }
    return result;
}

/**
 * Adds a new command label to the list.
 * @param Command The type of the label.
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewCommandLabel(int address, const char *name)
{
    bool result = addNewLabel(Command, address, name);
    if (result)
    {
        IC += sizeof(int);
        writeLabelToFile("ps.ob", getLabel(name, Command));
    }
    return result;
}

/**
 * Adds a new external label to the list.
 * @param Ext The type of the label.
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewExternalLabel(LabelType type, int address, const char *name)
{
    bool result = addNewLabel(Ext, address, name);
    if (result)
    {
        IC += sizeof(int);
        writeLabelToFile("ps.ext", getLabel(name, Ext));
    }
    return result;
}

/**
 * Adds a new entry label to the list.
 * @param Entry The type of the label.
 * @param address The address of the label.
 * @param name The name of the label.
 * @return true if the label was added successfully, false otherwise.
 */
bool addNewEntryLabel(LabelType type, int address, const char *name)
{
    bool result = addNewLabel(Entry, address, name);
    if (result)
    {
        IC += sizeof(int);
        writeLabelToFile("ps.ent", getLabel(name, Entry));
    }
    return result;
}
