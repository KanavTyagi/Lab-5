#pragma once

#include "Loader.h"


typedef enum {
    READING,
    WRITING
} Read_Or_Write;

typedef enum {
    WORD,
    BYTE
} Word_Or_Byte;



// This function is going to be used to read and write instruction to the memory
extern unsigned short bus(unsigned short mbr, unsigned short mar, Read_Or_Write R_OR_W, Word_Or_Byte W_O_B);
extern void display_instruction(void);