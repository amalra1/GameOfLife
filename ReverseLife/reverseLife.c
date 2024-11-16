#include "reverseLife.h"
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

// Checks if a living cell is in a normal state, returns 1 if normal and 0 otherwise
int normality(table_t* t, int line, int column)
{
    int neighbours = 0;
    int border1 = 0, border2 = 0;

    // Upper line
    
    // If it isn't on the first line
    if (line != 0)
    {
        // If it is on the first column
        if (column == 0)
        {
            border1 = column;
            border2 = column + 1;
        }

        // If it is on the last column
        else if (column == t->columns - 1)
        {
            border1 = column - 1;
            border2 = column;
        }

        // If it is on the columns among the first and last
        else
        {
            border1 = column - 1;
            border2 = column + 1;
        }

        for (int i = border1; i <= border2; i++)
            if (t->table[line - 1][i].status == ALIVE)
                neighbours++;
    }

    // Lower line
    // If it isn't on the last line
    if (line != t->lines - 1)
    {
        // If it is on the first column
        if (column == 0)
        {
            border1 = column;
            border2 = column + 1;
        }

        // If it is on the last column
        else if (column == t->columns - 1)
        {
            border1 = column - 1;
            border2 = column;
        }

        // If it is on the columns among the first and last
        else
        {
            border1 = column - 1;
            border2 = column + 1;
        }

        for (int i = border1; i <= border2; i++)
            if (t->table[line + 1][i].status == ALIVE)
                neighbours++;
    }

    if (neighbours > 3)
        return 0;

    // Left Side check
    // If it isn't on the first collumn
    if (column != 0)
        if (t->table[line][column - 1].status == ALIVE)
            neighbours++;

    if (neighbours > 3)
        return 0;

    // Right Side check
    // If it isn't on the last collumn
    if (column != t->columns - 1)
        if (t->table[line][column + 1].status == ALIVE)
            neighbours++;

    //printf("neighbours = %d\n", neighbours);

    if (neighbours == 2 || neighbours == 3)
        return 1;

    // Less than 2 or more than 3
    return 0;
}

// Checks if a dead cell can reproduce and become alive again, returns 1 if so and 0 otherwise
int reproduction(table_t* t, int line, int column)
{
    int neighbours = 0;
    int border1 = 0, border2 = 0;

    // Upper line
    
    // If it isn't on the first line
    if (line != 0)
    {
        // If it is on the first column
        if (column == 0)
        {
            border1 = column;
            border2 = column + 1;
        }

        // If it is on the last column
        else if (column == t->columns - 1)
        {
            border1 = column - 1;
            border2 = column;
        }

        // If it is on the columns among the first and last
        else
        {
            border1 = column - 1;
            border2 = column + 1;
        }

        for (int i = border1; i <= border2; i++)
            if (t->table[line - 1][i].status == ALIVE)
                neighbours++;
    }

    // Lower line
    // If it isn't on the last line
    if (line != t->lines - 1)
    {
        // If it is on the first column
        if (column == 0)
        {
            border1 = column;
            border2 = column + 1;
        }

        // If it is on the last column
        else if (column == t->columns - 1)
        {
            border1 = column - 1;
            border2 = column;
        }

        // If it is on the columns among the first and last
        else
        {
            border1 = column - 1;
            border2 = column + 1;
        }

        for (int i = border1; i <= border2; i++)
            if (t->table[line + 1][i].status == ALIVE)
                neighbours++;
    }

    if (neighbours > 3)
        return 0;

    // Left Side check
    // If it isn't on the first collumn
    if (column != 0)
        if (t->table[line][column - 1].status == ALIVE)
            neighbours++;

    if (neighbours > 3)
        return 0;

    // Right Side check
    // If it isn't on the last collumn
    if (column != t->columns - 1)
        if (t->table[line][column + 1].status == ALIVE)
            neighbours++;

    //printf("neighbours = %d\n", neighbours);

    if (neighbours != 3)
        return 0;
    else
        return 1;
}

void moveToNextState(table_t* t)
{
    table_t auxTable = initializeTable(t->lines, t->columns);

    // Go through the 'Past' table and fill the 'Future' one
    for (int i = 0; i < t->lines; i++)
    {
        for (int j = 0; j < t->columns; j++)
        {
            // LIVING CELL
            if (t->table[i][j].status == ALIVE)
            {
                // If it is ALIVE and not normal, surely got more than 3 or less than 2 neighbours
                // So... it dies
                if (!normality(t, i, j))
                    auxTable.table[i][j].status = DEAD;
                else
                    auxTable.table[i][j].status = ALIVE;
            }
            // DEAD CELL
            else
            {
                // Check Reproduction
                if (reproduction(t, i, j))
                    auxTable.table[i][j].status = ALIVE;
            }
        }
    }

    // Update original Table
    for (int i = 0; i < t->lines; i++)
        for (int j = 0; j < t->columns; j++)
            t->table[i][j].status = auxTable.table[i][j].status;

    // Free auxTable
    destroyTable(&auxTable);
}

// Function to count the number of lines (clauses) in the CNF string
int countClauses(const char* cnf) 
{
    int count = 0;
    const char* ptr = cnf;

    while (*ptr) 
    {
        if (*ptr == '\n')
            count++;
        ptr++;
    }

    return count;
}

// Function to write CNF to file
void writeCNFToFile(const char* filename, const char* cnf) 
{
    FILE* file = fopen(filename, "w");
    if (!file) 
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Count the number of variables and clauses
    int numVariables = 9;  // Adjust this as needed
    int numClauses = countClauses(cnf);

    // Write CNF header
    fprintf(file, "p cnf %d %d\n", numVariables, numClauses);

    // Write CNF constraints
    fprintf(file, "%s", cnf);

    fclose(file);
}

// Check if a given combination has exactly 3 alive neighbors
bool hasExactlyThreeAlive(int combination[], int size) 
{
    int count = 0;
    for (int i = 0; i < size; ++i) 
    {
        if (combination[i] > 0)
            count++;
    }

    return count == 3;
}

void generateThreeAliveNeighboursClauses(char* cnf, int* neighbors) 
{
    char clause[256];
    int combination[8];

    // All possible combinations of 8 neighbors
    for (int a = 0; a < 2; ++a) 
    {
        for (int b = 0; b < 2; ++b) 
        {
            for (int c = 0; c < 2; ++c) 
            {
                for (int d = 0; d < 2; ++d) 
                {
                    for (int e = 0; e < 2; ++e) 
                    {
                        for (int f = 0; f < 2; ++f) 
                        {
                            for (int g = 0; g < 2; ++g) 
                            {
                                for (int h = 0; h < 2; ++h) 
                                {
                                    combination[0] = a == 0 ? -neighbors[0] : neighbors[0];
                                    combination[1] = b == 0 ? -neighbors[1] : neighbors[1];
                                    combination[2] = c == 0 ? -neighbors[2] : neighbors[2];
                                    combination[3] = d == 0 ? -neighbors[3] : neighbors[3];
                                    combination[4] = e == 0 ? -neighbors[4] : neighbors[4];
                                    combination[5] = f == 0 ? -neighbors[5] : neighbors[5];
                                    combination[6] = g == 0 ? -neighbors[6] : neighbors[6];
                                    combination[7] = h == 0 ? -neighbors[7] : neighbors[7];

                                    if (hasExactlyThreeAlive(combination, 8)) 
                                    {
                                        // Generate a clause that negates this combination
                                        memset(clause, 0, sizeof(clause));
                                        for (int i = 0; i < 8; ++i) 
                                        {
                                            char idx[4];
                                            snprintf(idx, sizeof(idx), "%d", -combination[i]);
                                            strcat(clause, idx);
                                            strcat(clause, " ");
                                        }
                                        
                                        strcat(clause, "0\n");

                                        // Add unique clause to CNF
                                        if (strstr(cnf, clause) == NULL) 
                                            strcat(cnf, clause);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Can't have exactly 3 neighbours and was dead in t0
void staysDeadCNF(char* cnf, int line, int col) 
{
    // Cell 1 is dead
    strcat(cnf, "-1 0\n");

    int neighbors[8] = {2, 3, 4, 5, 6, 7, 8, 9};
    generateThreeAliveNeighboursClauses(cnf, neighbors);
}


void buildCNF(table_t* t, int line, int col)
{
    // Constraints buffer
    char cnf[81920] = "";

    // 1. Check if the selected cell is alive or dead
    if (t->table[line][col].status == ALIVE)
    {
        
    }
    else
    {
        // Generate the CNF file with constraints
        staysDeadCNF(cnf, line, col);
    }

    // Write the constraints into the cnf file
    writeCNFToFile("cnf.in", cnf);
}

void destroyTable(table_t* t) 
{
    for (int i = 0; i < t->lines; i++)
        free(t->table[i]);

    free(t->table);
    t->table = NULL;
}