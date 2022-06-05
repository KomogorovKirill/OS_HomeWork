#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct Args_tag
{
	int *resoultMatrix;
	int cols1;
	int rows2;
	FILE *file3;
} args_t;

static void *save(void *args)
{
	args_t *arg = (args_t*) args;
	for (int i = 0; i < arg->cols1 * arg->rows2; i++)
	{
		fprintf(arg->file3, "%7d ", (arg->resoultMatrix)[i]);
		if (i % arg->rows2 == (arg->rows2 - 1))
			fputc('\n', arg->file3);
	}
	return NULL;
}


int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("Using: %s <matrix1> <matrix2> <resoult>\n", argv[0]);
		return 1;
	}
	pthread_t pid;
	int count_cols1 = 2, count_rows1 = 2, count_cols2 = 2, count_rows2 = 2;
	int rows1 = 0, cols1 = 0, rows2 = 0, cols2 = 0;
	int **matrix1 = (int **)malloc(count_cols1 * sizeof(int *));
	for (int i = 0; i < count_cols1; i++)
		matrix1[i] = (int *)malloc(count_rows1 * sizeof(int));
	int **matrix2 = (int **)malloc(count_cols2 * sizeof(int *));
	for (int i = 0; i < count_cols2; i++)
		matrix2[i] = (int *)malloc(count_rows2 * sizeof(int));

	FILE *file1 = fopen(argv[1], "r");
	if (NULL == file1)
	{
		printf("Error! File '%s' not foud!\n", argv[1]);
		return 2;
	}
	FILE *file2 = fopen(argv[2], "r");
	if (NULL == file2)
	{
		printf("Error! File '%s' not found!\n", argv[2]);
		fclose(file1);
		return 3;
	}
	FILE *test = fopen(argv[3], "r");
	if (NULL != test)
	{
		printf("Error! File '%s' exist!\n", argv[3]);
		for (int i = 0; i < count_rows1; i++)
			free(matrix1[i]);
		for (int i = 0; i < count_rows2; i++)
			free(matrix2[i]);
		free(matrix1);
		free(matrix2);	
		fclose(file1);
		fclose(file2);
		fclose(test);
		return 4;
	}
	FILE *file3 = fopen(argv[3], "w");

	int buffer = 0;
	char ch = '\0';

	while (ch != '\n')
	{
		fscanf(file1, "%d%c", &buffer, &ch);
		if (ch == ' ' || ch == '\n')
			rows1++;
	}

	ch = '\0';
	while (ch != '\n')
	{
		fscanf(file2, "%d%c", &buffer, &ch);
		if (ch == ' ' || ch == '\n')
			rows2++;
	}

	fseek(file1, 0, SEEK_SET);
	fseek(file2, 0, SEEK_SET);

	for (; !feof(file1); cols1++)
	{
		if (cols1 == count_cols1)
		{
			count_cols1++;
			matrix1 = (int **)realloc((int **)matrix1, count_cols1 * sizeof(int *));
			if (NULL == matrix1)
			{
				puts("Memory error!");
				for (int i = 0; i < count_cols2; i++)
					free(matrix2[i]);
				free(matrix2);
				fclose(file1);
				fclose(file2);
				fclose(file3);
				return 5;
			}
			matrix1[count_cols1-1] = (int *)malloc(count_rows1 * sizeof(int *));
		}
        	for (int j = 0; j < rows1; j++)
		{
			if (j == count_rows1)
			{
				count_rows1++;
				matrix1[cols1] = realloc(matrix1[cols1], count_rows1 * sizeof(int));
				if (NULL == matrix1[cols1])
				{
					puts("Memory error!");
					for (int i = 0; i < cols1; i++)
						free(matrix1[i]);
					free(matrix1);
					for (int i = 0; i < count_cols2; i++)
						free(matrix2[i]);
					fclose(file1);
					fclose(file2);
					fclose(file3);
					return 5;
				}
			}
         		fscanf(file1, "%d ", &matrix1[cols1][j]);
		}
	}

	for (; !feof(file2); cols2++)
	{
		if (cols2 == count_cols2)
		{
			count_cols2++;
			matrix2 = (int **)realloc((int **)matrix2, count_cols2 * sizeof(int *));
			if (NULL == matrix2)
			{
				puts("Memory error!");
				for (int i = 0; i < count_cols1; i++)
					free(matrix1[i]);
				free(matrix1);
				fclose(file1);
				fclose(file2);
				fclose(file3);
				return 5;
			}
			matrix2[count_cols2-1] = (int *)malloc(count_rows2 * sizeof(int *));
		}
        	for (int j = 0; j < rows2; j++)
		{
			if (j == count_rows2)
			{
				count_rows2++;
				matrix2[cols2] = realloc(matrix2[cols2], count_rows2 * sizeof(int));
				if (NULL == matrix2[cols2])
				{
					puts("Memory error!");
					for (int i = 0; i < cols2; i++)
						free(matrix2[i]);
					free(matrix2);
					for (int i = 0; i < count_cols1; i++)
						free(matrix1[i]);
					free(matrix1);
					fclose(file1);
					fclose(file2);
					fclose(file3);
					return 5;
				}
			}
         		fscanf(file2, "%d ", &matrix2[cols2][j]);
		}
	}
	puts("First Matrix");
	for (int i = 0; i < cols1; i++)
	{
		for (int j = 0; j < rows1; j++)
			printf("%7d ", matrix1[i][j]);
		putchar('\n');
	}

	puts("Second Matrix");
	for (int i = 0; i < cols2; i++)
	{
		for (int j = 0; j < rows2; j++)
			printf("%7d ", matrix2[i][j]);
		putchar('\n');
	}

	if(cols1 != rows2)
	{
		puts("Multiplication impossible");
		for (int i = 0; i < count_rows1; i++)
			free(matrix1[i]);
		for (int i = 0; i < count_rows2; i++)
			free(matrix2[i]);
		free(matrix1);
		free(matrix2);
		fclose(file1);
		fclose(file2);
		fclose(file3);
		return 6;
    	}

	int resoultMatrix[cols1][rows2];
	for (int i = 0; i < cols1; i++)
		for (int j = 0; j < rows2; j++)
			resoultMatrix[i][j] = 0;

	for (int i = 0; i < cols1; i++)
		for (int j = 0; j < rows2; j++)
	    		for(int k = 0; k < rows1; k++)
	    			resoultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
	
	int Matr[cols1 * rows2];
	int tmp = -1;
	for (int i = 0; i < cols1; i++)
		for (int j = 0; j < rows2; j++)
		{
			Matr[++tmp] = resoultMatrix[i][j];
		}
	args_t args;
	args.resoultMatrix = Matr;
	args.cols1 = cols1;
	args.rows2 = rows2;
	args.file3 = file3;
	void *retval;
	pthread_create(&pid, NULL, save, (void *)&args);

	puts("Resoult Matrix");	
	for (int i = 0; i < cols1; i++)
    	{
    		for (int j = 0; j < rows2; j++)
    			printf("%7d", resoultMatrix[i][j]);
		putchar('\n');

    	}
	
	pthread_join(pid, &retval);

	for (int i = 0; i < count_rows1; i++)
		free(matrix1[i]);
	for (int i = 0; i < count_rows2; i++)
		free(matrix2[i]);
	free(matrix1);
	free(matrix2);
	fclose(file1);
	fclose(file2);
	fclose(file3);
    	return 0;
}
