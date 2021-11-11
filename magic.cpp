#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <byteswap.h>
#include <string>

#include "elf_names.h"

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("Not an ELF file");
		return 1;
	}

  char* filename = argv[1];
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "could not open file");
  }
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    // error
    fprintf(stderr, "error");
  } else {
    size_t file_size = statbuf.st_size;
    // ...
    void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;

		if (err(elf_header)) {
			printf("Not an ELF file");
		return 1;
		}

		int endian = 0;
		if (elf_header->e_ident[EI_DATA] == 1) {
			endian = 1;
		}
		else if (elf_header->e_ident[EI_DATA] == 2) {
			endian = 2;
		}
		else {
			endian = 3;
		}

		uint16_t typeGetter = elf_header->e_type;
		uint16_t machineGetter = elf_header->e_machine;

    const char* type = get_type_name(typeGetter);
    const char* machine = get_machine_name(machineGetter);
		
    printf("Object file type: %s\n", type);
    printf("Instruction set: %s\n", machine);

		switch(endian) {
			case 1: 
				printf("Endianness: Little endian\n");
				break;
			case 2: 
				printf("Endianness: Big Endian\n");
				break;
			default: 
				printf("Endianness: null\n");
		}
    
    Elf64_Shdr* topShdr = elf_sheader(elf_header);

    uint64_t size = ((uint64_t) elf_header->e_shnum);

		//trying to save the index of symtab
		int symInd = 0;
		int strTabInd = 0;
    for(int i = 0; (unsigned) i < size; i++) {
			//improve readability
			char * sName = elf_lookup_string(elf_header, topShdr[i].sh_name);
			if (strcmp(".symtab", sName) == 0) {
				symInd = i;
			}
			if (strcmp(".strtab", sName) == 0) {
				strTabInd = i;
			}

			unsigned sType= topShdr[i].sh_type;
			long unsigned sOffset = topShdr[i].sh_offset;
			long unsigned sSize = topShdr[i].sh_size;

			//prints out the info for section header
			printInfoSH(i, sName, sType, sOffset, sSize);
    }

		//how much symbols we have to print
		uint64_t symEntries = (topShdr[symInd].sh_size) / (topShdr[symInd].sh_entsize);

		//the top of the symbol table?
		Elf64_Sym* sym = (Elf64_Sym *)((char *)elf_header + topShdr[symInd].sh_offset);
	
		for (int i = 0; i < (int) symEntries; i++) {

			char *symName = sym_lookup_string(elf_header, topShdr[strTabInd].sh_offset, sym[i].st_name);

			printInfoSym(i, symName, sym[i].st_size, sym[i].st_info, sym[i].st_other);
		}
  }
  if(close(fd) < 0) {
    fprintf(stderr, "could not close file");
  }
	return 0;
}
