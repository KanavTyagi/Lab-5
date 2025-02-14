#pragma once

#include "Branch_Inst.h"
#include "Decode.h"
#include "Load_instruction.h"
#include "Loader.h"

#define GET_BITS_11_8(x) ( ((x) >> 8) & 0x0F  )

typedef enum {
	Registers,
	Constants,
} Reg_or_const;


extern void execute_ADD(Reg_or_const R_C, Word_Or_Byte W_B, unsigned char SSS, unsigned char DDD);
extern void compare_instructions(Reg_or_const R_C, Word_Or_Byte W_B, unsigned char SSS, unsigned char DDD);
extern void bit_instructions(Reg_or_const R_C, Word_Or_Byte W_B, unsigned char SSS, unsigned char DDD);
