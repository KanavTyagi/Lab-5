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
 * DISPLAY_BL: Displays the contents related to the Branch with Link (BL) instructions.
 * DISPLAY_Branch: Displays other branch instructions based on the decoded opcode.
 * DISPLAY_ARTH: Displays arithmetic and logical instructions with their details.
 */


#include "Loader.h"
#include "Decode.h"


// Turn on ad off diagnostics
//#define DEBUG

// This is the array that is going to be used to store the names of the arthametic and logical instructions that are to be used in this lab
char* arthamatic[Arthanetic_Instruction_Groups][Arthametic_Instruction] = { {"ADD" , "ADDC" ,"SUB", "SUBC"}, 
																			{"DADD", "CMP", "XOR", "AND"}, 
																			{"OR", "BIT", "BIC", "BIS"} };

// This is the array that is going to be used to store the names of the branch instructions that are to be used in this lab
char* branch[Branch_Instruction] = { "BEQ/BZ" , "BNE/BNZ" ,"BC/BHS", "BNC/BLO", "BN" , "BGE" ,"BLT", "BRA" };

// This is the array that is going to be used to store the values of the registers and the constant values
// The values had been initialized to the values that are given in the lab
unsigned short  Register_file[NUM_REGISTER_TYPES][NUM_REGISTERS] = { {0xF0A0, 0xF0A1, 0xF0A2, 0xF0A3, 0xF0A4, 0xF0A5, 0xF0A6, 0xF0A7}, // Registers 
																	{0x0000, 0x0001 ,0x0002 , 0x0004, 0x0008, 0x0010, 0x0020, 0xFFFF } }; // Constants


// this function is going to be used to pront the contents of the Branch always instruction
void DISPLAY_BL() {
	
	signed short offset = Instruction_Register & BL_Offset; // Extract 13-bit offset

#ifdef DEBUG
	printf("Encoded Offset Raw : %04X\n", offset);
#endif // DEBUG

	// Sign extend the 13-bit offset to 16 bits
	if (GET_BIT(offset, BL_Sign_Bit)) { // Check if bit 12 is set , which would mean that the offset is negative 
		offset |= BL_Sign_Extend; // Extend sign bit by OR-ing upper bits
#ifdef DEBUG
		printf("Offset Neagtive new offset calculated : %04X\n", offset);
#endif // DEBUG
	}

	// Shift left as the machine code is shifted right by 1 bit (bit 0 is always 0) because of how the program counter is incremented
	// and can only be even 
	offset <<= Offset_Shift;

	printf("Instruction: BL\n"); // Display the instruction
	printf("Encoded Offset: %X\n", (unsigned short)offset); // Display the offset

	signed short New_PC = Prog_Counter + offset; 
	printf("Current PC: %04X\n", Prog_Counter); // Display the current PC
	printf("New PC: %04X\n", New_PC); // Display what the new PC would have been

}
void DISPLAY_Branch(void) {
	// This is the instruction that is going to be used to execute teh BL instruction

	// WE WILL NEED TO GET THE OFFSETT FROM THE INSTRUCTION

	signed short offset = Instruction_Register & Branch_Offset; // Getting the offset from the instruction 

#ifdef DEBUG
	printf("instruction number : %04X\n", GET_BITS_12_10(Instruction_Register));

#endif // DEBUG
	// Now we need to check if the offset is negative or positive
	if (GET_BIT(Instruction_Register, Branch_Sign_Bit) == TRUE) { // Bit 9 is the sign bit so we would check if it is set or not
		// If the offset is negative then we need to sign extend it
#ifdef DEBUG
		printf("Negative\n");
		printf("Offset: %d\n", offset); // Displaying the offset
#endif // DEBUG

		offset |= Branch_sign_extend; // Extending the sign bit 
		
	}

	// Shift left as the machine code is shifted right by 1 bit (bit 0 is always 0) 
	offset <<= Offset_Shift;
	printf("Instruction: %s\n", branch[GET_BITS_12_10(Instruction_Register)]); // Display the instruction by getting it from the array using the buts 12 11 10
	printf("Encoded Offset: %0X\n", (unsigned short)offset); // Display the offset that is encoded in the instruction using typecasting to pront it accurately 

	signed int New_PC = Prog_Counter + offset; // Use wider type for PC calculation
	printf("Current PC: %04X\n", Prog_Counter);
	printf("New PC: %04X\n", New_PC);

}



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
	unsigned short Source = Register_file[R_C][GET_BITS_5_3(Instruction_Register)]; // Getting the source from the opcode
	unsigned short Destination = Register_file[0][GET_BITS_2_0(Instruction_Register)]; // Getting the destination from the opcode
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
