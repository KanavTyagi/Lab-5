/*
 * File Name: execute_instruction.c
 * Created: 29th January 2025
 * By: Kanav Tyagi
 *
 * Description:
 * Contains functions responsible for decoding and displaying various types of instructions,
 * and handling user interactions using a menu-driven interface.
 *
 * Functions:
 * Execute_BL: Displays the contents related to the Branch with Link (BL) instructions.
 * Execute_Branch: Displays other branch instructions based on the decoded opcode.
 * DISPLAY_ARTH: Displays arithmetic and logical instructions with their details.
 */


#include "Loader.h"
#include "Decode.h"
#include "Load_instruction.h"
#include "Branch_Inst.h"


// Turn on ad off diagnostics
//#define DEBUG


BranchEntry branch[] = {
	{ "BEQ/BZ",   cond_BEQ_BZ   },
	{ "BNE/BNZ",  cond_BNE_BNZ  },
	{ "BC/BHS",   cond_BC_BHS   },
	{ "BNC/BLO",  cond_BNC_BLO  },
	{ "BN",       cond_BN       },
	{ "BGE",      cond_BGE      },
	{ "BLT",      cond_BLT      },
	{ "BRA",      cond_BRA      },
};

PSW psw = { Initilizer }; // Initialize the program status word to be all zeros
// all flags are set to false  

// This is the array that is going to be used to store the values of the registers and the constant values
// The values had been initialized to the values that are given in the lab

// this function is going to be used to prompt the contents of the Branch always instruction
void Execute_BL() {
	
	signed short offset = Instruction_Register & BL_Offset; // Extract 13-bit offset

#ifdef DEBUG
	printf("Encoded Offset Raw : %04X\n", offset);
#endif // DEBUG

	// Sign extend the 13-bit offset to 16 bits
	if (GET_BIT(offset, BL_Sign_Bit)) { // Check if bit 12 is set , which would mean that the offset is negative 
		offset |= BL_Sign_Extend; // Extend sign bit by OR-ing upper bits
#ifdef DEBUG
		printf("Offset Neagtive new offset calculated : %04X\n", offset);
#endif // DEBUG
	}

	// Shift left as the machine code is shifted right by 1 bit (bit 0 is always 0) because of how the program counter is incremented
	// and can only be even 
	offset <<= Offset_Shift;
#ifdef DEBUG
	// Printing this while debuging to make sure we are updating the PC correctly 
	printf("Instruction: BL\n"); // Display the instruction
	printf("Encoded Offset: %X\n", (unsigned short)offset); // Display the offset

	signed short New_PC = PC + offset;
	printf("Current PC: %04X\n", PC); // Display the current PC
	printf("New PC: %04X\n", New_PC); // Display what the new PC would have been
#endif
	
	// Before we update the PC we need to store the return address in the link register
	LR = PC; // Store the return address in the link register
	PC = PC + offset; // Update the PC

}

// this would eexecute the Branching instructionss by calling the function for checking
// the needed flags 
void Execute_Branch(void) {
	// This is the instruction that is going to be used to execute teh BL instruction

	// the first thing we would do is check if the instruction's required flag is met or not 

	BranchEntry entry = branch[GET_BITS_12_10(Instruction_Register)]; // Get the entry from the table using the bits 12 11 10

	bool shouldBranch = entry.check_cond(); // Check if the condition is met
	
	if (shouldBranch) {
		// WE WILL NEED TO GET THE OFFSETT FROM THE INSTRUCTION

		signed short offset = Instruction_Register & Branch_Offset; // Getting the offset from the instruction 

#ifdef DEBUG
		printf("instruction number : %04X\n", GET_BITS_12_10(Instruction_Register));

#endif // DEBUG
		// Now we need to check if the offset is negative or positive
		if (GET_BIT(Instruction_Register, Branch_Sign_Bit) == TRUE) { // Bit 9 is the sign bit so we would check if it is set or not
			// If the offset is negative then we need to sign extend it
#ifdef DEBUG
			printf("Negative\n");
			printf("Offset: %d\n", offset); // Displaying the offset
#endif // DEBUG

			offset |= Branch_sign_extend; // Extending the sign bit 

		}

		// Shift left as the machine code is shifted right by 1 bit (bit 0 is always 0) 
		offset <<= Offset_Shift;
#ifdef DEBUG
		printf("Instruction: %s\n", branch[GET_BITS_12_10(Instruction_Register)]); // Display the instruction by getting it from the array using the buts 12 11 10
		printf("Encoded Offset: %0X\n", (unsigned short)offset); // Display the offset that is encoded in the instruction using typecasting to pront it accurately 

		signed int New_PC = PC + offset; // Use wider type for PC calculation
		printf("Current PC: %04X\n", PC);
		printf("New PC: %04X\n", New_PC);
#endif

		PC = PC + offset; // Update the PC
	}

#ifdef DEBUG
	if(!shouldBranch) {
		printf("Instruction: %s\n", branch[GET_BITS_12_10(Instruction_Register)]); // Display the instruction by getting it from the array using the buts 12 11 10
		printf("Condition not met\n"); // Display the condition not met
	}
#endif
	// we would use a table look up to see if we can see if we need to perform the instruction or not 

}

bool cond_BEQ_BZ(void) {
	// Branch if Zero
	return ZERO;
}

bool cond_BNE_BNZ(void) {
	// Branch if Not Zero
	return !ZERO;
}

bool cond_BC_BHS(void) {
	// Branch if Carry set => (unsigned higher or same)
	return CARRY;
} 

bool cond_BNC_BLO(void) {
	// Branch if Carry clear
	return !CARRY;
} 

bool cond_BN(void) {
	// Branch if Negative
	return NEGATIVE;
}

bool cond_BGE(void) {
	// Branch if Greater or Equal (signed)
	// Equivalent to: (N == V)
	return (NEGATIVE == OverFlow);
}

bool cond_BLT(void) {
	// Branch if Less (signed)
	// Equivalent to: (N != V)
	return (NEGATIVE != OverFlow);
}

bool cond_BRA(void) {
	// Branch Always
	return TRUE;
}