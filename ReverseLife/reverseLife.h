#ifndef REVERSE_LIFE
#define REVERSE_LIFE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
/*
    MADE BY
    PEDRO AMARAL CHAPELIN
*/

#define ALIVE 1
#define DEAD 0
#define MAX_CLAUSES 800000

typedef struct cell
{
    int status;     // Alive or dead
    int index;
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

// Writes the cnf obtainerd on a file
void writeCNFToFile(const char* filename, const char* cnf, int variablesNum);

// Checks if a Cell is alive or dead and construct the CNF file 'cnf.in' for the solver
void buildPastTable(table_t* t0, table_t* t1);

// Destroys table and frees memory
void destroyTable(table_t* t);

#endif