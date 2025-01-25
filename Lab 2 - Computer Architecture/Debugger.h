#pragma once

#include "Loader.h"
// Macros to extract specific bit fields from a 16-bit value
// Suppose bits are labeled as: [15..13] = top3, [12..10] = subOp, [9..0] = lower10
// Adjust the shifts and masks to match your actual instruction format.
#define GET_TOP3(x)    ( ((x) >> 13) & 0x7  )
#define GET_Byte_12_10(x)   ( ((x) >> 10) & 0x7  )
#define GET_BYTE_9_8(x)   ( ((x) >> 8) & 0x3  )


// This is the array that is going to be used to store the names of the 4
// instructions and be used to look up the names of the instructions
char* arthamatic[3][4] = { {"ADD" , "ADDC" ,"SUB", "SUBC"},  {"DADD", "CMP", "XOR", "AND"}, {"OR", "BIT", "BIC", "BIS"}};
char* branch[8] = { "BEQ/BZ" , "BNE/BNZ" ,"BC/BHS", "BNC/BLO", "BN" , "BGE" ,"BLT", "BRA" };
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

//Now define a union that has two "views" of the same 16 bits:
// 1. A 'raw' 16-bit value stored in a 'short'.
// 2. The above struct of bitfields that break it down.

typedef union {
    short full;   // All 16 bits at once
    Instr_Bits  bits;  // The same 16 bits, viewed as fields
} Instruction;



// This function is going to be used to read and write instruction to the memory
extern unsigned short bus(unsigned short mbr, unsigned short mar, Read_Or_Write R_OR_W, Word_Or_Byte W_O_B);
extern void display_instruction(void);
extern void decode_instruction(unsigned short instruction);
extern void find_instruction(void); 
extern void execute_BL(unsigned short instrction);
extern char* get_opcode_name(unsigned char opcode);
