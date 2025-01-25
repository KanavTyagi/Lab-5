#include "Loader.h"
#include "Debugger.h"


// Turn on ad off diagnostics
//#define DEBUG


void execute_BL(unsigned short instrction) {
	// This is the instruction that is going to be used to execute teh BL instruction

	// WE WILL NEED TO GET THE OFFSETT FROM THE INSTRUCTION

	short offset = instrction & 0x07FF; // Getting the offset from the instruction

	// Now we need to check if the offset is negative or positive
	// If the offset is negative then we need to sign extend it
	// If the offset is positive then we need to add it to the program counter
	offset = offset << 5; // Shifting the offset to the left by 5 bits



}

char* get_opcode_name(unsigned char opcode) {

}