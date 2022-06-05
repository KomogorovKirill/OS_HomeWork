#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char* argv[])
{
	char *ptr, *str;
	int pagesize;
	
	pagesize = sysconf(_SC_PAGE_SIZE);
	fprintf(stderr,"Page size = %d (0x%x)\n", pagesize, pagesize);
	
	if ( NULL == (ptr = malloc(pagesize << 1)) ) {
		perror("malloc");
		return __LINE__;
	}

	str = ptr + (pagesize - (((off_t)ptr) & (pagesize - 1)));
	fprintf(stderr, "PTR = %p\nSTR = %p\n", ptr, str);

	strncpy(str, "Hello, world!\n", pagesize);
	
#ifdef SAFE_MEMORY_BLOCK
	if ( (-1) == mprotect((void*)str, pagesize, PROT_READ) ) {
		perror("mprotect");
		free(ptr);
		return __LINE__;
	}
#endif

	fputs(str, stderr);

	strncpy(str, "Goodby, world!\n", pagesize);
	fputs(str, stderr);

	free(ptr);
	return 0;
}
