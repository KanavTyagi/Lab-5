#include "Loader.h"
#include "Debugger.h"


// Turn on ad off diagnostics
#define DEBUG
char* arthamatic[3][4] = { {"ADD" , "ADDC" ,"SUB", "SUBC"},  {"DADD", "CMP", "XOR", "AND"}, {"OR", "BIT", "BIC", "BIS"} };
char* branch[8] = { "BEQ/BZ" , "BNE/BNZ" ,"BC/BHS", "BNC/BLO", "BN" , "BGE" ,"BLT", "BRA" };
unsigned short  Register_file[2][8] = { {0xF0A0, 0xF0A1, 0xF0A2, 0xF0A3, 0xF0A4, 0xF0A5, 0xF0A6, 0xF0A7}, {0x0000, 0x0001 ,0x0002 , 0x0004, 0x0008, 0x0010, 0x0020, 0xFFFF } };


void DISPLAY_Branch(unsigned short instruction) {
	// This is the instruction that is going to be used to execute teh BL instruction

	// WE WILL NEED TO GET THE OFFSETT FROM THE INSTRUCTION

	short offset = instruction & 0x07FF; // Getting the offset from the instruction
#ifdef DEBUG
	printf("number : %02X\n", GET_Byte_12_10(instruction));

#endif // DEBUG
	// Now we need to check if the offset is negative or positive
	// If the offset is negative then we need to sign extend it
	// If the offset is positive then we need to add it to the program counter
	offset = offset << 5; // Shifting the offset to the left by 5 bits

	printf("%s\n", branch[GET_Byte_12_10(instruction)]); // Displaying the branch instruction
	printf("Offset: %04X\n", offset); // Displaying the offset



}


// Instruction: ADD 
// R / C: 1
// W / B : 1
// Source : 01
// Destination : A5
void DISPLAY_ARTH(unsigned short instrction) {
	
	unsigned short R_C = GET_BIT(instrction, 7); // Getting the R/C bit from the opcode
	unsigned short W_B = GET_BIT(instrction, 6); // Getting the W/B bit from the opcode
	unsigned short Source = Register_file[R_C][GET_BYTE_5_3(instrction)]; // Getting the source from the opcode
	unsigned short Destination = Register_file[0][GET_BYTE_2_0(instrction)]; // Getting the destination from the opcode
#ifdef DEBUG

	printf("%c\n", R_C);
	printf("%c\n", W_B);
	printf("BYTE 5-3: %02X\n", GET_BYTE_5_3(instrction));
	printf("BYTE 2-0: %02X\n", GET_BYTE_2_0(instrction));
	printf("BYTE 12 - 10: %02X  9 - 8 : %02X ", GET_Byte_12_10(instrction), GET_BYTE_9_8(instrction));
	printf("%02X\n", Source);
	printf("%02X\n", Destination);
#endif // DEBUG

	
	printf("Instruction: %s\n", arthamatic[GET_Byte_12_10(instrction)][GET_BYTE_9_8(instrction)]);
	printf("R/C: %d\n", R_C);
	printf("W/B: %d\n", W_B);
	if (W_B == 1) {
		printf("Source: %01X\n", Source & 0xFF);
		printf("Destination: %01X\n", Destination & 0xFF);
	}
	else {
		printf("Source: %02X\n", Source);
		printf("Destination: %02X\n", Destination);
	}

}
