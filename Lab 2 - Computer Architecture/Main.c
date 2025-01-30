

/* File name:  main.c
* Created : 19th January 2025
* By: Kanav Tyagi
* Contains the main entry point to the code
* and teh defination of global variables
*/

#include "Loader.h"
#include "Debugger.h"

union Memory memory = { -1 };
bool Error_Flag = FALSE;
unsigned short mbr = 0; // This is the memory buffer register that is going to be used to store the data that is going to be read from the memory
unsigned short mar = 0;

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
        printf("Starting Address: 0x%04X\n", Prog_Counter);
        instruction_menue();
        printf("-------------------------------------------------");
        //Check_memmory();
    }
    else {
        printf("Possibally corrupted file Process Terminated");
    }

    (void)_getch();
    return 0;
}

