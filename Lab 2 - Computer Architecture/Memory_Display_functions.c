/* File name:  Memory_Display_functions.c
* Created : 19th January 2025
* By: Kanav Tyagi
* Contains finctions responible for taking the input from the user and displaying the neccassasry portion of the memory
* Check_memmory: asks the user which of the memory they want to display adn how
* print_memory: Displays the memory contents between the start and end positions in either word format or byte format
*/

#include "Loader.h"
#include "Decode.h"
#include "Load_instruction.h"
#include "Branch_Inst.h"





// Uncomment the following line to enable DEBUG mode
//#define DEBUG


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
