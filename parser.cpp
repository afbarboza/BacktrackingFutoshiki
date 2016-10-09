#include "parser.h"
#include "futoshiki.h"

FILE *stream_input = NULL;


#define END_OF_TESTS -1

int get_number_tests(void)
{
    if (stream_input == NULL) {
        stream_input = (FILE *) fopen("futoshiki.dat", "r");
    }

    fscanf(stream_input, "%d", &number_case_test);
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

void init_play_matrix(void)
{
    register int i, j;
    play_matrix = (uint8_t **) malloc(size_current_test * sizeof(uint8_t *));

    for (i = 0; i < size_current_test; i++) {
        play_matrix[i] = (uint8_t *) malloc(size_current_test * sizeof(uint8_t));
        for (j = 0; j < size_current_test; j++) {
            int tmp;
            fscanf(stream_input, "%d", &tmp);
            play_matrix[i][j] = tmp;
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
            lines_map[i][j] = false;
        }
    }
}

void init_restrictions_map(void)
{
    int i, j;
    int x1, y1, x2, y2;

    /*aloca as matrizes que representam restricoes nas mesmas linhas e nas mesmas colunas*/
    line_restriction = (bool **) malloc(size_current_test * sizeof(bool *));
    columns_restriction = (bool **) malloc(size_current_test * sizeof(bool *));

    for (i = 0; i < size_current_test; i++) {
        line_restriction[i] = (bool *) malloc(size_current_test * sizeof(bool));
        columns_restriction[i] = (bool *) malloc(size_current_test * sizeof(bool));

        for (j = 0; j < size_current_test; j++) {
            line_restriction[i][j] = false;
            columns_restriction[i][j] = false;
        }
    }

    for (i = 0; i < number_current_restrictions; i++) {
        fscanf(stream_input, "%d %d %d %d", &x1, &y1, &x2, &y2);
        printf("%d %d %d %d\n", x1, y1, x2, y2);
        if (x1 == x2) { /* a restricao esta na mesma _linha_ */
            line_restriction[x1 - 1][y1 - 1] = true;
        } else if (y1 == y2) { /* a restricao esta na mesma _coluna_ */
            columns_restriction[x1 - 1][y1 - 1] = true;
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
