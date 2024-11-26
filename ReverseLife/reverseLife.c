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
    int index = 1;

    // Allocate memory for the table
    t.table = (cell_t**)malloc(lines * sizeof(cell_t*));
    for (int i = 0; i < lines; i++) 
    {
        t.table[i] = (cell_t*)malloc(columns * sizeof(cell_t));

        for (int j = 0; j < columns; j++)
        {
            t.table[i][j].status = DEAD;
            t.table[i][j].index = index;
            index++;
        }
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
void writeCNFToFile(const char* filename, const char* cnf, int variablesNum) 
{
    FILE* file = fopen(filename, "w");
    if (!file) 
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Count the number of variables and clauses
    int numClauses = countClauses(cnf);

    // Write CNF header
    fprintf(file, "p cnf %d %d\n", variablesNum, numClauses);

    // Write CNF constraints
    fprintf(file, "%s", cnf);

    fclose(file);
}

// Função que verifica se uma combinação tem exatamente 'aliveNum' vizinhos vivos
int hasAlive(int aliveNum, int* combination, int size) {
    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (combination[i] > 0) {
            count++;
        }
    }
    return count == aliveNum;
}

// Função que gera todas as combinações de vizinhos com exatamente 'aliveNum' vizinhos vivos
void generateAliveNeighboursClauses(int combinationSize, int aliveNum, char* cnf, int cellIndex, int* neighbors) 
{
    int n = 1 << combinationSize; // 2^combinationSize
    char clause[256];

    for (int i = 0; i < n; ++i) 
    {
        int combination[combinationSize];

        for (int j = 0; j < combinationSize; ++j) 
        {
            int sign = (i & (1 << j)) ? 1 : -1; // Alterna o sinal
            combination[j] = sign * neighbors[j];
        }

        if (hasAlive(aliveNum, combination, combinationSize)) 
        {
            memset(clause, 0, sizeof(clause));

            if (cellIndex != 0)
            {
                char cellIndexString[4];
                sprintf(cellIndexString, "%d", cellIndex);
                strcat(clause, cellIndexString);
                strcat(clause, " ");
            }

            for (int j = 0; j < combinationSize; ++j) 
            {
                char idx[4];
                snprintf(idx, sizeof(idx), "%d", -combination[j]);
                strcat(clause, idx);
                strcat(clause, " ");
            }

            strcat(clause, "0\n");
            strcat(cnf, clause);
        }
    }
}

int staysAliveCNF(char* cnf, int cellIndex, int* neighbors, int neighborsSize)
{
    int i = 0;

    // Clauses with 2 or 3 alive
    while (i <= 8)
    {
        if (i == 2 || i == 3) 
            generateAliveNeighboursClauses(neighborsSize, i, cnf, cellIndex, neighbors);
        i++;
    }

    return 1;
}

int isBornCNF(char* cnf, int cellIndex, int* neighbors, int neighborsSize)
{
    int i = 0;

    // Clauses with 3 alive
    while (i <= 8)
    {
        if (i == 3) 
            generateAliveNeighboursClauses(neighborsSize, i, cnf, cellIndex, neighbors);
        i++;
    }

    return 1;
}

int staysDeadCNF(char* cnf, int cellIndex, int* neighbors, int neighborsSize)
{
    int i = 0;

    // Clauses with less than 3 alive
    while (i <= 8)
    {
        if (i != 3) 
            generateAliveNeighboursClauses(neighborsSize, i, cnf, cellIndex, neighbors);
        i++;
    }

    return 1;
}

int underPopulationCNF(char* cnf, int cellIndex, int* neighbors, int neighborsSize)
{
    int i = 0;

    // Clauses with less than 2 alive
    while (i <= 8)
    {
        if (i < 2) 
            generateAliveNeighboursClauses(neighborsSize, i, cnf, cellIndex, neighbors);
        i++;
    }

    return 1;
}

int overPopulationCNF(char* cnf, int cellIndex, int* neighbors, int neighborsSize)
{
    int i = 0;

    // Clauses with more than 3 alive
    while (i <= 8)
    {
        if (i > 3) 
            generateAliveNeighboursClauses(neighborsSize, i, cnf, cellIndex, neighbors);
        i++;
    }

    return 1;
}

int getNeighborsIndexes(table_t* table, int x, int y, int* neighbors) 
{
    int neighborsIndex = 0;

    // Top
    if (x != 0)
    {
        if (y != 0)
        {
            neighbors[neighborsIndex] = table->table[x - 1][y - 1].index;
            neighborsIndex++;
        }

        neighbors[neighborsIndex] = table->table[x - 1][y].index;
        neighborsIndex++;

        if (y != table->columns - 1)
        {
            neighbors[neighborsIndex] = table->table[x - 1][y + 1].index;
            neighborsIndex++;
        }
    }

    // Left
    if (y != 0)
    {
        neighbors[neighborsIndex] = table->table[x][y - 1].index;
        neighborsIndex++;
    }
    
    // Right
    if (y != table->columns - 1)
    {
        neighbors[neighborsIndex] = table->table[x][y + 1].index;
        neighborsIndex++;
    }

    // Bottom
    if (x != table->lines - 1)
    {
        if (y != 0)
        {
            neighbors[neighborsIndex] = table->table[x + 1][y - 1].index;
            neighborsIndex++;
        }

        neighbors[neighborsIndex] = table->table[x + 1][y].index;
        neighborsIndex++;

        if (y != table->columns - 1)
        {
            neighbors[neighborsIndex] = table->table[x + 1][y + 1].index;
            neighborsIndex++;
        }
    }

    return neighborsIndex;
}

void buildPastTable(table_t* t0, table_t* t1)
{
    int neighbors[8], neighborsSize = 0;
    char clause[64];
    char cnf[MAX_CLAUSES];

    for (int i = 0; i < t1->lines; i++)
    {
        for (int j = 0; j < t1->columns; j++)
        {
            // Reset neighbors
            for (int i = 0; i < 8; i++)
                neighbors[i] = 0;

            // Get cell's neighbors indexes
            neighborsSize = getNeighborsIndexes(t1, i, j, neighbors);

            // Generate all the possible combinations of neighbors that could have generated the cells
            if (t1->table[i][j].status == ALIVE)
            {
                staysDeadCNF(cnf, t1->table[i][j].index, neighbors, neighborsSize);
                underPopulationCNF(cnf, 0, neighbors, neighborsSize);
                overPopulationCNF(cnf, 0, neighbors, neighborsSize);
            }

            else
            {
                staysAliveCNF(cnf, -t1->table[i][j].index, neighbors, neighborsSize);
                isBornCNF(cnf, 0, neighbors, neighborsSize);
            }
        }
    }

    // Write to the file
    writeCNFToFile("cnf.in", cnf, t1->lines * t1->columns);
}

void destroyTable(table_t* t) 
{
    for (int i = 0; i < t->lines; i++)
        free(t->table[i]);

    free(t->table);
    t->table = NULL;
}