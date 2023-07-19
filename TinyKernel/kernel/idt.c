//
// Created by ziya on 22-6-27.
//

#include "../include/asm/system.h"
#include "../include/linux/head.h"
#include "../include/linux/traps.h"
#include "../include/linux/kernel.h"

#define INTERRUPT_TABLE_SIZE    256

interrupt_gate_t interrupt_table[INTERRUPT_TABLE_SIZE] = {0};

xdt_ptr_t idt_ptr;

extern void interrupt_handler_entry();
extern void keymap_handler_entry();
extern void clock_handler_entry();
extern void system_call_entry();
extern void hd_handler_entry();

// 是在汇编中定义的
extern int interrupt_handler_table[0x2f];

void idt_init() {
    printk("init idt...\n");

    for (int i = 0; i < INTERRUPT_TABLE_SIZE; ++i) {
        interrupt_gate_t* p = &interrupt_table[i];

        int handler = (int)interrupt_handler_entry;

        if (i <= 0x15) {
            handler = (int)interrupt_handler_table[i];
        }

        if (0x20 == i) {
            handler = (int)clock_handler_entry;
        }

        if (0x21 == i) {
            handler = (int)keymap_handler_entry;
        }

        if (0x2e == i) {
            handler = (int)hd_handler_entry;
        }

        if (0x80 == i) {
            handler = (int)system_call_entry;
        }

        p->offset0 = handler & 0xffff;
        p->offset1 = (handler >> 16) & 0xffff;
        p->selector = 1 << 3;
        p->reserved = 0;      
        p->type = 0b1110;     // interrupt gate
        p->segment = 0;       // system seg
        p->DPL = (0x80 == i)? 3 : 0;
        p->present = 1;       // valid
    }

    // write idt table to kernel
    write_xdt_ptr(&idt_ptr, INTERRUPT_TABLE_SIZE * 8, interrupt_table);

    asm volatile("lidt idt_ptr;");
}