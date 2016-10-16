#ifndef FUTOSHIKI_H_INCLUDED
#define FUTOSHIKI_H_INCLUDED

#include <cstdio>
#include <cstdint>
#include <cstdlib>

#define INVALID_MATRIX false
#define VALID_MATRIX true

#define NO_RESTRICTION      0
#define GREAT_RESTRICTION   1
#define LESS_RESTRICTION    2
// number_case_test - numero de casos de teste do jogo
extern int number_case_test;

// size_current_test - dimensao do caso de teste atual
extern int size_current_test;

// number_current_restrictions - numero de restricoes para o caso de teste atual
extern int number_current_restrictions;

// play_matrix - representa o tabuleiro do jogo
extern int **play_matrix;

// lines_map - dado um numero, mapeia em quais linhas aquele numero ja foi usado.
extern bool **lines_map;

// row_map - dado um numero, mapeia em quais colunas aquele numero ja foi usado.
extern bool **columns_map;

// line_restriction - representa as restricoes de menor presentes nas linhas
extern int **line_restriction;

// columns_restriction - representa as restricoes de menor presentes nas colunas
extern int **columns_restriction;

bool exist_less_greater(int value, int line, int column);

bool validate_play_matrix(void);

/**
* init_domain_variables - inicializa o dominio de cada variavel
* A cada nivel de recursao do backtracking, esses dominios vao diminuindo
* e heuristicas vao sendo aplicados sobre esse dominio.
*/
void init_domain_variables(void);

int apply_heuristic_mrv(int *next_i, int *next_j);

void restore_domain_mrv(int value, int line, int column);

#endif // FUTOSHIKI_H_INCLUDED
