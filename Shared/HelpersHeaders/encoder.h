#ifndef ENCODER_H
#define ENCODER_H

/* Encodes a label operand.
 * @param fileName the name of the file.
 * @param AREcode the code representing the Addressing, Relocatable, and External attributes of the operand.
 * @param address the address of the label.
 */
void encodLabelOperand(char *fileName, char AREcode, int address);

/* Encodes an immediate operand.
 * @param fileName the name of the file.
 * @param immidiate the immediate value.
 */
void encodImmidiate(char *fileName, int immidiate);

/* Sets the A, R, E bits of a command code.
 * @param AREcode the code representing the Addressing, Relocatable, and External attributes.
 * @param commandCode the command code.
 */
void setARE(char AREcode, int *commandCode);

/* Converts a decimal number to a binary array.
 * @param arr the binary array.
 * @param decimalNumber the decimal number.
 */
void setBinaryArray(int arr[], int decimalNumber);

/* Encodes an external label.
 * @param fileName the name of the file.
 */
void encodExternLabel(char *fileName);

/* Encodes an instruction code.
 * @param fileName the name of the file.
 * @param AREcode the code representing the Addressing, Relocatable, and External attributes of the instruction.
 * @param srcAddressing the source addressing mode.
 * @param opcode the opcode of the instruction.
 * @param dstAddressing the destination addressing mode.
 */
void encodInstructionCode(char *fileName, char AREcode, int srcAddressing, int opcode, int dstAddressing);

/* Encodes a register operand.
 * @param fileName the name of the file.
 * @param register1 the first register.
 * @param register2 the second register.
 */
void encodeRegister(char *fileName, int register1, int register2);

/* Sets a range of cells in a binary code array to represent a given number.
 * @param startCell the start index of the range.
 * @param endCell the end index of the range.
 * @param number the number to represent.
 * @param arr the binary code array.
 */
void setBinaryCodeInRange(int startCell, int endCell, int number, int arr[]);

/* Encodes a binary array to a Base64 string.
 * @param binaryArray the binary array.
 * @param length the length of the binary array.
 * @return the Base64 string.
 */
char *encodeToBase64(const int *binaryArray, size_t length);

#endif /*ENCODER_H*/
