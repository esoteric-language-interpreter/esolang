#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <errno.h>

#ifndef EXTENSION_INTEG_FUNCTIONS_H
#define EXTENSION_INTEG_FUNCTIONS_H

#define TAPE_SIZE 300
#define COMMAND_SIZE 50

#define UNARY_OP "{_@][\""
#define UNARY_OP_SIZE 5

#define BINARY_OP "}+-*/%`<"
#define BINARY_OP_SIZE 9


bool is_binary(char c);

bool is_unary(char c);

int16_t compute_binary(char op, int16_t x, int16_t y, uint16_t *tape, uint16_t *max_address);

int16_t compute_unary(char op, int16_t x, uint16_t *tape, uint16_t max_address, time_t start_time);

void get_arg(const char *program, char *x, int *i);

bool valid_args(const char *x, int16_t arg_x);


#endif //EXTENSION_INTEG_FUNCTIONS_H
