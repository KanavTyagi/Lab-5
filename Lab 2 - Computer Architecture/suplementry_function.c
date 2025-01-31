/*
 * File Name: supementry_function.c
 * Created: 29th January 2025
 * By: Kanav Tyagi
 *
 * Contains functions responsible for handling user interactions related to program
 * counter manipulation and custom instruction input. 
 *
 * change_program_counter: Allows the user to update the program counter by inputting a new hexadecimal value.
 * input_custom_instruction: Lets the user input a custom instruction in hexadecimal format and caalls the function to decode it.
 * read_next_instruction: Fetches the next instruction from memory, calls function to decodes it, and updates the program counter accordingly.
 *
 */



#include "Decode.h"
#include "Loader.h"


 // uncomment the line below to enable debug mode
//#define DEBUG

// This function is going to be used to change the program counter
// by taking the input from the user 
// the program counter should always be even as the memory is word addressable
// if it is invalud  then the program counter would not be updated 
// and we would display an error message and take the user back to the main menue

void change_program_counter() {

	unsigned int new_pc; // Variable to store the new program counter

    printf("Enter new Program Counter (hexadecimal, e.g., 1A3F): ");
	if (scanf("%x", &new_pc) == TRUE) { // Read a hexadecimal number

        if (validate_PC(new_pc)) {
            Prog_Counter = new_pc; // Update the Program Counter
			printf("Program Counter updated to: %04X\n", Prog_Counter); // Display the updated Program Counter
#ifdef DEBUG
            printf("Program Counter: %04X\n", new_pc);
#endif // DEBUG
        }
        // If validate_PC returns false, an error message has already been printed
    }

	else { // Invalid input would take the user back to the main menue 
        printf("Invalid input, Failed Reading\n");
    }

	getchar(); // Clear input buffer
}

// Function to validate the Program Counter (PC)
// Returns true if PC is valid, false otherwise
// making it a seperate function as we would need to validate the program counter in the future
// implementation of some of the function
bool validate_PC(unsigned int new_pc) {

    // But we would need to check if the program counter is even or odd as the memory is word addressable
    // and the program counter should always be even
    // if it is odd than that would be an invalid input 
    // (not sure if there is a need for having a constant to replace teh number 0  here, so not sure if this is regarded as a magic number or not)
	if (GET_BIT(new_pc, 0) != FALSE) { // getting the 0th bit of the new pc to check if it is even or odd if it is odd then this bit would be set
        printf("Invalid input! Program Counter must be even.\n");
#ifdef DEBUG
        printf("Invalid Program Counter: %04X\n", new_pc);
#endif // DEBUG
        return false;
    }
    else {
		return true;
    }

    // we dont need to check for the program counter to be in the range of the memory as the memory 
    // is 64Kib and the program counter is 16 bits so it will roll over and not go out of the memory
}



// Function to input a custom instruction from the user and decode it
void input_custom_instruction() {
    unsigned short instruction_read;

    printf("Enter a custom instruction in hex: ");
	if (scanf("%hx", &Instruction_Register) != TRUE) { // Read a hexadecimal number
		// If that is not a valid input then we would display an error message
		// and the user wwould go back to the main menue
        printf("Invalid input. Please enter a valid hex instruction.\n");
    }
	else { // if the instruction is valid then decode it

#ifdef DEBUG
        printf("Stored custom instruction: 0x%04X\n", Instruction_Register);
#endif

		// Decode the custom instruction entered by the user
        decode_instruction();
    }
    getchar(); // Clear input buffer


}



// Function to read the next instruction from memory and decode it
// and then update the program counter accordingly
void read_next_instruction() {

    
	// Read the instruction from memory
    fetch_instruction();
   

    // Check for end of program
    if (Instruction_Register == FALSE) { // If the instruction read is 0x0000 then the program has ended
		printf("End of program.\n"); // Display the end of the program

    }
    else {
		printf("Instruction read: 0x%04X \n", Instruction_Register); // Display the instruction that is read from the memory

        decode_instruction(); // Call the instruction that Decodes the instruction


    }
}
