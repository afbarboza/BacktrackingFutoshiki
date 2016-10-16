#include "futoshiki.h"
#include <list>

/*dado uma linha e uma coluna, retorna o indice dessa celula no vetor de dominios */
#define GET_INDEX_DOMAIN(i, j)  ((i * size_current_test) + j)

extern int size_current_test;

std::vector< std::list<int> > domain_each_variable;


void init_domain_variables(void)
{
    for (int i = 0; i < size_current_test; i++) {
        domain_each_variable.push_back(std::list<int>());
    }

    for (int i = 0; i < size_current_test; i++) {
        for (int j = 1; j <= size_current_test; j++) {
            domain_each_variable[i].push_back(j);
        }
    }
}


int apply_heuristic_mrv(void)
{
    int index_most_restricted = 0;
    int size_most_restricted = domain_each_variable[0].size();

    for (int i = 1; i < size_current_test; i++) {
        if (domain_each_variable[i].size() < size_most_restricted) {
            size_most_restricted = domain_each_variable[i].size();
            index_most_restricted = i;
        }
    }
    return domain_each_variable[index_most_restricted].pop_back();
}
