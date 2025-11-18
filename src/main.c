#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printGridFile(int grid[9][9],FILE *file)
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (grid[row][col] == 0)
            {
                fprintf(file,"_ ");
            }
            else
            {
                fprintf(file,"%d ", grid[row][col]);
            }

            if (col % 3 == 2 && col != 8)
            {
                fprintf(file,"| ");
            }
        }
        fprintf(file,"\n");
        if (row % 3 == 2 && row != 8)
        {
            fprintf(file,"------|-------|------\n");
        }
    }
    fprintf(file,"\n\n");
}

void printGrid(int grid[9][9])
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (grid[row][col] == 0)
            {
                printf("_ ");
            }
            else
            {
                printf("%d ", grid[row][col]);
            }

            if (col % 3 == 2 && col != 8)
            {
                printf("| ");
            }
        }
        printf("\n");
        if (row % 3 == 2 && row != 8)
        {
            printf("------|-------|------\n");
        }
    }
}

int isColSafe(int grid[9][9], int col, int num)
{
    for (int row = 0; row < 9; row++)
    {
        if (grid[row][col] == num)
            return 0;
    }
    return 1;
}

int isRowSafe(int grid[9][9], int row, int num)
{
    for (int col = 0; col < 9; col++)
    {
        if (grid[row][col] == num)
            return 0;
    }
    return 1;
}

int isBoxSafe(int grid[9][9], int row, int col, int num)
{
    int rowStart = row / 3 * 3;
    int colStart = col / 3 * 3;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[rowStart + i][colStart + j] == num)
                return 0;
        }
    }

    return 1;
}

int isSafe(int grid[9][9], int row, int col, int num)
{
    int colSafe = isColSafe(grid, col, num);
    int rowSafe = isRowSafe(grid, row, num);
    int boxSafe = isBoxSafe(grid, row, col, num);
    return (colSafe & rowSafe & boxSafe);
}

int solveSudoku(int grid[9][9])
{
    int row = -1, col = -1;
    int isEmpty = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (grid[i][j] == 0)
            {
                row = i;
                col = j;
                isEmpty = 1;
                break;
            }
        }
    }

    // No empty spaces
    if (!isEmpty)
    {
        return 1;
    }

    for (int num = 1; num <= 9; num++)
    {
        if (isSafe(grid, row, col, num))
        {
            grid[row][col] = num;

            // Recursive call
            if (solveSudoku(grid))
            {
                return 1;
            }

            // Backtracking
            grid[row][col] = 0;
        }
    }
    return 0;
}

void fillBox(int grid[9][9], int row, int col)
{
    int num;
    int startRow = row / 3 * 3;
    int startCol = col / 3 * 3;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            do
            {
                num = rand() % 9 + 1;
            } while (!isBoxSafe(grid, row, col, num));
            grid[startRow + i][startCol + j] = num;
        }
    }
}

void fillDiagonal(int grid[9][9])
{
    for (int i = 0; i < 3; i++)
    {
        fillBox(grid, 3 * i, 3 * i);
    }
}

int fillRemaining(int grid[9][9], int i, int j)
{
    if (j >= 9 && i < 8)
    {
        i++;
        j = 0;
    }
    if (i >= 9 && j >= 9)
    {
        return 1;
    }
    if (i < 3)
    {
        if (j < 3)
            j = 3;
    }
    else if (i < 6)
    {
        if (j == (int)(i / 3) * 3)
            j += 3;
    }
    else
    {
        if (j == 6)
        {
            i++;
            j = 0;
            if (i >= 9)
                return 1;
        }
    }

    for (int num = 1; num <= 9; num++)
    {
        if (isSafe(grid, i, j, num))
        {
            grid[i][j] = num;

            // Recursive call
            if (fillRemaining(grid, i, j + 1))
            {
                return 1;
            }
            
            // Backtracking
            grid[i][j] = 0;
        }
    }
    return 0;
}

void removeKDigits(int grid[9][9], int k)
{
    while (k != 0)
    {
        int cellId = rand() % 81;
        int i = cellId / 9;
        int j = cellId % 9;
        if (grid[i][j] != 0)
        {
            grid[i][j] = 0;
            --k;
        }
    }
}

void copyGrid(int src[9][9], int dest[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

void sudokuGenerator(int grid[9][9], int k)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            grid[i][j] = 0;
        }
    }

    fillDiagonal(grid);
    fillRemaining(grid, 0, 3);
    removeKDigits(grid, k);
}

int main()
{
    srand(time(NULL));

    int puzzle[9][9], solution[9][9];
    int k = 30;

    printf("Enter a difficulty level - \n1. Easy (30 empty cells)\n2. Medium (45 empty cells)\n3. Hard (60 empty cells)\n");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        k = 30;
        break;
    case 2:
        k = 45;
        break;
    case 3:
        k = 60;
        break;
    default:
        printf("Invalid choice, defaulting to Medium difficulty.\n");
        k = 45;
    }

    FILE *prob;
    FILE *soln;

    printf("Do you want to append the file(Y/N)?(Y is default)");
    char choiceFile;
    getchar();
    scanf("%c",&choiceFile);

    char* mode="a";
    if(choiceFile=='N'){
        mode="w";
    }

    prob = fopen("../doc/problem.txt", mode);
    soln = fopen("../doc/solution.txt", mode);

    sudokuGenerator(puzzle, k);
    copyGrid(puzzle, solution);

    printf("Generated Sudoku Puzzle:\n");
    printGrid(puzzle);
    printGridFile(puzzle,prob);

    if (solveSudoku(solution))
    {
        printf("\nSolution:\n");
        printGrid(solution);
        printGridFile(solution,soln);
    }
    else
    {
        printf("No solution exists\n");
    }
    return 0;
}
