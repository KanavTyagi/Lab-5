/*
 * File Name: fetch.c
 * Created: 29th January 2025
 * By: Kanav Tyagi
 *
 * Contains functions responsible for managing memory access operations, fetching and decoding instructions,
 * and handling user interactions through a menu interface.
 *
 * bus: Handles reading from and writing to memory based on the specified parameters and uses the memory buffer register and memory address register.
 * fetch_instruction: Retrieves the next instruction from memory using the program counter.
 * decode_instruction: Decodes an instruction passed to it and calls the corresponding function to execute the operation.
 * instruction_menu: Presents a menu to the user for interacting with the memory and instructions. 
 *
 * DEBUG: When enabled, provides detailed diagnostic logs for memory operations and instruction handling.
 * TESTING: When enabled, allows for testing the output of instructions. (functionality removed and move to the menue as an option for user to add a custom instruction)
 *
 */


#include "Loader.h"
#include "Decode.h"
#include "Load_instruction.h"
#include "Branch_Inst.h"


// Turn on and off diagnostics 
//#define DEBUG

// To test the instruction output uncomment the line to be able to test the output of the instructions
//#define TESTING

// This function is going to be used to read and write instruction to the memory
// When Fetching instructions 
// mar gets the current PC
// mbr gets the data from the memory location at mar which is the Program Counter 
// can access it in word or byte
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


// Function to fetch an instruction from memory using the bus function
// As we are fetching instructions we would be reading from the memory 
// The instruction is 2 bytes long so we would read a word from the memory
void fetch_instruction(void) {

    // To Read instruction from memory we would use the bus 
	mar = PC; // Set the memory address register to the program counter
	bus(mar, &mbr, READING, WORD); // Read a word from memory as the instructions are 2 bytes long 
    Instruction_Register = mbr; // Get the instruction that is stored in the memory buffer register
    validate_PC(PC);
    if (PC < Max_PC) { // If the program counter is at the end of the memory then the program has ended
        PC += Prog_Step;  // Increment program counter by 2 here as this is where we would have read an instruction from the memory 
        // and would need to update the program counter to the next instruction
        // this is going to be for just  the purpose of the lab 2 
        // for the next tasks change this

    }
    else {
        
    }


    validate_PC(PC); // Validate the program counter after updating it
	
	// Returns the instruction that is read from the memory	return instruction_read; // Return the instruction that is read from the memory
}

// Function to decode an instruction
// This function will take an instruction as input and decode it 
// The instruction passed can be from the memory or a custom instruction that has been entered by the user
void decode_instruction(void) {

    // Get the top 3 bits of the instruction to decode the first step
	unsigned first3 = GET_TOP3(Instruction_Register);
    
	// Decode the instruction based on the first 3 bits
    switch (first3) {
    
	case 0: // Branch with link instruction is the only instruction in this group
        DISPLAY_BL();
        break;

	case 1: // Branch instructions other the branch with link
        DISPLAY_Branch();
        break;

	case 2: 
        switch (GET_BITS_12_10(Instruction_Register)) {
        case 0:
        case 1:
		case 2:
            DISPLAY_ARTH();
            break;
			// case 3 adn case 4 are going to be invalid instructions
            // case 5 is illegal not to be implemented ever do that change ;ater
        case 6:
			printf("Load_instruction()\n");
            Load_instruction(); // execute the load instruction
            break;
        case 7:
			printf("Store_instruction()\n"); // execute the store instruction
			break;
        default:
			printf("Invalid Instruction\n");
			printf("0x%04X\n", Instruction_Register); // Display the instruction that is read from the memory
			break;
        }
        // Arthametic and logical instructions group
        // And for the instruction that would need to be decoded in this lab 
		// any with the next 3 bit being more than 2 would be invalid
        if (GET_BITS_12_10(Instruction_Register) >= 2) {
            printf("Invalid Instruction\n");
            printf("0x%04X\n", Instruction_Register); // Display the instruction that is read from the memory
            break;
        }
		else { //  If the instruction is valid then decode it
            
        }
	case 3: // These would be the move instructions and we would not be needed to decode in this lab 
		    // so we would just display the instruction
        printf("Move instruction: 0x%04X\n", Instruction_Register);
        break;
	case 4: // These both would be the load instructions and we would not be needed to decode in this lab 
    case 5: // so we would just display the instruction
        printf("LDR: 0x%04X\n", Instruction_Register);
		
        break;
	case 6: // These both would be the store instructions and we would not be needed to decode in this lab
	case 7: // so we would just display the instruction
        printf("STR: 0x%04X\n", Instruction_Register);
        break;
	default: // If the instruction is not in the above groups then it is invalid for our use case 
        printf("Unknown instruction.\n");
		printf("0x%04X\n", Instruction_Register); // Display the instruction that is read from the memory
        break;
    }
}







// Main menu function
// This function is going to be used to give the user the menue
// then according to the users choice the appropriate function would be called
void instruction_menue(void) {
	char choice; // User choice
	int continue_program = TRUE;  // Control flag to make sure we can exit the loop
                                  // Would need to be updated in the next tasks 
								  // Implement the Ctrl + C to exit the program

    while (continue_program) {
        // Display Menu
        printf("\n===== MENU =====\n");
        printf("Current Program Counter : %04X\n", PC);
        printf("0. Exit\n");
        printf("1. Read next instruction\n");
        printf("2. Input custom instruction\n");
        printf("3. Display memory\n");
        printf("4. Change program counter\n");
        printf("5. Print out the Register\n");
        printf("Enter your choice: ");

        // Get user choice
        scanf(" %c", &choice);
        getchar(); // Clear input buffer
		printf("\n");   
        // Handle user choice
        switch (choice) {
        case '0': // Exit program
            printf("Exiting...\n");
            continue_program = FALSE;  // Exit loop
            break;

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
		case '5': // Print out the Register
			print_register_file();
			break;

        default:
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }
}




