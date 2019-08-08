#include "brainfuck.h"

void printMem(uint8_t *memory, int num) {
	for (int i = 0; i < num; ++i) {
		printf("memory at %d : %d\n", i, memory[i]);
	}
	putchar('\n');
}

int main(int argc, char **argv) {
	assert(argc == 2);

	//Open input file
	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		perror("file cannot be read");
		exit(EXIT_FAILURE);
	}

	//Find size of instruction file
	fseek(file, 0l, SEEK_END);
	int size = ftell(file);
	rewind(file);

	//Write to instruction array
	uint8_t *instructions = calloc(size, sizeof(uint8_t));
	if (instructions == NULL) {
		perror("instruction_array_allocation");
		exit(EXIT_FAILURE);
	}
	fread(instructions, sizeof(uint8_t), size, file);

	fclose(file);

	//Initialise memory
	uint8_t *tape;
	tape = (uint8_t *) calloc(MEMORY_SIZE, sizeof(uint8_t)); //sets all memory locations to zero
	assert(tape != NULL);

	uint8_t *data_ptr;//initialise data pointer to point to beginning of memory
	data_ptr = tape;

	brainfuck(instructions, size, data_ptr);

	fclose(file);
	free(instructions);
	free(tape);

}

void brainfuck(uint8_t *instructions, int size, uint8_t *data_ptr) {
	int instr = 0;
	for (; instr < size; instr++) {
		switch (instructions[instr]) {
			case '>':
				++data_ptr;
				break;
			case '<':
				--data_ptr;
				break;
			case '+':
				++(*data_ptr);
				break;
			case '-':
				--(*data_ptr);
				break;
			case '.':
				putchar((int) *data_ptr);
				break;
			case ',':
				*data_ptr = (uint8_t) getchar();
				break;
			case '[':
				if (*data_ptr == 0) {
					//To deal with nested loops
					int loop = 1;
					while (loop > 0) {
						uint8_t current_char = instructions[++instr];
						if (current_char == ']') {
							loop--;
						} else if (current_char == '[') {
							loop++;
						}
					}
				}
				break;
			case ']':
				if (*data_ptr != 0) {
					//To deal with nested loops
					int loop = 1;
					while (loop > 0) {
						uint8_t current_char = instructions[--instr];
						if (current_char == '[') {
							loop--;
						} else if (current_char == ']') {
							loop++;
						}
					}
				}
				break;
			default:
				break;
		}
	}
}

