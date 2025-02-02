#pragma once
//////////////////// This is lab 3 dont be cnfused with the lab 2 file //////////////////////
/*
 * File Name: Decode.h 
 * Created: 31st January 2025
 * By: Kanav Tyagi
 *
 * Description:
 * This header file defines macros for bit manipulation, enumerations for operation modes,
 * structures and unions for instruction handling, and declares external variables and functions
 * related to instruction decoding and memory operations.
 */
#include "Loader.h"
#include "Branch_Inst.h"


// Macros to extract specific bit fields from a 16-bit value
#define GET_TOP3(x)    ( ((x) >> 13) & 0x7  ) // this can get the top 3 bits from the value
#define GET_BITS_12_10(x)   ( ((x) >> 10) & 0x07  ) // this can get the 12th to 10th bit from the value
#define GET_BITS_9_8(x)   ( ((x) >> 8) & 0x3  ) // this can get the 9th to 8th bit from the value
#define GET_BIT(value, n)  ( ((value) >> (n)) & 0x1 ) // this can get the specified bit from the value
#define GET_BITS_5_3(x)   ( ((x) >> 3) & 0x7  ) // this can get the 5th to 3rd bit from the value
#define GET_BITS_2_0(x)   ( ((x) >> 0) & 0x7  ) // this can get the 2nd to 0th bit from the value



#define R_C_BIT 7 // This is the bit that is going to be used to get the R/C bit
#define W_B_BIT 6 // This is the bit that is going to be used to get the W/B bit
#define Arthanetic_Instruction_Groups 3 // This is the number of Arthametic and logical instructions groups we have created for the easy look up table 
#define Arthametic_Instruction 4 // This is the number of Arthametic and logical instructions in each group that we have created 
#define NUM_REGISTER_TYPES 2  // Two types: registers and constants
#define NUM_REGISTERS 8       // Number of registers/constants
#define Prog_Step 2 // This is the number of steps that the program counter is going to be incremented by
#define Initilizer 0 // This is the value that is going to be used to initilize the various variables including the memory the mbr , mar etc.. 
#define Offset_Shift 1 // This is the number of bits that the offset is going to be shifted by in the BL instruction
#define Max_PC 0xFFFE // This is the maximum value that the program counter can have




// This is the array that is going to be used to store the names of the 4
// instructions and be used to look up the names of the instructions
extern char* arthamatic[Arthanetic_Instruction_Groups][Arthametic_Instruction]; // This is the array that is going to be used to store the names of the Arthametic and logical instructions instructions
extern unsigned short Register_file[NUM_REGISTER_TYPES][NUM_REGISTERS]; // This is the array that is going to be used to store the values of the registers  and the constant values
extern unsigned short mbr; // This is the memory buffer register that is going to be used to store the data that is going to be read from the memory
extern unsigned short mar; // This is the memory address register that is going to be used to store the address that is going to be read from the memory
extern unsigned short Instruction_Register; // this is the register that is going to be used to store the instruction that is going to be read from the memory using the bus

// These enum will help us keep track of eh states weather we are writing or reading 
typedef enum {
    READING,
    WRITING
} Read_Or_Write;


//This will help track if we are using the word or the byte 
typedef enum {
    WORD,
    BYTE
} Word_Or_Byte;




// This function is going to be used to read and write instruction to the memory
extern void bus(unsigned short mar, unsigned short* mbr, Read_Or_Write R_OR_W, Word_Or_Byte W_O_B);
extern void fetch_instruction(void); // This function is going to be used to fetch the instruction from the memory
extern void decode_instruction(void); // This function is going to be used to decode the instruction
extern void read_next_instruction(void); // This function is going to be used to read the next instruction from the memory
extern void input_custom_instruction(void); // This function is going to be used to input the custom instruction from the user
extern void change_program_counter(void); // This function is going to be used to UPDATE the program counter 
extern void instruction_menue(void);  // This function is going to be used to give the user the menue 

extern void DISPLAY_ARTH(); // This function is going to be used to display the Arthametic and logical instructions

extern bool validate_PC(unsigned int new_pc); // This function is going to be used to validate the program counter