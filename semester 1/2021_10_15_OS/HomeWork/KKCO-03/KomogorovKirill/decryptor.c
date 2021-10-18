#include <stdio.h>
#include <stdlib.h>

#define BYTE_LEN 28

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Usage: %s <file_name> <key>\n", argv[0]);
		return 1;
	}
	short int key = argv[2][0], buffer = 0;
	FILE *bin = fopen(argv[1], "rb+");
	if (NULL == bin)
	{
		printf("File %s not Found!\n", argv[1]);
		return 1;
	}

	for (int i = 0; i < BYTE_LEN; i++)
	{
		buffer = fgetc(bin);
		fseek(bin, i, SEEK_SET);
		buffer ^= key;
		fputc(buffer, bin);
		fseek(bin, i+1, SEEK_SET);
	}
	fclose(bin);
	return 0;
}
