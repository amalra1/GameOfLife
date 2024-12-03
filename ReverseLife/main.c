#include <stdio.h>
#include "reverseLife.h"
#include <core/Solver.h>

#define MAX_TRIES 10

void writeNewHeader(char* filename, int numVariables, int clausesNumber)
{
    FILE* file = fopen(filename, "w");
    if (!file) 
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    
    fprintf(file, "p cnf %d %d\n", numVariables, clausesNumber);

    fclose(file);
}

void copyToTemp(const char *sourceFile, const char *destFile) 
{
    FILE *source = fopen(sourceFile, "r");
    if (!source) 
    {
        perror("Failed to open source file");
        exit(EXIT_FAILURE);
    }

    FILE *dest = fopen(destFile, "w");
    if (!dest) 
    {
        perror("Failed to open destination file");
        fclose(source);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), source); // Skips first line
    while (fgets(line, sizeof(line), source))
        fputs(line, dest);

    fclose(source);
    fclose(dest);
}

void getDifferentSolution(table_t* t, int* t0AliveNum, int* minAliveNum)
{
    int clausesNumber;
	int aliveNum;
    
    // Writes new constraint into the cnf.in file to get a different solution
    clausesNumber = getClausesNumber("cnf.in");
    writeNewHeader("cnf.header", t->lines * t->columns, clausesNumber + 1);
    copyToTemp("cnf.in", "cnf.temp");
    joinFiles("cnf.header", "cnf.temp", "cnf.in");
    addTableConstraint("cnf.in", t);

    // Run minisat again
    system("./mergesat cnf.in cnf.out > /dev/null 2>&1");
    fillPastTable(t);
    // For validation
    printTable(t);
	aliveNum = aliveCells(t);

    if (aliveNum < *minAliveNum)
        *minAliveNum = aliveNum;
        

    printf("Alive cells: %d\n", aliveNum);
	moveToNextState(t);
    logTable(t, "pastTable2.txt");
}

int main()
{
    int t1AliveNum, t0AliveNum = 0, previousT0AliveNum = 0;
    int lines, columns;

    scanf("%d", &lines);
    scanf("%d", &columns);
    int minAliveNum = lines * columns;

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
    t1AliveNum = aliveCells(&t1);

    buildPastTable(&t0, &t1);

    // Execute minisat SAT Solver
    system("./mergesat cnf.in cnf.out > /dev/null 2>&1");

    if (fillPastTable(&t0))
    {
        t0AliveNum = aliveCells(&t0);

        // For validation
        printTable(&t0);
        printf("Alive cells: %d\n", t0AliveNum);
        moveToNextState(&t0);
        logTable(&t0, "pastTable.txt");
    }
    else
        printf("No past table found. [UNSAT]\n");


	for (int i = 0; i < MAX_TRIES; i++)
		getDifferentSolution(&t0, &t0AliveNum, &minAliveNum);

    printf("Minimum alive cells found: %d\n", minAliveNum);

    // Deletes the created inputs
    fclose(fopen("cnf.in", "w"));
    fclose(fopen("cnf.temp", "w"));
    fclose(fopen("cnf.header", "w"));

    // Frees
    destroyTable(&t1);
    destroyTable(&t0);
    for (int i = 0; i < lines; i++)
        free(presentState[i]);
    free(presentState);

    return 0;
}
