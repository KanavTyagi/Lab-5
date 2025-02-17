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
#include "ALU.h"

 // Uncomment the following line to enable DEBUG mode
// #define DEBUG

// uncomment the following to print the registers in the bytes format
// #define Chk_Mem_Bytes

#define DEBUGPSW
unsigned carry[2][2][2] = { 0, 0, 1, 0, 1, 0, 1, 1 };
unsigned overflow[2][2][2] = { 0, 1, 0, 0, 0, 0, 1, 0 };


regis_file Register_file = {
		.WORD = {
			{ 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }, // Registers being initialized to 0
			{ 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0xFFFF }  // Constants getting the constant value 
		}
};
// This is the function that is going to be used to print the register file

void print_register_file(void) {

	for (int i = 0; i < NUM_REGISTERS; i++) {
		printf("Register %d: 0x%04X\n", i, Register_file.WORD[0][i]);
	}
#ifdef Chk_Mem_Bytes
	for (int i = 0; i < NUM_REGISTERS * 2; i++) {
		printf("Register %d: 0x%02X\n", i / 2, Register_file.BYTE[0][i]);
	}
#endif
}
void print_PSW(void) {
	printf("Carry : %d\n", CARRY);
	printf("Zero :  %d\n", psw.Z);
	printf("Neagative :  %d\n", NEGATIVE);
	printf("OverFlow :  %d\n", OverFlow);
}


// this function would be used to update the psw register 
// it wil get the value of the registers and the results it would
// use the truth table to to update the rgisters 

void update_psw(unsigned short src, unsigned short dst, unsigned short res,
	unsigned short wb, Add_OR_Comp Add_comp)
{
	/*
	 - Update the PSW bits (V, N, Z, C)
	 - Using src, dst, and res values and whether word or byte
	 - ADD, ADDC, SUB, and SUBC
	*/
	
	unsigned short mss, msd, msr; /* Most significant src, dst, and res bits */

	if (wb == 0)
	{
		mss = B15(src);
		msd = B15(dst);
		msr = B15(res);
	}
	else /* Byte */
	{
		mss = B7(src);
		msd = B7(dst);
		msr = B7(res);
		res &= 0x00FF;	/* Mask high byte for 'z' check */
	}
	// the zero and the negative flags would be needed to set in all cases 
	/* Zero */
	ZERO = (res == 0);

	/* Negative */
	NEGATIVE = (msr == 1);

	// we would only need to update the overflow and carry flags if we are doing
	// the addidtion operations
	if (Add_comp == Addition) {
		/* oVerflow */
		OverFlow = overflow[mss][msd][msr];
		/* Carry */
		CARRY = carry[mss][msd][msr];

	}
#ifdef DEBUGPSW
	printf("Carry : %d\n", CARRY);
	printf("Zero :  %d\n", psw.Z);
	printf("Neagative :  %d\n", NEGATIVE);
	printf("OverFlow :  %d\n", OverFlow);
#endif
	
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
		Source_Register(SSS, REGISTER) = (increment ? Source_Register(SSS, REGISTER) + 2 : Source_Register(SSS, REGISTER) - 2);
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
		mar = PC;
		bus(mar, &mbr, READING, W_B_BIT);
		Destination_Register(DDD) = mbr;
		//printf("\n\n\n\testing success \n\n\n\n");
	}
	else if (GET_BIT(Instruction_Register, W_B_BIT) == BYTE) {
		// if the W/B bit is  set then we would have to load a byte from the memory
		// we would have to get the byte from the memory and then load it into the destination register
		bus(mar, &mbr, READING, W_B_BIT);

		Destination_Register_Low_Byte(DDD) = (unsigned char)mbr;
	}

	// now we would check if we want to do the post increment or decrement
	if (!prepose) {
		// in this there is chance that there is no change 
		// so what we would do is directly add the increment and the decrement bit
		// if either of them is set then this would increment or decrement the register
		// if neither is set then this would not do anything

		Source_Register(SSS, REGISTER) += (increment - decrement)*2;

	}
#ifdef DEBUG
	printf("\n function Ended \n");
	printf("Source Register R%d : %04X\n", SSS, Source_Register(SSS, REGISTER));
	printf("Destination Register R%d : %04X\n", DDD, Destination_Register(DDD));
	printf("Memory at source is : %04X \n", MEMORY_WORD(Source_Register(SSS, REGISTER)));
	printf("Memory at Destination is : %04X \n", MEMORY_WORD(Destination_Register(DDD, REGISTER)));
#endif 
}

// now we would need to implement the move instruction

void Execute_Move(void) {
	// first we would need to get the destination register
	unsigned short DDD = GET_DDD(Instruction_Register); // This is the destination register index from the register file
	// now we could access the refgister using the  macros we have

#ifdef DEBUG
	// print to check if the right regoster is being used
	printf("Value Read : %d\n", MOVE_OP_BITS(Instruction_Register));
	printf("Before Moving\n");
	printf("Destination Register R%d : %04X\n", DDD, Destination_Register(DDD));
#endif
	//  now we need to extarct the value from the instruction
	unsigned char value = GET_MOVE_VALUE(Instruction_Register);


	// we would get the bits that would tell us what move fucntion we are dealing with 
	switch (MOVE_OP_BITS(Instruction_Register)) {
		// since we are going to be modifying the bytes then we would access the memory as byte

	case MOVL: // the highbbyte would be unchanged and the lower byte will be modified
		Destination_Register_Low_Byte(DDD) = value; // changing the low byte 
		break;

	case MOVLZ: // the high byte woul be cleared and the lower byte modified
		Destination_Register_Low_Byte(DDD) = value;
		Destination_Register_High_Byte(DDD) = 0x00; // this would make the higher bits 0 and the loweer would still be the updated value 
		break;

	case MOVLS: // the high byte would be set and the low would be modified
		Destination_Register_Low_Byte(DDD) = value;
		Destination_Register_High_Byte(DDD) = 0xFF; // this would make the higher bits 1 and the loweer would still be the updated value 
		break;

	case MOVH: // the high byte modified and the lowerbyte unchanged
		Destination_Register_High_Byte(DDD) = value;
		break;
	};
#ifdef DEBUG
	// print to check if the right regoster is being used
	printf("after Moving\n");
	printf("Destination Register R%d : %04X\n", DDD, Destination_Register(DDD));
#endif

}