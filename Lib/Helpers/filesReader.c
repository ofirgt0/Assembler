#include <stdio.h>

void logNewLine(const char *line) { //TODO: DELETE THIS FUNCTION AND IMPLEMENT IN THE EXECUTER
    printf("Processing line: %s\n", line);
}

// Function to read and process a file
// Input: filename - the name of the file to be read and processed
void fileReader(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    char line[256];
    // first run - save label, macro
    while (fgets(line, sizeof(line), file) != NULL) {
        
        removePrefixSpaces(line);
        if(line == '\0')
            continue;

        startFirstRun(line);
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        
        removePrefixSpaces(line);
        if(line == '\0')
            continue;

        logNewLine(line);
        parseCommand(line);
    }


    fclose(file);
}

void removePrefixSpaces(char* command){
    int i, j;
    for (i = 0, j = 0; command[i]; i++)
    {
        if (command[i] == ' ' || command[i] == '\n' || command[i] == '\t')
        {
            command++;
        }
        else{
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }
    
    // Iterate through each command-line argument (file name)
    for (i = 1; i < argc; i++) 
        fileReader(argv[i]);

    return 0;
}