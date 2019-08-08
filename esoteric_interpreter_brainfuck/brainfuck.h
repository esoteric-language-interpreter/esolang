//
// Created by Arjun Banerjee on 2019-06-12.
//

#ifndef ARM11_10_BRAINFUCK_H
#define ARM11_10_BRAINFUCK_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <assert.h>

#define MEMORY_SIZE 100000

void brainfuck(uint8_t *instructions, int size, uint8_t *data_ptr);
#endif //ARM11_10_BRAINFUCK_H
