 # Define the C compiler to use
CC = gcc

# Define any compile-time flags
CFLAGS = -c -Wall -ansi -pedantic

# Define any directories containing header files 
INCLUDES = -I./Assembler/Shared/PassesParserHeader -I./Assembler/Shared/HelpersHeaders -I./Assembler/Shared/MemoryServicesHeaders

# Define any libraries to link into executable:
LIBS = -lm

# Define the executable file 
MAIN = Assembler

# Define the object files
OBJS = filesReader.o commandsIdentifier.o macroServices.o dataServices.o errorsHandler.o writeToFile.o encoder.o

.PHONY: all clean

all: $(MAIN)
	@echo  Assembler has been compiled

$(MAIN): $(OBJS)
	$(CC) -Wall -ansi -pedantic -o $(MAIN) $^ $(LIBS)

filesReader.o: Assembler/Lib/Helpers/filesReader.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

commandsIdentifier.o: Assembler/Lib/PassesParser/commandsIdentifier.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

macroServices.o: Assembler/Lib/MemoryServices/macroServices.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

dataServices.o: Assembler/Lib/MemoryServices/dataServices.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

errorsHandler.o: Assembler/Lib/Helpers/errorsHandler.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

writeToFile.o: Assembler/Lib/Helpers/writeToFile.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

encoder.o: Assembler/Lib/Helpers/encoder.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)











