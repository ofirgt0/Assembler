#ifndef ENCODER_H
#define ENCODER_H

#define fileSuffix_commands ".ob"
#define fileSuffix_ENTRY ".ent"
#define fileSuffix_EXTERN ".ext"
#define fileSuffix_MACRO ".am"

/**
 * Encodes a label operand into a 12-bit binary code,
 * then translates it to a base64 representation and appends it to the output file.
 * @param fileName The name of the assembly file currently being processed.
 * @param AREcode The Absolute, Relocatable, or External code.
 * @param address The address of the label.
 */
void encodLabelOperand(char *fileName, char AREcode, int address);

/**
 * Encodes an immediate value into a 12-bit binary code,
 * then translates it to a base64 representation and appends it to the output file.
 * @param fileName The name of the assembly file currently being processed.
 * @param immediate The immediate value to be encoded.
 */
void encodImmidiate(char *fileName, int immidiate);

/**
 * Sets the Absolute, Relocatable, or External (ARE) code on a given command code.
 * @param AREcode The Absolute, Relocatable, or External code.
 * @param commandCode The binary command code where ARE bits need to be set.
 */
void setARE(char AREcode, int *commandCode);

/**
 * Encodes an external label and adds the encoding to the output file.
 * @param fileName The name of the assembly file currently being processed.
 */
void encodExternLabel(char *fileName);

/**
 * Encodes a given instruction code into a 12-bit binary code,
 * then translates it to a base64 representation and appends it to the output file.
 * @param fileName The name of the assembly file currently being processed.
 * @param AREcode The Absolute, Relocatable, or External code.
 * @param srcAddressing The source addressing method.
 * @param opcode The opcode of the instruction.
 * @param dstAddressing The destination addressing method.
 */
void encodInstructionCode(char *fileName, char AREcode, int srcAddressing, int opcode, int dstAddressing);

/**
 * Sets a portion of a binary code array based on the given number.
 * Useful for encoding different parts of the instruction, like opcode, addressing method, etc.
 * @param startCell The start index in the array.
 * @param endCell The end index in the array.
 * @param number The number to be converted into binary.
 * @param arr The binary code array.
 */
void setBinaryCodeInRange(int startCell, int endCell, int number, int arr[]);

/**
 * Converts a binary code array into a base64 encoded string.
 * @param intArray The binary code array.
 * @param length The length of the binary code array.
 * @return The base64 encoded string representation of the binary code.
 */
char *binaryArrayToBase64(int *inrArray, int length);

/**
 * Concatenates two given strings and returns the resulting string.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return The concatenated string.
 */
char *concatenateStrings(const char *str1, const char *str2);

/**
 * Converts a negative decimal number into its 2's complement binary representation.
 * @param arr The binary array to store the 2's complement representation.
 * @param decimalNumber The negative number to be converted.
 * @param borderCell The last cell in the binary array that will hold the representation.
 */
void setNegativeBinaryArray(int arr[], int decimalNumber, int borderCell);

/**
 * Encodes a register-based instruction using given register details into a 12-bit binary code,
 * then translates it to a base64 representation and appends it to the output file.
 * @param fileName The name of the assembly file currently being processed.
 * @param register1 The source register.
 * @param register2 The destination register.
 */
void encodeRegister(char *fileName, int register1, int register2);

/**
 * Converts a positive decimal number into its binary representation.
 * @param binaryArray The array to store the binary representation.
 * @param decimalNumber The positive number to be converted.
 * @param borderCell The last cell in the binary array that will hold the representation.
 */
void setBinaryArray(int binaryArray[], int decimalNumber, int borderCell);

#endif /*ENCODER_H*/