#include <diosolver/extended_euclidean.h>
#include <diosolver/interval.h>
#include <diosolver/diophantine.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void clear_screen(void) {
#if defined(_WIN32)
    system("cls");
#else
    system("clear");
#endif
}

bool ask_tf(const char *prompt) {
    char ch;
    do {
        printf("%s", prompt);
        scanf(" %c", &ch);
        ch = tolower(ch);
        if (ch != 't' && ch != 'f') {
            printf("Please enter 't' or 'f'!\n");
        }
    } while (ch != 't' && ch != 'f');
    return ch == 't';
}

Interval ask_intvl(char var) {
    int low, high;
    printf("\nPlease specify the domain of %c\n", var);
    printf("From: ");
    scanf("%d", &low);
    printf("To: ");
    scanf("%d", &high);
    bool left_open = ask_tf("Include left endpoint [t/f]: ");
    bool right_open = ask_tf("Include right endpoint [t/f]: ");

    Interval intvl = make_interval(low, high, left_open, right_open);
    if (is_valid_interval(intvl)) {
        return intvl;
    }

    char *intvl_str = interval_to_str(intvl);
    printf("\n%s is not a valid interval!\n", intvl_str);
    free(intvl_str);
    return ask_intvl(var);
}

Interval select_domain(char var) {
    printf("\nSelect the domain of %c:\n", var);
    printf("1. Real\n");
    printf("2. Positive\n");
    printf("3. Negative\n");
    printf("4. Nonpositive\n");
    printf("5. Nonnegative\n");
    printf("6. Custom\n");

    int choice;
    do {
        printf("Enter your choice [1-6]: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 6) {
            printf("Please enter a number from 1 to 6!\n");
        }
    } while (choice < 1 || choice > 6);

    switch (choice) {
    case 1:
        return REAL;
    case 2:
        return POS;
    case 3:
        return NEG;
    case 4:
        return NONPOS;
    case 5:
        return NONNEG;
    };
    return ask_intvl(var);
}

void solve_lde(int a, int b, int c, Interval xi, Interval yi) {
    calist *result = lde_result(make_lde_in(a, b, c, xi, yi));
    for (size_t i = 0; i < calist_size(result); ++i) {
        const char *line = (const char *) calist_get(result, i);
        printf("%s", line);
    }
    calist_destroy(result);
}


int main(void) {
    // --- Tests ---
    // clear_screen();
    // solve_lde(0, 0, 0, REAL, NONNEG);
    // solve_lde(0, 0, 10, REAL, REAL);
    // solve_lde(0, 5, 10, POS, POS);
    // solve_lde(0, -5, -10, POS, POS);
    // solve_lde(0, 4, 14, NONPOS, POS);
    // solve_lde(-5, 0, 10, NEG, NONPOS);
    // solve_lde(-5, 0, 10, make_interval(-10, -5, true, false), REAL);
    // solve_lde(-5, 0, -11, NEG, NONNEG);

    // solve_lde(1, -1, 0, POS, make_interval(0, 5, false, false));
    // solve_lde(-1, 5, 2, NEG, POS);
    // solve_lde(7, -1, -5, REAL, NONPOS);

    // solve_lde(9, 5, 137, POS, POS);
    // solve_lde(-9, 5, 137, POS, POS);
    // solve_lde(9, -5, 137, POS, POS);
    // solve_lde(-9, -5, 137, POS, POS);
    // solve_lde(9, 5, -137, POS, POS);
    // solve_lde(-9, 5, -137, POS, POS);
    // solve_lde(9, -5, -137, make_interval(-20, 30, true, true), POS);
    // solve_lde(9, -5, -137, make_interval(-20, 60, true, true), NONNEG);
    // solve_lde(-9, -5, -137, POS, POS);
    // solve_lde(10, 8, 100, POS, POS);
    // solve_lde(10, 8, 100, NONNEG, NONNEG);

    // solve_lde(10, 8, 99, POS, POS);
    // solve_lde(7, 21, 5, NEG, REAL);

    clear_screen();
    printf("Linear Diophantine Equation Solver\n");
    printf("Format: ax + by = c\n\n");

    int a, b, c;
    printf("Enter the value of a: ");    
    scanf("%d", &a);
    printf("Enter the value of b: ");    
    scanf("%d", &b);
    printf("Enter the value of c: ");    
    scanf("%d", &c);

    Interval xi = select_domain('x');
    Interval yi = select_domain('y');

    clear_screen();
    printf("Result:\n");
    solve_lde(a, b, c, xi, yi);

    return 0;
}
