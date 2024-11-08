#include <stdio.h>
#include "reverseLife.h"

int main()
{
    int lines, columns;

    scanf("%d", &lines);
    scanf("%d", &columns);

    table_t t = initializeTable(lines, columns);

    // Mallocs presentState matrix
    int** presentState = (int**)malloc(lines * sizeof(int*));
    for (int i = 0; i < lines; i++)
        presentState[i] = (int*)malloc(columns * sizeof(int));

    // Takes the presentState matrix from the file
    for (int i = 0; i < lines; i++)
        for (int j = 0; j < columns; j++)
            scanf("%d", &presentState[i][j]);

    setInitialState(&t, presentState);

    // MAIN LOGIC GOES HERE

    //printTable(&t);

    // Frees
    destroyTable(&t);
    for (int i = 0; i < lines; i++)
        free(presentState[i]);
    free(presentState);

    return 0;
}