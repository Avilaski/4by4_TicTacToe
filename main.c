#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
#define MAX_ROWS 4
#define MAX_COLUMNS 4
#define MAX_POSITIONS 16
#define NUM_PATTERNS 3
#define PATTERN_SIZE 4

typedef int boolean;

typedef struct {
    int row;
    int column;
} coordinate;

// ===== WINNING PATTERNS (C - T) =====
const coordinate winPatterns[NUM_PATTERNS][PATTERN_SIZE] = {
    { {1,1}, {1,2}, {1,3}, {1,4} },   // Top row
    { {1,4}, {2,3}, {3,2}, {4,1} },   // Anti-diagonal
    { {4,1}, {4,2}, {4,3}, {4,4} }    // Bottom row
};

// ======== HELPER FUNCTIONS =========

// Scans a coordinate array for a specific position
int scanGrid(coordinate Grid[], coordinate pos, int size) {
    for (int i = 0; i < size; i++) {
        if (pos.row == Grid[i].row && pos.column == Grid[i].column)
            return 1;
    }
    return 0;
}

// Removes a position from a coordinate array
void removeFromArray(coordinate arr[], coordinate pos, int *arrSize) {
    for (int i = 0; i < *arrSize; i++) {
        if (arr[i].row == pos.row && arr[i].column == pos.column) {
            for (int j = i; j < *arrSize - 1; j++) {
                arr[j] = arr[j + 1];
            }
            (*arrSize)--;
            break;
        }
    }
}

// Adds a position to an array of coordinates
void addToArray(coordinate arr[], coordinate pos, int *arrSize) {
    if (*arrSize < MAX_POSITIONS) {
        arr[*arrSize] = pos;
        (*arrSize)++;
    }
}

// Initialize every position in the 4x4 grid as free
void populateFreePositions(coordinate freePositions[]) {
    int index = 0;
    for (int i = 1; i <= MAX_ROWS; i++) {
        for (int j = 1; j <= MAX_COLUMNS; j++) {
            freePositions[index].row = i;
            freePositions[index].column = j;
            index++;
        }
    }
}

// Displays the current board state
void displayGrid(coordinate Uno[], int unoSize, coordinate Tres[], int tresSize) {
    printf("\nGrid:\n");
    for (int i = 1; i <= MAX_ROWS; i++) {
        for (int j = 1; j <= MAX_COLUMNS; j++) {
            coordinate temp = {i, j};
            if (scanGrid(Uno, temp, unoSize))
                printf(" U ");
            else if (scanGrid(Tres, temp, tresSize))
                printf(" T ");
            else
                printf(" . ");
        }
        printf("\n");
    }
}

// Validate coordinate input from the user
boolean safeInput(coordinate *pos) {
    char buffer[100];
    printf("Enter move (row,column): ");
    fgets(buffer, sizeof(buffer), stdin);

    int parsed = sscanf(buffer, "%d,%d", &pos->row, &pos->column);

    if (parsed != 2) {
        printf("Invalid format. Use row,column (e.g., 2,3)\n");
        return false;
    }

    if (pos->row < 1 || pos->row > MAX_ROWS || pos->column < 1 || pos->column > MAX_COLUMNS) {
        printf("Out of bounds! Row/column must be from 1 to 4.\n");
        return false;
    }

    return true;
}

// Check if a player's coordinates match any winning pattern
boolean checkWin(coordinate player[], int size) {
    for (int i = 0; i < NUM_PATTERNS; i++) {
        int matchCount = 0;
        for (int j = 0; j < PATTERN_SIZE; j++) {
            if (scanGrid(player, winPatterns[i][j], size)) {
                matchCount++;
            }
        }
        if (matchCount == PATTERN_SIZE) {
            return true;
        }
    }
    return false;
}

// Main move function with input + error handling
void NextPlayerMove(boolean *turn, boolean *go,
                    coordinate freePositions[], int *freeSize,
                    coordinate Uno[], int *unoSize,
                    coordinate Tres[], int *tresSize) {
    coordinate pos;
    boolean valid = false;

    while (!valid) {
        if (!safeInput(&pos)) continue;

        if (*turn && *go && scanGrid(freePositions, pos, *freeSize)) {
            addToArray(Uno, pos, unoSize);
            removeFromArray(freePositions, pos, freeSize);
            *turn = !*turn;
            *go = !*go;
            valid = true;
        }
        else if (!*turn && (scanGrid(Uno, pos, *unoSize) || scanGrid(Tres, pos, *tresSize))) {
            removeFromArray(Uno, pos, unoSize);
            removeFromArray(Tres, pos, tresSize);
            addToArray(freePositions, pos, freeSize);
            *turn = !*turn;
            valid = true;
        }
        else if (*turn && !*go && scanGrid(freePositions, pos, *freeSize)) {
            addToArray(Tres, pos, tresSize);
            removeFromArray(freePositions, pos, freeSize);
            *go = !*go;
            valid = true;
        }
        else {
            printf("Invalid move: Either occupied or not allowed this turn.\n");
        }
    }
}

// ============ MAIN FUNCTION ============
int main() {
    coordinate Uno[MAX_POSITIONS], Tres[MAX_POSITIONS], freePositions[MAX_POSITIONS];
    int unoSize = 0, tresSize = 0, freeSize = MAX_POSITIONS;

    boolean turn = true;  // true: Uno; false: Dos
    boolean go = false;   // Uno toggle between Uno/Tres
    boolean over = false;

    populateFreePositions(freePositions);

    printf("=== GAME START ===\n");

    while (!over) {
        displayGrid(Uno, unoSize, Tres, tresSize);
        printf("Turn: %s\n", turn ? (go ? "Uno (→Uno)" : "Uno (→Tres)") : "Dos");

        NextPlayerMove(&turn, &go, freePositions, &freeSize, Uno, &unoSize, Tres, &tresSize);

        if (checkWin(Uno, unoSize)) {
            printf("Uno Wins!\n");
            over = true;
        } else if (checkWin(Tres, tresSize)) {
            printf("Tres Wins!\n");
            over = true;
        } else if (freeSize == 0) {
            printf("Dos Wins! (No more free positions)\n");
            over = true;
        }
    }

    displayGrid(Uno, unoSize, Tres, tresSize);
    printf("=== GAME OVER ===\n");
    return 0;
}
