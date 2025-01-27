#pragma once

#include "Loader.h"
// Macros to extract specific bit fields from a 16-bit value
// Suppose bits are labeled as: [15..13] = top3, [12..10] = subOp, [9..0] = lower10
// Adjust the shifts and masks to match your actual instruction format.
#define GET_TOP3(x)    ( ((x) >> 13) & 0x7  ) // this can get the top 3 bits from the value
#define GET_Byte_12_10(x)   ( ((x) >> 10) & 0x07  ) // this can get the 12th to 10th bit from the value
#define GET_BYTE_9_8(x)   ( ((x) >> 8) & 0x3  ) // this can get the 9th to 8th bit from the value
#define GET_BIT(value, n)  ( ((value) >> (n)) & 0x1 ) // this can get the specified bit from the value
#define GET_BYTE_5_3(x)   ( ((x) >> 3) & 0x7  ) // this can get the 5th to 3rd bit from the value
#define GET_BYTE_2_0(x)   ( ((x) >> 0) & 0x7  ) // this can get the 2nd to 0th bit from the value


// This is the array that is going to be used to store the names of the 4
// instructions and be used to look up the names of the instructions
extern char* arthamatic[3][4]; // This is the array that is going to be used to store the names of the Arthametic and logical instructions instructions
extern char* branch[8]; // This is the array that is going to be used to store the names of the Branch instructions
extern unsigned short Register_file[2][8]; // This is the array that is going to be used to store the values of the registers  and the constant values

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
extern unsigned short bus(unsigned short mbr, unsigned short mar, Read_Or_Write R_OR_W, Word_Or_Byte W_O_B);
extern void display_instruction(void);
extern void decode_instruction(unsigned short instruction);
extern void find_instruction(void);  // This function is going to be used to find the instruction
extern void DISPLAY_Branch(unsigned short instrction); // This function is going to be used to display the branch instructions
extern void DISPLAY_ARTH(unsigned short instrction); // This function is going to be used to display the Arthametic and logical instructions
