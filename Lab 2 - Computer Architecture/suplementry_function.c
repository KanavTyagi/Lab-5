#include "Debugger.h"
#include "Loader.h"


// This function is going to be used to change the program counter
// by taking the input from the user
void change_program_counter() {

	unsigned int new_pc; // Variable to store the new program counter

    printf("Enter new Program Counter (hexadecimal, e.g., 1A3F): ");
	if (scanf("%x", &new_pc) == 1) { // Read a hexadecimal number
		Prog_Counter = new_pc; // Update the program counter
        printf("Updated Program Counter: %04X\n", Prog_Counter);
    }
	else { // Invalid input would take the user back to the main menue 
        printf("Invalid input!\n");
    }

	getchar(); // Clear input buffer
}

// Function to input a custom instruction from the user and decode it
void input_custom_instruction() {
    unsigned short instruction_read;

    printf("Enter a custom instruction in hex: ");
    if (scanf("%hx", &instruction_read) != TRUE) {
        printf("Invalid input. Please enter a valid hex instruction.\n");
        getchar(); // Clear input buffer in case of invalid input
        return;
    }
    getchar(); // Clear input buffer

    printf("Stored custom instruction: 0x%04X\n", instruction_read);

    // Decode the custom instruction
    decode_instruction(instruction_read);
}


