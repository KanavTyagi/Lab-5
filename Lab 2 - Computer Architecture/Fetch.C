#include "Loader.h"
#include "Debugger.h"

// Turn on and off diagnostics 
//#define DEBUG

// To test the instruction output uncomment the line to be able to test the output of the instructions
//#define TESTING

// This function is going to be used to read and write instruction to the memory
// When Fetching instructions 
// mar gets PC
// mbr gets the data from the memory location at mar which is the Program Counter 
// can access it in word or byte
// When reading n
void bus(unsigned short mar, unsigned short* mbr, Read_Or_Write R_OR_W, Word_Or_Byte W_O_B) {

	// Check if we are reading or writing
	if (R_OR_W == READING) {
		// If we are reading, check whether it's a word or a byte
		if (W_O_B == WORD) {
			// Read a word from memory
			*mbr = memory.word[mar >> 1];
#ifdef DEBUG
			printf("Read Word from Memory at address 0x%04X: 0x%04X\n", mar, *mbr);
#endif
			
		}
		else if (W_O_B == BYTE) {
			// Read a byte from memory
			*mbr = memory.byte[mar] & 0x00FF; // Ensure only the lower byte is kept
#ifdef DEBUG
			printf("Read Byte from Memory at address 0x%04X: 0x%02X\n", mar, *mbr);
#endif
			
		}
	}
	else if (R_OR_W == WRITING) {
		// If we are writing, check whether it's a word or a byte
		if (W_O_B == WORD) {
			// Write a word to memory
			memory.word[mar >> 1] = *mbr;
#ifdef DEBUG
			printf("Wrote Word to Memory at address 0x%04X: 0x%04X\n", mar, *mbr);
#endif
			
		}
		else if (W_O_B == BYTE) {
			// Write a byte to memory
			memory.byte[mar] = *mbr & 0x00FF; // Store only lower 8 bits
#ifdef DEBUG
			printf("Wrote Byte to Memory at address 0x%04X: 0x%02X\n", mar, *mbr);
#endif
			
		}
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
	char update_PC = 'y';
	
	printf("\start reading : ");
	conti = getchar();

	char test;
	while (conti == 'y' || conti == 'Y') {
		unsigned short instruction_read = 0;

		// Reading the instruction from the memory
		// So we would set the value of mar to the program counter
		mar = Prog_Counter;
		bus(mar, &mbr, READING, WORD);
		instruction_read = mbr; // Getting the instruction from the memory buffer register
		printf("Instruction read 0x%04X \n", instruction_read);

		//////////////////////////////////////////////////////////////////////////////////
#ifdef TESTING

		char input[10];       // Buffer to store user input

		// Prompt the user for hexadecimal input
		printf("Enter testing data in hexadecimal (e.g., 1A3F or 0x1A3F): ");

		// Read the input as a string
		if (scanf("%9s", input) != 1) {  // Limit input to prevent buffer overflow
			fprintf(stderr, "Error reading input.\n");
			return 1;
		}

		// Optional: Remove "0x" or "0X" prefix if present
		char* hexStr = input;
		if (strlen(input) > 2 && input[0] == '0' && (input[1] == 'x' || input[1] == 'X')) {
			hexStr += 2;
		}

		// Check if all characters are valid hexadecimal digits
		for (size_t i = 0; i < strlen(hexStr); i++) {
			if (!isxdigit((unsigned char)hexStr[i])) {
				fprintf(stderr, "Invalid hexadecimal input.\n");
				return 1;
			}
		}

		// Use sscanf to parse the hexadecimal string
		int sscanfResult = sscanf(hexStr, "%hx", &instruction_read);
		if (sscanfResult != 1) {
			fprintf(stderr, "Failed to parse hexadecimal input.\n");
			return 1;
		}

		// Display the stored value
		printf("Stored data as unsigned short: %u (decimal) or 0x%04X (hexadecimal)\n", instruction_read, instruction_read);
#endif
		//////////////////////////////////////////////////////////////////////////////////

		// Need to check if something does exist there or not if it is 0 then move out of the loop 
		// Just for the porpose of the lab 2 
		if (instruction_read == 0 || instruction_read == -1) {
			printf("End of the program\n");
			conti = 'n';
		}
		else {
			unsigned first3 = GET_TOP3(instruction_read);

			switch (first3) {
			case 0:
				// This is the branch with link instruction
				DISPLAY_BL(instruction_read);
				break;
			case 1:
				//printf("Branch second\n");
				DISPLAY_Branch(instruction_read);
				break;
			case 2:
				if (GET_BITS_12_10(instruction_read) >= 2) {
					printf("Invalid Instruction\n");
					printf("0x%04X \n", memory.word[Prog_Counter / 2]);
					break;
				}
				else
				{
					DISPLAY_ARTH(instruction_read);
					break;
				}

			case 3:
				printf("Move instructions \n");
				printf("0x%04X \n", memory.word[Prog_Counter / 2]);
				break;
			case 4:
			case 5:
				printf("LDR \n");
				printf("%04X \n", memory.word[Prog_Counter / 2]);
				break;
			case 6:
			case 7:
				printf("STR \n");
				printf("%04X \n", memory.word[Prog_Counter / 2]);
				break;
			default:
				printf("Shit is fucked gove up on life \n");
				break;

			}
			printf("Continue reading (y/n)?: ");
			scanf(" %c", &conti);
			getchar(); // Gets rid of the newline in the buffer

			printf("Do you want to update Program Counter (Y/N): ");
			update_PC = getchar();

			if (update_PC == 'y' || update_PC == 'Y') {
				UPDATE_PC();					
			}
			else {
				Prog_Counter += 2;
			}
			

		}
		
		
	}

}