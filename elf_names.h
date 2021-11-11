#ifndef ELF_NAMES_H
#define ELF_NAMES_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <string>
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <byteswap.h>
#include <sstream>

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

// translate value of e_type field to a string
const char *get_type_name(uint16_t value);

// translate value of e_machine field to a string
const char *get_machine_name(uint16_t value);

/** gets section header so we can index
* PARAMETERS
* elf_header pointer to elf header
* returns section header 
*/

Elf64_Shdr *elf_sheader(Elf64_Ehdr *elf_header);

/** gets a particular section header
* PARAMETERS
* elf_header pointer to elf header
* idx the index of the header
* returns a particluar index of the header table
*/

Elf64_Shdr *elf_section(Elf64_Ehdr *elf_header, int idx);

/** gets section header string table
* PARAMETERS
* elf_header pointer to elf header
* returns the section header string table
*/

char *elf_str_table(Elf64_Ehdr *elf_header);

/** gets the beginning of the section header string table
* PARAMETERS
* elf_header pointer to elf header
* offset offset to get to the string table
* return pointer to the beginning of the section header string table
*/

char *elf_lookup_string(Elf64_Ehdr *elf_header, int offset);

/** gets pointer to the beginning of symbol string table 
* PARAMETERS
* elf_header pointer to elf header
* offset1 offset to get to the beginning of the symbol string table
* return pointer to the beginning of symbol string table 
*/

char *sym_str_table(Elf64_Ehdr *elf_header, int offset1);

/** looks up a section header string 
* PARAMETERS
* elf_headr the pointer to the elf header
* offset1 offset to get to .strtab
* offset2 offset to get to the symbol name in .strtab
* return string of symbol 
*/

char *sym_lookup_string(Elf64_Ehdr *elf_header, int offset1, int offset2);

void decToHex(int dec);

/** prints out the section headers 
* PARAMETERS
* index index the position of the symbol
* name name of section header
* type type of section header
* offset offset of section header
* size size of section header
*/

void printInfoSH(int index, char* name, unsigned int type, int offset, int size);


/** prints out the symbol table.
* PARAMETERS
* index index the position of the symbol
* name name of symbol 
* size size of symbol 
* info info of symbol 
* other other of symbol 
*/

void printInfoSym(int index, char* name, unsigned int size, int info, int other);

/** checks if is elf file.
* PARAMETERS
* elf_header the elf_header pointer 
* return true if has error, false otherwise
*/
bool err (Elf64_Ehdr* elf_header);

#ifdef __cplusplus
}
#endif

#endif // ELF_NAMES_H
