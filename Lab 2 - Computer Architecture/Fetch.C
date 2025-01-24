#include "Loader.h"
#include "Debugger.h"



// This function is going to be used to read and write instruction to the memory
// When Fetching instructions 
// mar gets PC
// mbr gets the data from the memory location at mar which is the Program Counter 
// can access it in word or byte
// When reading n
unsigned short bus(unsigned short mbr, unsigned short mar, Read_Or_Write R_OR_W, Word_Or_Byte W_O_B) {

	// First thing we need to know is weather to read or write 

	if (R_OR_W == READING) {
		// If we are reading then need to know weather we are reading word or byte 
		if (W_O_B == WORD) {
			// Gettig the instruction from the memory
			mbr = memory.word[mar >> 1]; 
			return mbr;
		}
		else if (W_O_B == BYTE) {
			mbr = memory.byte[mar];
			mbr = mbr & 0x00FF; // making sure we clare the upper bits 
			return mbr; 
		}

		else if (R_OR_W == WRITING) {
			
			if (W_O_B == WORD) {

				memory.word[mar >> 1] = mbr;
				return memory.word[mar >> 1];
				
			}
			else if (W_O_B == BYTE) {

				memory.word[mar] = mbr & 0x00FF;
				return memory.word[mar];

			}
		}
		



	}
}



void display_instruction(void) {

	int i = 0; 
	while (i < 5) {
		unsigned short instruction_read = 0;

		instruction_read = bus(instruction_read, Prog_Counter, READING, WORD);

		printf("%04X \n", instruction_read);

		Prog_Counter++; 
		i++;
	}

}