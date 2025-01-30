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
//void decode_instruction(unsigned short instruction) {
//
//}



// Function to fetch an instruction from memory using the bus function
unsigned short fetch_instruction() {
	unsigned short instruction_read = 0; // Initialize to 0 

    // Read instruction from memory
	mar = Prog_Counter; // Set the memory address register to the program counter
	bus(mar, &mbr, READING, WORD); // Read a word from memory
	instruction_read = mbr; // Get the instruction that is stored in the memory buffer register
	
	return instruction_read; // Return the instruction that is read from the memory
}

// Function to decode an instruction
// This function will take an instruction as input and decode it
// The instruction passed can be from the memory or a custom instruction
void decode_instruction(unsigned short instruction_read) {

	unsigned first3 = GET_TOP3(instruction_read); // Get the top 3 bits of the instruction to decode the first step
    
	// Decode the instruction based on the first 3 bits
    switch (first3) {
    
	case 0: // Branch with link instruction is the only instruction in this group
        DISPLAY_BL(instruction_read);
        break;

	case 1: // Branch instructions other the branch with link
        DISPLAY_Branch(instruction_read);
        break;

	case 2: // Arthametic and logical instructions group
        // And for the instruction that would need to be decoded in this lab 
		// any with the next 3 bit being more than 2 would be invalid
        if (GET_BITS_12_10(instruction_read) >= 2) {
            printf("Invalid Instruction\n");
            printf("0x%04X\n", instruction_read); // Display the instruction that is read from the memory
            break;
        }
        else {
            DISPLAY_ARTH(instruction_read);
            break;
        }
    case 3:
        printf("Move instruction: 0x%04X\n", instruction_read);
        break;
    case 4:
    case 5:
        printf("LDR: 0x%04X\n", instruction_read);
        break;
    case 6:
    case 7:
        printf("STR: 0x%04X\n", instruction_read);
        break;
    default:
        printf("Unknown instruction.\n");
		printf("0x%04X\n", instruction_read); // Display the instruction that is read from the memory
        break;
    }
}


// Function to read the next instruction from memory and decode it
void read_next_instruction() {

	//Prog_Counter += 2; // updatins the program counter to the read the next instructions
    unsigned short instruction_read = fetch_instruction();
    printf("Instruction read: 0x%04X \n", instruction_read);

    // Check for end of program
    if (instruction_read == 0 || instruction_read == (unsigned short)-1) {
        printf("End of program.\n");
        return;
    }

    decode_instruction(instruction_read);
	Prog_Counter += 2;  // Increment program counter by 2 here as this is where we would have read an instruction from the memory 
	// and would need to update the program counter to the next instruction
	// this is going to be for just  the purpose of the lab 2 
    // for the next change this 
}





// Main menu function
// This function is going to be used to give the user the menue
// then according to the users choice the appropriate function would be called
void instruction_menue(void) {
	char choice; // User choice
	int continue_program = 1;  // Control flag to make sure we can exit the loop

    while (continue_program) {
        // Display Menu
        printf("\n===== MENU =====\n");
        printf("Current Program Counter : %04X\n", Prog_Counter);
        printf("1. Read next instruction\n");
        printf("2. Input custom instruction\n");
        printf("3. Display memory\n");
        printf("4. Change program counter\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        // Get user choice
        scanf(" %c", &choice);
        getchar(); // Clear input buffer
		printf("\n");   
        // Handle user choice
        switch (choice) {

		case '1': // Read next instruction from memory
            read_next_instruction();
            break;

		case '2': // Input custom instruction and decode it
            input_custom_instruction();
            break;

		case '3': // Display memory contents
            display_memory();
            break;

		case '4': // Change program counter
            change_program_counter();
            break;

		case '5': // Exit program
            printf("Exiting...\n");
            continue_program = 0;  // Exit loop
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }
}





////////////////////////////////////////////////////////////////////
// this function is going to read teh instruction from the memory and then
// Will call the function to decode it 
//void find_instruction(void) {
//
//	char conti = 'y';
//	char update_PC = 'y';
//	
//	printf("\start reading : ");
//	conti = getchar();
//
//	char test;
//	while (conti == 'y' || conti == 'Y') {
//		unsigned short instruction_read = 0;
//
//		// Reading the instruction from the memory
//		// So we would set the value of mar to the program counter
//		mar = Prog_Counter;
//		bus(mar, &mbr, READING, WORD);
//		instruction_read = mbr; // Getting the instruction from the memory buffer register
//		printf("Instruction read 0x%04X \n", instruction_read);
//
//		//////////////////////////////////////////////////////////////////////////////////
//#ifdef TESTING
//
//		char input[10];       // Buffer to store user input
//
//		// Prompt the user for hexadecimal input
//		printf("Enter testing data in hexadecimal (e.g., 1A3F or 0x1A3F): ");
//
//		// Read the input as a string
//		if (scanf("%9s", input) != 1) {  // Limit input to prevent buffer overflow
//			fprintf(stderr, "Error reading input.\n");
//			return 1;
//		}
//
//		// Optional: Remove "0x" or "0X" prefix if present
//		char* hexStr = input;
//		if (strlen(input) > 2 && input[0] == '0' && (input[1] == 'x' || input[1] == 'X')) {
//			hexStr += 2;
//		}
//
//		// Check if all characters are valid hexadecimal digits
//		for (size_t i = 0; i < strlen(hexStr); i++) {
//			if (!isxdigit((unsigned char)hexStr[i])) {
//				fprintf(stderr, "Invalid hexadecimal input.\n");
//				return 1;
//			}
//		}
//
//		// Use sscanf to parse the hexadecimal string
//		int sscanfResult = sscanf(hexStr, "%hx", &instruction_read);
//		if (sscanfResult != 1) {
//			fprintf(stderr, "Failed to parse hexadecimal input.\n");
//			return 1;
//		}
//
//		// Display the stored value
//		printf("Stored data as unsigned short: %u (decimal) or 0x%04X (hexadecimal)\n", instruction_read, instruction_read);
//#endif
//		//////////////////////////////////////////////////////////////////////////////////
//
//		// Need to check if something does exist there or not if it is 0 then move out of the loop 
//		// Just for the porpose of the lab 2 
//		if (instruction_read == 0 || instruction_read == -1) {
//			printf("End of the program\n");
//			conti = 'n';
//		}
//		else {
//			unsigned first3 = GET_TOP3(instruction_read);
//
//			switch (first3) {
//			case 0:
//				// This is the branch with link instruction
//				DISPLAY_BL(instruction_read);
//				break;
//			case 1:
//				//printf("Branch second\n");
//				DISPLAY_Branch(instruction_read);
//				break;
//			case 2:
//				if (GET_BITS_12_10(instruction_read) >= 2) {
//					printf("Invalid Instruction\n");
//					printf("0x%04X \n", memory.word[Prog_Counter / 2]);
//					break;
//				}
//				else
//				{
//					DISPLAY_ARTH(instruction_read);
//					break;
//				}
//
//			case 3:
//				printf("Move instructions \n");
//				printf("0x%04X \n", memory.word[Prog_Counter / 2]);
//				break;
//			case 4:
//			case 5:
//				printf("LDR \n");
//				printf("%04X \n", memory.word[Prog_Counter / 2]);
//				break;
//			case 6:
//			case 7:
//				printf("STR \n");
//				printf("%04X \n", memory.word[Prog_Counter / 2]);
//				break;
//			default:
//				printf("Shit is fucked gove up on life \n");
//				break;
//
//			}
//			printf("Continue reading (y/n)?: ");
//			scanf(" %c", &conti);
//			getchar(); // Gets rid of the newline in the buffer
//
//			printf("Do you want to update Program Counter (Y/N): ");
//			update_PC = getchar();
//
//			if (update_PC == 'y' || update_PC == 'Y') {
//				UPDATE_PC();					
//			}
//			else {
//				Prog_Counter += 2;
//			}
//			
//
//		}
//		
//		
//	}
//
//}