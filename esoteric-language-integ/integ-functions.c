#include "integ-functions.h"


bool valid_args(const char *x, int16_t arg_x) {
    return x[0] != '\0' && arg_x == 0;
}

void get_arg(const char *program, char *x, int *i) {
    int depth = 1, cnt = *i + 1, j = 0;

    while (depth > 0) {
        cnt++;
        if (program[cnt] == '(') {
            depth++;
        } else if (program[cnt] == ')') {
            depth--;
        }
        if (depth > 0) {
            x[j] = program[cnt];
            j++;
        }
    }
    x[j] = '\0';
    *i = cnt;
}


int16_t compute_unary(char op, int16_t x, uint16_t *tape, uint16_t max_address, time_t start_time) {
    int max_num, min_num;
    char c;
    time_t curr_time = time(NULL);
    switch (op) {
        case '{':
            return x < 0 ? -1 : tape[x];

        case '_': // deallocates all addresses between maxAddress and x
            max_num = x > max_address ? x : max_address;
            min_num = x >= 0 && x < max_address ? x : max_address;
            for (int i = min_num; i <= max_num; ++i) {
                tape[i] = 0;
            }
            return x;

        case '@':
            return max_address;

        case ']': // character as x
            printf("%c", x);
            return x;

        case '[':
            c = fgetc(stdin);
            return c;

        default: // " time in seconds since the start of the epoch, rounded down
            return (int16_t) curr_time - start_time;
    }
}

int16_t compute_binary(char op, int16_t x, int16_t y, uint16_t *tape, uint16_t *max_address) {
    int max_num, min_num;
    switch (op) {
        case '}':
            if (x < 0) {
                return -1;
            }
            tape[x] = y;
            *max_address = x > *max_address ? x : *max_address;
            return y;

        case '+':
            return x + y;

        case '-':
            return x - y;

        case '*':
            return x * y;

        case '/':
            return x / y;

        case '%':
            return x % y;

        case '`':
            max_num = x > y ? x : y;
            min_num = x > y ? x : y;
            return rand() % (max_num - min_num + 1) + min_num;

        default: // <
            return x < y ? 0 : 1;
    }
}


bool is_unary(char c) {
    for (int i = 0; i < UNARY_OP_SIZE; ++i) {
        if (UNARY_OP[i] == c) {
            return true;
        }
    }
    return false;
}

bool is_binary(char c) {
    for (int i = 0; i < BINARY_OP_SIZE; ++i) {
        if (BINARY_OP[i] == c) {
            return true;
        }
    }
    return false;
}
