#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

volatile unsigned int in1 = 0, out1 = 0, in2 = 0, out2 = 0, flag_ob1 = 1, flag_cur = 0, flag_ob2 = 0, i = 0;

static void *ob1(void *none)
{
	while (1)
	{
		if (flag_ob1)
		{
			pthread_mutex_lock(&mutex);
			i++;
			in1++;
			printf("Work ob1 (local):  %d\n", in1);
			printf("Work ob1 (remove): %d\n", out2);
			flag_ob1 = 0;
			flag_cur = 1;
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	}
}

static void *ob2(void *none)
{
	while (1)
	{
		if (flag_ob2)
		{
			pthread_mutex_lock(&mutex);
			i++;
			printf("Work ob2 (remove): %d\n", out1);
			in2++;
			printf("Work ob2 (local):  %d\n", in2);
			flag_ob2 = 0;
			flag_cur = 1;
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	}
}

int main()
{
	pthread_t pid_ob1, pid_ob2;
	pthread_create(&pid_ob1, NULL, ob1, NULL);
	pthread_create(&pid_ob2, NULL, ob2, NULL);
	pthread_detach(pid_ob1);
	pthread_detach(pid_ob2);
	
	while (1)
	{	
		if (flag_cur)
		{
			pthread_mutex_lock(&mutex);
			puts("Work cur");
			out1 = in1;
			out2 = in2;
			flag_cur = 0;
			if (i % 2 == 0)
				flag_ob1 = 1;
			else
				flag_ob2 = 1;
			pthread_mutex_unlock(&mutex);
			sleep(1);
		}
	}
	return 0;
}
