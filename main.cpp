#include <iostream>
#include "futoshiki.h"
#include "parser.h"

using namespace std;

<<<<<<< HEAD
extern int      number_case_test;
extern int      size_current_test;
extern int      number_current_restrictions;
extern uint8_t  **play_matrix;
extern bool     **lines_map;
extern bool     **columns_map;
extern uint8_t  **line_restriction;
extern uint8_t  **columns_restriction;
=======
extern int number_case_test;
extern int size_current_test;

extern int  **play_matrix;
extern bool **lines_map;
extern bool **columns_map;
extern int  **line_restriction;
extern int  **columns_restriction;


bool SolveFutoshiki(int x, int y);
>>>>>>> sol_sem_poda

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

        printf("Estado inicial:\n");
        printf("Tabuleiro:\n");
        for (int i = 0; i < size_current_test; i++) {
            for (int j = 0; j < size_current_test; j++) {
                if (play_matrix[i][j] == 0)
                    play_matrix[i][j] = 7;

                printf("%d \t", play_matrix[i][j]);
            }
            printf("\n");
        }

<<<<<<< HEAD
        //getchar();
        init_restrictions_map();
        init_lines_map();
        init_columns_map();

        int val, l, c;
        printf("restriction: <value> <line> <column>\n");
        scanf("%d %d %d", &val, &l, &c);
        if (exist_less_greater(val, l, c) == true)
            cout << "hey... you cannot put these value there.\n" << endl;
=======
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

        SolveFutoshiki(0,0);
>>>>>>> sol_sem_poda

        free_data();
    }
    return 0;
}
;

bool SolveFutoshiki(int x, int y){
    printf("Estado atual: (analisando (%d, %d) )\n", x, y);
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", play_matrix[i][j]);
        }
        printf("\n");
    }
    getchar();

    // Se foi até a última casa e tentou executar na linha de baixo significa que o jogo terminou com sucesso
    if (y == size_current_test) return true;

    // Se casa atual já está no tabuleiro inicial pula para a próxima
    if (play_matrix[y][x] != 0){
        if (x == (size_current_test -1))    return SolveFutoshiki(0, y + 1);
        else                                return SolveFutoshiki(x + 1, y);
    }

    // Procura número que caiba na casa atual, indo de 1 a 'D'
    bool result = false;
    for (int i = 1; i <= size_current_test; i++){
        if (!lines_map[i-1][y] && !columns_map[i-1][x]) {
            play_matrix[y][x]   = i;
            lines_map  [i-1][y] = true;
            columns_map[i-1][x] = true;

            if (x == (size_current_test -1))    result = SolveFutoshiki(0, y + 1);
            else                                result = SolveFutoshiki(x + 1, y);

            if (!result){
                lines_map  [(play_matrix[y][x]) -1][y] = false;
                columns_map[(play_matrix[y][x]) -1][x] = false;
            }
        }
        // Se achou resultado true, retorna, senão segue o loop até testar todos os números
        if (result) return true;
    }
    // Se chegou até aqui então não achou nenhum número, e a disposição atual do tabuleiro não possui solução.
    // Assim, limpa a casa atual, reseta as variáveis correspondentes e retorna falso

    play_matrix[y][x] = 0;

    printf("Estado atual: (analisando (%d, %d): %d)\n", x, y, play_matrix[y][x]);
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d \t", play_matrix[i][j]);
        }
        printf("\n");
    }
    getchar();

    return false;
}
