# Assembler
A repository for an assembly-to-machine language converter developed as part of a bachelor's degree program.

# macroService.c: 
- hold a static link list of macros.
    * each macro contain:
        * macroName
        * address of the first line in the macro
        * address of the last line in the macro
- the class export 3 function:
* addNewMacro:
    @input: macroName, address of the first line
    @output: boolean value if the macroName is valid.
* setExistMacro - the function will get the last line of the macro and set it in the rellevant macro 
* sendMacro - the function run on the lines in the macro and send them to the executer service

# executer.c
- holding a static list of Lines to execute.
* addNewLine:
    @input: line object, fileName