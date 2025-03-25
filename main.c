#include <stdio.h>
#include <locale.h>


#define true 1
#define false 0
#define MAX_ROWS 4
#define MAX_COLUMNS 4

/* */
typedef int boolean;

typedef struct 
{
    int row;
    int column;
} pos;


/*  Populates the 4x4 grid with their corresponding positions.
 *  @param grid Integer array to store the coordinates of each position
 *
**/
void populateGrid(int grid[][MAX_COLUMNS])
{
    int i, j;

    for (i = 1; i <= MAX_ROWS; i++)
    {
        for (j = 1; j <= MAX_COLUMNS; j++)
            grid[i][j] = j;
    }

}

/*  Populates the 4x4 grid with their corresponding positions.
 *  @param grid Integer array to store the coordinates of each position
 *
**/
void printGrid(int grid[][MAX_COLUMNS])
{
    int i, j;

    for (i = 1; i <= MAX_ROWS; i++)
    {
        for (j = 1; j <= MAX_COLUMNS; j++)
            printf("(%d, %d)\n", i, grid[i][j]);
    }

    printf("               XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    printf("           █ █ X         X         X         X         X\n");
    printf("            ▀█ X         X         X         X         X\n");
    printf("             ▀ X         X         X         X         X\n");
    printf("               X         X         X         X         X\n");
    printf("               XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    printf("           ▀▀█ X         X         X         X         X\n");
    printf("            ▀▄ X         X         X         X         X\n");
    printf("           ▀▀  X         X         X         X         X\n");
    printf("               X         X         X         X         X\n");
    printf("               XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    printf("           ▀▀▄ X         X         X         X         X\n");
    printf("           ▄▀  X         X         X         X         X\n");
    printf("           ▀▀▀ X         X         X         X         X\n");
    printf("               X         X         X         X         X\n");
    printf("               XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    printf("           ▀█  X         X         X         X         X\n");
    printf("            █  X         X         X         X         X\n");
    printf("           ▀▀▀ X         X         X         X         X\n");
    printf("               X         X         X         X         X\n");
    printf("               XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    printf("                                                        \n");
    printf("                    ▀█        ▀▀▄       ▀▀█       █ █   \n");
    printf("                     █        ▄▀         ▀▄        ▀█   \n");
    printf("                    ▀▀▀       ▀▀▀       ▀▀          ▀   \n");
}

int main ()
{
    setlocale(LC_ALL, "en_US.UTF-8");               // Enable UTF-8 support for these fonts
    int grid[MAX_ROWS][MAX_COLUMNS];

    populateGrid(grid);
    printGrid(grid);

    printf("\n\n______________________\n\n");
    printf(" TEST MODIFIED VALUE:\n");
    printf("______________________\n\n");
    grid[2][3] = 5005;
    printGrid(grid);

    

    return 0;
}