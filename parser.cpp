#include "parser.h"
#include <set>

using namespace std;

int tipo_de_busca = 2; // 0 = Sem poda, 1 = Verif. adiante, 2 = Verfif. adiante e MVR

FILE *stream_input = NULL;

int  number_total_test = 0;
int  number_case_test = 0;
int  size_current_test = 0;
int  number_current_restrictions = 0;
int  atribuicoes = 0;
int  **play_matrix = NULL;
bool **lines_map = NULL;
bool **columns_map = NULL;
int  **line_restriction = NULL;
int  **columns_restriction = NULL;

int ***verif_ahead = NULL;

int **mvr = NULL;
int  *mvr_line_count = NULL;    // Conta quantas casas ainda não foram preenchidas em uma certa linha

#define END_OF_TESTS -1

int get_number_tests(void)
{
    if (stream_input == NULL) {
        stream_input = (FILE *) fopen("futoshiki.dat", "r");
    }

    fscanf(stream_input, "%d", &number_case_test);
    number_total_test = number_case_test;
    return number_case_test;
}

int get_size_test(void)
{
    if (feof(stream_input))
        return END_OF_TESTS;
    fscanf(stream_input, "%d", &size_current_test);

    if (size_current_test < 4 || size_current_test > 9) {
        fprintf(stderr, "error: size restriction overflow/underflow\n");
        exit(1);
    }

    return size_current_test;
}

int get_number_restriction(void)
{
    fscanf(stream_input, "%d", &number_current_restrictions);
    return number_current_restrictions;
}

// Atualiza valores das outras matrizes, por isso precisa ser a última função init chamada
void init_play_matrix(void)
{
    register int i, j;
    play_matrix = (int **) malloc(size_current_test * sizeof(int *));

    for (i = 0; i < size_current_test; i++) {
        play_matrix[i] = (int *) malloc(size_current_test * sizeof(int));
        for (j = 0; j < size_current_test; j++) {
            int tmp;
            fscanf(stream_input, "%d", &tmp);
            play_matrix[i][j] = tmp;
        }
    }

    for (i = 0; i < size_current_test; i++) {
        for (j = 0; j < size_current_test; j++) {
            int tmp = play_matrix[i][j];

            if (tmp != 0){
                lines_map[tmp-1][i] = true;
                columns_map[tmp-1][j] = true;

                if (tipo_de_busca >= 1){
                    for (int k = 0; k < size_current_test; k++){
                        if(j != 0+k){
                            if (verif_ahead[i][0+k][tmp-1] > 0) verif_ahead[i][0+k][tmp-1] = 0;
                            else                                verif_ahead[i][0+k][tmp-1]--;

                            if (tipo_de_busca >= 2) mvr[i][0+k] = count_mvr(i, 0+k);
                        }
                        if(i != 0+k){
                            if (verif_ahead[0+k][j][tmp-1] > 0) verif_ahead[0+k][j][tmp-1] = 0;
                            else                                verif_ahead[0+k][j][tmp-1]--;

                            if (tipo_de_busca >= 2) mvr[0+k][j] = count_mvr(0+k, j);
                        }
                    }
                }
            }

            if (tmp > 0 && tipo_de_busca >= 2){
                mvr_line_count[i]--;
                mvr[i][j] = count_mvr(i, j);
            }
        }
    }
}

void init_lines_map(void)
{
    register int i, j;
    lines_map = (bool **) malloc(size_current_test * sizeof(bool *));
    for (i = 0; i < size_current_test; i++) {
        lines_map[i] = (bool *) malloc(size_current_test * sizeof(bool));
        for (j = 0; j < size_current_test; j++) {
            lines_map[i][j] = false;
        }
    }
}

void init_columns_map(void)
{
    register int i, j;
    columns_map = (bool **) malloc(size_current_test * sizeof(bool *));
    for (i = 0; i < size_current_test; i++) {
        columns_map[i] = (bool *) malloc(size_current_test * sizeof(bool));
        for (j = 0; j < size_current_test; j++) {
            columns_map[i][j] = false;
        }
    }
}

void init_restrictions_map(void)
{
    int i, j;
    int x1, y1, x2, y2;

    /*aloca as matrizes que representam restricoes nas mesmas linhas e nas mesmas colunas*/
    line_restriction = (int **) malloc(size_current_test * sizeof(int *));
    columns_restriction = (int **) malloc(size_current_test * sizeof(int *));

    for (i = 0; i < size_current_test; i++) {
        line_restriction[i] = (int *) malloc(size_current_test * sizeof(int));
        columns_restriction[i] = (int *) malloc(size_current_test * sizeof(int));

        for (j = 0; j < size_current_test; j++) {
            line_restriction[i][j] = 0;
            columns_restriction[i][j] = 0;
        }
    }

    for (i = 0; i < number_current_restrictions; i++) {
        fscanf(stream_input, "%d %d %d %d", &x1, &y1, &x2, &y2);
        //printf("%d %d %d %d\n", x1, y1, x2, y2);
        if (x1 == x2) { /* a restricao esta na mesma coluna */
            if (y1 > y2)    line_restriction[x1 - 1][y2 - 1] = 1;
            else            line_restriction[x1 - 1][y1 - 1] = -1;
        }

        else if (y1 == y2) { /* a restricao esta na mesma linha */
            if (x1 > x2)    columns_restriction[x2 - 1][y1 - 1] = 1;
            else            columns_restriction[x1 - 1][y1 - 1] = -1;
        }
    }
}

void free_data(void)
{
    register int i;
    for (i = 0; i < size_current_test; i++) {
        free(play_matrix[i]);
        free(lines_map[i]);
        free(columns_map[i]);
        free(line_restriction[i]);
        free(columns_restriction[i]);
    }
    free(play_matrix);
    free(lines_map);
    free(columns_map);
    free(line_restriction);
    free(columns_restriction);
}

void init_verif_ahead(void)
{
    register int i, j, k;
    verif_ahead = (int ***) malloc(size_current_test * sizeof(int **));
    for (i = 0; i < size_current_test; i++) {
        verif_ahead[i] = (int **) malloc(size_current_test * sizeof(int *));
        for (j = 0; j < size_current_test; j++) {
            verif_ahead[i][j] = (int *) malloc(size_current_test * sizeof(int));
            for (k = 0; k < size_current_test; k++){
                verif_ahead[i][j][k] = k+1;
            }
        }
    }
}

void init_mvr(void)
{
    register int i, j;
    mvr = (int **) malloc(size_current_test * sizeof(int *));
    for (i = 0; i < size_current_test; i++) {
        mvr[i] = (int *) malloc(size_current_test * sizeof(int));
        for (j = 0; j < size_current_test; j++) {
            mvr[i][j] = size_current_test;
        }
    }

    mvr_line_count = (int *) malloc(size_current_test * sizeof(int));
    for (i = 0; i < size_current_test; i++) {
        mvr_line_count[i] = size_current_test;
    }
}

int count_mvr(int i, int j){
    int count = 0;
    for (int k = 0; k < size_current_test; k++){
        if (verif_ahead[i][j][k] > 0) count++;
    }
    if (play_matrix[i][j] > 0) count--;
    return count;
}
