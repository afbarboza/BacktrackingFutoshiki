#include "futoshiki.h"
#include <list>
#include <cstdio>
#include <climits>

/*dado uma linha e uma coluna, retorna o indice dessa celula no vetor de dominios */
#define GET_INDEX_DOMAIN(i, j)  ((i * size_current_test) + j)

extern int  number_case_test;
extern int  size_current_test;
extern int  number_current_restrictions;
extern int  **play_matrix;
extern bool **lines_map;
extern bool **columns_map;
extern int  **line_restriction;
extern int  **columns_restriction;

/* indice para variavel mais restrita */
int most_constrained_variable = 0;

/* tamanho do dominio da variavel mais restrita */
int size_most_constrained = size_current_test;

/**
 * struct var_domain - representa o dominio de cada celula.
 * @x: a linha da celula no tabuleiro
 * @y: a coluna da celula no tabuleiro
 * @domain_list: a lista de possiveis valores que a celula ainda pode assumir.
 *              se (domain_list.size() == 0) entao deve haver backtrack. (forward checking)
 */
struct var_domain {
    int             x;
    int             y;
    bool            have_assignment;
    std::list<int>  *domain_list;
};

using namespace std;

struct var_domain *set_domains = NULL;

static void init_valid_domains(int i, int j)
{
    if (play_matrix[i][j] != 0) { /** checa valores iniciais e imutaveis da celula - entrada */
        set_domains[GET_INDEX_DOMAIN(i, j)].have_assignment = true;
        set_domains[(GET_INDEX_DOMAIN(i, j))].domain_list = NULL;
    } else { /** atribui um dominio a cada celula*/
        set_domains[GET_INDEX_DOMAIN(i, j)].have_assignment = false;
        set_domains[(GET_INDEX_DOMAIN(i, j))].domain_list = new list< int >();
        for (int d = 1; d <= size_current_test; d++) {
                if (!lines_map[d-1][i] && !columns_map[d-1][j])
                    set_domains[GET_INDEX_DOMAIN(i, j)].domain_list->push_back(d);
        }
    }
}

void init_domain_variables(void)
{
    int n_cells = size_current_test * size_current_test;
    set_domains = (struct var_domain *) malloc(n_cells * sizeof(struct var_domain));

    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            set_domains[GET_INDEX_DOMAIN(i, j)].x = i;
            set_domains[GET_INDEX_DOMAIN(i, j)].y = j;
            init_valid_domains(i, j);
        }
    }
}

/**
* apply_heuristiv_mrv - determina qual eh a celula com
* o menor numero de valores possiveis de serem aplicados.
* Essa funcao faz uma poda no dominio de cada variavel.
*
* @next_i: ao final da funcao, contem a linha da celula que sera atribuida.
* @next_j: ao final da funcao, contem a coluna da celula que sera atribuida
*
* retorna: 1) o valor a ser atrbuido na celula play_matrix[*next_i][*next_j] ou
*          2) -1, caso nao haja nenhum valor a ser atribuido para aquela celula.
*           Se -1 for retornado, o algoritmo deve fazer backtracking. (MVR puro)
*           Se forward checking for aplicado, o algoritmo nao retornara nunca -1.
*/
bool apply_heuristic_mrv(int *x, int *y)
{
    int minimum_domain = INT_MAX;

    int next_i = -1;
    int next_j = -1;

    bool all_domains_empty = true;

    /* captura a celula com o menor numero de valores possiveis */
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            // trata variaveis que nao podem ser alteradas no decorrer do jogo
            if (set_domains[GET_INDEX_DOMAIN(i, j)].domain_list == NULL)
                continue;

            int current_size = set_domains[GET_INDEX_DOMAIN(i, j)].domain_list->size();
            bool is_list_empty = set_domains[GET_INDEX_DOMAIN(i, j)].domain_list->empty();
            bool have_assignment = set_domains[GET_INDEX_DOMAIN(i, j)].have_assignment;

            // senao, selecionamos ela como a celula que ira receber a proxima atribuicao
            if (current_size < minimum_domain && !is_list_empty && !have_assignment) {
                next_i = i;
                next_j = j;
                minimum_domain = current_size;
                all_domains_empty = false;
            }
        }
    }

    if (next_i != -1 || next_j != -1) {
        *x = next_i;
        *y = next_j;
    }
    return all_domains_empty;
}

/**
 * neste trecho de codigo ocorre a poda do dominio de cada variavel
 * ao atribuir @value_assigned para uma celula (i,j), devemos retirar das celulas (i,n)  ou (n, j)
 * a possibilidade de atribuir @value_assigned.
 */
void restrict_search_space(int value_assigned, int x, int y)
{
    set_domains[GET_INDEX_DOMAIN(x, y)].have_assignment = true;
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            if (set_domains[GET_INDEX_DOMAIN(i, j)].domain_list == NULL)
                continue;

            if (i == x || j == y)
                // celulas na mesma linha ou na mesma coluna nao poderao receber o valor atribuido para aquela celua - poda
                set_domains[GET_INDEX_DOMAIN(i, j)].domain_list->remove(value_assigned);
        }
    }
}

void restore_domain_mrv(int value, int line, int column)
{
    set_domains[GET_INDEX_DOMAIN(line, column)].have_assignment = false;
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            if (set_domains[GET_INDEX_DOMAIN(i, j)].domain_list == NULL)
                continue;

                if (i == line || j == column)
                    set_domains[GET_INDEX_DOMAIN(i, j)].domain_list->push_back(value);
        }
    }
}

void select_unassigned_variable(int *x, int *y)
{
    for (int i = 0; i < size_current_test; i++) {
        for (int j = 0; j < size_current_test; j++) {
            if (set_domains[GET_INDEX_DOMAIN(i, j)].domain_list == NULL)
                continue;
            if (!set_domains[GET_INDEX_DOMAIN(i, j)].have_assignment) {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

bool check_assigned_variable(int x, int y)
{
    return (set_domains[GET_INDEX_DOMAIN(x, y)].have_assignment);
}
