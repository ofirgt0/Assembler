#ifndef DATASERVICE_H
#define DATASERVICE_H

#include "errorsHandler.h"

#define EXTERN_LABEL_TYPE "extern"
#define ENTRY_LABEL_TYPE "entry"
#define NORMAL_LABEL_TYPE ""
#define DATA_LABEL_TYPE "data"
#define STRING_LABEL_TYPE "string"

#define ARE_CODE_A 'A'
#define ARE_CODE_R 'R'
#define ARE_CODE_E 'E'

#define MAX_LABEL_NAME_LENGTH 31

/** Structure to represent a label */
struct Label
{
    char name[MAX_LABEL_NAME_LENGTH];
    int address; 
};

/** Enumeration for different types of labels */
enum LabelType
{
    NormalCommand, 
    External,      
    Entry,        
    Data,          
    String         
};

/** Structure to represent a node in a linked list of labels */
struct GenericLabelNode
{
    struct Label *label; 
    char *string;        
    int *data;           
    int size;            
    struct GenericLabelNode *next;
};

/** Increase the Instruction Counter (IC) by a specified value */
void increaseIC(int value);

/** Prepare for the second run by writing IC and DC to the output file */
void prepareSecondRun(char *fileName);

/** Check if a label exists and update entries accordingly */
bool isLabelExist(char *label, int lineNumber, char *fileName);

/** Update the address of an entry label */
void updateEntryLabelAddress(char *entryName, int address);

/** Convert a character to a string */
char *charToString(char c);

/** Send a string value to the output file */
void sendStringValue(char *fileName, char *labelName);

/** Send data values to the output file */
void sendDataValue(char *fileName, char *labelName);

/** Send labels to their respective output files */
void sendLabelsToFile(const char *filename);

/** Calculate the length of a string until the newline character */
int getStringLenUntilNewLine(char *string);

/** Add an array of data values with a label */
bool addData(int data[], char *labelName, int length);

/** Add a string with a label */
bool addString(char *string, char *labelName);

/** Add a new label */
bool addNewLabel(char *labelName);

/** Add a new entry label */
bool addNewEntry(char *entryName);

/** Add a new external label */
bool addNewExtern(char *externName);

/** Search for the address of a label */
int searchLabel(char *labelName);

/** Search for the address of an external label */
int searchExternLabel(char *externName);

/** Search for the address of an entry label */
int searchEntry(char *entryName);

/** Search for a data label in the linked list */
struct GenericLabelNode *searchDataLabel(char *labelName);

/** Search for a string label in the linked list */
struct GenericLabelNode *searchStringLabel(char *labelName);

#endif