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

    //printTable(&auxTable);

    // Update original Table
    for (int i = 0; i < t->lines; i++)
        for (int j = 0; j < t->columns; j++)
            t->table[i][j].status = auxTable.table[i][j].status;

    // Free auxTable
    destroyTable(&auxTable);
}

void writeCNFToFile(const char* filename, const char* cnf) 
{
    FILE* file = fopen("cnf.in", "w");
    if (!file) 
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Write CNF header
    // Assuming a maximum of 8 variables and clauses for illustration; adjust as needed
    int numVariables = 8;   
    fprintf(file, "p cnf %d\n", numVariables);

    // Write CNF constraints
    fprintf(file, "%s", cnf);

    fclose(file);
}

void generateAliveCNF(char* cnf, int line, int col)
{

}

// Function to check if a combination has at least one alive element
int hasAtLeastOneAlive(int combination[], int size) {
    for (int i = 0; i < size; ++i) {
        if (combination[i] > 0) {
            return 1;
        }
    }
    return 0;
}

void generateCombinationsOf6(int combinations[1764][6]) 
{
    int arr[8] = {2, 3, 4, 5, 6, 7, 8, 9};
    int index = 0;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = i + 1; j < 8; ++j)
        { 
            for (int k = j + 1; k < 8; ++k) 
            {
                for (int l = k + 1; l < 8; ++l)
                { 
                    for (int m = l + 1; m < 8; ++m)
                    { 
                        for (int n = m + 1; n < 8; ++n)
                        {
                            for (int v = 0; v < 64; ++v) 
                            { 
                                int combination[6] = {
                                    (v & 1) ? -arr[i] : arr[i],
                                    (v & 2) ? -arr[j] : arr[j],
                                    (v & 4) ? -arr[k] : arr[k],
                                    (v & 8) ? -arr[l] : arr[l],
                                    (v & 16) ? -arr[m] : arr[m],
                                    (v & 32) ? -arr[n] : arr[n],
                                };

                                if (hasAtLeastOneAlive(combination, 6)) {
                                    for (int idx = 0; idx < 6; ++idx) {
                                        combinations[index][idx] = combination[idx];
                                    }
                                    index++;
                                }
                            }
                        }
                    }
                }
            }
        }
    } 
}

void generateCombinationsOf2(int combinations[84][2]) 
{
    int index = 0;

    // Loop through all pairs of neighbors
    for (int i = 0; i < 8; ++i) 
    {
        for (int j = i + 1; j < 8; ++j) 
        {           
            // Combination where the first neighbor is dead
            combinations[index][0] = -(i + 2);
            combinations[index][1] = j + 2;
            index++;
            
            // Combination where the second neighbor is dead
            combinations[index][0] = i + 2;
            combinations[index][1] = -(j + 2);
            index++;
            
            // Combination where both neighbors are dead
            combinations[index][0] = -(i + 2);
            combinations[index][1] = -(j + 2);
            index++;

            //printf("%d %d\n", i + 1, j + 1);
        }
    }
}

// Generates the CNF file based on the rules of an alive cell in the present
// Rules: Life | Preservation
void generateDeadCNF(char* cnf, int line, int col)
{
    // Buffer to store the clause 
    char clause[64];
    int combinationsOf2[84][2];
    int combinationsOf6[1764][6];
    
    // 1. Preservation

    // Considering 
    // X: The cell itself
    // C: All of the size 2 combinations of neighours;
    // N: All neighbours
        // (!x OR (OR(!c)) OR (OR(N - c)))

    // Generate combinations of 2 where at least one is dead 
    // (28(Combinations) * 3(variations)) = 84
    generateCombinationsOf2(combinationsOf2);

    // for (int i = 0; i < 84; i++)
    // {
    //     for (int j = 0; j < 2; j++)
    //     {
    //         printf("%d ", combinationsOf2[i][j]);
    //     }
    //     printf("\n");
    // }
    
    // Generate combinations of 6 where at least one is alive
    // (28(Combinations) * 63(variations)) = 1764
    generateCombinationsOf6(combinationsOf6);

    // for (int i = 0; i < 1764; i++)
    // {
    //     for (int j = 0; j < 6; j++)
    //     {
    //         printf("%d ", combinationsOf6[i][j]);
    //     }
    //     printf("\n");
    // }


    // IMPORTANT - Literal "1" is the current cell
    // - Literals "[2-9] are the neighbours"
    
    // Generate CNF clauses for each combination of 2
    for (int i = 0; i < 84; ++i) 
    {
        memset(clause, 0, sizeof(clause));
        strcat(clause, "1 ");

        for (int j = 0; j < 2; ++j) 
        {
            if (combinationsOf2[i][j] < 0) 
            {
                strcat(clause, "-");
                char idx[4];
                snprintf(idx, sizeof(idx), "%d", -combinationsOf2[i][j]);
                strcat(clause, idx);
            } 
            else
            {
                char idx[4];
                snprintf(idx, sizeof(idx), "%d", combinationsOf2[i][j]);
                strcat(clause, idx);
            }
            strcat(clause, " ");
        }
        strcat(clause, "0\n");
        strcat(cnf, clause);
    }

    // Generate CNF clauses for each combination of 6
    for (int i = 0; i < 1764; ++i) 
    {
        memset(clause, 0, sizeof(clause));
        strcat(clause, "1 ");

        for (int j = 0; j < 6; ++j) 
        {
            if (combinationsOf6[i][j] < 0) 
            {
                strcat(clause, "-");
                char idx[4];
                snprintf(idx, sizeof(idx), "%d", -combinationsOf6[i][j]);
                strcat(clause, idx);
            } 
            else 
            {
                char idx[4];
                snprintf(idx, sizeof(idx), "%d", combinationsOf6[i][j]);
                strcat(clause, idx);
            }
            strcat(clause, " ");
        }
        strcat(clause, "0\n");
        strcat(cnf, clause);
    }
    
    // 2. Life 
}

void buildCNF(table_t* t, int line, int col)
{
    // Constraints buffer
    char cnf[81920] = "";

    // 1. Check if the selected cell is alive or dead
    if (t->table[line][col].status == ALIVE)
    {
        // Test Loneliness, Overcrowding and Stagnation

        // Generate the CNF file with constraints
        generateAliveCNF(cnf, line, col);
    }
    else
    {
        // Test Preservation, Life

        // Generate the CNF file with constraints
        generateDeadCNF(cnf, line, col);
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