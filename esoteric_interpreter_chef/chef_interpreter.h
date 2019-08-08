//
// Created by Clara Lebbos on 2019-06-12.
//

#ifndef ARM11_10_CHEF_INTERPRETER_H
#define ARM11_10_CHEF_INTERPRETER_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define LINE_SIZE 511
#define NEXT_USEFUL_LINE 3
#define DRY 0
#define LIQUID 1
#define UNKNOWN -1


typedef struct Node {
    char *label;
    int value;
    int type; // 0 for dry, 1 for liquid, -1 for unknown
} Ingredient;

typedef struct HashMap {
    Ingredient *table;
    int size;
    int currSize;
} HashMap;

typedef struct SymbolNode {
	char *label;
	int fileLocation;
} SymbolNode;

typedef struct SymbolTable {
	SymbolNode *table;
	int size;
	int currSize;
} SymbolTable;

typedef struct IngredientNode {
	Ingredient *ingredient;
	struct IngredientNode *next;
	struct IngredientNode *prev;
} IngredientNode;

typedef struct MixingBowl {
	IngredientNode *first;
	IngredientNode *last;
} MixingBowl;

typedef struct MixingBowlNode {
	MixingBowl *bowl;
	struct MixingBowlNode *next;
	struct MixingBowlNode *prev;
} MixingBowlNode;

typedef struct MixingBowlCollection_s {
	MixingBowlNode *first;
	MixingBowlNode *last;
} MixingBowlArr;

typedef struct BakingTray_s {
	IngredientNode *first;
	IngredientNode *last;
} BakingTray;

typedef struct BakingTrayNode {
	BakingTray *tray;
	struct BakingTrayNode *next;
	struct BakingTrayNode *prev;
} BakingTrayNode;

typedef struct BakingTrayCollection_s {
	BakingTrayNode *first;
	BakingTrayNode *last;
} BakingTrayArr;

int toDigit(char *num);

void functionParser(struct HashMap *ingredients, char *line, MixingBowlArr *bowls, BakingTrayArr *trays);

void freeIngredientTable(struct HashMap *ingredients);

void ingredientInitialization(struct HashMap *ingredients, char *line);

Ingredient *filterIngredients(struct HashMap *ingredients, int type);

Ingredient *getIngredient(struct HashMap *ingredients, char *label);

void displayHashMap(struct HashMap *ingredients);

void insert_ingredient(struct HashMap *ingredients, int value, Ingredient *ingredient);


#endif //ARM11_10_CHEF_INTERPRETER_H
