
/* File name:  file_operations.c
* Created : 19th January 2025
* By: Kanav Tyagi
* Contains functions responible for opeing the file and reading it line by line
* file_open: Function to open he file passed by either drag adn drop or typed in
* read_file: Function reads the file line by line and then calls other fuinction to validate adn process the records
* Contains finctions responible for proccessing the S records passed as buffer in them
* parse_s_record: Reads the buffer determines the type of s record and then calls the needed S Reccord
* parse_S0: Reads the S0 and saves the file name in a variable
* parse_S1: Reads the S1 and saves the data at the needed location
* parse_S9: Reads the S9 records and saaves the location of the program counter
* Validate_CheckSum: Validates the S Record by checking the check Sum
* Contains finctions responible for taking the input from the user and displaying the neccassasry portion of the memory
* Check_memmory: asks the user which of the memory they want to display adn how
* print_memory: Displays the memory contents between the start and end positions in either word format or byte format
*/


#include "Loader.h"



// Uncomment the following line to enable DEBUG mode 
//#define DEBUG

// Define globals
char file_name[FILENAME_SIZE] = { FALSE }; // Initilizing the variable for reading the file name 
unsigned short Prog_Counter = FALSE; // Initializing the startong address to be read later 

// Function to open the file and initiate reading
FILE* file_open(const char* name_file)
{

    // Open the file in read mode
    FILE* file_pointer = fopen(name_file, "r");

    // If read fails and the file is not read then need to quit
    if (file_pointer == NULL) {
        printf("Error opening file");
        return NULL;
    }

    // Call the function to read file
    read_file(file_pointer);

    // Returns the file pointer 
    return file_pointer;
}

// Function to read the file line by line
void read_file(FILE* file_pointer)
{
    // Buffer to hold the records that are being read 
    char buffer[Buffer_size];
    int record_count = FALSE; // To keep track of the number of records being read 


    // Reads the file untill eother the end of file is reached or the Error Flag is raised 
    while ((fgets(buffer, sizeof(buffer), file_pointer) != NULL) && Error_Flag == FALSE) {
        // Trim newline characters
        buffer[strcspn(buffer, "\r\n")] = FALSE;

        // Check if line starts with 'S'
        if (buffer[S_INDEX] != 'S') {
            printf("Line %d does not start with 'S': % s\n", record_count, buffer);
            Error_Flag = TRUE;
            return;
        }

        // Ensure the line has at least the type character
        if (strlen(buffer) < 2) {
            printf("Warning: Line too short to contain S-Record type: %s\n", buffer);
            Error_Flag = TRUE;
            return;
        }

        // Checking the frsr index as that has the data for the type of S Record
        char s_type = buffer[TYPE_OFFSET];

        // Thgis could only be 0 , 1 , 9, 
        // If any otheer is detrected than we would beed to quit as the data is corrupted
        if (s_type != '0' && s_type != '1' && s_type != '9') {
            printf("Unknown S-Record type '%c' in line: %s\n", s_type, buffer);
            Error_Flag = TRUE;
            return;
        }

        // Need to Validate checksum before proccesing 
        if (!Validate_CheckSum(buffer)) {
            printf("Invalid checksum for line: %s\n", buffer);
            Error_Flag = TRUE;
            return;
        }

        // Parse the S-Record
        parse_s_record(buffer);

        // Inrementing the Record Counter
        record_count++;

    }
}


// This flunction will be called to check that elemnts are added properly
// Function to display memory
void display_memory() {
    unsigned short Start, End;
    char W_or_B;

    printf("Enter the starting and ending address in hex: ");
    scanf("%hx %hx", &Start, &End);
    getchar(); // Clear input buffer

    printf("Do you want the data in Word (w) or Byte (b)? ");
    scanf(" %c", &W_or_B);
    getchar(); // Clear input buffer

    print_memory(Start, End, W_or_B);
}



// This function is going to take the start and end location as arguments adn display the memory between those location (inclusive)
// It will also display them according to the variable thats been passed here that is either as a word or a byte 
void print_memory(const short start, const short end, char w_or_b) {

    // First need to check if we want to print it in word format or byte format

    if (w_or_b == 'b') {

        // Printing as byte 
        printf("Printing in Byte Format \n");

        // The for loop will go through the memory with a step of 1
        for (int i = start; i <= end; i++) { // jump will be of 1 as we willbe reading 2 bytes at a time

            printf("[%hX] : ", i); // Displaying the Memory location

            printf("%02X : ", memory.byte[i]); // Displaying the data /instruction saved in hex 
            printf("%c\n", memory.byte[i]); // Displaying the data /instruction saved as a charcter if not withing printable asscii then just displays blank 
        }


    }
    else if (w_or_b == 'w') {

        // Reading as a word 
        printf("Printing in word Format \n");

        // This for loop will have a 
        for (int i = start; i <= end; i += WORD_SIZE) { // jump will of 2 as we willbe reading 2 bytes at a time

            printf("[%hX] : ", i); // Displaying the Memory location

            printf("%04X ", memory.word[i / WORD_SIZE]); // Displaying the Memory location
            printf("%02c\n", memory.byte[i + 1]); // Following the LittleEndian format the MSB is in the second memmory location
        }

    }

}


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
    int i = 0; // Varible to iterate through the filename and savethe data in it, Defineing it outside as we need to make sure to add the null terminator at the end
    for (int j = 0; j < data_length; j++) {
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

    for (int i = 0; i < data_length; i++) {

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

    Prog_Counter = (unsigned short)address;

#ifdef DEBUG
    printf("S9 Record Parsed:\n");
    //printf("  Starting Address: 0x%04X\n", Prog_Counter);
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