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

extern int **mvr;
extern int *mvr_line_count;

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

        if (tipo_de_busca >= 1) init_verif_ahead();
        if (tipo_de_busca >= 2) init_mvr();

        init_lines_map();
        init_columns_map();


        init_play_matrix();
        init_restrictions_map();

        atribuicoes = 0;

        printf("%d\n", number_total_test - number_case_test);
        if(SolveFutoshiki(0,0)) PrintResult();

        if(atribuicoes >= 1000000)
            //printf("no de atribuicoes: %d\n", atribuicoes);
            printf("Numero de atribuicoes excede limite maximo\n");

        free_data();
    }
    return 0;
}

// X e Y indo de '0' a 'D - 1'
bool SolveFutoshiki(int x, int y){
    //Conta quantas atribui��es j� foram feitas e para se chegar a 10^6
    atribuicoes++;
    if (atribuicoes > 1000000) return false;

    // Se foi at� a �ltima casa e tentou executar na linha de baixo significa que o jogo terminou com sucesso
    if (y == size_current_test) return true;

    // Se casa atual j� est� no tabuleiro inicial pula para a pr�xima
    if (play_matrix[y][x] != 0){
        if (x == (size_current_test -1))    return SolveFutoshiki(0, y + 1);
        else                                return SolveFutoshiki(x + 1, y);
    }

    // Procura n�mero que caiba na casa atual, indo de 1 a 'D'
    bool result = false;
    if(tipo_de_busca >= 2) mvr_line_count[y]--;
    for (int i = 1; i <= size_current_test; i++){

        // Checa se o n� 'i' atual j� est� presente na coluna 'x' ou na linha 'y'
        bool alreadyPresent;
        if (tipo_de_busca <= 0) alreadyPresent = (!lines_map[i-1][y] && !columns_map[i-1][x]);
        else alreadyPresent = (verif_ahead[y][x][i-1] > 0);

        if (alreadyPresent) {
            // Checa as restri��es de maior ou menor
            bool restricted = false;
            // Checa se uma casa n�o foi ainda preenchida para descart�-la da checagem de restir��es
            bool mvrCheck = true;

            if (x > 0){
                if (tipo_de_busca >= 2 ) mvrCheck = (play_matrix[y][x-1] != 0);
                if (  mvrCheck && (
                     (line_restriction[y][x-1] ==  1 && play_matrix[y][x-1] < i) ||
                     (line_restriction[y][x-1] == -1 && play_matrix[y][x-1] > i) ) )
                {
                    restricted = true;
                }
            }

            if (y > 0){
                if (tipo_de_busca >= 2 ) mvrCheck = (play_matrix[y-1][x] != 0);
                if (  mvrCheck && (
                     (columns_restriction[y-1][x] ==  1 && play_matrix[y-1][x] < i) ||
                     (columns_restriction[y-1][x] == -1 && play_matrix[y-1][x] > i) ) )
                {
                    restricted = true;
                }
            }

            if (tipo_de_busca >= 2){
                if (x < size_current_test-1){
                    if (tipo_de_busca >= 2 ) mvrCheck = (play_matrix[y][x+1] != 0);
                    if (  mvrCheck && (
                         (line_restriction[y][x] ==  1 && play_matrix[y][x+1] > i) ||
                         (line_restriction[y][x] == -1 && play_matrix[y][x+1] < i) ) )
                    {
                        restricted = true;
                    }
                }
                if (y < size_current_test-1){
                    if (tipo_de_busca >= 2 ) mvrCheck = (play_matrix[y+1][x] != 0);
                    if (  mvrCheck && (
                         (columns_restriction[y][x] ==  1 && play_matrix[y+1][x] > i) ||
                         (columns_restriction[y][x] == -1 && play_matrix[y+1][x] < i) ) )
                    {
                        restricted = true;
                    }
                }
            }

            // Se n�o h� restri��es para o n� 'i', continua a recurs�o
            if(!restricted){
                play_matrix[y][x]   = i;

                if (tipo_de_busca >= 2) {
                    mvr[y][x]--;
                }

                else if (tipo_de_busca == 0){
                    lines_map  [i-1][y] = true;
                    columns_map[i-1][x] = true;
                }

                // Se a busca � com verifica��o adiante, atualiza e checa validade
                bool verifyAhead = false;
                if (tipo_de_busca >= 1) verifyAhead = SetVerifAhead(x, y, i);

                if (tipo_de_busca <= 0 || (tipo_de_busca == 1 && verifyAhead)){
                    if (x == (size_current_test -1))    result = SolveFutoshiki(0, y + 1);
                    else                                result = SolveFutoshiki(x + 1, y);
                }

                // Se a busca tem MVR, decidir qual a pr�xima casa a ser marcada
                else if (tipo_de_busca >= 2 && verifyAhead){
                    int smallest = size_current_test+1;
                    int sX = 0, sY = 0;

                    for (int a = 0; a < size_current_test && smallest > 1; a++){
                        if (mvr_line_count[a] > 0){
                            for (int b = 0; b < size_current_test && smallest > 1; b++){
                                if (mvr[a][b] < smallest && play_matrix[a][b] == 0){
                                    smallest = mvr[a][b];
                                    sY = a;
                                    sX = b;
                                }
                            }
                        }
                    }
                    result = SolveFutoshiki(sX, sY);
                }
            }

            if (!result){
                if (tipo_de_busca == 0){
                    lines_map  [i -1][y] = false;
                    columns_map[i -1][x] = false;
                }
                else {
                    CleanVerifAhead(x, y);
                    if (tipo_de_busca >= 2) {
                        mvr[y][x] = count_mvr(y, x);
                        if (play_matrix[y][x] > 0) mvr[y][x]++;
                    }
                }
            }
        }
        // Se achou resultado true, retorna, sen�o segue o loop at� testar todos os n�meros
        if (result) return true;
    }
    // Se chegou at� aqui ent�o n�o achou nenhum n�mero, e a disposi��o atual do tabuleiro n�o possui solu��o.
    // Assim, limpa a casa atual, reseta as vari�veis correspondentes e retorna falso
    play_matrix[y][x] = 0;
    if (tipo_de_busca >= 2) mvr_line_count[y]++;

    return false;
}


// Checa se uma casa do tabuleiro ainda � v�lida ao usar busca com verifica��o adiante
bool isValid(int x, int y){
    bool valid = false;
    for (int i = 0; i < size_current_test; i++){
        if(verif_ahead[y][x][i] > 0) valid = true;
    }
    return valid;
}

bool SetVerifAhead(int x, int y, int i){
    // Se algu�m marcar stillValid como falso, busca com verifica��o adiante para
    bool stillValid = true;
    int x2, y2;

    if (tipo_de_busca <= 1){
        x2 = x+1;
        y2 = y+1;
    }
    else{
        x2 = 0;
        y2 = 0;
    }

    for (int k = 0; k < size_current_test; k++){
        if (x2+k < size_current_test && x2+k != x){
            if(verif_ahead[y][x2+k][i-1] == i) {
                verif_ahead[y][x2+k][i-1] = 0;

                if (tipo_de_busca >= 2) mvr[y][x2+k]--;
                if (stillValid) stillValid = isValid(x2+k, y);
            }
            else
                verif_ahead[y][x2+k][i-1]--;
        }
        if (y2+k < size_current_test && y2+k != y) {
            if(verif_ahead[y2+k][x][i-1] == i){
                verif_ahead[y2+k][x][i-1] = 0;

                if (tipo_de_busca >= 2) mvr[y2+k][x]--;
                if (stillValid) stillValid = isValid(x, y2+k);
            }
            else
                verif_ahead[y2+k][x][i-1]--;
        }
    }

    return stillValid;
}

void CleanVerifAhead(int x, int y){
    int x2, y2;
    if (tipo_de_busca <= 1){
        x2 = x+1;
        y2 = y+1;
    } else{
        x2 = 0;
        y2 = 0;
    }

    for (int k = 0; k < size_current_test; k++){
        if (x2+k < size_current_test && x2+k != x){
            if( verif_ahead[y][x2+k][play_matrix[y][x] -1] == 0 ){
                verif_ahead[y][x2+k][play_matrix[y][x] -1] = play_matrix[y][x];
                if (tipo_de_busca >= 2) mvr[y][x2+k]++;
            }

            else if (verif_ahead[y][x2+k][play_matrix[y][x] -1] < 0)
                     verif_ahead[y][x2+k][play_matrix[y][x] -1]++;
        }
        if (y2+k < size_current_test && y2+k != y) {
            if( verif_ahead[y2+k][x][play_matrix[y][x] -1] == 0){
                verif_ahead[y2+k][x][play_matrix[y][x] -1] = play_matrix[y][x];
                if (tipo_de_busca >= 2) mvr[y2+k][x]++;
            }

            else if( verif_ahead[y2+k][x][play_matrix[y][x] -1] < 0)
                     verif_ahead[y2+k][x][play_matrix[y][x] -1]++;
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
                if (tipo_de_busca >= 2) printf("(%d)", mvr[i][j]);
                printf("%d {", play_matrix[i][j]);
                for (int k = 0; k < size_current_test; k++){
                    printf("%d, ", verif_ahead[i][j][k]);
                }
                printf("}  ");

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

    if (tipo_de_busca >= 2){
        printf("\nMVR Linhas:\n");
        for (int i = 0; i < size_current_test; i++) {
            printf("%d \t", mvr_line_count[i]);
        }
        printf("\n");
    }
    //getchar();
}

void PrintCurrent(){
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            if (tipo_de_busca == 0) printf("%d \t", play_matrix[i][j]);
            else{
                if (tipo_de_busca >= 2) printf("(%d)", mvr[i][j]);
                printf("%d {", play_matrix[i][j]);
                for (int k = 0; k < size_current_test; k++){
                    printf("%d,", verif_ahead[i][j][k]);
                }
                printf("}  ");
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
