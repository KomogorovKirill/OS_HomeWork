#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int size = 2048;
void **memory;
int count_mem = -1;
void *new(size_t bytes)
{
	if (count_mem == -1)
	{
		memory = (void **)malloc(size);
		for (int i = 0; i < size; i++)
			memory[i] = NULL;
	}
	count_mem++;
	if (count_mem == size)
	{
		size++;
		memory = (void **)realloc(memory, size);
	}
	memory[count_mem] = (void *)malloc(bytes);
	if (NULL == memory[count_mem])
		puts("Memory error!");
	return memory[count_mem];
}

void __attribute__ ((destructor)) destructor(void)
{
	for (int i = 0; i < size; i++)
		free(memory[i]);
	free(memory);
}
