#include <stdio.h>
#include "gameOfLife.h"

int main()
{
    int generations = GENERATIONS_MAX;
    int lines, columns;

    scanf("%d", &lines);
    scanf("%d", &columns);

    table_t t = initializeTable(lines, columns);

    int** initialState = (int**)malloc(lines * sizeof(int*));
    for (int i = 0; i < lines; i++)
        initialState[i] = (int*)malloc(columns * sizeof(int));

    // Scan for initial State configuration
    for (int i = 0; i < lines; i++)
        for (int j = 0; j < columns; j++)
            scanf("%d", &initialState[i][j]);
    
    setInitialState(&t, initialState);

    //printTable(&t);

    // Starts Life
    for (int i = 0; i < generations; i++)
    {
        printf("GENERATION: %d\n\n", i + 1);
        printTable(&t);    
        moveToNextState(&t);
    }
    

    // Frees
    for (int i = 0; i < lines; i++)
        free(initialState[i]);
    free(initialState);
    destroyTable(&t);

    return 0;
}