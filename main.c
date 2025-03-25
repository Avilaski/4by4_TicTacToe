



#include <stdio.h>
#include <locale.h>


#define true 1
#define false 0
#define MAX_ROWS 4
#define MAX_COLUMNS 4
#define MAX_POSITIONS 16

typedef int boolean;

typedef struct 
{
    int row;
    int column;
} coordinate;

/* HELPER FUNCTIONS */

/* Scans a coordinate array for a specific position
 * @param Grid[]    The coordinate grid where the position will be searched in
 * @param pos       Variable containing the row and column (coordinate) of a specific position
 * @return flag     Returns 1 if the position is found within the grid, else it returns 0
 * 
**/
int scanGrid(coordinate Grid[], coordinate pos)
{
    int i, flag = 0;
    for (i = 0; i < MAX_POSITIONS; i++)
    {
        if (pos.row == Grid[i].row && pos.column == Grid[i].column)
            flag = 1;
    }

    return flag;
}

/* Used at the start of a game to initialize every position in the 4x4 grid as Free Positions
 * @param freePositions[] Coordinate array where all free positions are found
 * 
**/
void populateFreePositions(coordinate freePositions[])
{
    int i, j, h = 0;

    for (i = 0; i < MAX_ROWS; i++)
    {
        for (j = 0; j < MAX_COLUMNS; j++)
        {
            if (h < MAX_POSITIONS)
            {
                freePositions[h].row = i + 1;
                freePositions[h].column = j + 1;
                h++;
            }
        }
    }
}

/* Function Implementations */

/* 
 *
 *
 * 
**/

/* Adds a position to an array of coordinates
 * @param arr[]     Coordinate grid where a new position would be added
 * @param pos       The position to be added to arr[]
 * @param *arrSize  The number of elements in the array to be updated per addition
 * 
**/
void addToArray(coordinate arr[], coordinate pos, int *arrSize)
{
    arr[*arrSize].row = pos.row;
    arr[*arrSize].column = pos.column;

    *arrSize += 1;
}

/* 
 *
 *
 * 
**/
// void removeFromArray(coordinate arr[], coordinate pos, int *arrSize)
// {
//     arr[*arrSize].row = pos.row;
//     arr[*arrSize].column = pos.column;

//     *arrSize += 1;
// }


/* Function used to input and remove positions, based on player turns
 * @param pos       A variable containing a specific position
 * @param turn      Boolean used in conditionals to determine whose turn it is
 * @param go        Second boolean also used to determine whose turn it is
 * @param freePositions Coordinate array containing all free positions
 * @param Uno       Coordinate array where a position is stored depending on player turns
 * @param Tres      Second coordinate array where a second position is stored depending on player turns
**/
void NextPlayerMove(coordinate pos, boolean turn, boolean go, coordinate freePositions[], coordinate Uno[], int *arrSize)
{
    int i, j;
    printf("Enter a coordinate\n");

    scanf("%d, %d", &pos.row, &pos.column);
    
    if (turn && !go && scanGrid(freePositions,pos))     //scanGrid used to determine if the inputted position is available
    {
        addToArray(Uno, pos, arrSize);
    }
}

int main ()
{
    coordinate Uno[MAX_POSITIONS];
    coordinate Tres[MAX_POSITIONS];
    coordinate Dos;

    coordinate freePositions[MAX_POSITIONS];
    coordinate winningPositions[MAX_POSITIONS];
    coordinate pos;

    boolean turn = true;
    boolean go = false;
    boolean over = false;

    int unoSize = 0;
    int tresSize = 0;
    int freePosSize = 16;

    populateFreePositions(freePositions);


    return 0;
}