#include <iostream>
#include "parser.h"

using namespace std;

extern int number_case_test;
extern int size_current_test;
extern uint8_t **play_matrix;

int main()
{
    get_number_tests();
    while (number_case_test--) {
        get_size_test();
        get_number_restriction();
        init_play_matrix();
        for (int i = 0; i < size_current_test; i++) {
            for (int j = 0; j < size_current_test; j++) {
                printf("%d \t", play_matrix[i][j]);
            }
            printf("\n");
        }
        getchar();
        init_restrictions_map();
        init_lines_map();
        init_columns_map();
        free_data();
    }
    return 0;
}
;
