#include "futoshiki.h"

extern int  number_case_test;
extern int  size_current_test;
extern int  number_current_restrictions;
extern int  **play_matrix;
extern bool **lines_map;
extern bool **columns_map;
extern int  **line_restriction;
extern int  **columns_restriction;

/**
* look_repetition_line - checa se ha numeros repetidos
*                       na mesma linha.
* @line: a linha a ser varrida
* @number: o numero que se busca saber se ha repeticoes
*/
static bool look_repetition_line(int line, int number)
{
    int n_ocurrences = 0;
    for (int column_counter = 0; column_counter < size_current_test; column_counter++) {
        if (play_matrix[line][column_counter] == number)
            n_ocurrences++;
    }

    if (n_ocurrences > 1)
        return true;
    return false;
}

/**
* look_repetition_column - checa se ha numeros repetidos
*                       na mesma coluna.
* @column: a coluna a ser varrida
* @number: o numero que se busca saber se ha repeticoes
*/
static bool look_repetition_column(int column, int number)
{
    int n_ocurrences = 0;
    for (int line_counter = 0; line_counter < size_current_test; line_counter++) {
        if (play_matrix[line_counter][column] == number)
            n_ocurrences++;
    }

    if (n_ocurrences > 1)
        return true;
    return false;
}

/**
* validate_play_matrix - checha se as restricoes de mesma linhas
*                       e mesma coluna sao obedecidas.
*
*/
bool validate_play_matrix(void)
{
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            bool repeated_same_line = look_repetition_line(i, play_matrix[i][j]);
            bool repeated_same_column = look_repetition_column(j, play_matrix[i][j]);
            if (repeated_same_column || repeated_same_line)
                return INVALID_MATRIX;
        }
    }
    return VALID_MATRIX;
}


static bool check_inequality_lines(uint8_t value, int line, int column)
{
    /*checking for restrictions in line */
    if (line_restriction[line][column-1] != NO_RESTRICTION) {
        if (line_restriction[line][column-1] == GREAT_RESTRICTION && value > line_restriction[line][column-1])
            return true;  /*value should be greater than the right cell  - restriction found */
        else if (line_restriction[line][column-1] == LESS_RESTRICTION && value < line_restriction[line][column-1])
            return true; /*value should be greater than the left cell  - restriction found */
    }
    return false;
}

static bool check_inequality_columns(uint8_t value, int line, int column)
{
    /*checking for restrictions in columns*/
    if (columns_restriction[line-1][column] != NO_RESTRICTION) {
        if (columns_restriction[line-1][column] == GREAT_RESTRICTION && value > columns_restriction[line - 1][column])
            return true; /*value should be greater than the cell below - restriction found */
        else if (columns_restriction[line -1][column] == LESS_RESTRICTION && value < columns_restriction[line - 1][column])
            return true; /*value should be smaller than the cell below - restriction found */
    }
    return false;
}

/**
* exist_less_greater - checa se um valor @value pode ser colocado
*   em uma dada linha @line e coluna @column sem violar as condicoes
*   de maior/menor do tabuleiro.
*/
bool exist_less_greater(uint8_t value, int line, int column)
{
    bool inequality_line = false;
    bool inequality_column = false;

    /*valores na linha zero e coluna zero nao tem restricoes*/
    if (line == 0 && column == 0)
        return false;

    /*cannot check for backward index lesser than 0*/
    if (line == 0) {
        return check_inequality_lines(value, line, column);
    }

    if (column == 0) {
        return check_inequality_columns(value, line, column);
    }
    inequality_column = check_inequality_lines(value, line, column);
    inequality_line = check_inequality_columns(value, line, column);

    return (inequality_line || inequality_column);
}
