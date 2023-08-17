# Define the C compiler to use
CC = gcc

# Define any compile-time flags
CFLAGS = -c -Wall -ansi -pedantic

# Define any directories containing header files 
INCLUDES = -I./Assembler/Shared/PassesParserHeader -I./Assembler/Shared/HelpersHeaders -I./Assembler/Shared/MemoryServicesHeaders

# Define any libraries to link into executable:
LIBS = -lm

# Define the executable file 
MAIN = assembler

# Define the object files
OBJS = assembler.o commandsIdentifier.o macroService.o dataService.o errorsHandler.o writeToFile.o encoder.o helpfulFunctions.o

.PHONY: all clean

all: $(MAIN)
	@echo  Assembler has been compiled

$(MAIN): $(OBJS)
	$(CC) -Wall -ansi -pedantic -o $(MAIN) $^ $(LIBS)

assembler.o: Assembler/Lib/Helpers/assembler.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

commandsIdentifier.o: Assembler/Lib/PassesParser/commandsIdentifier.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

macroService.o: Assembler/Lib/MemoryServices/macroService.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

dataService.o: Assembler/Lib/MemoryServices/dataService.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

errorsHandler.o: Assembler/Lib/Helpers/errorsHandler.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

writeToFile.o: Assembler/Lib/Helpers/writeToFile.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

encoder.o: Assembler/Lib/Helpers/encoder.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@
	
helpfulFunctions.o: Assembler/Lib/Helpers/helpfulFunctions.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@	

# Clean target to remove object files and the executable
clean:
	$(RM) *.o *~ $(MAIN)

