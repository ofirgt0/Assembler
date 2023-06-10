#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "errorsHandler.h"
#include "dataService.h"
#define MAX_LABEL_NAME_LENGTH 31

// TODO: add structure for binary words
/*
* addNewLabel:
  @input: newLabel
  - add the label to the array based on the the label type (externals, entries, commands).
* getLabel:
  @input: labelName
  - the function return the address of the label. if not exist - return default 0.

- note: consider the option to use generic function that will do the functionallty and few func that will call this one with rellevant file name and list. */

/* Define the label lists */
static Label *extLabelList = NULL;
static Label *entryLabelList = NULL;
static Label *commandLabelList = NULL;
static Label *dataLabelList = NULL;
static Label *stringsLabelList = NULL;

/**
 * Retrieves a label by its name from the hash table.
 * @param labelName The label name to search for.
 * @param type The type of the label.
 * @return A pointer to the found label, or NULL if not found.
 */
Label *getLabel(const char *labelName, LabelType type)
{
    Label *currentLabel = NULL;

    switch (type)
    {
    case Ext:
        currentLabel = extLabelList;
        break;
    case Entry:
        currentLabel = entryLabelList;
        break;
    case Command:
        currentLabel = commandLabelList;
        break;
    case Data:
        currentLabel = dataLabelList;
        break;
    case Strings:
        currentLabel = stringsLabelList;
        break;
    }

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
    Label *labelList = NULL;

    switch (type)
    {
    case Ext:
        labelList = extLabelList;
        break;
    case Entry:
        labelList = entryLabelList;
        break;
    case Command:
        labelList = commandLabelList;
        break;
    case Data:
        labelList = dataLabelList;
        break;
    case Strings:
        labelList = stringsLabelList;
        break;
    }

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

    switch (type)
    {
    case Ext:
        extLabelList = labelList;
        break;
    case Entry:
        entryLabelList = labelList;
        break;
    case Command:
        commandLabelList = labelList;
        break;
    case Data:
        dataLabelList = labelList;
        break;
    case Strings:
        stringsLabelList = labelList;
        break;
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
bool addNewDataOrStrings(LabelType type, int address, const char *name)
{
    bool result = addNewLabel(type, address, name);
    if (result)
    {
        if (type == Data)
        {
            data[dataCount++] = address;
            // IC += address; /*Not so clear – need to check again about the intent of the implementation*/
        }
        else if (type == Strings)
        {
            strings[stringsCount++] = strdup(name);
            // DC += address; /*Not so clear – need to check again about the intent of the implementation*/
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
bool addNewCommand(int address, const char *name)
{
    bool result = addNewLabel(Command, address, name);
    if (result)
    {
        // IC += sizeof(int); /*Not so clear – need to check again about the intent of the implementation*/
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
bool addNewExternal(LabelType type, int address, const char *name)
{
    bool result = addNewLabel(Ext, address, name);
    if (result)
    {
        // IC += sizeof(int); /*Not so clear – need to check again about the intent of the implementation*/
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
bool addNewEntry(LabelType type, int address, const char *name)
{
    bool result = addNewLabel(Entry, address, name);
    if (result)
    {
        // IC += sizeof(int); /*Not so clear – need to check again about the intent of the implementation*/
    }
    return result;
}
