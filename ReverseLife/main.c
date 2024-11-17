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

    // Testing for the middle cell
    buildPastTable(&t0, &t1, lines/2, columns/2);

    printTable(&t1);
    printTable(&t0);

    // Frees
    destroyTable(&t1);
    for (int i = 0; i < lines; i++)
        free(presentState[i]);
    free(presentState);

    return 0;
}