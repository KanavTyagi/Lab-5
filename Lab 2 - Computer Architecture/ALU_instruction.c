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
#include "ALU.h"


#define DEBUG

 // This is the array that is going to be used to store the names of the arthametic and logical instructions that are to be used in this lab
char* arthamatic[Arthanetic_Instruction_Groups][Arthametic_Instruction] = { {"ADD" , "ADDC" ,"SUB", "SUBC"},
																			{"DADD", "CMP", "XOR", "AND"},
																			{"OR", "BIT", "BIC", "BIS"} };

// This function is going to be used to display the Arthametic and logical instructions
// we would need to get the source and the destination from the instruction
// and then we would need to get the R/C and W/B bits from the instruction
// and then we would use the other parts of the opcodes to get the instruction name 
// Instruction: ADD 
// R / C: 1
// W / B : 1
// Source : 01
// Destination : A5
void DISPLAY_ARTH(void) {


	

	if (GET_BITS_12_10(Instruction_Register) == 0) {

		execute_ADD();
	}

	unsigned short R_C = GET_BIT(Instruction_Register, R_C_BIT); // Getting the R/C bit from the opcode which is saved in bit 7
	unsigned short W_B = GET_BIT(Instruction_Register, W_B_BIT); // Getting the W/B bit from the opcode which is saved in bit 7
	unsigned short Source = Register_file.WORD[R_C][GET_BITS_5_3(Instruction_Register)]; // Getting the source from the opcode
	unsigned short Destination = Register_file.WORD[0][GET_BITS_2_0(Instruction_Register)]; // Getting the destination from the opcode
#ifdef DEBUG1

	printf("%c\n", R_C);
	printf("%c\n", W_B);
	printf("BYTE 5-3: %02X\n", GET_BITS_5_3(Instruction_Register));
	printf("BYTE 2-0: %02X\n", GET_BITS_2_0(Instruction_Register));
	printf("BYTE 12 - 10: %02X  9 - 8 : %02X ", GET_BITS_12_10(Instruction_Register), GET_BITS_9_8(Instruction_Register));
	printf("%02X\n", Source);
	printf("%02X\n", Destination);
#endif // DEBUG


	printf("Instruction: %s\n", arthamatic[GET_BITS_12_10(Instruction_Register)][GET_BITS_9_8(Instruction_Register)]);
	printf("R/C: %d\n", R_C);
	printf("W/B: %d\n", W_B);

	// Check if W/B is set to byte or word
	if (W_B == TRUE) {
		printf("Source: %01X\n", Source & 0xFF); // Mask the source to get the lower byte
		printf("Destination: %01X\n", Destination & 0xFF); // Mask the destination to get the lower byte
	}
	else {
		printf("Source: %02X\n", Source);
		printf("Destination: %02X\n", Destination);
	}

}

// the following code would be used to execute the ADD ADDC SUB SUBC functions 
// and we would update the psw within this.  
/// Add Tested
/// Sub Tested
void execute_ADD(void) {
	// first we would need 
	
	bool R_C = GET_BIT(Instruction_Register, R_C_BIT); // Getting the R/C bit from the opcode which is saved in bit 7
	bool W_B = GET_BIT(Instruction_Register, W_B_BIT); // Getting the W/B bit from the opcode which is saved in bit 7


	unsigned short SSS = GET_BITS_5_3(Instruction_Register); // Getting the source from the opcode
	unsigned short DDD = GET_BITS_2_0(Instruction_Register); // Getting the destination from the opcode
	// we would get the value from the register file
	// we would store it in the variable as if it is a subtract then we would need to take the cmplement
	unsigned int source_val = Source_Register(SSS, R_C);


	// we would need to keep track of this as we would use this
	// val to update the value of the psw register. 
	unsigned short Destination_val = Destination_Register(DDD); 
	// we would use this to keep a track of the carry flag and if we need to add 
	// 1 to the compliment when subtracting 
	unsigned char Extra_add_bit = Initilizer;
#ifdef DEBUG
	printf("Source: %1X\n", SSS);
	printf("Destination: %1X\n", DDD);
	printf("BYTE 12 - 10: %02X  9 - 8 : %02X ", GET_BITS_12_10(Instruction_Register), GET_BITS_9_8(Instruction_Register));
	printf("Source val : %04X\n", source_val);
	printf("Destination val: %04X\n", Destination_Register(DDD));
#endif 




	// if it is the SUB or the SUBC then we would need to take the compliment of the 
	// value in the stored bit
	if (GET_BITS_9_8(Instruction_Register) == 2 || GET_BITS_9_8(Instruction_Register) == 3){
		source_val = ~source_val;
		
		// we would also need to increment the extra add bit as that
		// would be need to add 1 to make it 2's compliment 
		source_val += 1; 
	}

	// also if the function is ADDC or SUBC then we would also need to take into account the 
	// carry bit. 
	if (GET_BITS_9_8(Instruction_Register) == 3 || GET_BITS_9_8(Instruction_Register) == 1) {
		// now we would need to take into account the carry bit 
		Extra_add_bit += CARRY;
	}
	// if we are doing the instruction on word that is the whole 2 bytes then
	// we would access the register as words
	if (W_B == WORD) {

		// we are going to typecast the source val as a signed short
		Destination_Register(DDD) = Destination_Register(DDD) + (signed short)source_val;
	}
	// implement the byte add logic 
	else {
		//////////////////////////////////
		// will do this on the lower bytes of the register only
		// add the carry here as well and since we are already getting the value of carry
		// in the Extra_add_bit we dont need to vory about it
		Destination_Register_Low_Byte(DDD) = Extra_add_bit + (signed short)source_val;

	}
#ifdef DEBUG
	printf("\n\n After Function being performed \n\n");
	printf("Value that was used as the source : %04X\n", (unsigned short)source_val); 
	printf("Source val R%d : %04X\n", SSS, Source_Register(SSS,REGISTER));
	printf("Destination val R%d: %04X\n", DDD, Destination_Register(DDD));
#endif 

	// now we would need to update the psw registers 
	// we would need to pass the sorurce the original destination registe val
	// and then the result after the calculation
	// and since we are already updating the destination register that would 
	// be the current value of the destination register
	// and we are not using pointers as we dont want the value to be updated 
	update_psw((unsigned short)source_val, Destination_val, Destination_Register(DDD), W_B);



}