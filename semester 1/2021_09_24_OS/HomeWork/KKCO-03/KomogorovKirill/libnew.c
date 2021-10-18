#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 2048

void *memory[SIZE] = {NULL};
int count_mem = 0;
int flag = 0;
void *new(size_t bytes)
{
	count_mem++;
	if (count_mem >= SIZE)
	{
		flag = 1;
		puts("Error! the limit for creating pointers has been exceeded!");
	}
	memory[count_mem] = malloc(bytes);
	if (NULL == memory[count_mem])
		puts("Memory error!");
	return memory[count_mem];
}

void __attribute__ ((destructor)) destructor(void)
{
	if (flag)
		for (int i = 0; i < SIZE; i++)
			free(memory[i]);
	else
		for (int i = 0; i <= count_mem; i++)
			free(memory[i]);
}
