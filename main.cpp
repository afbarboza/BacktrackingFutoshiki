#include <iostream>
#include "futoshiki.h"
#include "parser.h"

using namespace std;

extern int      number_case_test;
extern int      size_current_test;
extern int      number_current_restrictions;
extern uint8_t  **play_matrix;
extern bool     **lines_map;
extern bool     **columns_map;
extern uint8_t  **line_restriction;
extern uint8_t  **columns_restriction;

int main(void)
{
    get_number_tests();
    while (number_case_test--) {
        get_size_test();
        get_number_restriction();
        init_play_matrix();
        for (int i = 0; i < size_current_test; i++) {
            for (int j = 0; j < size_current_test; j++) {
                if (play_matrix[i][j] == 0)
                    play_matrix[i][j] = 7;

                printf("%d \t", play_matrix[i][j]);
            }
            printf("\n");
        }

        //getchar();
        init_restrictions_map();
        init_lines_map();
        init_columns_map();

        int val, l, c;
        printf("restriction: <value> <line> <column>\n");
        scanf("%d %d %d", &val, &l, &c);
        if (exist_less_greater(val, l, c) == true)
            cout << "hey... you cannot put these value there.\n" << endl;

        free_data();
    }
    return 0;
}
;
