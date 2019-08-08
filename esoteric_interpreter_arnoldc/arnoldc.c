//
// Created by Guljahon Zohidova on 2019-06-12.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "structure_functions.h"

list_elem *string_parse(list_elem *curr_elem, struct List *list, HashMap *table);

void main_iteration(struct List *list, HashMap *table);

int main(int argc, char **argv) {

    FILE *file = fopen(argv[1], "r");


    if (file == NULL) {
        perror("could not open file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int var_count = 0;
    char *line = calloc(100, sizeof(char));
    char *line_cpy = calloc(100, sizeof(char));

    struct List *list = malloc(sizeof(struct List));
    list->header = malloc(sizeof(list_elem));
    list->footer = malloc(sizeof(list_elem));
    list->header->prev = NULL;
    list->footer->next = NULL;
    list->header->next = list->footer;
    list->footer->prev = list->header;

    while (fgets(line, 100, file) != NULL) {
        strcpy(line_cpy, line);

        strtok(line_cpy, SPACE);
        if (strcmp(line_cpy, "HEY") == 0) {
            var_count++;
        }

        strtok(line, "\n");

        if (strcmp("IT'S SHOWTIME", line) == 0) {
            continue;
        } else {
            insert_elem(list, line);
        }

    }

    fclose(file);
    HashMap *var_table = malloc(sizeof(struct HashMap));

    var_table->table = malloc(var_count * sizeof(struct Node));
    var_table->size = var_count;
    var_table->currSize = 0;


    free(line);
    free(line_cpy);
    main_iteration(list, var_table);
    free_hash_map(var_table);
    free_list(list);


    return EXIT_SUCCESS;

}

void main_iteration(struct List *list, HashMap *var_table) {
    list_elem *curr_elem = list->header->next;

    while (curr_elem != NULL && strcmp(curr_elem->value, "YOU HAVE BEEN TERMINATED") != 0) {
        curr_elem = string_parse(curr_elem, list, var_table);
    }

}


void print_stmnt(char *line, HashMap *table) {

    char *cpy = calloc(100, sizeof(char));
    strcpy(cpy, line);
    char *var = strtok(cpy, SPACE);
    while (strcmp(var, "HAND") != 0) {
        var = strtok(NULL, SPACE);
    }
    var = strtok(NULL, "\0");

    if (var[0] == '"') {
        strtok(++var, "\"");
        printf("%s\n", var);
    } else {
        int16_t val = get_value(table, var);
        if (val == INT16_MAX) {
            perror("print_stmnt could not find varname");
        }
        printf("%d\n", val);
    }

}


list_elem *declare_var(list_elem *curr, HashMap *table) {

    // initialising variable name

    char *line = calloc(100, sizeof(char));
    strcpy(line, curr->value);
    strtok(line, SPACE);
    char *var = retrieve_element("TREE");
    curr = curr->next;

    // initialising value
    char *next_line = calloc(100, sizeof(char));
    strcpy(next_line, curr->value);

    strtok(next_line, SPACE);
    char *value = retrieve_element("UP");

    int16_t var_val;
    if (value[0] == '@') {
        var_val = macros(++value);
    } else {
        var_val = strtol(value, NULL, 10);
    }

    // inserting variable name and value into table
    insert_node(table, var_val, var);
    free(next_line);
    return curr->next;
}


void operations(char *line, HashMap *table, int16_t *stack_val) {

    char *first_word, *second_word = NULL;

    char *cpy = calloc(100, sizeof(char));
    strcpy(cpy, line);

    first_word = strtok(cpy, SPACE);

    if (strcmp(first_word, "GET") == 0) {
        second_word = strtok(NULL, SPACE);
        if (strcmp(second_word, "UP") == 0) {
            *stack_val += parse_op(strtok(NULL, SPACE), table);
        } else {
            *stack_val -= parse_op(strtok(NULL, SPACE), table);
        }
    } else if (strcmp(first_word, "YOU'RE") == 0) {
        *stack_val *= parse_op(retrieve_element("FIRED"), table);
    } else if (strcmp(first_word, "HE") == 0) {
        *stack_val /= parse_op(retrieve_element("SPLIT"), table);
    } else if (strcmp(first_word, "YOU") == 0) {
        *stack_val = *stack_val == parse_op(retrieve_element("ME"), table);
    } else if (strcmp(first_word, "LET") == 0) {
        *stack_val = *stack_val > parse_op(retrieve_element("BENNET"), table);
    } else if (strcmp(first_word, "CONSIDER") == 0) {
        *stack_val = *stack_val || parse_op(retrieve_element("DIVORCE"), table);
    } else if (strcmp(first_word, "KNOCK") == 0) {
        *stack_val = *stack_val && parse_op(retrieve_element("KNOCK"), table);
    }

}

list_elem *var_assignment(list_elem *curr_elem, HashMap *table) {

    char *curr_line = calloc(100, sizeof(char));
    strcpy(curr_line, curr_elem->value);

    strtok(curr_line, SPACE);
    char *var = retrieve_element("CHOPPER");

    char *next_line = calloc(100, sizeof(char));
    curr_elem = curr_elem->next;
    strcpy(next_line, curr_elem->value);

    strtok(next_line, SPACE);
    char *first_operand = retrieve_element("INVITATION");
    curr_elem = curr_elem->next;

    int16_t *stack_val = malloc(sizeof(int16_t));
    *stack_val = parse_op(first_operand, table);

    while (strcmp(curr_elem->value, "YOU HAVE BEEN TERMINATED") != 0) {
        strcpy(next_line, curr_elem->value);
        char *first_word = strtok(next_line, SPACE);
        if (strcmp(first_word, "ENOUGH") == 0) {
            break;
        } else {
            operations(curr_elem->value, table, stack_val);
        }
        curr_elem = curr_elem->next;
    }


    insert_node(table, *stack_val, var);

    free(next_line);
    free(curr_line);
    free(stack_val);

    return curr_elem->next;

}

list_elem *condition(list_elem *curr_elem, HashMap *table) {

    char *curr_line = calloc(100, sizeof(char));
    strcpy(curr_line, curr_elem->value);

    char *var = strtok(curr_line, SPACE);
    while (strcmp(var, "PLEASE") != 0) {
        var = strtok(NULL, SPACE);
    }
    var = strtok(NULL, SPACE);


    if (parse_op(var, table) == 0) {
        while (strcmp(curr_elem->value, "YOU HAVE BEEN TERMINATED") == 0) {
            strcpy(curr_line, curr_elem->value);
            if (strcmp(strtok(curr_line, "\n"), "BULLSHIT") == 0) {
                return curr_elem->next;
            }
            if (strcmp(strtok(curr_line, SPACE), "YOU") == 0) {
                return curr_elem->next;
            }
            curr_elem = curr_elem->next;
        }
    }
    free(curr_line);
    return curr_elem->next;
}

void loop_iterate(list_elem *loop_header, struct List *list, HashMap *table) {

    list_elem *curr_elem = loop_header->next;
    while (strcmp(curr_elem->value, "CHILL") != 0) {
        curr_elem = string_parse(curr_elem, list, table);
    }
}

int16_t eval_condition(char *line, HashMap *table) {
    char *cpy = calloc(100, sizeof(char));
    strcpy(cpy, line);
    char *var = strtok(cpy, SPACE);
    while (strcmp(var, "AROUND") != 0) {
        var = strtok(NULL, SPACE);
    }
    var = strtok(NULL, SPACE);
    free(cpy);
    return parse_op(var, table);
}


list_elem *loop(list_elem *curr_elem, struct List *list, HashMap *table) {

    list_elem *loop_header = curr_elem;

    int16_t eval = eval_condition(loop_header->value, table);
    while (eval != 0) {
        loop_iterate(loop_header, list, table);
        eval = eval_condition(loop_header->value, table);
    }

    return find_elem(list, "CHILL");
}


list_elem *string_parse(list_elem *curr_elem, struct List *list, HashMap *table) {

    char *first_word, *second_word = NULL;
    char *line_cpy = calloc(100, sizeof(char));

    strcpy(line_cpy, curr_elem->value);
    first_word = strtok(line_cpy, SPACE);

    if (strcmp("TALK", first_word) == 0) {
        print_stmnt(curr_elem->value, table);
        return curr_elem->next;
    } else if (strcmp("HEY", first_word) == 0) {
        return declare_var(curr_elem, table);
    } else if (strcmp("GET", first_word) == 0) {
        second_word = strtok(NULL, SPACE);
        if (strcmp(second_word, "TO") == 0) {
            return var_assignment(curr_elem, table);
        }
    } else if (strcmp(first_word, "BECAUSE") == 0) {
        return condition(curr_elem, table);
    } else if (strcmp(first_word, "BULLSHIT") == 0) {
        while (strcmp(curr_elem->value, "YOU HAVE BEEN TERMINATED") == 0) {
            strcpy(line_cpy, curr_elem->value);
            if (strcmp(strtok(line_cpy, SPACE), "YOU") == 0) {
                return curr_elem->next;
            }
            curr_elem = curr_elem->next;
        }
        return curr_elem->next;
    } else if (strcmp(first_word, "STICK") == 0) {
        return loop(curr_elem, list, table);
    }


    free(line_cpy);
    return curr_elem->next;


}
