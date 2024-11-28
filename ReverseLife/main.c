#include <stdio.h>
#include "reverseLife.h"

int main()
{
    int lines, columns;

    scanf("%d", &lines);
    scanf("%d", &columns);

    table_t t1 = initializeTable(lines, columns);
    table_t t0 = initializeTable(lines, columns);

    // Mallocs presentState matrix
    int** presentState = (int**)malloc(lines * sizeof(int*));
    for (int i = 0; i < lines; i++)
        presentState[i] = (int*)malloc(columns * sizeof(int));

    // Takes the presentState matrix from the file
    for (int i = 0; i < lines; i++)
        for (int j = 0; j < columns; j++)
            scanf("%d", &presentState[i][j]);

    setInitialState(&t1, presentState);

    buildPastTable(&t0, &t1);

    // Execute minisat SAT Solver
    system("minisat cnf.in cnf.out");

    // Deletes cnf.in
    fclose(fopen("cnf.in", "w"));
    fclose(fopen("cnf.temp", "w"));
    fclose(fopen("cnf.header", "w"));

    // Gets result from 'cnf.out' and fills t0
    if (fillPastTable(&t0))
        printTable(&t0);
    else
        printf("No past table found. [UNSAT]\n");

    // For validation
    moveToNextState(&t0);
    logTable(&t0, "pastTable.txt");

    // Frees
    destroyTable(&t1);
    destroyTable(&t0);
    for (int i = 0; i < lines; i++)
        free(presentState[i]);
    free(presentState);

    return 0;
}