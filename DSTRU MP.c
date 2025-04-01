#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* Below are the definitions */
#define MAX_ROWS 4
#define MAX_COLUMNS 4
#define MAX_POSITIONS 16

typedef struct {
    int row;
    int column;
} Coordinate;

/* Winning patterns, did not remove T from C now */
const Coordinate WINNING_PATTERNS[4][4] = {
    {{1,1}, {1,2}, {1,3}, {1,4}},  // Top row
    {{1,1}, {2,2}, {3,3}, {4,4}},  // Diagonal
    {{1,4}, {2,3}, {3,2}, {4,1}},  // Anti-diagonal
    {{4,1}, {4,2}, {4,3}, {4,4}}   // Bottom row
};

/* Function implementations */
/* HELPER FUNCTIONS */

/* Improved scanGrid - now returns true/false properly */
/* Scans the current grid for the indicated position
 * @param grid: Struct array of coordinates where positions are stored
 * @param gridSize: 
 * @param pos: The position to find in the grid
**/
bool isPositionInGrid(Coordinate grid[], int gridSize, Coordinate pos) {
    bool result = false;
    for (int i = 0; i < gridSize; i++) {
        if (pos.row == grid[i].row && pos.column == grid[i].column) {
            result = true;       // Early return might be fine, makes the code look cleaner
        }
    }
    return result;
}

/* Remove position from free positions */
void removeFromFreePositions(Coordinate freePositions[], int *freePosSize, Coordinate pos) {
    for (int i = 0; i < *freePosSize; i++) {
        if (freePositions[i].row == pos.row && freePositions[i].column == pos.column) {
            // Shift all elements after i one position left
            for (int j = i; j < *freePosSize - 1; j++) {
                freePositions[j] = freePositions[j+1];
            }
            (*freePosSize)--;
            break;
        }
    }
}

/* Initialize all positions in the 4x4 grid */
void populateFreePositions(Coordinate freePositions[]) {
    int index = 0;
    for (int i = 1; i <= MAX_ROWS; i++) {
        for (int j = 1; j <= MAX_COLUMNS; j++) {
            freePositions[index].row = i;
            freePositions[index].column = j;
            index++;
        }
    }
}

/* Check if a player has a winning pattern */
bool hasWinningPattern(Coordinate playerPositions[], int playerSize) {
    for (int p = 0; p < 3; p++) {  // Check each winning pattern
        bool hasPattern = true;
        for (int i = 0; i < 4; i++) {  // Check all positions in pattern
            bool found = false;
            for (int j = 0; j < playerSize; j++) {  // Check player's positions
                if (playerPositions[j].row == WINNING_PATTERNS[p][i].row && 
                    playerPositions[j].column == WINNING_PATTERNS[p][i].column) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                hasPattern = false;
                break;
            }
        }
        if (hasPattern) return true;
    }
    return false;
}

/* GAME FUNCTIONS */

void NextPlayerMove(Coordinate *pos, bool *turn, bool *go, 
                   Coordinate freePositions[], int *freePosSize,
                   Coordinate Uno[], int *unoSize, 
                   Coordinate Tres[], int *tresSize) {
    char input[10];
    printf("Player %s's turn. Enter coordinates (row,column) \n", 
           (*turn && !*go) ? "Uno" : "Tres");
    printf("Stuck? Reset the board! (Type 'reset') \n");
    scanf("%s", input);

    // Check if the player wants to reset the game
    if (strcmp(input, "reset") == 0) {
        // Clear the board
        *unoSize = 0;
        *tresSize = 0;
        *freePosSize = MAX_POSITIONS;
        populateFreePositions(freePositions);
        printf("The board has been reset!\n");
        return;
    }

    // Parse the input as coordinates
    if (sscanf(input, "%d,%d", &pos->row, &pos->column) != 2) {
        printf("Invalid input. Try again.\n");
        return;
    }

    // Validate input
    if (pos->row < 1 || pos->row > 4 || pos->column < 1 || pos->column > 4) {
        printf("Invalid coordinates. Try again.\n");
        return;
    }

    if (!isPositionInGrid(freePositions, *freePosSize, *pos)) {
        printf("Position already taken. Try again.\n");
        return;
    }

    if (*turn && *go) {  // Uno's turn
        Uno[*unoSize] = *pos;
        (*unoSize)++;
        *turn = false;
        *go = false;
    } else if (*turn && !*go) {  // Tres's turn
        Tres[*tresSize] = *pos;
        (*tresSize)++;
        *go = true;
    } else if (turn == false) {
        
    }

    removeFromFreePositions(freePositions, freePosSize, *pos);
}

void printBoard(Coordinate Uno[], int unoSize, Coordinate Tres[], int tresSize) {
    printf("\nCurrent Board:\n");
    printf("  1 2 3 4\n");
    for (int row = 1; row <= 4; row++) {
        printf("%d ", row);
        for (int col = 1; col <= 4; col++) {
            Coordinate current = {row, col};
            if (isPositionInGrid(Uno, unoSize, current)) {
                printf("U ");
            } else if (isPositionInGrid(Tres, tresSize, current)) {
                printf("T ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    bool playAgain = true;

    while (playAgain) {
        Coordinate Uno[MAX_POSITIONS] = {0};
        Coordinate Dos[MAX_POSITIONS] = {0};
        Coordinate Tres[MAX_POSITIONS] = {0};
        Coordinate freePositions[MAX_POSITIONS];
        Coordinate currentPos;

        bool turn = true;  // true for Uno's turn, false for Tres's
        bool go = false;   // false for Uno's turn, true for Tres's
        bool over = false;

        int unoSize = 0;
        int tresSize = 0;
        int freePosSize = MAX_POSITIONS;

        populateFreePositions(freePositions);

        printf("CCDSTRU Game - Starting...\n");
        printf("Winning patterns are:\n");
        printf("1. Top row (1,1)-(1,4)\n");
        printf("2. Anti-diagonal (1,4)-(4,1)\n");
        printf("3. Bottom row (4,1)-(4,4)\n\n");

        while (!over) {
            printBoard(Uno, unoSize, Tres, tresSize);

            NextPlayerMove(&currentPos, &turn, &go, freePositions, &freePosSize, Uno, &unoSize, Tres, &tresSize);

            // Check winning conditions after each move
            if (hasWinningPattern(Uno, unoSize)) {
                printf("\nUno wins!\n");
                over = true;
            } else if (hasWinningPattern(Tres, tresSize)) {
                printf("\nTres wins!\n");
                over = true;
            } else if (freePosSize == 0) {
                printf("\nGame ended in a draw!\n");
                over = true;
            }
        }

        printBoard(Uno, unoSize, Tres, tresSize);

        // Ask if players want to play again
        char choice;
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            playAgain = true;  // Reset the game
        } else {
            playAgain = false; // Exit the program
            printf("Thank you for playing!\n");
        }
    }

    return 0;
}
