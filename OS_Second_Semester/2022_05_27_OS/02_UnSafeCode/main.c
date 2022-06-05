#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char* argv[])
{
	char *ptr;
	void (*func)(void);
	int pagesize;
	
	pagesize = sysconf(_SC_PAGE_SIZE);
	fprintf(stderr,"Page size = %d (0x%x)\n", pagesize, pagesize);
	
	if ( NULL == (ptr = malloc(pagesize << 1)) ) {
		perror("malloc");
		return __LINE__;
	}

	func = (void(*)())(ptr + (pagesize - (((off_t)ptr) & (pagesize - 1))));
	fprintf(stderr, "PTR = %p\nFUNC = %p\n", ptr, func);
	((char*)func)[0] = 0xc3;
	
#ifdef UNSAFE_MEMORY_BLOCK
	if ( (-1) == mprotect((void*)func, pagesize, PROT_READ|PROT_EXEC) ) {
		perror("mprotect");
		free(ptr);
		return __LINE__;
	}
#endif
	
	fprintf(stderr, "DEBUG: %d\n", __LINE__);
	func();
	fprintf(stderr, "DEBUG: %d\n", __LINE__);
	free(ptr);
	return 0;
}
