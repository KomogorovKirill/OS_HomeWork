#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include "crc32.h"
#include "rc4.h"

void __attribute__((section(".upx"))) hello_world()
{
  	char hello_str[] = "Hello, World!\n";
  	printf(hello_str);
}

extern char upx_end;
extern char upx_start;

int main(int argc, char *argv[])
{
	int page_size = sysconf(_SC_PAGE_SIZE), mpr_status = 0;
	size_t SizeSection = (&upx_end) - (&upx_start);
  	unsigned char *dec = (unsigned char *) malloc(SizeSection);
  	if(argc != 2)
  	{
    		printf("Usage: %s <key>\n", argv[0]);
    		exit(1);
  	}

  	void *ptr = &upx_start;
  	ptr -= ((off_t)ptr) & (page_size - 1);
	mpr_status = mprotect(ptr , page_size , PROT_READ|PROT_WRITE|PROT_EXEC);
  	if (-1 == mpr_status)
	{
    		perror("mprotect");
    		exit(2);
  	}

  	RC4(argv[1], &upx_start, dec, SizeSection);

  	if(crc32(dec, SizeSection) == *(uint32_t *)(&upx_end))
  	{

   	 	memcpy(&upx_start, dec, SizeSection);
  	}
  	else 
  	{
    		puts("You password invalid!");
		free(dec);
    		exit(3);
  	}

  	hello_world();
  	free(dec);
  	return 0;
}
