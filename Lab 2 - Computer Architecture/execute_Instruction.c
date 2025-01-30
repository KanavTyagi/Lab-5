#include "Loader.h"
#include "Debugger.h"


// Turn on ad off diagnostics
#define DEBUG


char* arthamatic[Arthanetic_Instruction_Groups][Arthametic_Instruction] = { {"ADD" , "ADDC" ,"SUB", "SUBC"},  {"DADD", "CMP", "XOR", "AND"}, {"OR", "BIT", "BIC", "BIS"} };
char* branch[Branch_Instruction] = { "BEQ/BZ" , "BNE/BNZ" ,"BC/BHS", "BNC/BLO", "BN" , "BGE" ,"BLT", "BRA" };
unsigned short  Register_file[NUM_REGISTER_TYPES][NUM_REGISTERS] = { {0xF0A0, 0xF0A1, 0xF0A2, 0xF0A3, 0xF0A4, 0xF0A5, 0xF0A6, 0xF0A7}, {0x0000, 0x0001 ,0x0002 , 0x0004, 0x0008, 0x0010, 0x0020, 0xFFFF } };


// this function is going to be used to pront the contents of the Branch always instruction
void DISPLAY_BL(unsigned short instruction) {
	
	signed short offset = instruction & 0x1FFF; // Extract 13-bit offset
	///printf("Encoded Offset 1 : %04X\n", offset);


	

	///printf("Encoded Offset: %04X\n", offset);


	// Sign extend the 13-bit offset to 16 bits
	if (GET_BIT(offset,12) ) { // Check if bit 12 is set (negative number)
		offset |= 0xE000; // Extend sign bit by OR-ing upper bits
	}

	// Shift left as the machine code is shifted right by 1 bit (bit 0 is always 0) 
	offset <<= 1; 

	printf("Instruction: BL\n");
	printf("Encoded Offset: %X\n", (unsigned short) offset);

	signed int New_PC = Prog_Counter + offset; // Use wider type for PC calculation
	printf("Current PC: %04X\n", Prog_Counter);
	printf("New PC: %04X\n", New_PC);

}
void DISPLAY_Branch(unsigned short instruction) {
	// This is the instruction that is going to be used to execute teh BL instruction

	// WE WILL NEED TO GET THE OFFSETT FROM THE INSTRUCTION

	signed short offset = instruction & 0x03FF; // Getting the offset from the instruction 

#ifdef DEBUG
	printf("instruction number : %04X\n", GET_BITS_12_10(instruction));

#endif // DEBUG
	// Now we need to check if the offset is negative or positive
	if (GET_BIT(instruction, 9) == TRUE) { // Bit 9 is the sign bit so we would check if it is set or not
		// If the offset is negative then we need to sign extend it
#ifdef DEBUG
		printf("Negative\n");
		printf("Offset: %d\n", offset); // Displaying the offset
#endif // DEBUG

		offset |= 0xFC00; // Extending the sign bit 
		
	}

	// Shift left as the machine code is shifted right by 1 bit (bit 0 is always 0) 
	offset <<= 1;
	printf("Instruction: %s\n", branch[GET_BITS_12_10(instruction)]);
	printf("Encoded Offset: %0X\n", (unsigned short)offset);

	signed int New_PC = Prog_Counter + offset; // Use wider type for PC calculation
	printf("Current PC: %04X\n", Prog_Counter);
	printf("New PC: %04X\n", New_PC);

}


// Instruction: ADD 
// R / C: 1
// W / B : 1
// Source : 01
// Destination : A5
void DISPLAY_ARTH(unsigned short instruction) {
	
	unsigned short R_C = GET_BIT(instruction, R_C_BIT); // Getting the R/C bit from the opcode which is saved in bit 7
	unsigned short W_B = GET_BIT(instruction, W_B_BIT); // Getting the W/B bit from the opcode which is saved in bit 7
	unsigned short Source = Register_file[R_C][GET_BITS_5_3(instruction)]; // Getting the source from the opcode
	unsigned short Destination = Register_file[0][GET_BITS_2_0(instruction)]; // Getting the destination from the opcode
#ifdef DEBUG

	printf("%c\n", R_C);
	printf("%c\n", W_B);
	printf("BYTE 5-3: %02X\n", GET_BITS_5_3(instruction));
	printf("BYTE 2-0: %02X\n", GET_BITS_2_0(instruction));
	printf("BYTE 12 - 10: %02X  9 - 8 : %02X ", GET_BITS_12_10(instruction), GET_BITS_9_8(instruction));
	printf("%02X\n", Source);
	printf("%02X\n", Destination);
#endif // DEBUG

	
	printf("Instruction: %s\n", arthamatic[GET_BITS_12_10(instruction)][GET_BITS_9_8(instruction)]);
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
