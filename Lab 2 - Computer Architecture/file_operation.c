/* File name:  file_operations.c
* Created : 19th January 2025
* By: Kanav Tyagi
* Contains functions responible for opeing the file and reading it line by line
* file_open: Function to open he file passed by either drag adn drop or typed in
* read_file: Function reads the file line by line and then calls other fuinction to validate adn process the records
*/

#include "Loader.h"
#include "Decode.h"
#include "Load_instruction.h"
#include "Branch_Inst.h"


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