

/* File name:  main.c
* Created : 19th January 2025
* By: Kanav Tyagi
* Contains the main entry point to the code
* and the various global variables would be initialized here 
*/

#include "Loader.h"
#include "Decode.h"
#include "Load_instruction.h"


union Memory memory = { Initilizer }; // Initialize the memory to 0 as there cant be an instruction 0x0000 (can be but doesnt make sense to have a bracnh with offset 0)
bool Error_Flag = FALSE; // This flag is going to be used to check if there is an error in the file that is being processed
unsigned short mbr = Initilizer; // This is the memory buffer register that is going to be used to store the data that is going to be read from the memory
unsigned short mar = Initilizer; // This is the memory address register that is going to be used to store the address that is going to be read from the memory
unsigned short starting_address = Initilizer; // This is the starting address of the program
unsigned short Instruction_Register = Initilizer; // This is the instruction register that is going to be used to store the instruction that is going to be read from the memory


int main(int argc, char* argv[]) {


    char filename[FILENAME_SIZE];  // Buffer to store the filename

    // Check if a filename is provided as a command-line argument
    if (argc > 1) {
        strncpy(filename, argv[1], FILENAME_SIZE - 1);
        filename[FILENAME_SIZE - 1] = '\0'; // Ensure null termination
    }
    else {
        // Prompt the user to enter the filename
        printf("Enter the S-Record filename: ");
        if (scanf("%255s", filename) != 1) { // Limit input to prevent buffer overflow
            printf("Failed to read the filename.\n");
            return;
        }
    }

    // Open and process the S-Record file
    if (file_open(filename) == NULL) {

        printf("Failed to process the file: %s\n", filename);

        return;
    }

    if (Error_Flag == FALSE) {
        // Display the extracted filename and starting address
        printf("Source Filename: %s\n", file_name);
        printf("Starting Address: 0x%04X\n", starting_address);
        instruction_menue();
        printf("-------------------------------------------------");
        //Check_memmory();
    }
    else {
        printf("\nPossibally corrupted file Process Terminated\n");
    }

    (void)_getch();
    return false;
}

