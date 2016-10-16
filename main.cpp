#include <iostream>
#include "parser.h"

using namespace std;

extern int tipo_de_busca;

extern int number_total_test;
extern int number_case_test;
extern int size_current_test;
extern int atribuicoes;

extern int  **play_matrix;
extern bool **lines_map;
extern bool **columns_map;
extern int  **line_restriction;
extern int  **columns_restriction;
extern int ***verif_ahead;


bool SolveFutoshiki (int x, int y);
bool SetVerifAhead  (int x, int y, int i);
void CleanVerifAhead(int x, int y);

// DEBUG =============
void PrintStateZero();
void PrintResult();
void PrintCurrent();
// ===================

int main(void)
{
    get_number_tests();
    while (number_case_test--) {
        get_size_test();
        get_number_restriction();

        if (tipo_de_busca >= 1)
            init_verif_ahead();

        init_lines_map();
        init_columns_map();

        init_play_matrix();
        init_restrictions_map();

        atribuicoes = 0;

        //PrintStateZero();

        printf("%d\n", number_total_test - number_case_test);
        if(SolveFutoshiki(0,0)) PrintResult();

        if(atribuicoes <= 1000000) printf("no de atribuicoes: %d\n\n", atribuicoes);
        else printf("Numero de atribuicoes excede limite maximo\n");

        free_data();
    }
    return 0;
}

// X e Y indo de '0' a 'D - 1'
bool SolveFutoshiki(int x, int y){
    //printf("\nEstado atual: (analisando (%d, %d) )\n", x, y);
    //PrintCurrent();

    //Conta quantas atribuições já foram feitas e para se chegar a 10^6
    atribuicoes++;
    if (atribuicoes > 1000000) return false;

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

        // Checa se o nº 'i' atual já está presente na coluna 'x' ou na linha 'y'
        bool alreadyPresent;
        if (tipo_de_busca == 0) alreadyPresent = (!lines_map[i-1][y] && !columns_map[i-1][x]);
        else alreadyPresent = (verif_ahead[y][x][i-1] > 0);

        if (alreadyPresent) {
            // Checa as restrições de maior ou menor
            bool restricted = false;
            if (x > 0){
                if ( (line_restriction[y][x-1] ==  1 && play_matrix[y][x-1] < i) ||
                     (line_restriction[y][x-1] == -1 && play_matrix[y][x-1] > i) )
                {
                    // printf("Restricao L %d em (%d, %d)\n", line_restriction[y][x-1], x-1, y);
                    restricted = true;
                }
            }

            if (y > 0){
                if ( (columns_restriction[y-1][x] ==  1 && play_matrix[y-1][x] < i) ||
                     (columns_restriction[y-1][x] == -1 && play_matrix[y-1][x] > i) )
                {
                    // printf("Restricao C %d em (%d, %d)\n", columns_restriction[y-1][x], x, y-1);
                    restricted = true;
                }
            }

            // Se não há restrições para o nº 'i', continua a recursão
            if(!restricted){
                play_matrix[y][x]   = i;

                if (tipo_de_busca == 0){
                    lines_map  [i-1][y] = true;
                    columns_map[i-1][x] = true;
                }

                // Se a busca é com verificação adiante, atualiza e checa validade
                bool verifyAhead = false;
                if (tipo_de_busca >= 1) verifyAhead = SetVerifAhead(x, y, i);

                if (tipo_de_busca == 0 || verifyAhead){
                    if (x == (size_current_test -1))    result = SolveFutoshiki(0, y + 1);
                    else                                result = SolveFutoshiki(x + 1, y);
                }
            }

            if (!result){
                if (tipo_de_busca == 0){
                    lines_map  [i -1][y] = false;
                    columns_map[i -1][x] = false;
                }
                else {
                    CleanVerifAhead(x, y);
                }
            }
        }
        // Se achou resultado true, retorna, senão segue o loop até testar todos os números
        if (result) return true;
    }
    // Se chegou até aqui então não achou nenhum número, e a disposição atual do tabuleiro não possui solução.
    // Assim, limpa a casa atual, reseta as variáveis correspondentes e retorna falso
    play_matrix[y][x] = 0;

    //printf("Backtracking: (analisando (%d, %d))\n", x, y);
    //PrintCurrent();

    return false;
}


// Checa se uma casa do tabuleiro ainda é válida ao usar busca com verificação adiante
bool isValid(int x, int y){
    bool valid = false;
    for (int i = 0; i < size_current_test; i++){
        if(verif_ahead[y][x][i] > 0) valid = true;
    }
    //if (!valid) printf("\n(%d, %d) nao valida\n\n", x, y);
    return valid;
}

bool SetVerifAhead(int x, int y, int i){
    // Se alguém marcar stillValid como falso, busca com verificação adiante para
    bool stillValid = true;

    for (int k = 1; k < size_current_test; k++){
        if (x+k < size_current_test){
            if(verif_ahead[y][x+k][i-1] == i) {
                verif_ahead[y][x+k][i-1] = 0;
                if (stillValid) stillValid = isValid(x+k, y);
            }
            else
                verif_ahead[y][x+k][i-1]--;
        }
        if (y+k < size_current_test) {
            if(verif_ahead[y+k][x][i-1] == i){
                verif_ahead[y+k][x][i-1] = 0;
                if (stillValid) stillValid = isValid(x, y+k);
            }
            else
                verif_ahead[y+k][x][i-1]--;
        }
    }

    return stillValid;
}

void CleanVerifAhead(int x, int y){
    for (int k = 1; k < size_current_test; k++){
        if (x+k < size_current_test){
            if( verif_ahead[y][x+k][play_matrix[y][x] -1] == 0 )
                verif_ahead[y][x+k][play_matrix[y][x] -1] = play_matrix[y][x];

            else if (verif_ahead[y][x+k][play_matrix[y][x] -1] < 0)
                     verif_ahead[y][x+k][play_matrix[y][x] -1]++;
        }
        if (y+k < size_current_test) {
            if( verif_ahead[y+k][x][play_matrix[y][x] -1] == 0)
                verif_ahead[y+k][x][play_matrix[y][x] -1] = play_matrix[y][x];

            else if( verif_ahead[y+k][x][play_matrix[y][x] -1] < 0)
                     verif_ahead[y+k][x][play_matrix[y][x] -1]++;
        }
    }
}


// DEBUG =============
void PrintStateZero(){
    printf("Estado inicial:\n");
    printf("Tabuleiro:\n");
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            if (tipo_de_busca == 0) printf("%d \t", play_matrix[i][j]);
            else{
                printf("%d {", play_matrix[i][j]);
                for (int k = 0; k < size_current_test; k++){
                    printf("%d, ", verif_ahead[i][j][k]);
                }
                printf("}   ");
            }
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

void PrintCurrent(){
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            if (tipo_de_busca == 0) printf("%d \t", play_matrix[i][j]);
            else{
                printf("%d {", play_matrix[i][j]);
                for (int k = 0; k < size_current_test; k++){
                    printf("%d, ", verif_ahead[i][j][k]);
                }
                printf("}   ");
            }
        }
        printf("\n");
    }
    //getchar();
}

void PrintResult(){
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            printf("%d ", play_matrix[i][j]);
        }
        printf("\n");
    }
}
