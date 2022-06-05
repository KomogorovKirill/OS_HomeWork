#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include "crc32.h"
#include "rc4.h"

void __attribute__((section(".upx"))) hello_world()
{
  	char hello_str[] = "Hello, World!\n";
  	printf(hello_str);
}

extern char upx_end;
extern char upx_start;

char* upx_dec_sect(char* password)
{
  	size_t SizeSection = (&upx_end) - (&upx_start);
  	unsigned char *deciphertext = malloc(SizeSection);
  	unsigned char *ciphertext = malloc(SizeSection);

  	RC4(password, &upx_start, deciphertext, SizeSection);

  	if(crc32(deciphertext, SizeSection) == *(int*)(&upx_end))
  	{
    		memcpy(ciphertext, &upx_start, SizeSection);
   	 	memcpy(&upx_start, deciphertext, SizeSection);
  	}
  	else 
  	{
    		puts("You password invalid!");
    		free(ciphertext);
    		return NULL;
  	}
    
  	free(deciphertext);
  	return ciphertext;
}

int main(int argc, char *argv[])
{
  	void *ptr = NULL;
  	char* ciphertext;
	int page_size = sysconf(_SC_PAGE_SIZE), mpr_status = 0;

  	if(argc != 2)
  	{
    		printf("Usage: %s <key>\n", argv[0]);
    		exit(1);
  	}

  	ptr = &upx_start;
  	ptr -= ((off_t)ptr) & (page_size - 1);
	mpr_status = mprotect(ptr , page_size , PROT_READ|PROT_WRITE|PROT_EXEC);
  	if (mpr_status == -1)
	{
    		perror("mprotect");
    		exit(2);
  	}

  	ciphertext = upx_dec_sect(argv[1]);
  	if (NULL == ciphertext)
  	{
    		puts("Error decoding section!");
    		exit(3);
  	}

  	hello_world();
  	memcpy(&upx_start, ciphertext, (&upx_end) - (&upx_start));
  	free(ciphertext);
  	return 0;
}
