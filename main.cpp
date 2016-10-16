#include <iostream>
#include "parser.h"
#include "futoshiki.h"

#define NO_RESTRICTION      0 // 0 = Sem poda, 1 = Verif. adiante, 2 = Verfif. adiante e MVR

using namespace std;

extern int number_case_test;
extern int size_current_test;

extern int  **play_matrix;
extern bool **lines_map;
extern bool **columns_map;
extern int  **line_restriction;
extern int  **columns_restriction;

int assigned_cells = 0;

bool SolveFutoshiki(int x, int y);
void PrintStateZero();
void PrintResult();

int main(void)
{
    get_number_tests();
    while (number_case_test--) {
        get_size_test();
        get_number_restriction();

        init_lines_map();
        init_columns_map();

        init_play_matrix();
        init_restrictions_map();
        init_domain_variables();

        SolveFutoshiki(0,0);

        PrintResult();

        free_data();
    }
    return 0;
}

// X e Y indo de '0' a 'D - 1'
bool SolveFutoshiki(int x, int y){

    /*checa se o tablueiro ja esta todo preenchido*/
    if (assigned_cells == ((size_current_test * size_current_test) - 1)) {
        if (validate_play_matrix() == true) {
            PrintStateZero();
            return true;
        } else {
            return false; // se chegou ate aqui, a posicao atual do tabuleiro eh invalida
        }
    }

    int value = apply_heuristic_mrv(&x, &y);
    if (value == -1) {
        return false;
    }

    if (lines_map[value-1][y] || columns_map[value-1][x]) {
        restore_domain_mrv(value, x, y);
        return false;
    }

    // verifica se respeita as restricoes de maior ou menor
    if (exist_less_greater(value, x, y) == true) {
        restore_domain_mrv(value, x, y);
        return false;
    }

    play_matrix[x][y] = value;
    assigned_cells++;
    return SolveFutoshiki(0, 0);
}

void PrintStateZero(){
    printf("Estado inicial:\n");
    printf("Tabuleiro:\n");
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", play_matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nMapa de linhas:\n");
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", lines_map[i][j]? 1 : 0);
        }
        printf("\n");
    }

    printf("\nMapa de colunas:\n");
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", columns_map[i][j]? 1 : 0);
        }
        printf("\n");
    }

    printf("\nRestrições de linhas:\n");
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", line_restriction[i][j]);
        }
        printf("\n");
    }

    printf("\nRestrições de colunas:\n");
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", columns_restriction[i][j]);
        }
        printf("\n");
    }
    getchar();
}

void PrintResult(){
    printf("Solucao:\n");
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", play_matrix[i][j]);
        }
        printf("\n");
    }
}
