#pragma once

#include "Loader.h"
#include "Decode.h"
#include "Load_instruction.h"

#define Branch_Instruction 8 // This is the number of branch instructions minus the branch with link instruction
#define BL_Sign_Bit 12 // This is the bit that is going to be used to get the sign bit from the BL instruction
#define Branch_Sign_Bit 9 // This is the bit that is going to be used to get the sign bit from the Branch instructions
#define Branch_Offset 0x03FF // This is the mask that is going to be used to get the offset from the Branch instructions
#define BL_Offset 0x1FFF // This is the mask that is going to be used to get the offset from the BL instruction
#define BL_Sign_Extend 0xE000 // This is the mask that is going to be used to sign extend the BL instruction
#define Branch_sign_extend 0xFC00 // This is the mask that is going to be used to sign extend the Branch instruction

// This is the enum that is going to be used to keep track of the 
// different types of branch instructions that we hav

typedef struct {
    const char* name;
    bool (*check_cond)(void);
} BranchEntry;


extern BranchEntry branch[Branch_Instruction]; // This is the array that is going to be used to store the names of the Branch instructions

extern bool cond_BEQ_BZ(void); // This is the function that is going to be used to check if the condition is met for the BEQ/BZ instruction
extern bool cond_BNE_BNZ(void); // This is the function that is going to be used to check if the condition is met for the BNE/BNZ instruction
extern bool cond_BC_BHS(void); // This is the function that is going to be used to check if the condition is met for the BC/BHS instruction
extern bool cond_BNC_BLO(void); // This is the function that is going to be used to check if the condition is met for the BNC/BLO instruction
extern bool cond_BN(void); // This is the function that is going to be used to check if the condition is met for the BN instruction
extern bool cond_BGE(void); // This is the function that is going to be used to check if the condition is met for the BGE instruction
extern bool cond_BLT(void); // This is the function that is going to be used to check if the condition is met for the BLT instruction
extern bool cond_BRA(void); // This is the function that is going to be used to check if the condition is met for the BRA instruction So always return true
extern void Execute_Branch(void); // This function is going to be used to display the branch instructions
extern void Execute_BL(void); // This function is going to be used to display the Load instructions
