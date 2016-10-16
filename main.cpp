#include <iostream>
#include "parser.h"

#define NO_RESTRICTION      0 // 0 = Sem poda, 1 = Verif. adiante, 2 = Verfif. adiante e MVR

using namespace std;

extern int number_case_test;
extern int size_current_test;

extern int  **play_matrix;
extern bool **lines_map;
extern bool **columns_map;
extern int  **line_restriction;
extern int  **columns_restriction;



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

        SolveFutoshiki(0,0);

        PrintResult();

        free_data();
    }
    return 0;
}

// X e Y indo de '0' a 'D - 1'
bool SolveFutoshiki(int x, int y){
    /*
    printf("Estado atual: (analisando (%d, %d) )\n", x, y);
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", play_matrix[i][j]);
        }
        printf("\n");
    }
    getchar();
    */


    // Se foi at� a �ltima casa e tentou executar na linha de baixo significa que o jogo terminou com sucesso
    if (y == size_current_test) return true;

    // Se casa atual j� est� no tabuleiro inicial pula para a pr�xima
    if (play_matrix[y][x] != 0){
        if (x == (size_current_test -1))    return SolveFutoshiki(0, y + 1);
        else                                return SolveFutoshiki(x + 1, y);
    }

    // Procura n�mero que caiba na casa atual, indo de 1 a 'D'
    bool result = false;
    for (int i = 1; i <= size_current_test; i++){
        // Checa se o n� 'i' atual j� est� presente na coluna 'x' ou na linha 'y'
        if (!lines_map[i-1][y] && !columns_map[i-1][x]) {

            // Checa as restri��es de maior ou menor
            bool restricted = false;
            if (x > 0){
                if ( (line_restriction[y][x-1] ==  1 && play_matrix[y][x-1] < i) ||
                     (line_restriction[y][x-1] == -1 && play_matrix[y][x-1] > i) )
                {
                    //printf("Restricao L %d em (%d, %d)\n", line_restriction[y][x-1], x-1, y);
                    restricted = true;
                }
            }

            if (y > 0){
                if ( (columns_restriction[y-1][x] ==  1 && play_matrix[y-1][x] < i) ||
                     (columns_restriction[y-1][x] == -1 && play_matrix[y-1][x] > i) )
                {
                    //printf("Restricao C %d em (%d, %d)\n", columns_restriction[y-1][x], x, y-1);
                    restricted = true;
                }
            }

            // Se n�o h� restri��es para o n� 'i', continua a recurs�o
            if(!restricted){
                play_matrix[y][x]   = i;
                lines_map  [i-1][y] = true;
                columns_map[i-1][x] = true;

                if (x == (size_current_test -1))    result = SolveFutoshiki(0, y + 1);
                else                                result = SolveFutoshiki(x + 1, y);
            }

            if (!result){
                lines_map  [i -1][y] = false;
                columns_map[i -1][x] = false;
            }
        }
        // Se achou resultado true, retorna, sen�o segue o loop at� testar todos os n�meros
        if (result) return true;
    }
    // Se chegou at� aqui ent�o n�o achou nenhum n�mero, e a disposi��o atual do tabuleiro n�o possui solu��o.
    // Assim, limpa a casa atual, reseta as vari�veis correspondentes e retorna falso
    play_matrix[y][x] = 0;

    /*
    printf("Backtracking: (analisando (%d, %d))\n", x, y);
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", play_matrix[i][j]);
        }
        printf("\n");
    }
    getchar();
    */

    return false;
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

    printf("\nRestri��es de linhas:\n");
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", line_restriction[i][j]);
        }
        printf("\n");
    }

    printf("\nRestri��es de colunas:\n");
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
