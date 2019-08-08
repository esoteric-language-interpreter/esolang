//
// Created by Guljahon Zohidova on 2019-06-13.
//


#ifndef ARM11_10_STRUCTURE_FUNCTIONS_H
#define ARM11_10_STRUCTURE_FUNCTIONS_H

#include <zconf.h>

#define SPACE " "

typedef struct Node {
    char *var_name;
    int16_t value;
} Node;

typedef struct HashMap {
    struct Node *table;
    int size;
    int currSize;
} HashMap;

struct List {
    struct list_elem *header;
    struct list_elem *footer;
};

typedef struct list_elem {
    char *value;
    struct list_elem *next;
    struct list_elem *prev;
} list_elem;

// functions for manipulating a doubly linked list

void free_list(struct List *list);

void insert_elem(struct List *list, char *value);

list_elem *find_elem(struct List *list, char *value);

// functions for manipulating a hashmap

void free_hash_map(HashMap *hashMap);

void insert_node(struct HashMap *map, int16_t value, char *var_name);

int16_t get_value(HashMap *table, char *var_name);

int16_t parse_op(char *operand, HashMap *table);

int16_t macros(char *line);

char *retrieve_element(const char *delim);




#endif //ARM11_10_STRUCTURE_FUNCTIONS_H
