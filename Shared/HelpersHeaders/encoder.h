/**
 * Function: convertCommandToOpcode
 * -----------------------------------
 * This function takes a command name and finds its corresponding opcode based on the
 * predefined list of command names.
 * The command opcode are from: 0-15 (in decimal base).
 * @param commandName The name of the command to be converted to opcode.
 * @return
 *      1. If the provided command name is recognized, it returns the opcode which is a 0-based index.
 *      2. If the command name is not recognized, the program prints an error message and exits with status code 1.
 */
int convertCommandToOpcode(char *commandName);

/**
 * Function: convertOpCodeToBinary
 * --------------------------------
 * Converts an integer opcode to its binary representation.
 * @param opcode
 *      The opcode to be converted to binary.
 * @return
 *      A string that represents the binary form of the opcode.
 */
const char *convertOpCodeToBinary(int opcode);

/**
 * Function: encodeInstructionByDirectAddressing
 * -------------------------------------------------
 * This function encodes an instruction, using the Direct Addressing method.
 * @param commandName The name of the command.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 */
void encodeInstructionByDirectAddressing(char *commandName, int ARE, int srcRegister, int dstRegister, OperandCount operandCount);

/**
 * Function: encodeInstructionByImmediateAddressing
 * -------------------------------------------------
 * This function encodes an instruction using the Immediate Addressing method.
 * @param commandName The name of the command.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 */
void encodeInstructionByImmediateAddressing(char *commandName, int ARE, int srcRegister, int dstRegister, OperandCount operandCount);

/**
 * Function: encodeInstructionByDirectRegisterAddressing
 * --------------------------------------------------------
 * This function encodes an instruction using Direct Register Addressing method.
 * @param commandName The name of the command.
 * @param ARE The encoding method.
 * @param srcRegister The source register.
 * @param dstRegister The destination register.
 * @param operandCount The number of operands.
 */
void encodeInstructionByDirectRegisterAddressing(char *commandName, int ARE, int srcRegister, int dstRegister, OperandCount operandCount);

#endif /* INSTRUCTION_ENCODER_H */