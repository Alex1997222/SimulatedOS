#ifndef ZIYA_OSKERNEL_TEACH_MM_H
#define ZIYA_OSKERNEL_TEACH_MM_H

#include "types.h"

#define PAGE_SIZE 4096

typedef struct {
    unsigned int  base_addr_low;    //The lower 32 bits of the memory base address
    unsigned int  base_addr_high;   //The upper 32 bits of the memory base address
    unsigned int  length_low;       //The lower 32 bits of the memory block length
    unsigned int  length_high;      //The upper 32 bits of the memory block length
    unsigned int  type;             //Describes the type of memory block
}check_memmory_item_t;

typedef struct {
    unsigned short          times;
    check_memmory_item_t*   data;
}check_memory_info_t;

typedef struct {
    uint    addr_start;     // The starting address of available memory is generally 1M
    uint    addr_end;       // Available memory end address
    uint    valid_mem_size;
    uint    pages_total;    // The total number of pages in the physical memory of the machine
    uint    pages_free;     // How many pages are left in the physical memory of the machine
    uint    pages_used;     // How many pages are used in the physical memory of the machine
}physics_memory_info_t;

typedef struct {
    uint            addr_base;          // Available physical memory starts at 3M
    uint            pages_total;        // How many pages in total? How many pages in the physical memory of the machine - 0x30000 (3M)
    uint            bitmap_item_used;  // If 1B maps a page, how many pages are used
    uchar*          map;
}physics_memory_map_t;

void print_check_memory_info();

void memory_init();
void memory_map_int();

void* virtual_memory_init();

// allocate/free memory
void* get_free_page();
void free_page(void* p);

// allocate/free memory
void* kmalloc(size_t size);
void kfree_s(void *obj, int size);

#endif 
