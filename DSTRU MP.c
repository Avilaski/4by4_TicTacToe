#include <stdio.h>
#include <stdbool.h>    // Dont need to #define bool and true or false
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

/* Scans the current grid for the indicated position
 * @param grid: Struct array of coordinates where positions are stored
 * @param gridSize: The number of elements inside the grid
 * @param pos: The position to find in the grid
 * @return true or false depending on if the position was found in the array
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

/* Removes a position from the specified grid
 * @param grid: Struct array of coordinates where positions are stored
 * @param gridSize: The number of elements inside the grid
 * @param pos: The position to find in the grid
 * @return The grid with the position removed
**/
void removeFromGrid(Coordinate grid[], int *gridSize, Coordinate pos) {
    for (int i = 0; i < *gridSize; i++) {
        if (grid[i].row == pos.row && grid[i].column == pos.column) {
            // Shift all elements after i one position left
            for (int j = i; j < *gridSize - 1; j++) {
                grid[j] = grid[j+1];
            }
            (*gridSize)--;
            break;
        }
    }
    // printf("Position (%d, %d) not found in grid.\n", pos.row, pos.column); // Debugging
}

/* Initializes the coordinate array freePositions with all the positions in a 4x4 grid
 * @param: freePositions Array of coordinates where all the free positions are stored
 * @return: freePositions initialized with all coordinates in the 4x4 grid
**/
void populateFreePositions(Coordinate freePositions[]) {
    int index = 0;  // Track the current position in the array
    for (int i = 1; i <= MAX_ROWS; i++) {
        for (int j = 1; j <= MAX_COLUMNS; j++) {
            freePositions[index].row = i;
            freePositions[index].column = j;
            index++;  // Move to the next position in the array
        }
    }
}

/* Checks a coordinate array if it has a winning set of position
 * @param: playerPositions The coordinate array whose occupied positions are to be checked
 * @param: playerSize Number of positions occupied by the coordinate array
 * @return true or false depending on if the coordinate array has a winning position
*/
bool hasWinningPattern(Coordinate playerPositions[], int playerSize) {
    for (int i = 0; i < 4; i++) {  // i goes through the ROWS (the patterns) in winning_patterns array 
        bool hasPattern = true;
        for (int j = 0; j < 4; j++) {  // j goes through the COLUMNS in winning_patterns array
            bool found_coordinate = false;
            for (int k = 0; k < playerSize; k++) {  // j just goes through the coordinate array
                if (playerPositions[k].row == WINNING_PATTERNS[i][j].row && 
                    playerPositions[k].column == WINNING_PATTERNS[i][j].column) {
                    found_coordinate = true;
                }
            }
            if (!found_coordinate) {    // Guys the function assumes that the pattern is true, until the loop above doesn't find a coordinate in that pattern
                hasPattern = false;
            }
        }
        if (hasPattern) {     // If at the end of the loop above, the pattern is still true, then the function ahs a winning pattern
            return true;
        }
    }
    return false;
}

/* GAME FUNCTIONS */

/* function that takes in a position, based on calculated turns, then either adds or subtracts them from Uno or Tres
 * @param turn: Pointer to the turn variable, updated and used for determining player turns
 * @param go: Pointer to the go variable, also used for determining players
 * @param freePositions: Array of free position coordinates, updated whenever a coordinate is added or subtracted from the Uno or Tres
 * @param freePosSize: Pointer to the freePositionSize, also updated the same way as above
 * @param Uno: Array of coordinates that are stored in Uno
 * @param UnoSize: Pointer to the current number of elements in Uno
 * @param TresL Array of coordinates that are stored in Tres
 * @param TresSize: Pointer to the current number of elements in Tres
 * @return Adds or removes a coordinate from freePositions, Uno, and or Tres based on whose turn it is
*/
void NextPlayerMove(bool *turn, bool *go, 
                   Coordinate freePositions[], int *freePosSize,
                   Coordinate Uno[], int *unoSize, 
                   Coordinate Tres[], int *tresSize) {
    char input[10];
    Coordinate pos; // I removed the currentPos coordinate in main since this is the only function that uses currentpos

    printf("Player %s's turn. Enter coordinates (row,column) \n", 
           (*turn && *go) ? "Uno" :         // look up "if statements inside printf in C", no need to check the turns again just to print this
           (*turn && !*go) ? "Tres" :
           "Dos");
    printf("If you're stuck type 'reset' to clear the board\n");
    scanf("%s", input);

    if (strcmp(input, "reset") == 0) {
        *unoSize = 0;
        *tresSize = 0;
        *freePosSize = MAX_POSITIONS;
        populateFreePositions(freePositions);
        printf("The board has been reset!\n");
        return;
    }

    //Puts the scanned input in the row and position
    if (sscanf(input, "%d,%d", &pos.row, &pos.column) != 2) { //also checks if user entered integers
        printf("Invalid input. Try again.\n");
        return;
    }

    if (pos.row < 1 || pos.row > 4 || pos.column < 1 || pos.column > 4) {
        printf("Invalid coordinates. Try again.\n");
        return;
    }

    if (*turn && *go) {  // Uno's turn
        if (!isPositionInGrid(freePositions, *freePosSize, pos)) {
            printf("Position already taken. Try again.\n");
            return;
        }

        Uno[*unoSize] = pos;
        (*unoSize)++;
        *turn = false;
        *go = false;
        
        removeFromGrid(freePositions, freePosSize, pos);
    } else if (*turn && !*go) {  // Tres's turn
        if (!isPositionInGrid(freePositions, *freePosSize, pos)) {
            printf("Position already taken. Try again.\n");
            return;
        }

        Tres[*tresSize] = pos;
        (*tresSize)++;
        *go = true;
        removeFromGrid(freePositions, freePosSize, pos);
    } else if (*turn == false) {    // Dos turn
        if (isPositionInGrid(Uno, *unoSize, pos)) {
            removeFromGrid(Uno, unoSize, pos);
        } else if (isPositionInGrid(Tres, *tresSize, pos)) {
            removeFromGrid(Tres, tresSize, pos);
        } else {
            printf("Invalid position. Dos can only remove positions from Uno or Tres.\n");
            return;
        }
        freePositions[*freePosSize] = pos;
        (*freePosSize)++;

        *turn = true;
    }
}

/* Prints the board based on the coordinates in Uno and Tres
 * @param Uno: Array of coordinates that are in Uno
 * @param unoSize: Number of elements inside uno
 * @oaram Tres: Array of coordinates that are in Tres
 * @param tresSize: Number of elements inside tres
 * @return The board with the current positions in their respective coordinates
*/
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
        Coordinate Tres[MAX_POSITIONS] = {0};
        Coordinate freePositions[MAX_POSITIONS];

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
        printf("2. Diagonal (1,1)-(4,4)\n");
        printf("3. Anti-diagonal (1,4)-(4,1)\n");
        printf("4. Bottom row (4,1)-(4,4)\n\n");

        while (!over) {
            printBoard(Uno, unoSize, Tres, tresSize);

            NextPlayerMove(&turn, &go, freePositions, &freePosSize, Uno, &unoSize, Tres, &tresSize);

            // Check winning conditions after each move
            if (hasWinningPattern(Uno, unoSize)) {
                printf("\nUno wins!\n");
                over = true;
            } else if (hasWinningPattern(Tres, tresSize)) {
                printf("\nTres wins!\n");
                over = true;
            } else if (freePosSize == 0) {
                printf("\nDos wins!\n");
                over = true;
            }
        }

        printBoard(Uno, unoSize, Tres, tresSize);

        char choice;
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            playAgain = true;  // Reset game
        } else {
            playAgain = false; // Exit the program
            printf("Thank you for playing!\n");
        }
    }

    return 0;
}
