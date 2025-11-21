#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "hpdf.h"

#define GRID_PIXEL 300             // size of one grid (square) in points
#define CELL_SIZE (GRID_PIXEL / 9) // size of one cell
#define PAGE_TOP 780               // starting y for drawing
#define START_X 60                 // left margin
#define BOTTOM_MARGIN 60
#define GRID_SPACING 40 // space between puzzle and solution or between grids
#define TITLE_SPACE 24  // space occupied by title text

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

int read_next_grid(FILE *fp, int grid[9][9])
{
    if (!fp)
        return 0;
    char line[256];
    int r = 0;

    while (1)
    {
        long pos = ftell(fp);
        if (!fgets(line, sizeof(line), fp))
            return 0;

        int found = 0;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if ((line[i] >= '0' && line[i] <= '9') || line[i] == '_')
            {
                found = 1;
                break; 
            }
        }
        if (found)
        {
            fseek(fp, pos, SEEK_SET);
            break;
        }
    }

    while (r < 9)
    {
        if (!fgets(line, sizeof(line), fp))
        {
            return 0; 
        }

        int hasDigitOrBlank = 0;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if ((line[i] >= '0' && line[i] <= '9') || line[i] == '_')
            {
                hasDigitOrBlank = 1;
                break;
            }
        }
        if (!hasDigitOrBlank)
            continue;

        int c = 0;
        for (int i = 0; line[i] != '\0' && c < 9; i++)
        {
            if (line[i] >= '0' && line[i] <= '9')
            {
                grid[r][c++] = line[i] - '0';
            }
            else if (line[i] == '_')
            {
                grid[r][c++] = 0;
            }
        }

        if (c < 9)
        {
            for (; c < 9; c++)
                grid[r][c] = 0;
        }
        r++;
    }

    while (fgets(line, sizeof(line), fp))
    {
        int found = 0;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if ((line[i] >= '0' && line[i] <= '9') || line[i] == '_')
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            fseek(fp, -((long)strlen(line)), SEEK_CUR);
            break;
        }
    }

    return 1;
}

HPDF_Page add_new_page(HPDF_Doc pdf)
{
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    return page;
}


void draw_grid_on_page(HPDF_Doc pdf, HPDF_Page page, int grid[9][9], int x, int yTop)
{
    if (!page)
        return;

    HPDF_Page_SetLineWidth(page, 2);
    HPDF_Page_Rectangle(page, x, yTop - GRID_PIXEL, GRID_PIXEL, GRID_PIXEL);
    HPDF_Page_Stroke(page);

    for (int i = 1; i < 9; i++)
    {
        if (i % 3 == 0)
            HPDF_Page_SetLineWidth(page, 1.5);
        else
            HPDF_Page_SetLineWidth(page, 0.5);

        HPDF_Page_MoveTo(page, x, yTop - i * CELL_SIZE);
        HPDF_Page_LineTo(page, x + GRID_PIXEL, yTop - i * CELL_SIZE);
        HPDF_Page_Stroke(page);

        HPDF_Page_MoveTo(page, x + i * CELL_SIZE, yTop);
        HPDF_Page_LineTo(page, x + i * CELL_SIZE, yTop - GRID_PIXEL);
        HPDF_Page_Stroke(page);
    }

    HPDF_Font font = HPDF_GetFont(pdf, "Courier-Bold", NULL);
    float font_size = CELL_SIZE * 0.6f;
    if (font_size < 10)
        font_size = 10;
    if (font_size > 24)
        font_size = 24;
    HPDF_Page_SetFontAndSize(page, font, font_size);

    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (grid[r][c] != 0)
            {
                char s[4];
                snprintf(s, sizeof(s), "%d", grid[r][c]);

                float cx = x + c * CELL_SIZE + CELL_SIZE / 2.0f;
                float cy = yTop - r * CELL_SIZE - CELL_SIZE / 2.0f;

                float text_w = strlen(s) * (font_size * 0.5f);
                float text_h = font_size;

                float tx = cx - text_w / 2.0f;
                float ty = cy - text_h / 2.0f + 3;

                HPDF_Page_BeginText(page);
                HPDF_Page_TextOut(page, tx, ty, s);
                HPDF_Page_EndText(page);
            }
        }
    }
}

void printPdf()
{
    FILE *prob = fopen("doc/problem.txt", "r");
    FILE *soln = fopen("doc/solution.txt", "r");

    if (!prob)
    {
        printf("ERROR: cannot open doc/problem.txt\n");
        if (soln)
            fclose(soln);
        return;
    }
    if (!soln)
    {
        printf("ERROR: cannot open doc/solution.txt\n");
        fclose(prob);
        return;
    }

    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        printf("ERROR: HPDF_New failed (check libharu setup)\n");
        fclose(prob);
        fclose(soln);
        return;
    }

    HPDF_Page page = add_new_page(pdf);

    int yTop = PAGE_TOP;

    int puzzleGrid[9][9], solutionGrid[9][9];
    int puzzleIndex = 0;

    while (1)
    {
        int gotPuzzle = read_next_grid(prob, puzzleGrid);
        int gotSolution = read_next_grid(soln, solutionGrid);

        if (!gotPuzzle && !gotSolution)
            break;
        
        if (!gotPuzzle)
        {
            memset(puzzleGrid, 0, sizeof(puzzleGrid));
        }
        if (!gotSolution)
        {
            memset(solutionGrid, 0, sizeof(solutionGrid));
        }

        puzzleIndex++;

        if (yTop - TITLE_SPACE - GRID_PIXEL < BOTTOM_MARGIN)
        {
            page = add_new_page(pdf);
            yTop = PAGE_TOP;
        }
        
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Bold", NULL), 16);
        char title[64];
        snprintf(title, sizeof(title), "Puzzle %d", puzzleIndex);
        HPDF_Page_TextOut(page, START_X, yTop + 6, title);
        HPDF_Page_EndText(page);

        draw_grid_on_page(pdf, page, puzzleGrid, START_X, yTop);
        yTop = yTop - GRID_PIXEL - GRID_SPACING;

        if (yTop - TITLE_SPACE - GRID_PIXEL < BOTTOM_MARGIN)
        {
            page = add_new_page(pdf);
            yTop = PAGE_TOP;
        }

        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica-Bold", NULL), 16);
        snprintf(title, sizeof(title), "Solution %d", puzzleIndex);
        HPDF_Page_TextOut(page, START_X, yTop + 6, title);
        HPDF_Page_EndText(page);

        draw_grid_on_page(pdf, page, solutionGrid, START_X, yTop);
        yTop = yTop - GRID_PIXEL - GRID_SPACING;

        if (yTop - GRID_PIXEL < BOTTOM_MARGIN)
        {
            page = add_new_page(pdf);
            yTop = PAGE_TOP;
        }
    }

    const char *outname = "doc/sudoku.pdf";
    HPDF_STATUS st = HPDF_SaveToFile(pdf, outname);
    if (st != HPDF_OK)
    {
        printf("ERROR: HPDF_SaveToFile failed with status %d\n", (int)st);
    }
    else
    {
        printf("PDF created: %s\n", outname);
    }

    HPDF_Free(pdf);
    fclose(prob);
    fclose(soln);
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

int noOfPuzzles()
{
    printf("Enter the number of puzzles to generate: ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

void writeFile(int puzzle[9][9], int solution[9][9], FILE *prob, FILE *soln)
{
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

        int pdfChoice=0;
        if(pm != 1)
        {
            printf("Do you want to generate a PDF file? (1 for Yes, 0 for No): ");
            scanf("%d", &pdfChoice);
        }

        int n = noOfPuzzles();

        FILE *prob = fopen("doc/problem.txt", "w");
        FILE *soln = fopen("doc/solution.txt", "w");
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
            sudokuGenerator(puzzle, difficulty());

            copyGrid(puzzle, solution);
            solveSudoku(solution);

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

        if (pm != 1 && pdfChoice)
        {
            printPdf();
        }
    }
    else if (m == 2)
    {
        FILE *prob = fopen("doc/problem.txt", "r");
        if (!prob)
        {
            printf("Failed to open doc/problem.txt\n");
            return 1;
        }

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                int val = 0;
                int c = getc(prob);
                while (c != EOF && (c == ' ' || c == '\n' || c == '|' || c == '-'))
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
                    val = c - '0';
                }
                else
                {
                    val = 0;
                }

                puzzle[i][j] = val;
            }
        }

        fclose(prob);

        if (!solveSudoku(puzzle))
        {
            printf("No solution exists for the provided puzzle.\n");
            return 1;
        }

        FILE *soln = fopen("doc/solution.txt", "w");
        if (!soln)
        {
            printf("Failed to open doc/solution.txt for writing.\n");
            return 1;
        }
        printGridFile(puzzle, soln);
        fclose(soln);

        printf("Solved puzzle and wrote solution to doc/solution.txt\n");
    }
    else
    {
        printf("Invalid mode selected. Exiting.\n");
        return 1;
    }

    return 0;
}
