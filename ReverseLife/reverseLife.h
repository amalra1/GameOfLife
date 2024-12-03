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

// Counts how many alive cells a table t has and returns it
int aliveCells(table_t* t);

// Verifies the 4 game rules of a table and moves it to a next state
void moveToNextState(table_t* t);

// Prints table
void printTable(table_t* t);

// Prints table in a file
void logTable(table_t* t, char* filename);

// Writes the cnf obtainerd on a file
void writeCNFToFile(const char* filename, const char* cnf, int variablesNum);

// Writes the header of the cnf input file
void writeHeader(const char* filename, int variablesNum, int numClauses);

// Writes the clauses to a temporary file, without the header
void writeToTemp(const char* filename, const char* cnf);

// Join temp and header files into the cnf input file ready for the SAT solver
void joinFiles(const char* header, const char* temp, const char* output); 

// Checks if a Cell is alive or dead and construct the CNF file 'cnf.in' for the solver
void buildPastTable(table_t* t0, table_t* t1);

// Writes constraints to not have the previos generated solution and find another one
void addTableConstraint(char* filename, table_t* t0);

// Accesses the the input cnf file with the given name and returns the number of clauses of the header
int getClausesNumber(char* filename);

// Gets result from 'cnf.out' file and fill t0 with it
int fillPastTable(table_t* t);

// Destroys table and frees memory
void destroyTable(table_t* t);

#endif
