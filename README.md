
# Assembler Project | Written By: Ofir Ginat & Liron Yannai | Semester 2023B 

# Course name: Systems programming lab.
# Lecturer: Hadassah Rabkin.
# Semester Study: 2023B.
# Written by: Ofir Ginat & Liron Yannai.

# Introduction: 
This repository houses the main assembler code developed for the Systems Programming Lab, 2023B, led by lecturer Hadassah Rabkin.
The program's primary function is to read and process multiple assembly and macro files provided as command-line arguments.


# Core Modules & Files #

# assembler:
Main Driver: This is the program's heart, orchestrating the assembly process.
File Reading and Processing: It efficiently manages the two-phase assembly operation via the fileReader function.
Utility Features: It offers several handy tools, such as getFileNameWithExtension for filename construction and layoutBulkOfLines for multi-line macro processing.

# encoder :
Encoding Mechanisms: Translates 'extern' labels using the ARE encoding scheme and facilitates binary representation of data and instructions.
Interactions: Works closely with writeToFile to persist encoded results and with errorsHandler to manage discrepancies.

# errorsHandler :
Error Tracking: Monitors and logs errors throughout the assembly process.
Feedback: Provides feedback to the user, highlighting any issues detected during the assembly process.

# writeToFile :
File Operations: Handles the task of writing encoded data, instructions, and other relevant information to output files.
Collaborations: Leverages encoder for data to write and errorsHandler to manage potential issues during file operations.

# dataService :
Data Management: Manages operations related to data directives, labels, and memory structures.
Main Driver Interactions: Plays a pivotal role during the first run of fileReader by storing data-related information.

# macroService :
Macro Handling: Manages macro-related operations, such as defining, storing, and expanding macros.
First Run Utility: Particularly useful during the first run of the fileReader to capture and organize macro-related data.

# commandsIdentifier :
Command Interpretation: Identifies and parses assembly commands. A cornerstone during the second run of the fileReader function.


## Program Flow:

# Beginning Steps: The program starts with a basic check to ensure that valid file names are provided as command-line arguments. It then sets up the environment for each file by initializing static variables. This guarantees that each file is processed in a clean and isolated environment, ensuring no mix-ups or overlaps.

#Two-Pass System:

1. First Pass: This is the discovery phase. During this run, assembler.c invokes the dataService and macroService components. This is where it identifies and stores labels, macros, entries, externs, data, and strings. The main goal is to gather as much information about the assembly file without committing to any irreversible actions. By the end of this pass, the program has a comprehensive understanding of the data and the macro structures in the input file.

2. Second Pass: With a rich knowledge base established in the first run, the program is now ready to dive deep. The commandsIdentifier comes into play here, processing each line, parsing the commands, and converting them into their binary representations. This phase is where the actual encoding happens. It's also the phase where any unresolved references from the first pass are addressed.
Interactions: Throughout both passes, assembler.c maintains close communication with dataService for handling data-related tasks and commandsIdentifier for command interpretation and processing. It also constantly coordinates with errorsHandler to ensure that any discrepancies or issues are immediately flagged and addressed.
Finalizing Steps: Once both passes are completed, assembler.c ensures that the results are appropriately written to output files. This involves interactions with the writeToFile module, which handles the task of persisting the processed data.