CC = gcc
CFLAGS = -Wall -ansi -pedantic 

# List of directories containing header files
INCLUDE_DIRS = \
    Assembler/Shared/DataStructures \
    Assembler/Shared/ExecutersHeaders \
    Assembler/HelpersHeaders \
    Assembler/MemoryServiceHeaders

# List of source files
SRCS = \
    Assembler/Lib/Engines/commandsExecuter.c \
    Assembler/Lib/Engines/commandsIdentifier.c \
    Assembler/Lib/Helpers/errorHandler.c \
    Assembler/Lib/Helpers/stringConverter.c \
    Assembler/Lib/Helpers/writeToFile.c \
    Assembler/Lib/MemoryService/dataService.c \
    Assembler/Lib/MemoryService/macroService.c \
    Assembler/Shared/DataStructures/data.c

# Generate object file names from source file names
OBJS = $(SRCS:.c=.o)

# Name of the executable
EXEC = program

# Default target
all: $(EXEC)

# Rule to compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIRS) -c $< -o $@

# Rule to link object files into an executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# Clean target to remove object files and the executable
clean:
	rm -f $(OBJS) $(EXEC)
