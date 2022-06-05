#include <elf.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

union Elf_Ehdr
{
	Elf32_Ehdr arch32;
	Elf64_Ehdr arch64;
};

static int ges32(char* name, void* data, int len, off_t *off, int *size);
static int ges64(char* sname, void* data, int len, off_t *off, int *size);
int get_elf_section(char* name, void* data, int len, off_t *off, int *size)
{
	int arch = ELFCLASS32;
  
	if (len < EI_NIDENT) {
		return __LINE__;
	}
	if (memcmp( data, ELFMAG, sizeof(uint32_t)) != 0) {
		printf("Файл не соответствует формату ELF\n");
		return __LINE__;
	}
  
	arch = (unsigned int) ((((unsigned char*) data)[EI_CLASS]));
	
	return (arch == ELFCLASS32) ?
		(ges32(name, data, len, off, size)) :
		(ges64(name, data, len, off, size));
}

static int ges32(char* sname, void* data, int len, off_t *off, int *size)
{
	Elf32_Ehdr *hdr = (Elf32_Ehdr*) data;
	Elf32_Shdr *shdr;
	Elf32_Half i;
	char* ptr;
	char* names;
	int found = 0;
	
	if (len < sizeof(Elf32_Ehdr))
		return __LINE__;
	
	/* Определяем число секций */
	if ((!hdr->e_shnum) || (hdr->e_shstrndx == SHN_UNDEF)) {
		fprintf( stderr, "Секции не найдены\n");
		return __LINE__;
	}
  
	if (len < hdr->e_shoff)
		return __LINE__;
	
	/* Получаем массив заголовков */
	shdr = (Elf32_Shdr*) (data + hdr->e_shoff);
  
	if (len < (shdr[hdr->e_shstrndx].sh_offset))
		return __LINE__;
	/* Получаем массив имен */
	names = (char*) (data + shdr[hdr->e_shstrndx].sh_offset);
  
	for (i = 0; i < hdr->e_shnum; i++) {
		/* Берем только исполняемые секции */
		if ((shdr[i].sh_flags & SHF_EXECINSTR) != SHF_EXECINSTR)
			continue;
		
		ptr = &(names[shdr[i].sh_name]);
		
		if ((!found) && (strcmp(ptr, sname) == 0)) {
			*off = shdr[i].sh_offset;
			*size = shdr[i].sh_size;
			found = 1;
		}
	}
	if (found) return 0;
	fprintf( stderr, "Секция %s не найдена\n", sname);
	return __LINE__;
	
}

static int ges64(char* sname, void* data, int len, off_t *off, int *size)
{
	Elf64_Ehdr *hdr = (Elf64_Ehdr*) data;
	Elf64_Shdr *shdr;
	Elf64_Half i;
	char* ptr;
	char* names;
	int found = 0;
	
	/* Определяем число секций */
	if ((!hdr->e_shnum) || (hdr->e_shstrndx == SHN_UNDEF)) {
		fprintf( stderr, "Секции не найдены\n");
		return __LINE__;
	}
	
	/* Получаем массив заголовков */
	shdr = (Elf64_Shdr*) (data + hdr->e_shoff);
	
	/* Получаем массив имен */
	names = (char*) (data + shdr[hdr->e_shstrndx].sh_offset);
	
	for (i = 0; i < hdr->e_shnum; i++) {
		/* Берем только исполняемые секции */
		if ((shdr[i].sh_flags & SHF_EXECINSTR) != SHF_EXECINSTR)
			continue;
		ptr = &(names[shdr[i].sh_name]);
		if ((!found) && (strcmp(ptr, sname) == 0)) {
			*off = shdr[i].sh_offset;
			*size = shdr[i].sh_size;
			found = 1;
		}
	}
	if (found) return 0;
	fprintf( stderr, "Секция %s не найдена\n", sname);
	return __LINE__;
}
