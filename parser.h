#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <cstdio>
#include <cstdlib>

/**
 * get_number_tests: retorna a quantidade de casos de teste
 * no arquivo de entrada.
 */
int get_number_tests(void);

/**
 * get_size_test: retorna o tamanho do caso de teste corrente
 * isto e, retorna a quantidade de linhas e colunas
 * do caso de teste.
 * retorna -1, caso todos os testes tenham sido percorridos
 */
int get_size_test(void);

/**
 * get_number_restriction: retorna o numero de restricoes
 * para o caso de teste corrente.
 *
 */
int get_number_restriction(void);

/**
 * alloc_play_matriz - aloca e inicializa o tabuleiro do jogo.
 *                     (o tabuleiro eh uma variavel global)
 */
void init_play_matrix(void);

/**
 * alloc_lines_map - aloca e inicializa o mapa que indica
 *                   que dado um numero, em quais linhas
 *                   aquele numero ja foi utilizado.
 */
void init_lines_map(void);

/**
 * alloc_columns_map - aloca e inicializa o mapa que indica
 *                   que dado um numero, em quais colunas
 *                   aquele numero ja foi utilizado.
 */
void init_columns_map(void);

/**
 * init_restrictions_map - aloca e inicializa os mapas de
 *                          restricoes do tabuleiro
 */
void init_restrictions_map(void);

/**
 * free_data - desaloca toda a memoria
 * alocada dinamicamente para um caso de teste.
 */
void free_data(void);

/**
 *  inicializa a matriz 3d verif_ahead usada na verficação adiante
 */
void init_verif_ahead(void);

/**
 *  inicializa a matriz mvr
 */
void init_mvr(void);

int count_mvr(int i, int j);

#endif // PARSER_H_INCLUDED
