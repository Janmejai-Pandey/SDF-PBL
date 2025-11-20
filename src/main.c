#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hpdf.h"

void printGridFile(int grid[9][9], FILE *file)
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (grid[row][col] == 0)
            {
                fprintf(file, "_ ");
            }
            else
            {
                fprintf(file, "%d ", grid[row][col]);
            }

            if (col % 3 == 2 && col != 8)
            {
                fprintf(file, "| ");
            }
        }
        fprintf(file, "\n");
        if (row % 3 == 2 && row != 8)
        {
            fprintf(file, "------|-------|------\n");
        }
    }
    fprintf(file, "\n\n");
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

void printPdf()
{
    FILE *prob = fopen("../doc/problem.txt", "r");
    FILE *soln = fopen("../doc/solution.txt", "r");

    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        printf("Failed to create PDF object\n");
        return;
    }

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    HPDF_Font font = HPDF_GetFont(pdf, "Courier", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);

    // Add content from problem and solution files to the PDF
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, 800, "Sudoku Problem:");
    char line[256];
    int y = 780;
    while (fgets(line, sizeof(line), prob))
    {
        HPDF_Page_TextOut(page, 50, y, line);
        y -= 15; // Move down for next line
    }
    y -= 30; // Extra space before solution
    HPDF_Page_TextOut(page, 50, y, "Sudoku Solution:");
    y -= 20;
    while (fgets(line, sizeof(line), soln))
    {
        HPDF_Page_TextOut(page, 50, y, line);
        y -= 15; // Move down for next line
    }
    HPDF_Page_EndText(page);

    const char *filename = "sudoku.pdf";
    HPDF_SaveToFile(pdf, filename);
    HPDF_Free(pdf);

    printf("PDF file '%s' created successfully.\n", filename);
}

int mode()
{
    printf("1: Generate Sudoku Puzzle\n2: Solve Sudoku Puzzle\n");
    int choice;
    scanf("%d", &choice);
    return choice;
}

int difficulty()
{
    printf("Select Difficulty Level:\n1. Easy\n2. Medium\n3. Hard\n");
    int choice;
    scanf("%d", &choice);
    int k;
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
    return k;
}

int printMode()
{
    printf("Select Print Mode:\n1. Console\n2. File(Default)\n");
    int choice;
    scanf("%d", &choice);
    return choice;
}

void isPDF()
{
    printf("Do you want to generate a PDF file? (1 for Yes, 0 for No): ");
    int choice;
    scanf("%d", &choice);
    if (choice)
    {
        printPdf();
    }
}

int noOfPuzzles()
{
    printf("Enter the number of puzzles to generate: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

void writeFile(int puzzle[9][9], int solution[9][9], FILE *prob, FILE *soln)
{
    // write puzzle to prob and solution to soln
    if (!prob || !soln)
        return;

    printGridFile(puzzle, prob);
    printGridFile(solution, soln);
}

int main()
{
    srand(time(NULL));

    int puzzle[9][9];
    int solution[9][9];

    int m = mode();

    if (m == 1)
    {
        int pm = printMode();
        int pdfChoice;
        printf("Do you want to generate a PDF file? (1 for Yes, 0 for No): ");
        scanf("%d", &pdfChoice);
        int n = noOfPuzzles();

        // open files - overwrite existing
        FILE *prob = fopen("../doc/problem.txt", "w");
        FILE *soln = fopen("../doc/solution.txt", "w");
        if (!prob || !soln)
        {
            printf("Failed to open output files in doc/.\n");
            if (prob)
                fclose(prob);
            if (soln)
                fclose(soln);
            return 1;
        }

        for (int t = 0; t < n; t++)
        {
            // generate
            sudokuGenerator(puzzle, difficulty());

            // copy and solve for solution
            copyGrid(puzzle, solution);
            if (!solveSudoku(solution))
            {
                // in the unlikely event the solver fails, retry this puzzle
                t--;
                continue;
            }

            if (pm == 1)
            {
                printf("--- Puzzle %d ---\n", t + 1);
                printGrid(puzzle);
                printf("\n--- Solution %d ---\n", t + 1);
                printGrid(solution);
                printf("\n");
            }
            else
            {
                writeFile(puzzle, solution, prob, soln);
            }
        }

        fclose(prob);
        fclose(soln);

        if (pm!=1&&pdfChoice)
        {
            printPdf();
        }
    }
    else if (m == 2)
    {
        // Solve a puzzle from ../doc/problem.txt and write solution to ../doc/solution.txt
        FILE *prob = fopen("../doc/problem.txt", "r");
        if (!prob)
        {
            printf("Failed to open ../doc/problem.txt\n");
            return 1;
        }

        // read first puzzle from file into grid; we expect numbers and '_' for blanks
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                int val = 0;
                // read next non-space, non-separator char(s)
                int c = getc(prob);
                while (c != EOF && (c == ' ' || c == '\n' || c == '|' || c == '-' ))
                    c = getc(prob);
                if (c == EOF)
                {
                    fclose(prob);
                    printf("Unexpected end of file while reading puzzle.\n");
                    return 1;
                }

                if (c == '_')
                {
                    val = 0;
                }
                else if (c >= '0' && c <= '9')
                {
                    // handle possibly two-digit reads but sudoku digits are single
                    val = c - '0';
                }
                else
                {
                    // unknown char, treat as empty
                    val = 0;
                }

                puzzle[i][j] = val;
            }
        }

        fclose(prob);

        // solve
        if (!solveSudoku(puzzle))
        {
            printf("No solution exists for the provided puzzle.\n");
            return 1;
        }

        // write solution to file
        FILE *soln = fopen("../doc/solution.txt", "w");
        if (!soln)
        {
            printf("Failed to open ../doc/solution.txt for writing.\n");
            return 1;
        }
        printGridFile(puzzle, soln);
        fclose(soln);

        printf("Solved puzzle and wrote solution to ../doc/solution.txt\n");
    }
    else
    {
        printf("Invalid mode selected. Exiting.\n");
        return 1;
    }

    return 0;
}
