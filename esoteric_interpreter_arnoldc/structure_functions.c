//
// Created by Guljahon Zohidova on 2019-06-13.
//

#include "structure_functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void free_list(struct List *list) {
    list_elem *tmp;
    list_elem *curr_elem = list->header->next;
    while (curr_elem != list->footer) {
        tmp = curr_elem;
        curr_elem = curr_elem->next;
        free(tmp);
    }
    free(list->header);
    free(list->footer);
    free(list);
}

void insert_elem(struct List *list, char *value) {

    list_elem *new_elem = malloc(sizeof(struct list_elem));
    new_elem->value = calloc(strlen(value) + 1, sizeof(char));
    strcpy(new_elem->value, value);

    new_elem->next = list->footer;
    new_elem->prev = list->footer->prev;

    list->footer->prev->next = new_elem;
    list->footer->prev = new_elem;
}

list_elem *find_elem(struct List *list, char *value) {
    list_elem *curr_elem = list->header->next;

    while (curr_elem != NULL && strcmp(curr_elem->value, value) != 0) {
        curr_elem = curr_elem->next;
    }
    return curr_elem;
}

void free_hash_map(HashMap *hashMap) {
    for (int i = 0; i < hashMap->currSize; i++) {
        free(hashMap->table[i].var_name);
    }
    free(hashMap->table);
    free(hashMap);
}

void insert_node(struct HashMap *map, int16_t value, char *var_name) {
    for (int i = 0; i < map->currSize; i++) {
        if (strcmp(map->table[i].var_name, var_name) == 0) {
            map->table[i].value = value;
            return;
        }
    }

    if (map->currSize == map->size) {
        perror("HASHMAP is full");
        exit(EXIT_FAILURE);
    }

    Node *newNode = malloc(sizeof(Node));

    if (!newNode) {
        perror("could not create new node");
        exit(EXIT_FAILURE);
    }

    newNode->var_name = calloc(15, sizeof(char));

    if (!newNode->var_name) {
        perror("could not allocate var_name");
        exit(EXIT_FAILURE);
    }

    strcpy(newNode->var_name, var_name);
    newNode->value = value;

    map->table[map->currSize] = *newNode;
    map->currSize++;

}

int16_t get_value(HashMap *table, char *var_name) {
    for (int i = 0; i < table->currSize; i++) {
        if (strcmp(table->table[i].var_name, var_name) == 0) {
            return table->table[i].value;
        }
    }
    return INT16_MAX;
}

int16_t macros(char *line) {
    if (strcmp(line, "I") == 0) {
        return 0;
    } else {
        return 1;
    }
}

int16_t parse_op(char *operand, HashMap *table) {

    int16_t val;

    val = strtol(operand, NULL, 10);

    if (val == 0) {
        if (operand[0] == '@') {
            val = macros(++operand);
        } else {
            val = get_value(table, operand);

        }
    }
    return val;
}

char *retrieve_element(const char *delim) {
    char *next_word = strtok(NULL, SPACE);
    while (strcmp(next_word, delim) != 0) {
        next_word = strtok(NULL, SPACE);
    }
    next_word = strtok(NULL, SPACE);
    return next_word;
}

