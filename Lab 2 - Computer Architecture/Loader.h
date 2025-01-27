
// SRead.h
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Constants
#define MEMSIZE (1 << 16)        // 64 KiB Memory size
#define FILENAME_SIZE 256       // Maximum filename length
#define MAX_RECORD 10          // Maximum number of S-Records
#define ADDRESS_OFFSET 4	  // Index offset for gettting to the Address Location
#define LENGTH_OFFSET 2		 //  Index offset for gettting to the Length Location
#define TYPE_OFFSET 1	    //  Index offset for gettting to the S Record Type specifier Location
#define ADDRESS_SIZE 2     // Size of Address in Bytes
#define CHECK_SUM_SIZE 1  //  Size of Check Sum in Bytes
#define DATA_INDEX 8 // "S" + "1" + length(2) + address (4) = 8th index is where we want to start reading from
#define Hex_Char_Per_Byte 2		// Number of characters in 1 hex digit of size 1 byte
#define TRUE 1 // Used to set the flag to true
#define FALSE 0 // Used to set the flag to false
#define Buffer_size 1024     // Buffer size to read the records from the file
#define CHECK_VALIDATE 0xFF //  Used to validate the check sum by Anding it to the sum of all 
#define S_INDEX 0		   // Index offset for gettting to the 1st Location in the memory
#define WORD_SIZE 2 // Tells the size of word in Bytes i.e. 2




extern bool Error_Flag; // Flag to be raised whenever an error has occured 
// Memory Array to store the data and instructions
union Memory {
	unsigned short word[MEMSIZE >> 1];
	unsigned char byte[MEMSIZE];
};

// Global Variable to srtoe the memory array
extern union Memory memory; // Memmory array where data and instructions will be saved 


// Global Variable to store the file name and the starting address
extern char file_name[FILENAME_SIZE]; // To read the file name from the S0 Record 
extern unsigned short Prog_Counter; // To read the position of the Program counter

// Functions to read file 
extern FILE* file_open(const char* name_file); // Function to open he file passed by either drag adn drop or typed in
extern void read_file(FILE* file_pointer); // Function reads the file line by line and then calls other fuinction to validate adn process the records 

// Functions to read the s records 
extern void parse_s_record(const char* buffer); // Reads the buffer determines the type of s record and then calls the needed S Reccord 
extern void parse_S0(const char* buffer, unsigned int byte_count, unsigned int address); // Reads the S0 and saves the file name in a variable
extern void parse_S1(const char* buffer, unsigned int byte_count, unsigned int address); // Reads the S1 and saves the data at the needed location
extern void parse_S9(const char* buffer, unsigned int byte_count, unsigned int address); // Reads the S9 records and saaves the location of the program counter

// Functions to Validate S reccord 
extern bool Validate_CheckSum(const char* buffer); // Validates the S Record by checking the check Sum

// Functions to Writee the contents of the memory
extern void Check_memmory(void); // asks the user which of the memory they want to display adn how 
extern void print_memory(const short start, const short end, char w_or_b); // Displays the memory contents between the start and end positions in either word format or byte format
