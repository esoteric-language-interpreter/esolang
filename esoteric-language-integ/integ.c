#include "integ-functions.h"

// Assume all operations are well written

void read_file(FILE *infile, char *program);

int16_t run(char *program, uint16_t *tape, uint16_t *max_address, time_t start_time);

int main(int argc, char **argv) {
    assert(argc == 2);
    FILE *infile = fopen(argv[1], "r");
    if (!infile) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    time_t start_time = time(NULL);
    uint16_t *tape = calloc(TAPE_SIZE, sizeof(uint16_t));
    if (!tape) {
        perror("Could not initialise tape");
        exit(EXIT_FAILURE);
    }
    uint16_t max_address = 0;

    // Finding file size
    fseek(infile, 0, SEEK_END); // seek to end of file
    int fileSize = ftell(infile) / sizeof(char); // get current file pointer
    fseek(infile, 0, SEEK_SET); // seek back to beginning of file

    char *program = calloc(fileSize, sizeof(char));
    read_file(infile, program);

    fclose(infile);
    run(program, tape, &max_address, start_time);

    free(tape);
    free(program);
    return EXIT_SUCCESS;
}

void read_file(FILE *infile, char *program) {
    char c = fgetc(infile);
    int i = 0;
    while (c != EOF) {
        if (c != ' ' && c != '\n' && c != '\t') {
            program[i] = c;
            i++;
        }
        c = fgetc(infile);
    }
    program[i] = '\0';
}


int16_t run(char *program, uint16_t *tape, uint16_t *max_address, time_t start_time) {
    int i;
    char opcode;
    char x[COMMAND_SIZE],
            y[COMMAND_SIZE],
            z[COMMAND_SIZE],
            initial_x[COMMAND_SIZE], *rest;

    int16_t arg_x, arg_y, res = 0;

    for (i = 0; program[i] != '\0'; ++i) {
        errno = 0;
        opcode = program[i];

        // reading the first operand: x
        get_arg(program, x, &i);
        strcpy(initial_x, x);
        arg_x = (int) strtol(x, &rest, 10);

        // checking if the the operand is a nested operator
        if (valid_args(x, arg_x) && errno != 0) {
            arg_x = run(x, tape, max_address, start_time);
        }
        errno = 0;

        // conditional check
        if (opcode == '?') {
            // reading the second and third operands: y and z
            get_arg(program, y, &i);
            get_arg(program, z, &i);

            if (arg_x == 0) {
                res = run(y, tape, max_address, start_time);
            } else {
                res = run(z, tape, max_address, start_time);
            }

            // loop check
        } else if (opcode == '~') {
            // reading the second operand: y - the contents of the loop
            get_arg(program, y, &i);
            arg_y = (int) strtol(y, &rest, 10);

            while (arg_x == 0) {
                // if the second operand can be evaluated, them run
                // else return 0
                if (valid_args(y, arg_y) && errno != 0) {
                    arg_y = run(y, tape, max_address, start_time);
                } else {
                    return 0;
                }

                errno = 0;
                // calculating arg_x again to check the condition of the loop
                arg_x = (int) strtol(initial_x, &rest, 10);
                if (valid_args(initial_x, arg_x) && errno != 0) {
                    arg_x = run(x, tape, max_address, start_time);
                }

            }
            return arg_y;

            // unary check
        } else if (is_unary(opcode)) {
            // computing the result for a unary operation
            res = compute_unary(opcode, arg_x, tape, *max_address, start_time);
            if (res == -1) {
                return false;
            }

            // binary check
        } else if (is_binary(opcode)) {

            // reading the second operand: y
            get_arg(program, y, &i);
            arg_y = (int) strtol(y, &rest, 10);

            if (valid_args(y, arg_y) && errno != 0) {
                arg_y = run(y, tape, max_address, start_time);
            }

            // computing the result for a binary operation
            res = compute_binary(opcode, arg_x, arg_y, tape, max_address);
            if (res == -1) {
                return -1;
            }

            // invalid character
        } else {
            perror("invalid character");
            exit(EXIT_FAILURE);
        }
    }
    return res;
}
