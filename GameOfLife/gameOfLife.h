#ifndef GAME_OF_LIFE
#define GAME_OF_LIFE
#include <stdio.h>
#include <stdlib.h>
/*
    MADE BY
    PEDRO AMARAL CHAPELIN
*/

#define ALIVE 1
#define DEAD 0
#define GENERATIONS_MAX 5

typedef struct cell
{
    int status;     // Alive or dead
} cell_t;

typedef struct table
{
    int lines;
    int columns;
    cell_t** table;
} table_t;

// Initializes a table, sets every cell as DEAD and returns the table
table_t initializeTable(int lines, int columns);

// Receives a initial state of the table and sets it
void setInitialState(table_t* t, int** initialState);

// Verifies the 4 game rules of a table and moves it to a next state
void moveToNextState(table_t* t);

// Prints table
void printTable(table_t* t);

// Destroys table and frees memory
void destroyTable(table_t* t);

#endif