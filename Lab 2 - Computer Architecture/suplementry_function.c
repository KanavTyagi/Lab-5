#include "Debugger.h"
#include "Loader.h"


void UPDATE_PC() {
    unsigned int new_pc;

    printf("Enter new Program Counter (hexadecimal, e.g., 1A3F): ");
    if (scanf("%x", &new_pc) == 1) {
        Prog_Counter = new_pc;
        printf("Updated Program Counter: %04X\n", Prog_Counter);
    }
    else {
        printf("Invalid input! Please enter a valid hexadecimal number.\n");
        while (getchar() != '\n'); // Clear input buffer
    }
}
