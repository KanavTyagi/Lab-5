
/* File name:  file_operations.c
* Created : 19th January 2025
* By: Kanav Tyagi
* Contains finctions responible for proccessing the S records passed as buffer in them
* parse_s_record: Reads the buffer determines the type of s record and then calls the needed S Reccord
* parse_S0: Reads the S0 and saves the file name in a variable
* parse_S1: Reads the S1 and saves the data at the needed location
* parse_S9: Reads the S9 records and saaves the location of the program counter
* Validate_CheckSum: Validates the S Record by checking the check Sum
*/


#include "Loader.h"
#include "Decode.h"
#include "Load_instruction.h"
#include "Branch_Inst.h"



// Uncomment the following line to enable DEBUG mode 
//#define DEBUG


// Function to parse an S-Record based on its type
void parse_s_record(const char* buffer)
{

    // The Method to extract the byte_count adn the address 
    // Would be the same for all S records so extracing those now

    // Extract the lenght of the record
    // The length is in the index 2-3
    unsigned int byte_count;
    sscanf(buffer + LENGTH_OFFSET, "%2x", &byte_count);

    // Extracting the starting address
    // The address is stored in record index 4 - 7
    unsigned int address;
    sscanf(buffer + ADDRESS_OFFSET, "%4x", &address);

    // Use switch casses to perform the diffrent tasks for the s records
    char s_type = buffer[TYPE_OFFSET]; // Read the S Record type 
    switch (s_type) { // Start the switch case and call the neccassary functions 
    case '0':
        parse_S0(buffer, byte_count, address);
        break;
    case '1':
        parse_S1(buffer, byte_count, address);
        break;
    case '9':
        parse_S9(buffer, byte_count, address);
        break;
    default:
        // This case is already handled in read_file, but included for completeness
        printf("Unsupported S - Record type '%c'\n", s_type);
        Error_Flag = TRUE;
    }
}

// Function to parse S0 record and extract filename
void parse_S0(const char* buffer, unsigned int byte_count, unsigned int address)
{

    // Calculate data length: byte_count - address_bytes (2 bytes) - checksum (1 byte)
    int data_length = byte_count - ADDRESS_SIZE - CHECK_SUM_SIZE;

    if (data_length <= FALSE) { // If no data then we need to quit readingg
        printf("No data found in S0 record.\n");
        Error_Flag = TRUE;
        return;
    }


    // Data starts at index 8 : "S" + "0" + length(2) + address (4) = 8
    int i = Initilizer; // Varible to iterate through the filename and savethe data in it, Defineing it outside as we need to make sure to add the null terminator at the end
    for (int j = Initilizer; j < data_length; j++) {
        unsigned int byte;
        if (sscanf(buffer + DATA_INDEX + j * Hex_Char_Per_Byte, "%2x", &byte) != TRUE) {
            printf("Invalid hex byte in record\n");
            Error_Flag = TRUE;
        }
        file_name[i++] = (unsigned char)byte; // Save the byte to the array adn then increment the variable to iterate through the filename
    }
    file_name[i] = '\0'; // Adding the null terminator to the string reading the file name

#ifdef DEBUG
    printf("S0 Record Parsed:\n");
    printf("  Filename: %s\n", file_name);
#endif
}

// Function to parse S1 record and load data into memory
// Getting the buffer ppointer and not the array
void parse_S1(const char* buffer, unsigned int byte_count, unsigned int address)
{
    // S1: S1 + byte count (2 digits) + address (4 digits) + data + checksum (2 digits)


    // Data length: byte_count - address_bytes (2 bytes) - checksum (1 byte)
    int data_length = byte_count - ADDRESS_SIZE - CHECK_SUM_SIZE;
    if (data_length <= FALSE) { // If no data then we need to quit readingg
        printf("No data in S1 record.\n");
        Error_Flag = TRUE;
        return;
    }

    // Check memory bounds 
    if (address + data_length > MEMSIZE) {
        printf("Data in S1 record exceeds memory bounds. Address: 0x%04X, Data length: %d\n", address, data_length);
        Error_Flag = TRUE;
        return;
    }

    // Extract and load data bytes into memory

    for (int i = Initilizer; i < data_length; i++) {

        unsigned int byte; // Initialize a variable to hold the converted byte value
        // data_start marks the start of the data in the s record
        // i*2 lets us read it 2 at a time which is needed for reading the inputs as hex digits 
        if (sscanf(buffer + DATA_INDEX + i * Hex_Char_Per_Byte, "%2x", &byte) != TRUE) { //  Reading the data byte by byte 

            printf("Invalid hex byte in record\n"); // We are expecting 
            Error_Flag = TRUE;
            return;
        }

        // Had to use Typecasting instead of direfctly reading into a char in the sscanf
        // Cause it started giving error "Memory Corrupted" 
        // And Typecasting seemed to work
        memory.byte[address + i] = (unsigned char)byte; // Saving the data into the memory

    }
    // While debugging need to make sure that the bytse are being read properly
#ifdef DEBUG
    printf("S1 Record Parsed:\n");
    printf("  Address: 0x%04X\n", address);
    printf("  Data (%d bytes): ", data_length);
    for (int i = 0; i < data_length; i++) {
        printf("%02X ", memory.byte[address + i]);
    }
    printf("\n");
#endif
}

// Function to parse S9 record and extract starting address
void parse_S9(const char* buffer, unsigned int byte_count, unsigned int address)
{

    // No data field in S9 only need the Startong address

    starting_address = (unsigned short)address; // Saving the starting address to the global variable 
	// Saving it in a diffrent variable as we might need to use it later
	PC = starting_address; // Saving the starting address to the Program Counter

#ifdef DEBUG
    printf("S9 Record Parsed:\n");
    printf("  Starting Address: 0x%04X\n", PC);
#endif
}

// Function to validate checksum of an S-Record
bool Validate_CheckSum(const char* buffer)
{

    // Extract byte count
    unsigned int byte_count;
    if (sscanf(buffer + LENGTH_OFFSET, "%2x", &byte_count) != TRUE) {
        printf("Failed to read length from buffer: %.2s\n", buffer + LENGTH_OFFSET);
        Error_Flag = TRUE;
        return false;
    }


    // Total number of hex characters expected: 2 ('S' and type) + byte_count * 2
    if (strlen(buffer) < (LENGTH_OFFSET + byte_count * Hex_Char_Per_Byte)) {
        printf("Record too short, Likely Missing Dat\n");

        Error_Flag = TRUE;
        return false;
    }

    int i = 0;
    // Sum all bytes from byte count to checksum
    unsigned int sum = 0;
    for (i = LENGTH_OFFSET; i <= LENGTH_OFFSET + byte_count * Hex_Char_Per_Byte; i += Hex_Char_Per_Byte) {
        unsigned int byte;
        sscanf(buffer + i, "%2x", &byte);
        sum += (unsigned char)byte;
    }


    // Check if the least significant byte of the sum is 0xFF
    if ((sum & CHECK_VALIDATE) == CHECK_VALIDATE) {
#ifdef DEBUG
        printf("Validated\n");
#endif
        return true;
    }
    else {
        Error_Flag = TRUE;

#ifdef DEBUG
        printf("Validation Error\n");
#endif
        return false;
    }
}