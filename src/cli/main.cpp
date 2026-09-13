#include <diosolver/diophantine.h>
#include <diosolver/extended_euclidean.h>
#include <diosolver/interval.h>
#include <iostream>
#include <string>

static void clear_screen() {
#if defined(_WIN32)
    std::system("cls");
#else
    std::system("clear");
#endif
}

static int read_int(const std::string &prompt) {
    for (;;) {
        int value;
        std::cout << prompt;
        std::cin >> value;

        if (!std::cin) {
            std::cout << "Please enter a valid integer!" << std::endl;
            std::cin.clear();
            std::cin.ignore();
        } else {
            return value;
        }
    }
}

static bool read_tf(const std::string &prompt) {
    for (;;) {
        char tf;
        std::cout << prompt;
        std::cin >> tf;

        if (!std::cin) {
            std::cout << "Please enter a valid character!" << std::endl;
            std::cin.clear();
            std::cin.ignore();
            continue;
        }

        tf = static_cast<char>(std::tolower(tf));
        if (tf == 't' || tf == 'f') {
            return tf == 't';
        }
        std::cout << "Please enter either 't' or 'f'!" << std::endl;
    }
}

static Interval ask_interval(const char var) {
    for (;;) {
        std::cout << "Please specify the domain of " << var << std::endl;
        const int low = read_int("From: ");
        const int high = read_int("To: ");
        const bool left_open = read_tf("Include left endpoint [t/f]: ");
        const bool right_open = read_tf("Include right endpoint [t/f]: ");

        const Interval intvl = make_interval(low, high, left_open, right_open);
        if (is_valid_interval(intvl)) {
            return intvl;
        }

        char *intvl_str = interval_to_str(intvl);
        std::cout << std::string{intvl_str} << " is not a valid interval!\n";
        free(intvl_str);
    }
}

static Interval select_domain(const char var) {
    for (;;) {
        printf("\nSelect the domain of %c:\n", var);
        printf("1. Real\n");
        printf("2. Positive\n");
        printf("3. Negative\n");
        printf("4. Nonpositive\n");
        printf("5. Nonnegative\n");
        printf("6. Custom\n");

        switch (read_int("Enter your choice [1-6]: ")) {
        case 1:
            return REAL_INTERVAL;
        case 2:
            return POSITIVE_INTERVAL;
        case 3:
            return NEGATIVE_INTERVAL;
        case 4:
            return NONPOSITIVE_INTERVAL;
        case 5:
            return NONNEGATIVE_INTERVAL;
        case 6:
            return ask_interval(var);
        default:
            printf("Please enter a number from 1 to 6!\n");
        }
    }
}

static void solve_lde(const int a, const int b, const int c, const Interval &xi,
                      const Interval &yi) {
    calist *result = lde_result(make_lde_in(a, b, c, xi, yi));
    for (size_t i = 0; i < calist_size(result); ++i) {
        const auto line = static_cast<const char *>(calist_get(result, i));
        printf("%s", line);
    }
    calist_destroy(result);
}

int main() {
    clear_screen();
    std::cout << "Linear Diophantine Equation Solver" << std::endl;
    std::cout << "Format: ax + by = c" << std::endl << std::endl;

    const int a = read_int("Enter the value of a: ");
    const int b = read_int("Enter the value of b: ");
    const int c = read_int("Enter the value of c: ");
    const Interval xi = select_domain('x');
    const Interval yi = select_domain('y');

    clear_screen();
    std::cout << "Result:" << std::endl;
    solve_lde(a, b, c, xi, yi);
}
