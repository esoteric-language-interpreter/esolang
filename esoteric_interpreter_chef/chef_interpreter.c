#include "chef_interpreter.h"
#include "chef_method_functions.h"

void insert_ingredient(struct HashMap *ingredients, int value, Ingredient *ingredient) {
    int numOfIngredients = 0;
    assert(ingredients->currSize < ingredients->size);
    assert(ingredient->label != NULL);
    ingredients->table[ingredients->currSize] = *ingredient;
    ingredients->currSize++;
    numOfIngredients = ingredients->currSize;
}

void displayHashMap(struct HashMap *ingredients) {
    for (int i = 0; i < ingredients->currSize; i++) {
        printf("label:%s, address: %d, type: %d\n",
               ingredients->table[i].label,
               ingredients->table[i].value,
               ingredients->table[i].type);
    }
}

// if label not in map, returns 1
Ingredient* getIngredient(struct HashMap *ingredients, char *label) {
    for (int i = 0; i < ingredients->currSize; i++) {
        if (strncmp(ingredients->table[i].label, label, strlen(label)) == 0) {
            return &ingredients->table[i];
        }
    }
    return NULL;
}



Ingredient *filterIngredients(struct HashMap *ingredients, int type) {
    Ingredient *listOfIngredients = malloc(ingredients->currSize * sizeof(Ingredient));
    int currIndex = 0;
    for (int i = 0; i < ingredients->currSize; i++) {
        if (ingredients->table[i].type == type) {
            listOfIngredients[currIndex] = ingredients->table[i];
            currIndex++;
        }
    }
    return listOfIngredients;
}

void ingredientInitialization(struct HashMap *ingredients, char *line) {
    if (strncmp(line, "Ingredients.", 12)) {

        Ingredient *ingredient = malloc(sizeof(Ingredient));
        ingredient->value = (atoi(strtok(line, " "))); // get value of ingredient
        char *potential_type = strtok(NULL, " ");
        ingredient->label = malloc(sizeof(char) * 50);

        if ((strncmp(potential_type, "g", 1) && strncmp(potential_type, "kg", 2) &&
             strncmp(potential_type, "pinch", 5) &&
             strncmp(potential_type, "pinches", 7)) == 0) {

            ingredient->type = DRY;
            char *label = strtok(NULL, "\n");
            strcat(label, " ");
            strcpy(ingredient->label, label);

        } else if ((strcmp(potential_type, "ml") && strcmp(potential_type, "l") && strcmp(potential_type, "dash") &&
                    strcmp(potential_type, "dashes")) == 0) {
            ingredient->type = LIQUID;
            char *label = strtok(NULL, "\n");
            strcat(label, " ");
            strcpy(ingredient->label, label);

        } else {
            ingredient->type = UNKNOWN;
            strcpy(ingredient->label, potential_type);
            strcat(ingredient->label, " ");
            char *stringAfterLabel = strtok(NULL, "\0");
            if (stringAfterLabel != NULL) {
                strcat(ingredient->label, stringAfterLabel);
                strcat(ingredient->label, " ");
            }
            ingredient->label = strtok(ingredient->label, "\n");
            strcat(ingredient->label, " ");
        }

        insert_ingredient(ingredients, ingredient->value, ingredient);
    }

}

void freeIngredientTable(struct HashMap *ingredients) {
    for (int i = 0; i < ingredients->currSize; i++) {
        free(ingredients->table[i].label);
    }
}


int main(int argc, char **argv) {

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("could not open file");
        exit(EXIT_FAILURE);
    }

    char line[128];



    // Initialising ingredients table
    struct HashMap *ingredients;
    ingredients = malloc(sizeof(HashMap));
	assert(ingredients != NULL);
    ingredients->table = malloc(100 * sizeof(struct Node));
    ingredients->size = 100;
    ingredients->currSize = 0;
    assert(ingredients->table != NULL);

	// Initialising symbol table
	SymbolTable *symbolTable;
	symbolTable = malloc(sizeof(symbolTable));
	assert(symbolTable != NULL);
	symbolTable->table = malloc(20 * sizeof(SymbolNode));
	symbolTable->size = 20;
	symbolTable->currSize = 0;
	assert(symbolTable->table != NULL);

	// Initialising mixing bowl array
	MixingBowlArr *bowls;
	bowls = calloc(1, sizeof(MixingBowlArr));
	assert(bowls != NULL);

	// Initialising tray array
	BakingTrayArr *trays;
	trays = calloc(1, sizeof(BakingTrayArr));
	assert(trays != NULL);

    while (fgets(line, sizeof(line), file) != NULL) {
        char stringCopy[strlen(line)];
        strcpy(stringCopy, line);
        //Keep original line
        char *function = strtok(stringCopy, " ");

        if (strncmp(line, "Ingredients.", 12) == 0) {
            while (strcmp(fgets(line, sizeof(line), file), "\n") != 0) {
                ingredientInitialization(ingredients, line);
                // go to ingredients to initialize values
            }
        } else if (strcmp(function, "Refrigerate") == 0){
            strtok(stringCopy, "for");
            char *numOfHours = strtok(NULL, "hours");
            if (numOfHours != NULL){
                serves(trays, toDigit(numOfHours));
            } else {
                break;
            }
        }

        functionParser(ingredients, line, bowls, trays);
    }
    freeIngredientTable(ingredients);

}

char *returnWordsBefore(char *string, char *target) {
	assert(string != NULL && target != NULL);
	if (strstr(string, target) == NULL) {
		return NULL;
	}

	char *word;
	word = malloc(sizeof(char) * 100);
	strncpy(word, string, strlen(string));
	char *prev;
	prev = calloc(sizeof(char), 100);
	word = strtok(word, " ");
    strcat(prev, word);
    strcat(prev, " ");
    while (word != NULL) {
		word = strtok(NULL, " ");
		if (strcmp(word, target) == 0) {
			return prev;
		}
		strcat(prev, word);
		strcat(prev, " ");
	}
	return prev;
}

char *returnWordBeforeTarget(char *string, char *target) {
	assert(string != NULL && target != NULL);
	if (strstr(string, target) == NULL) {
		return NULL;
	}

	char *word;
	word = malloc(sizeof(char) * 100);
	strncpy(word, string, strlen(string));
	char *prev;
	prev = calloc(sizeof(char), 100);
	word = strtok(word, " ");
    while (word != NULL) {
		word = strtok(NULL, " ");
		if (strcmp(word, target) == 0) {
			return prev;
		}
		strcpy(prev, word);
	}
	return prev;
}

void functionParser(struct HashMap *ingredients, char *line, MixingBowlArr *bowls, BakingTrayArr *trays) {
	if (strncmp(line, "\n", 1) == 0 || strncmp(line, "Method.", 7) == 0) {
        return;
    }
    char copy[strlen(line)];
    strcpy(copy, line);

    char *function = strtok(line, " ");
    char *stringAfterFunctionName = strtok(NULL, "\0");
    char *ingredientName = returnWordsBefore(stringAfterFunctionName, "into");
    if (ingredientName == NULL){
        ingredientName = strtok(stringAfterFunctionName, " ");
    }
    char *bowlNumber = returnWordBeforeTarget(stringAfterFunctionName, "mixing");
    Ingredient *ingredient;
    if (ingredientName != NULL){
        ingredient = getIngredient(ingredients, ingredientName);
    }


    if (strncmp(function, "Take", 4) == 0) {
        take(ingredient);

    } else if (strncmp(function, "Put", 3) == 0) {
        put(bowls, toDigit(bowlNumber), ingredient);

    } else if (strncmp(function, "Fold", 4) == 0) {
        fold(ingredient, toDigit(bowlNumber), bowls);

    } else if (strncmp(function, "Add", 3) == 0) {
        add(ingredient, toDigit(bowlNumber), bowls);

    } else if (strncmp(function, "Remove", 6) == 0) {
        removeIngredient(ingredient, toDigit(bowlNumber), bowls);

    } else if (strncmp(function, "Combine", 7) == 0) {
        combine(ingredient, toDigit(bowlNumber), bowls);

    } else if (strncmp(function, "Divide", 6) == 0) {
        divide(ingredient, toDigit(bowlNumber), bowls);

    } else if (strncmp(function, "Add", 3) == 0) {
        if (strncmp(strtok(NULL, " "), "dry", 3) == 0) {
            char *bowlNum = returnWordBeforeTarget(copy, "mixing");
            addDry(ingredients, toDigit(bowlNum), bowls);
        } else {
            add(ingredient, toDigit(bowlNumber), bowls);
        }

    } else if (strncmp(function, "Liquify", 7) == 0) {
        if (strncmp(ingredientName, "contents", 8) == 0) {
            // liquefy bowl

            char *bowlNum = returnWordBeforeTarget(copy, "mixing");
            liquefyBowl(toDigit(bowlNum), bowls);
        } else {
            liquefy(*ingredient);
        }

    } else if (strncmp(function, "Stir", 4) == 0) {
        if (strncmp(strtok(NULL, " "), "the", 3) == 0) {
            // stir bowl
            char *bowlNum = returnWordBeforeTarget(copy, "mixing");
            char *numOfMins = returnWordBeforeTarget(copy, "minutes");
            stir(toDigit(numOfMins), bowls, toDigit(bowlNum));
        } else {
            //stir ingredient
            char *bowlNum = returnWordBeforeTarget(copy, "mixing");
            stirIngredient(ingredient, bowls, toDigit(bowlNum));
        }

    } else if (strncmp(function, "Mix", 3) == 0){
        //mix
        char *bowlNum = returnWordBeforeTarget(copy, "mixing");
        mix(bowls, toDigit(bowlNum));

    } else if (strncmp(function, "Clean", 5) == 0){
        //clean
        char *bowlNum = returnWordBeforeTarget(copy, "mixing");
        clean(bowls, toDigit(bowlNum));

    } else if (strncmp(function, "Pour", 4) == 0){
        //pour
        char *bowlNum = returnWordBeforeTarget(copy, "mixing");
        char *trayNum = returnWordBeforeTarget(copy, "baking");
        pour(bowls, toDigit(bowlNum), trays, toDigit(trayNum));

    } else if (strncmp(function, "Serves", 6) == 0){
        serves(trays, toDigit(ingredientName));
    } else {
        return;
    }
}

int toDigit(char *num){
    int total = 0;
    if (strncmp(num, "the", 3) == 0){
        return 1;
    }
    for (int i = 0; i < strlen(num); i++){
        if(isdigit(num[i])){
            total += 10 * i + atoi(&num[i]);
        }
    }
    return total;
}


int numOfwords(const char *sentence) {
    int count=0,i,len;
    char lastC;
    len=strlen(sentence);
    if(len > 0) {
        lastC = sentence[0];
    }
    for(i=0; i<=len; i++) {
        if((sentence[i]==' ' || sentence[i]=='\0') && lastC != ' ')
        {
            count++;
        }
        lastC = sentence[i];
    }
    return count;
}

char *trimRight(char *s) {
    if (s == NULL) {
        return s;
    }
    *s = s[strlen(s) - 1];
    while (isspace(*s)) {
        s--;
    }
    return s;
}

