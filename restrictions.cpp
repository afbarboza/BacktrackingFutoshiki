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
                return false;
        }
    }
    return true;
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
bool exist_less_greater(int value, int x, int y)
{
            // Checa as restrições de maior ou menor
    bool restricted = false;
    if (x > 0){
        if ( (line_restriction[y][x-1] ==  1 && play_matrix[y][x-1] < value) ||
            (line_restriction[y][x-1] == -1 && play_matrix[y][x-1] > value) )
            {
                    //printf("Restricao L %d em (%d, %d)\n", line_restriction[y][x-1], x-1, y);
                restricted = true;
            }
    }

    if (y > 0){
        if ( (columns_restriction[y-1][x] ==  1 && play_matrix[y-1][x] < value) ||
            (columns_restriction[y-1][x] == -1 && play_matrix[y-1][x] > value) )
             {
                    //printf("Restricao C %d em (%d, %d)\n", columns_restriction[y-1][x], x, y-1);
                 restricted = true;
             }
    }
    return restricted;
}
