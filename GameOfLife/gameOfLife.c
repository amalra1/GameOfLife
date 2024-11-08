#include "gameOfLife.h"
/*
    MADE BY
    PEDRO AMARAL CHAPELIN
*/

table_t initializeTable(int lines, int columns)
{
    table_t t;
    t.lines = lines;
    t.columns = columns;

    // Allocate memory for the table
    t.table = (cell_t**)malloc(lines * sizeof(cell_t*));
    for (int i = 0; i < lines; i++) 
    {
        t.table[i] = (cell_t*)malloc(columns * sizeof(cell_t));

        for (int j = 0; j < columns; j++)
            t.table[i][j].status = DEAD;
    }

    return t;
}

void setInitialState(table_t* t, int** initialState) 
{
    for (int i = 0; i < t->lines; i++)
        for (int j = 0; j < t->columns; j++)
            t->table[i][j].status = initialState[i][j];
}

void printTable(table_t* t)
{
    printf("\n");

    // Upper line border
    printf("x");
    for (int i = 0; i < t->columns - 1; i++)
        printf("----");
    printf("---x\n");

    // Table lines
    for (int i = 0; i < t->lines; i++)
    {
        printf("|");
        for (int j = 0; j < t->columns; j++)
        {
            if (t->table[i][j].status == ALIVE)
                printf(" O |");
            else
                printf("   |");
        }
        printf("\n");

        // Lower line border
        printf("x");
            for (int i = 0; i < t->columns - 1; i++)
                printf("----");
        printf("---x\n");
    }
}

// Checks if a living cell is in a normal state, returns 1 if normal and 0 otherwise
int normality(table_t* t, int line, int column)
{
    int neighbours = 0;
    int border1 = 0, border2 = 0;

    // Upper line
    
    // If it isn't on the first line
    if (line != 0)
    {
        // If it is on the first column
        if (column == 0)
        {
            border1 = column;
            border2 = column + 1;
        }

        // If it is on the last column
        else if (column == t->columns - 1)
        {
            border1 = column - 1;
            border2 = column;
        }

        // If it is on the columns among the first and last
        else
        {
            border1 = column - 1;
            border2 = column + 1;
        }

        for (int i = border1; i <= border2; i++)
            if (t->table[line - 1][i].status == ALIVE)
                neighbours++;
    }

    // Lower line
    // If it isn't on the last line
    if (line != t->lines - 1)
    {
        // If it is on the first column
        if (column == 0)
        {
            border1 = column;
            border2 = column + 1;
        }

        // If it is on the last column
        else if (column == t->columns - 1)
        {
            border1 = column - 1;
            border2 = column;
        }

        // If it is on the columns among the first and last
        else
        {
            border1 = column - 1;
            border2 = column + 1;
        }

        for (int i = border1; i <= border2; i++)
            if (t->table[line + 1][i].status == ALIVE)
                neighbours++;
    }

    if (neighbours > 3)
        return 0;

    // Left Side check
    // If it isn't on the first collumn
    if (column != 0)
        if (t->table[line][column - 1].status == ALIVE)
            neighbours++;

    if (neighbours > 3)
        return 0;

    // Right Side check
    // If it isn't on the last collumn
    if (column != t->columns - 1)
        if (t->table[line][column + 1].status == ALIVE)
            neighbours++;

    //printf("neighbours = %d\n", neighbours);

    if (neighbours == 2 || neighbours == 3)
        return 1;

    // Less than 2 or more than 3
    return 0;
}

// Checks if a dead cell can reproduce and become alive again, returns 1 if so and 0 otherwise
int reproduction(table_t* t, int line, int column)
{
    int neighbours = 0;
    int border1 = 0, border2 = 0;

    // Upper line
    
    // If it isn't on the first line
    if (line != 0)
    {
        // If it is on the first column
        if (column == 0)
        {
            border1 = column;
            border2 = column + 1;
        }

        // If it is on the last column
        else if (column == t->columns - 1)
        {
            border1 = column - 1;
            border2 = column;
        }

        // If it is on the columns among the first and last
        else
        {
            border1 = column - 1;
            border2 = column + 1;
        }

        for (int i = border1; i <= border2; i++)
            if (t->table[line - 1][i].status == ALIVE)
                neighbours++;
    }

    // Lower line
    // If it isn't on the last line
    if (line != t->lines - 1)
    {
        // If it is on the first column
        if (column == 0)
        {
            border1 = column;
            border2 = column + 1;
        }

        // If it is on the last column
        else if (column == t->columns - 1)
        {
            border1 = column - 1;
            border2 = column;
        }

        // If it is on the columns among the first and last
        else
        {
            border1 = column - 1;
            border2 = column + 1;
        }

        for (int i = border1; i <= border2; i++)
            if (t->table[line + 1][i].status == ALIVE)
                neighbours++;
    }

    if (neighbours > 3)
        return 0;

    // Left Side check
    // If it isn't on the first collumn
    if (column != 0)
        if (t->table[line][column - 1].status == ALIVE)
            neighbours++;

    if (neighbours > 3)
        return 0;

    // Right Side check
    // If it isn't on the last collumn
    if (column != t->columns - 1)
        if (t->table[line][column + 1].status == ALIVE)
            neighbours++;

    //printf("neighbours = %d\n", neighbours);

    if (neighbours != 3)
        return 0;
    else
        return 1;
}

void moveToNextState(table_t* t)
{
    table_t auxTable = initializeTable(t->lines, t->columns);

    // Go through the 'Past' table and fill the 'Future' one
    for (int i = 0; i < t->lines; i++)
    {
        for (int j = 0; j < t->columns; j++)
        {
            // LIVING CELL
            if (t->table[i][j].status == ALIVE)
            {
                // If it is ALIVE and not normal, surely got more than 3 or less than 2 neighbours
                // So... it dies
                if (!normality(t, i, j))
                    auxTable.table[i][j].status = DEAD;
                else
                    auxTable.table[i][j].status = ALIVE;
            }
            // DEAD CELL
            else
            {
                // Check Reproduction
                if (reproduction(t, i, j))
                    auxTable.table[i][j].status = ALIVE;
            }
        }
    }

    //printTable(&auxTable);

    // Update original Table
    for (int i = 0; i < t->lines; i++)
        for (int j = 0; j < t->columns; j++)
            t->table[i][j].status = auxTable.table[i][j].status;

    // Free auxTable
    destroyTable(&auxTable);
}

void destroyTable(table_t* t) 
{
    for (int i = 0; i < t->lines; i++)
        free(t->table[i]);

    free(t->table);
    t->table = NULL;
}