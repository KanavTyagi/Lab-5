#pragma once

#include "Loader.h"
// Macros to extract specific bit fields from a 16-bit value
#define GET_TOP3(x)    ( ((x) >> 13) & 0x7  ) // this can get the top 3 bits from the value
#define GET_BITS_12_10(x)   ( ((x) >> 10) & 0x07  ) // this can get the 12th to 10th bit from the value
#define GET_BITS_9_8(x)   ( ((x) >> 8) & 0x3  ) // this can get the 9th to 8th bit from the value
#define GET_BIT(value, n)  ( ((value) >> (n)) & 0x1 ) // this can get the specified bit from the value
#define GET_BITS_5_3(x)   ( ((x) >> 3) & 0x7  ) // this can get the 5th to 3rd bit from the value
#define GET_BITS_2_0(x)   ( ((x) >> 0) & 0x7  ) // this can get the 2nd to 0th bit from the value



#define R_C_BIT 7 // This is the bit that is going to be used to get the R/C bit
#define W_B_BIT 6 // This is the bit that is going to be used to get the W/B bit
#define Branch_Instruction 8 // This is the number of branch instructions minus the branch with link instruction
#define Arthanetic_Instruction_Groups 3 // This is the number of Arthametic and logical instructions groups we have created for the easy look up table 
#define Arthametic_Instruction 4 // This is the number of Arthametic and logical instructions in each group that we have created 
#define NUM_REGISTER_TYPES 2  // Two types: registers and constants
#define NUM_REGISTERS 8       // Number of registers/constants

// This is the array that is going to be used to store the names of the 4
// instructions and be used to look up the names of the instructions
extern char* arthamatic[3][4]; // This is the array that is going to be used to store the names of the Arthametic and logical instructions instructions
extern char* branch[8]; // This is the array that is going to be used to store the names of the Branch instructions
extern unsigned short Register_file[2][8]; // This is the array that is going to be used to store the values of the registers  and the constant values
extern unsigned short mbr; // This is the memory buffer register that is going to be used to store the data that is going to be read from the memory
extern unsigned short mar; // This is the memory address register that is going to be used to store the address that is going to be read from the memory



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


// This will help access the bits 
typedef struct {

    unsigned char top3 : 3;   // bits [15..13]
    unsigned char sub3 : 3;   // bits [12..10]
    unsigned short lower10 : 10;  // bits [9..0]

} Instr_Bits;


typedef union {
    short full;   // All 16 bits at once
    Instr_Bits  bits;  // The same 16 bits, viewed as fields
} Instruction;



// This function is going to be used to read and write instruction to the memory
extern void bus(unsigned short mar, unsigned short* mbr, Read_Or_Write R_OR_W, Word_Or_Byte W_O_B);
extern void display_instruction(void);
extern void decode_instruction(unsigned short instruction);
extern void find_instruction(void);  // This function is going to be used to find the instruction
extern void DISPLAY_Branch(unsigned short instruction); // This function is going to be used to display the branch instructions
extern void DISPLAY_ARTH(unsigned short instruction); // This function is going to be used to display the Arthametic and logical instructions
extern void DISPLAY_BL(unsigned short instruction); // This function is going to be used to display the Load instructions
extern void UPDATE_PC(); // This function is going to be used to update the program counter