#include <stdio.h>
#include "reverseLife.h"
#include <core/Solver.h>

// Função para contar o número de variáveis verdadeiras no modelo
int count_ones(int** table, int rows, int cols) {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (table[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

// Função para adicionar a restrição de que a tabela t deve ser igual a table
void addNegatedSolution(char* filename, table_t* t) {
    FILE* file = fopen(filename, "a");
    for (int i = 0; i < t->lines; i++) {
        for (int j = 0; j < t->columns; j++) {
            if (t->table[i][j].status == ALIVE) {
                fprintf(file, "-%d 0\n", t->table[i][j].index);
            } else {
                fprintf(file, "%d 0\n", t->table[i][j].index);
            }
        }
    }
    fclose(file);
}

void getMinimalAliveCells(table_t* t, int* t0AliveNum) {
    int clausesNumber, minAliveCells = t->lines * t->columns;
    int currentAliveCells;

    while (1) {
        // Conta as cláusulas antes de adicionar novas
        clausesNumber = getClausesNumber("cnf.in");

        // Adiciona novas cláusulas ao arquivo CNF
        addTableConstraint("cnf.in", t);
        addNegatedSolution("cnf.in", t);

        // Atualiza o cabeçalho do arquivo com o número correto de cláusulas
        writeHeader("cnf.header", t->lines * t->columns, clausesNumber + 1);
        writeToTemp("cnf.in", "cnf.temp");
        joinFiles("cnf.header", "cnf.temp", "cnf.in");

        // Executa o SAT Solver
        system("./mergesat cnf.in cnf.out");

        if (!fillPastTable(t)) {
            printf("Solução mínima encontrada com %d células vivas.\n", minAliveCells);
            break;
        }

        // Conta as células vivas na tabela atual
        currentAliveCells = aliveCells(t);

        printf("Current Alive Cells: %d, Min Alive Cells: %d\n", currentAliveCells, minAliveCells);

        // Check for progress
        if (currentAliveCells >= minAliveCells) {
            printf("No further minimization possible. Exiting.\n");
            break;
        }

        // Atualiza o mínimo encontrado
        minAliveCells = currentAliveCells;

        // Adiciona restrição para bloquear essa solução
        addNegatedSolution("cnf.in", t);
    }

    *t0AliveNum = minAliveCells;
}


int main() {
    int t1AliveNum, t0AliveNum = 0, previousT0AliveNum = 0;
    int lines, columns;

    scanf("%d", &lines);
    scanf("%d", &columns);

    table_t t1 = initializeTable(lines, columns);
    table_t t0 = initializeTable(lines, columns);

    // Mallocs presentState matrix
    int** presentState = (int**)malloc(lines * sizeof(int*));
    for (int i = 0; i < lines; i++)
        presentState[i] = (int*)malloc(columns * sizeof(int));

    // Recebe o estado atual da matriz
    for (int i = 0; i < lines; i++)
        for (int j = 0; j < columns; j++)
            scanf("%d", &presentState[i][j]);

    setInitialState(&t1, presentState);
    t1AliveNum = aliveCells(&t1);

    buildPastTable(&t0, &t1);

    // Executa o SAT Solver
    system("./mergesat cnf.in cnf.out");

    if (fillPastTable(&t0)) {
        t0AliveNum = aliveCells(&t0);

        // Validação e log
        printTable(&t0);
        printf("Células vivas: %d\n", t0AliveNum);
        moveToNextState(&t0);
        logTable(&t0, "pastTable.txt");
    } else {
        printf("Nenhuma tabela passada encontrada. [UNSAT]\n");
    }

    // Minimiza células vivas
    getMinimalAliveCells(&t0, &t0AliveNum);

    // Limpa arquivos temporários
    // fclose(fopen("cnf.in", "w"));
    fclose(fopen("cnf.temp", "w"));
    fclose(fopen("cnf.header", "w"));

    // Liberação de memória
    destroyTable(&t1);
    destroyTable(&t0);
    for (int i = 0; i < lines; i++)
        free(presentState[i]);
    free(presentState);

    return 0;
}
