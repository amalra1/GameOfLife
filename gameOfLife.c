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

void moveToNextState(table_t* t)
{
   
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

void destroyTable(table_t* t) 
{
    for (int i = 0; i < t->lines; i++)
        free(t->table[i]);

    free(t->table);
    t->table = NULL;
}