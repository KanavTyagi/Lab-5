/*
 * File Name: execute_instruction.c
 * Created: 29th January 2025
 * By: Kanav Tyagi
 *
 * Description:
 * Contains functions responsible for decoding and displaying various types of instructions,
 * and handling user interactions using a menu-driven interface.
 *
 * Functions:
 * Execute_BL: Displays the contents related to the Branch with Link (BL) instructions.
 * Execute_Branch: Displays other branch instructions based on the decoded opcode.
 * DISPLAY_ARTH: Displays arithmetic and logical instructions with their details.
 */


#include "Loader.h"
#include "Decode.h"
#include "Load_instruction.h"
#include "Branch_Inst.h"


 // This is the array that is going to be used to store the names of the arthametic and logical instructions that are to be used in this lab
char* arthamatic[Arthanetic_Instruction_Groups][Arthametic_Instruction] = { {"ADD" , "ADDC" ,"SUB", "SUBC"},
																			{"DADD", "CMP", "XOR", "AND"},
																			{"OR", "BIT", "BIC", "BIS"} };

// This function is going to be used to display the Arthametic and logical instructions
// we would need to get the source and the destination from the instruction
// and then we would need to get the R/C and W/B bits from the instruction
// and then we would use the other parts of the opcodes to get the instruction name 
// Instruction: ADD 
// R / C: 1
// W / B : 1
// Source : 01
// Destination : A5
void DISPLAY_ARTH(void) {

	unsigned short R_C = GET_BIT(Instruction_Register, R_C_BIT); // Getting the R/C bit from the opcode which is saved in bit 7
	unsigned short W_B = GET_BIT(Instruction_Register, W_B_BIT); // Getting the W/B bit from the opcode which is saved in bit 7
	unsigned short Source = Register_file.WORD[R_C][GET_BITS_5_3(Instruction_Register)]; // Getting the source from the opcode
	unsigned short Destination = Register_file.WORD[0][GET_BITS_2_0(Instruction_Register)]; // Getting the destination from the opcode
#ifdef DEBUG

	printf("%c\n", R_C);
	printf("%c\n", W_B);
	printf("BYTE 5-3: %02X\n", GET_BITS_5_3(Instruction_Register));
	printf("BYTE 2-0: %02X\n", GET_BITS_2_0(Instruction_Register));
	printf("BYTE 12 - 10: %02X  9 - 8 : %02X ", GET_BITS_12_10(Instruction_Register), GET_BITS_9_8(Instruction_Register));
	printf("%02X\n", Source);
	printf("%02X\n", Destination);
#endif // DEBUG


	printf("Instruction: %s\n", arthamatic[GET_BITS_12_10(Instruction_Register)][GET_BITS_9_8(Instruction_Register)]);
	printf("R/C: %d\n", R_C);
	printf("W/B: %d\n", W_B);

	// Check if W/B is set to byte or word
	if (W_B == TRUE) {
		printf("Source: %01X\n", Source & 0xFF); // Mask the source to get the lower byte
		printf("Destination: %01X\n", Destination & 0xFF); // Mask the destination to get the lower byte
	}
	else {
		printf("Source: %02X\n", Source);
		printf("Destination: %02X\n", Destination);
	}

}
