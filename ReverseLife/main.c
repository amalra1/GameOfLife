#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "reverseLife.h"
//#include <core/Solver.h>

// Coin flips
#define MAX_TRIES 200
#define TIME_LIMIT 270 // 4 minutes and 30 seconds in seconds

volatile int timeout = 0; // Flag to indicate timeout

void handle_timeout(int sig) 
{
    timeout = 1;
    printf("\nTime exceeded 4 minutes and 30 seconds.\n");
}

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

void switchMinTable(table_t* t, table_t* minT)
{
    for (int i = 0; i < t->lines; i++)
        for (int j = 0; j < t->columns; j++)
            minT->table[i][j].status = t->table[i][j].status;
}

void getDifferentSolution(table_t* t, table_t* minT, int* t0AliveNum, int* minAliveNum)
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
    system("./mergesat -mem-lim=500 -cpu-lim=300 -rtype=3 -rnd-init=3 -grow=50 cnf.in cnf.out > /dev/null 2>&1");
    //system("minisat cnf.in cnf.out");

    fillPastTable(t);
    aliveNum = aliveCells(t);

    if (aliveNum <= *minAliveNum)
    {
        *minAliveNum = aliveNum;
        switchMinTable(t, minT);
    }
}

int main()
{
    // Register signal handler for timeout
    signal(SIGALRM, handle_timeout);
    alarm(TIME_LIMIT); // Set timeout

    int t1AliveNum, t0AliveNum = 0;
    int lines, columns;

    scanf("%d", &lines);
    scanf("%d", &columns);
    int minAliveNum = lines * columns;

    table_t t1 = initializeTable(lines, columns);
    table_t t0 = initializeTable(lines, columns);
    table_t minTable = initializeTable(lines, columns);
    
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

    // Generate cnf file to generate the t0 table
    buildPastTable(&t0, &t1);

    // Call the SAT Solver
    system("./mergesat -mem-lim=500 -cpu-lim=300 -rtype=3 -rnd-init=3 -grow=50 cnf.in cnf.out > /dev/null 2>&1");
    //system("minisat cnf.in cnf.out");

    // Get solution from cnf.out file and fill the table
    fillPastTable(&t0);
    t0AliveNum = aliveCells(&t0);
    minAliveNum = t0AliveNum;
    logTable(&t0, "pastTable.txt");

    switchMinTable(&t0, &minTable);

    // Get new solution adding all cells but inverted as a clause to the cnf
    // Until MAX TRIES or Timeout max
    for (int i = 0; i < MAX_TRIES && !timeout; i++)
        getDifferentSolution(&t0, &minTable, &t0AliveNum, &minAliveNum);

    printf("Minimum alive cells found: %d\n", minAliveNum);
    printf("Minimum table:\n");
    printTable(&minTable);

    // Log to compare with the input == Should be exactly equal
    moveToNextState(&minTable);
    logTable(&minTable, "pastTable.txt");
    
    // Cleanup
    fclose(fopen("cnf.in", "w"));
    fclose(fopen("cnf.temp", "w"));
    fclose(fopen("cnf.header", "w"));

    destroyTable(&t1);
    destroyTable(&t0);
    for (int i = 0; i < lines; i++)
        free(presentState[i]);
    free(presentState);

    return 0;
}
