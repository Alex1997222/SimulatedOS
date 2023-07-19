#ifndef ZIYA_OSKERNEL_TEACH_HEAD_H
#define ZIYA_OSKERNEL_TEACH_HEAD_H

// gdt item
typedef struct gdt_item_t {
    unsigned short limit_low;      // Segment boundary 0 ~ 15 bits
    unsigned int base_low : 24;    // Base address 0 ~ 23 bits 16M
    unsigned char type : 4;        // segment type
    unsigned char segment : 1;     // 1 means code segment or data segment, 0 means system segment
    unsigned char DPL : 2;         // Descriptor Privilege Level 0 ~ 3
    unsigned char present : 1;     // presence bit, 1 in memory, 0 on disk
    unsigned char limit_high : 4;  // segment bounds 16 ~ 19;
    unsigned char available : 1;   // available
    unsigned char long_mode : 1;   // 64-bit extension flags
    unsigned char big : 1;         // 32 bit or 16 bit;
    unsigned char granularity : 1; // Granularity 4KB or 1B
    unsigned char base_high;       // Base address 24 ~ 31 bits
} __attribute__((packed)) gdt_item_t;

// interrupt gate item
typedef struct interrupt_gate_t {
    short offset0;    // Intra-segment offset 0 ~ 15 bits
    short selector;   // code snippet selector
    char reserved;    // Reserved
    char type : 4;    // Task gate/interrupt gate/trap gate
    char segment : 1; // segment = 0 system segment
    char DPL : 2;     // Minimum privileges for access using the int instruction
    char present : 1; // is it effective
    short offset1;    // Intra-segment offset 16 ~ 31 bits
} __attribute__((packed)) interrupt_gate_t;

typedef struct gdt_selector_t {
    char RPL : 2;
    char TI : 1;
    short index : 13;
} __attribute__((packed)) gdt_selector_t;

#pragma pack(2)
typedef struct xdt_ptr_t {
    short   limit;
    int     base;    
} xdt_ptr_t;
#pragma pack()

#endif
