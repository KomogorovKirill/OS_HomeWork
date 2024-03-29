#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include "elf_upx.h"
#include "rc4.h"
#include "crc32.h"

int main(int argc, char *argv[])
{
	unsigned char *ptr = NULL;
    	int sizeSect = 0, fd = 0, CRC = 0;
    	struct stat st;
    	off_t offset;
    	size_t pageSize = sysconf(_SC_PAGESIZE), nsize = 0;

    	if(argc != 3)
    	{
        	printf("Usage: %s <key> <elf_file>\n", argv[0]);
        	exit(1);
    	}
    
    	if (-1 == stat(argv[2],&st))
    	{
        	perror("stat");
        	exit(2);
    	}
    
    	char *data = (char *) malloc(st.st_size * sizeof(char));

    	if (NULL == data)
    	{
        	perror("malloc");
        	exit(3);
    	}
    
    	fd = open(argv[2], O_RDWR);
    	if (fd <= 0) 
    	{
		perror("open");
		exit(4);
    	}
    
    	read(fd, data, st.st_size);
    	
    	if (get_elf_section( ".upx", data, st.st_size, &offset, &sizeSect))
    	{
        	printf("File %s not found...\n", argv[2]);
        	exit(5);
    	}

    	nsize = (st.st_size + (pageSize - 1)) & (~(pageSize - 1));

    	ptr = mmap(NULL, nsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    	if (MAP_FAILED == ptr)
    	{
        	perror("mmap");
        	close(fd);
        	exit(6);
    	}

    	unsigned char *bytes = (unsigned char *) malloc(sizeSect - 4);  
    
	CRC = crc32(ptr + offset, sizeSect - 4);
    	RC4(argv[1], ptr + offset, bytes, sizeSect - 4);

    	memcpy(ptr + offset, bytes, sizeSect - 4);
    	*(uint32_t *)(ptr + offset + sizeSect - 4) = CRC;

    	munmap(ptr, nsize);
	close(fd);
   	free(data);
   	free(bytes);
    	return 0;
}
