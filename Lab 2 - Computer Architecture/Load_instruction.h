#pragma once

#include "Loader.h"
#include "Decode.h"

#define GET_SSS(x)   ( ((x) >> 3) & 0x7  ) // This is the macro that is going to be used to get the source register
#define GET_DDD(x)   ( ((x) >> 0) & 0x7  ) // This is the macro that is going to be used to get the destination register
#define Source_Register(x,n) Register_file.WORD[n][x] // This is the macro that is going to be used to get the source register
#define Destination_Register(x) Register_file.WORD[0][x] // This is the macro that is going to be used to get the destination register
#define Destination_Register_High_Byte(x) Register_file.BYTE[0][(x * 2) + 1] // we would use this to access the higher byte of a register
#define Destination_Register_Low_Byte(x) Register_file.BYTE[0][(x * 2)] // this would give us the lower byte of the register
#define MEMORY_WORD(x) memory.word[x >> 1] // This is the macro that is going to be used to get the word from the memory 
#define MEMORY_BYTE(x) memory.byte[x] // This is the macro that is going to be used to get the byte from the memory
#define MOVE_OP_BITS(x) (((x) >> 11) & 0x03 ) // this can get the 12th to 10th bit from the value
#define GET_MOVE_VALUE(x) (((x) >> 3)& 0xFF) // this would get us the value thats been stored in the move instruction opcode

#define PRPO 9 // This is the bit that is going to be used to get the prepose bit
#define DEC 8 // This is the bit that is going to be used to get the decrement bit
#define INC 7 // This is the bit that is going to be used to get the increment bit
#define REGISTER 0 // This is the value that is going to be used to get the register value
#define PC Register_file.WORD[0][7] // This is the program counter that is going to be used to store the address of the next instruction
#define SP Register_file.WORD[0][6] // This is the stack pointer that is going to be used to store the address of the top of the stack
#define LR Register_file.WORD[0][5] // This is the link register that is going to be used to store the return address of the function


// these are the psw structcture that is going to be used

typedef struct {
	bool N; // This is the negative flag
	bool Z; // This is the zero flag
	bool C; // This is the carry flag
	bool V; // This is the overflow flag

} PSW;

extern PSW psw; // This is the psw that is going to be used to store the flags

// These are the different types of load instructions that
// WE are going to be IMPLEMENTING 
enum {	MOVL, MOVLZ, MOVLS, MOVH };

extern void print_register_file(void); // This is the function that is going to be used to print the register file
extern void Load_instruction(void); // This is the function that is going to be used to load the instruction into the memory
extern void Execute_Move(void); // This is the function that is going to be used to execute the move instruction




