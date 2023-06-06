# Assembler

A repository for an assembly-to-machine language converter developed as part of a bachelor's degree program.

# filesReader - done, without tests

- main - the main will get files names from the cmd and will run in loop on them. every iteration will call the function fileReader
- fileReader - run on the given file, and call the function handleNeWLine in the executer

# macroService.c

- hold a static link list of macros.
  - each macro contain:
    - macroName
    - address of the first line in the macro
    - address of the last line in the macro
- the class export 3 function:

* addNewMacro:
  @input: macroName, address of the first line
  @output: boolean value if the macroName is valid.
* setExistMacro - the function will get the last line of the macro and set it in the rellevant macro
* sendMacro - the function run on the lines in the macro and send them to the executer service

# executer.c

- properties:
  - static list of Lines to execute.
  - registers values

* handleNewLine:
  @input: line object, fileName
  - write every line to the output file
  - add new line to the lines link list
  - check if it EOF: if it is - call execute
* execute: function that run on the link list, and execute the commands 1 by 1.
  - validate label exist with getLabel function

# dataService.c

- struct Label:
  - char labeName[]
  - int address
- properties:
  - data: dynamic 2D array of ints.
  - strings: dynamic 2D array of int (ascii values).
  - externals: dynamic array of labels structtures.
  - entries: dynamic array of labels structtures.
  - labels: dynamic array of labels structtures.
  - DC - counter of the data addresses.
  - IC - counter of the instructions addresses.

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

# errorsLogger.c

- properties:
  - static errorsCounter
    - note: the executer will check if there is errors. if there is - will not execute the commands

* private logError:
  @input: errorMsg, fileName, address.
  - the function increase the error counter by 1 and log the error msg in clean format.
* public function for each type of error that will call logError.

- note: we splite between the function to control the format of the log + if will be a change in the future in the log (write to file for example) - the change will be in one function.

# dataStructure.c

- will hold all of the sructures of all the program

# TODO:

- create makefile

# Questions for hadasa:

1. what we should do with the extern + entries exept save them in file? (page 41)
2. what the effect of A R E in the binary code?
3. what the binary code is use for if there is no output file of binary lines?
4. "if there is no macro in the src file - .am file will be the same as .as" - why? (page 40)
5. there is a case that label is initiate after
6. .data 1.1,3 ,4, 5 is a valid line?
7. .data 1 1,3 ,4,5 is a valid line?
8. there is an option that label is initiate inside macro? if there is - what will be the address of the label?
9. there is a range of addresses for each part of program? (for example 100 first addresses for instructions and 100-200 for data)

# algorithem explanation:

1. file reader start running on each fileName in list from main and for each line in file calling the command identifier
2. the command idetifier check if the command is regular command (without labels and not memory type).
  - try Get Label, save the label in the struct and remove the label from the command
  - if the command is regular
    - if !macroFlag parse to generic line struct and send to the memoryManger.
    - if macroFlag - parse to generic line struct and send to the macro service and save the line under the current macro
  - if not - check for prefix (.data, .string, .external, .extern) and save it in the data service in Line struct
3. the data service get a line from the commands identfier:
  - if there is a label - save label
  - if it is a data / string - save data
  - if there is extern/ entry - save them
  * note: for each of them - increase the DC
  - if it is a regular command - add an address and send to the commands executer
  * note: increase the IC
  * note: save in file the different data if needed

  functions:
  * getLabelAddress - the commands executer will ask for label address every time he will got label inside the command(like inside jmp)
  * insertData - for .string and .data
  * insertExternOrEntry - for .extern and .entry
  * memoryCreator - the main function of the file' with all the logic above.
