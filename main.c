#include "hclib.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Create a new board
    exboard_t* board = newboard();

    // Check if the board was created successfully
    if (board == NULL) {
        printf("Failed to create the board.\n");
        return 1;
    }

    // Convert the board to a string using stringboard
    char* boardStr = stringboard(board);

    // Check if the board was stringified successfully
    if (boardStr == NULL) {
        printf("Failed to stringify the board.\n");
        free(board);  // Free the allocated board memory
        return 1;
    }

    // Print the board string
    printf("%s\n", boardStr);

    // Free the allocated memory
    free(boardStr);
    free(board);

    return 0;
}
