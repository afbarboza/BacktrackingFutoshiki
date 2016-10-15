#ifndef FUTOSHIKI_H_INCLUDED
#define FUTOSHIKI_H_INCLUDED

<<<<<<< HEAD
#include <cstdio>
#include <cstdint>
#include <cstdlib>

#define INVALID_MATRIX false
#define VALID_MATRIX true

#define NO_RESTRICTION      0
#define GREAT_RESTRICTION   1
#define LESS_RESTRICTION    2

/*
    number_case_test - numero de casos de teste do jogo
 */
extern int number_case_test;

/*
    size_current_test - dimensao do caso de teste atual
*/
extern int size_current_test;

/*
   number_current_restrictions - numero de restricoes
   para o caso de teste atual
 */
extern int number_current_restrictions;

/*
    play_matrix - representa o tabuleiro do jogo
 */
extern uint8_t **play_matrix;

/*
    lines_map - dado um numero, mapeia em quais
                linhas aquele numero  ja foi usado.
 */
extern bool **lines_map;

/*
    row_map -   dado um numero, mapeia em quais
                colunas aquele numero ja foi usado.
 */
extern bool **columns_map;

/*
    line_restriction - representa as restricoes
                        de menor presentes nas linhas
*/
extern uint8_t **line_restriction;

/*
    columns_restriction - representa as restricoes
                            de menor presentes nas colunas
*/
extern uint8_t **columns_restriction;

bool exist_less_greater(uint8_t value, int line, int column);
=======
// number_case_test - numero de casos de teste do jogo
int number_case_test = 0;

// size_current_test - dimensao do caso de teste atual
int size_current_test = 0;

// number_current_restrictions - numero de restricoes para o caso de teste atual
int number_current_restrictions = 0;

// play_matrix - representa o tabuleiro do jogo
int **play_matrix   = NULL;

// lines_map - dado um numero, mapeia em quais linhas aquele numero ja foi usado.
bool **lines_map        = NULL;

// row_map - dado um numero, mapeia em quais colunas aquele numero ja foi usado.
bool **columns_map      = NULL;

// line_restriction - representa as restricoes de menor presentes nas linhas
int **line_restriction = NULL;

// columns_restriction - representa as restricoes de menor presentes nas colunas
int **columns_restriction = NULL;
>>>>>>> sol_sem_poda

#endif // FUTOSHIKI_H_INCLUDED
