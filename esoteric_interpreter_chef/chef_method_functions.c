#include "chef_method_functions.h"

MixingBowlNode *createBowl(MixingBowlNode *prev) {
	MixingBowlNode *node = calloc(sizeof(MixingBowlNode), 1);
	assert(node != NULL);
	MixingBowl *bowl = calloc(sizeof(MixingBowl), 1);
	assert(bowl != NULL);
	node->bowl = bowl;
	node->prev = prev;
	node->next = NULL;
	return node;
}

BakingTrayNode *createTray(BakingTrayNode *prev) {
	BakingTrayNode *node = calloc(sizeof(BakingTrayNode), 1);
	assert(node != NULL);
	BakingTray *tray = calloc(sizeof(BakingTray), 1);
	assert(tray != NULL);
	node->tray = tray;
	node->prev = prev;
	node->next = NULL;
	return node;
}

MixingBowl *getBowl(MixingBowlArr *bowls, int n) {
	assert(bowls != NULL);
	MixingBowlNode *mixingBowlNode = bowls->first;
	if (mixingBowlNode == NULL) {
		mixingBowlNode = createBowl(NULL);
		bowls->first = mixingBowlNode;
		bowls->last = mixingBowlNode;
		return mixingBowlNode->bowl;
	}

	for (int i = 0; i < n - 1; i++) {
		if (mixingBowlNode->next == NULL) {
			mixingBowlNode = createBowl(mixingBowlNode);
			bowls->last = mixingBowlNode;
		} else {
			mixingBowlNode = mixingBowlNode->next;
		}
	}

	return mixingBowlNode->bowl;

}

BakingTray *getTray(BakingTrayArr *trays, int p) {
	assert(trays != NULL);
	BakingTrayNode *bakingTrayNode = trays->first;
	if (bakingTrayNode == NULL) {
		bakingTrayNode = createTray(NULL);
		trays->first = bakingTrayNode;
		trays->last = bakingTrayNode;
		return bakingTrayNode->tray;
	}

	for (int i = 0; i < p - 1; i++) {
		if (bakingTrayNode->next == NULL) {
			bakingTrayNode = createTray(bakingTrayNode);
			trays->last = bakingTrayNode;
		} else {
			bakingTrayNode = bakingTrayNode->next;
		}
	}

	return bakingTrayNode->tray;

}


void put(MixingBowlArr *bowls, int n, Ingredient *ingredient) {
	assert(bowls != NULL && ingredient != NULL && n > 0);
	IngredientNode *node = malloc(sizeof(IngredientNode));
	if (node == NULL) {
		perror("put_ingredient_mixing_bowl");
		exit(EXIT_FAILURE);
	}

	MixingBowl *bowl = getBowl(bowls, n);

	node->ingredient = ingredient;
	node->prev = NULL;

	IngredientNode *top = bowl->first;
	if (top == NULL) {
		bowl->first = node;
		bowl->last = node;
		return;
	}

	bowl->first = node;
	node->next = top;
	top->prev = node;
}

void putTray(BakingTrayArr *trays, int p, Ingredient *ingredient) {
	assert(trays != NULL && ingredient != NULL && p > 0);
	IngredientNode *node = malloc(sizeof(IngredientNode));
	if (node == NULL) {
		perror("put_ingredient_mixing_bowl");
		exit(EXIT_FAILURE);
	}

	BakingTray *tray = getTray(trays, p);

	node->ingredient = ingredient;
	node->prev = NULL;

	IngredientNode *top = tray->first;
	if (top == NULL) {
		tray->first = node;
		tray->last = node;
		return;
	}

	tray->first = node;
	node->next = top;
	top->prev = node;
}

void freeIngredientNode(IngredientNode *ingredient) {
	assert(ingredient != NULL);
	free(ingredient->ingredient);
	free(ingredient);
}

void freeTray(BakingTray *tray) {
	assert(tray != NULL);

	IngredientNode *ingredientNode = tray->first;
	IngredientNode *temp;

	while (ingredientNode != NULL) {
		temp = ingredientNode->next;
		freeIngredientNode(ingredientNode);
		ingredientNode = temp;
	}
	free(tray);
}

void freeBowl(MixingBowl *bowl) {
	assert(bowl != NULL);

	IngredientNode *ingredientNode = bowl->first;
	IngredientNode *temp;

	while (ingredientNode != NULL) {
		temp = ingredientNode->next;
		freeIngredientNode(ingredientNode);
		ingredientNode = temp;
	}
	free(bowl);
}

void freeMixingBowlArray(MixingBowlArr *bowls) {
	assert(bowls != NULL);

	MixingBowlNode *bowlNode = bowls->first;

	if (bowlNode == NULL) {
		return;
	}

	MixingBowlNode *temp;
	while (bowlNode != NULL) {
		temp = bowlNode->next;
		freeBowl(bowlNode->bowl);
		free(bowlNode);
		bowlNode = temp;
	}

	free(bowls);
}

void freeBakingTrayArr(BakingTrayArr *trays) {
	assert(trays != NULL);

	BakingTrayNode *trayNode = trays->first;

	if (trayNode == NULL) {
		return;
	}

	BakingTrayNode *temp;
	while (trayNode != NULL) {
		temp = trayNode->next;
		freeTray(trayNode->tray);
		free(trayNode);
		trayNode = temp;
	}

	free(trays);
}


void take(Ingredient *ingredient) {
	scanf("%d", &ingredient->value);
}

void fold(Ingredient *ingredient, int n, MixingBowlArr *bowls) {
	assert(ingredient != NULL && n > 0 && bowls != NULL);

	MixingBowl *bowl = getBowl(bowls, n);

	IngredientNode *popped = bowl->first;
	if (popped == NULL) {
		ingredient->value = 0;
		return;
	}
	bowl->first = popped->next;
	bowl->first->prev = NULL;

	ingredient->value = popped->ingredient->value;

	freeIngredientNode(popped);
}

void add(Ingredient *ingredient, int n, MixingBowlArr *bowls) {
	assert(ingredient != NULL && bowls != NULL);

	MixingBowl *bowl = getBowl(bowls, n);
	Ingredient *top = bowl->first->ingredient;
	top->value += ingredient->value;
}

void removeIngredient(Ingredient *ingredient, int n, MixingBowlArr *bowls) {
	assert(ingredient != NULL && bowls != NULL);

	MixingBowl *bowl = getBowl(bowls, n);
	Ingredient *top = bowl->first->ingredient;
	top->value -= ingredient->value;
}

void combine(Ingredient *ingredient, int n, MixingBowlArr *bowls) {
	assert(ingredient != NULL && bowls != NULL);

	MixingBowl *bowl = getBowl(bowls, n);
	Ingredient *top = bowl->first->ingredient;
	top->value *= ingredient->value;
}

void divide(Ingredient *ingredient, int n, MixingBowlArr *bowls) {
	assert(ingredient != NULL && bowls != NULL);

	MixingBowl *bowl = getBowl(bowls, n);
	Ingredient *top = bowl->first->ingredient;
	top->value /= ingredient->value;
}

void addDry(struct HashMap *ingredients, int n, MixingBowlArr *bowls) {
	assert(ingredients != NULL && n > 0 && bowls != NULL);

	Ingredient *dry = filterIngredients(ingredients, DRY);

	int size = sizeof(dry) / sizeof(Ingredient);
	int total = 0;
	for (int i = 0; i < size; i++) {
		total += dry[i].value;
	}

	Ingredient *dryIngredient;
	dryIngredient = malloc(sizeof(Ingredient));
	assert(dryIngredient != NULL);

	dryIngredient->type = DRY;
	dryIngredient->value = total;
	dryIngredient->label = "dry ingredients";

	put(bowls, n, dryIngredient);
}

void liquefy(Ingredient ingredient) {
	ingredient.type = LIQUID;
}

void liquefyBowl(int n, MixingBowlArr *bowls) {
	assert(n > 0 && bowls != NULL);
	MixingBowl *bowl = getBowl(bowls, n);
	IngredientNode *ingredientNode = bowl->first;

	while (ingredientNode != NULL) {
		liquefy(*ingredientNode->ingredient);
		ingredientNode = ingredientNode->next;
	}
}

void stir(int numberOfMins, MixingBowlArr *bowls, int n) {
	assert(bowls != NULL && n > 0);

	MixingBowl *bowl = getBowl(bowls, n);
	IngredientNode *ingredient = bowl->first;

	if (numberOfMins > 1) {
		bowl->first = ingredient->next;
	}

	IngredientNode *temp;
	for (int i = 0; i < numberOfMins; i++) {
		if (ingredient->next == NULL) {
			bowl->last = ingredient;
			return;
		}
		temp = ingredient->next;
		temp->prev = ingredient->prev;
		ingredient->prev = temp;
		ingredient->next = temp->next;
		temp->next = ingredient;
	}
}

void stirIngredient(Ingredient *ingredient, MixingBowlArr *bowls, int n) {
	assert(bowls != NULL && ingredient != NULL && n > 0);

	stir(ingredient->value, bowls, n);
}

void mix(MixingBowlArr *bowls, int n) {
	assert(bowls != NULL && n > 0);

	MixingBowl *bowl = getBowl(bowls, n);

	int ingredientCount = 0;
	IngredientNode *ingredient = bowl->first;
	while (ingredient != NULL) {
		ingredient = ingredient->next;
		ingredientCount++;
	}

	int random = rand() % ingredientCount;
	int nTimes = rand() % 10;

	for (int i = 0; i < nTimes; i++) {
		stir(random, bowls, n);
	}
}

void clean(MixingBowlArr *bowls, int n) {
	assert(bowls != NULL && n > 0);

	MixingBowl *bowl = getBowl(bowls, n);

	IngredientNode *ingredientNode = bowl->first;

	IngredientNode *temp;
	while (ingredientNode != NULL) {
		temp = ingredientNode->next;
		freeIngredientNode(ingredientNode);
		ingredientNode = temp;
	}

	bowl->first = NULL;
	bowl->last = NULL;
}

void pour(MixingBowlArr *bowls, int nbowl, BakingTrayArr *trays, int ptray) {
	assert(bowls != NULL && trays != NULL && (nbowl + ptray > 0));

	MixingBowl *bowl = getBowl(bowls, nbowl);

	IngredientNode *ingredientNode = bowl->last;
	while (ingredientNode != NULL) {
		putTray(trays, ptray, ingredientNode->ingredient);
		ingredientNode = ingredientNode->prev;
	}
}

void printTray(BakingTray *tray) {
	assert(tray != NULL);

	IngredientNode *node = tray->first;
	while (node != NULL) {
		printf("%c", node->ingredient->value);
		node = node->next;
	}
}

void serves(BakingTrayArr *trays, int n) {
	assert(trays != NULL && n > 0);

	BakingTrayNode *tray = trays->first;
	for (int i = 0; i < n; ++i) {
		printTray(tray->tray);
		tray = tray->next;
	}
}




