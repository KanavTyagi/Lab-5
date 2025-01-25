#include "Loader.h"
#include "Debugger.h"

// Turn on ad off diagnostics 
#define DEBUG

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
#ifdef DEBUG
			printf("Read Word from Memory at address 0x%04X: 0x%04X\n", mar, mbr);
#endif
			// Return the instruction set that is read from teh memory in word form 
			return mbr;
		}
		else if (W_O_B == BYTE) {
			mbr = memory.byte[mar];
			mbr = mbr & 0x00FF; // making sure we clare the upper bits 
#ifdef DEBUG
			printf("Read Byte from Memory at address 0x%04X: 0x%02X\n", mar, mbr);
#endif

			// Return the instruction set that is read from teh memory in byte form 
			return mbr; 
		}

		else if (R_OR_W == WRITING) {
			
			if (W_O_B == WORD) {

				memory.word[mar >> 1] = mbr;
#ifdef DEBUG
				printf("Wrote Word to Memory at address 0x%04X: 0x%04X\n", mar, mbr);
#endif
				mar = mar + 2; 
				// Return the updated program counter would be 
				return mar;
				
			}
			else if (W_O_B == BYTE) {

				memory.word[mar] = mbr & 0x00FF;
#ifdef DEBUG
				printf("Wrote Byte to Memory at address 0x%04X: 0x%02X\n", mar, mbr);
#endif
				mar = mar + 1; 
				// Return the updated program counter would be 
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


// This function would need to decode the instructions that are passed into thiss 
// this would basically read the instruction and then display the contents in the specifed foramt 
// 
void decode_instruction(unsigned short instruction) {

}


// this function is going to read teh instruction from the memory and then
// Will call the function to decode it 
void find_instruction(void) {

	char conti = 'y';
	
	printf("\start reading : ");
	conti = getchar();
	
	while (conti == 'y' || conti == 'Y') {
		unsigned short instruction_read = 0;

		instruction_read = bus(instruction_read, Prog_Counter, READING, WORD);

		// Need to check if something does exist there or not if it is 0 then move to the next 
		unsigned first3 = GET_TOP3(instruction_read);

		switch (first3) {
			case 0:
				execute_BL(instruction_read);
				break;
			case 1:
				printf("Branch second\n");
				break;
			case 2:
				printf("All thee add Instructions\n");
				break;
			case 3:
				printf("Move instructions \n");
				break;
			case 4:
			case 5:
				printf("LDR \n");
				break;
			case 6:
			case 7:
				printf("STR \n");
				break;
			default:
				printf("Shit is fucked gove up on life \n");
				break;

		}
		printf("Continue reading (y/n)?: ");
		scanf(" %c", &conti);  
		Prog_Counter += 2;
	}

}