//
// Created by Arjun Banerjee on 2019-06-13.
//

#ifndef ARM11_10_CHEF_METHOD_FUNCTIONS_H
#define ARM11_10_CHEF_METHOD_FUNCTIONS_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chef_interpreter.h"


MixingBowlNode *createBowl(MixingBowlNode *prev);

BakingTrayNode *createTray(BakingTrayNode *prev);

MixingBowl *getBowl(MixingBowlArr *bowls, int n);

BakingTray *getTray(BakingTrayArr *trays, int p);

MixingBowl *getBowl(MixingBowlArr *bowls, int n);

void put(MixingBowlArr *bowls, int n, Ingredient *ingredient);

void putTray(BakingTrayArr *trays, int p, Ingredient *ingredient);

void freeIngredientNode(IngredientNode *ingredient);

void freeTray(BakingTray *tray);

void freeBowl(MixingBowl *bowl);

void freeMixingBowlArray(MixingBowlArr *bowls);

void freeBakingTrayArr(BakingTrayArr *trays);

void take(Ingredient *ingredient);

void fold(Ingredient *ingredient, int n, MixingBowlArr *bowls);

void add(Ingredient *ingredient, int n, MixingBowlArr *bowls);

void removeIngredient(Ingredient *ingredient, int n, MixingBowlArr *bowls);

void combine(Ingredient *ingredient, int n, MixingBowlArr *bowls);

void divide(Ingredient *ingredient, int n, MixingBowlArr *bowls);

void addDry(struct HashMap *ingredients, int n, MixingBowlArr *bowls);

void liquefy(Ingredient ingredient);

void liquefyBowl(int n, MixingBowlArr *bowls);

void stir(int numberOfMins, MixingBowlArr *bowls, int n);

void stirIngredient(Ingredient *ingredient, MixingBowlArr *bowls, int n);

void mix(MixingBowlArr *bowls, int n);

void clean(MixingBowlArr *bowls, int n);

void pour(MixingBowlArr *bowls, int nbowl, BakingTrayArr *trays, int ptray);

void serves(BakingTrayArr *trays, int n);

#endif //ARM11_10_CHEF_METHOD_FUNCTIONS_H
