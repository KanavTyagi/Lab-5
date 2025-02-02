/*
 * File Name: Register file function.v
 * Created: 31st January 2025
 * By: Kanav Tyagi
 *
 * Description:
 *
 * Functions:
 * void print_register_file(void): 
 *
 */


#include "Decode.h"
#include "Loader.h"
#include "Load_instruction.h"
#include "Branch_Inst.h"

 // Uncomment the following line to enable DEBUG mode
#define DEBUG




// This is the function that is going to be used to print the register file
void print_register_file(void) {

	for (int i = 0; i < NUM_REGISTERS; i++) {
		printf("Register %d: 0x%04X\n", i, Register_file[0][i]);
	}
}



// This is the function that is going to be used to load data into the memory
// this is going to get the data from the memry address stored in the SSS(543)
// and then load it into the memory location specifed in the DDD(210)
// can also update the value in the register as it can be used for indexed addressing
// allows pre pose increment and decrement of teh source only 
void Load_instruction(void) {

	// first we need to get the source and destination register

	unsigned short SSS = GET_SSS(Instruction_Register); // This is the source register index from the regoster file
	unsigned short DDD = GET_DDD(Instruction_Register); // This is the destination register index from the register file
	// now we could access the refgister using the  macros we have
#ifdef DEBUG
	printf("\n At function start\n");
	printf("Changing memory at source to be 0001\n");
	MEMORY_WORD(Source_Register(SSS, REGISTER)) = 0x0001;
	printf("Source Register R%d :  , %04X\n", SSS, Source_Register(SSS, REGISTER)); 
	printf("Destination Register R%d : %04X\n", DDD, Destination_Register(DDD));
	printf("Memory at source is : %04X \n", MEMORY_WORD(Source_Register(SSS, REGISTER)));
	printf("Memory at Destination is : %04X \n", MEMORY_WORD(Destination_Register(DDD, REGISTER)));

#endif

	// getting the bits into the variables
	// so we dont have to keep on checking the bits again and again using the macro 
	bool prepose = GET_BIT(Instruction_Register,PRPO); // This is the flag that is going to be used to check if the prepose bit is set
	bool decrement = GET_BIT(Instruction_Register, DEC); // This is the flag that is going to be used to check if the decrement bit is set
	bool increment = GET_BIT(Instruction_Register, INC); // This is the flag that is going to be used to check if the increment bit is set
#ifdef DEBUG
	printf("Prepose: %d\n", prepose);
	printf("Decrement: %d\n", decrement);
	printf("Increment: %d\n", increment);
#endif
	// if the prpose is set it would mean that we are looking for a pre increment or decrement 
	// so we would have to increment or decrement it before we can use it
	if (prepose) {

		// in this we know for sure that we are going to be using the registers as indexed addressing so will not use constants
		// we would have to increment or decrement the register before we can use it
		// we can only increment or decrement the source register
		// if we check the increment adn it is false then that would mean the decrement bit is set so we would
		// decrement the register SSS
		Source_Register(SSS, REGISTER) = (increment ? Source_Register(SSS, REGISTER) + 1 : Source_Register(SSS, REGISTER) - 1);
#ifdef DEBUG
		printf("Source Register R%d :  , %04X\n", SSS, Source_Register(SSS, REGISTER));
		printf("Destination Register R%d : %04X\n", DDD, Destination_Register(DDD));
#endif
	}

#ifdef DEBUG
	printf("\n Before memory access \n");
	printf("Memory at source is : %04X \n", MEMORY_WORD(Source_Register(SSS, REGISTER)));
	printf("Memory at Destination is : %04X \n", MEMORY_WORD(Destination_Register(DDD, REGISTER)));
#endif 

	// now we would implement the load instruction
	// if there is an post increment or decrement then we would have to increment or decrement
	// the register after we have used it
	// so we would do that after the instruction has been performed

	if (GET_BIT(Instruction_Register, W_B_BIT) == WORD) {
		// if the W/B bit is set then we would have to load a word from the memory
		// we would have to get the word from the memory and then load it into the destination register
		MEMORY_WORD(Destination_Register(DDD)) = MEMORY_WORD(Source_Register(SSS, REGISTER));
	}
	else if (GET_BIT(Instruction_Register, W_B_BIT) == BYTE) {
		// if the W/B bit is not set then we would have to load a byte from the memory
		// we would have to get the byte from the memory and then load it into the destination register
		MEMORY_BYTE(Destination_Register(DDD)) = MEMORY_BYTE(Source_Register(SSS, REGISTER));
	}

	// now we would check if we want to do the post increment or decrement
	if (!prepose) {
		// in this there is chance that there is no change 
		// so what we would do is directly add the increment and the decrement bit
		// if either of them is set then this would increment or decrement the register
		// if neither is set then this would not do anything

		Source_Register(SSS, REGISTER) += increment - decrement;
	}
#ifdef DEBUG
	printf("\n function Ended \n");
	printf("Source Register R%d : %04X\n", SSS, Source_Register(SSS, REGISTER));
	printf("Destination Register R%d : %04X\n", DDD, Destination_Register(DDD));
	printf("Memory at source is : %04X \n", MEMORY_WORD(Source_Register(SSS, REGISTER)));
	printf("Memory at Destination is : %04X \n", MEMORY_WORD(Destination_Register(DDD, REGISTER)));
#endif 
}


///