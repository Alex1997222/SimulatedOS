#ifndef ZIYA_OSKERNEL_RESEARCH_TASK_H
#define ZIYA_OSKERNEL_RESEARCH_TASK_H

#include "types.h"
#include "mm.h"

// process limit
#define NR_TASKS 64

typedef void* (*task_fun_t)(void*);

//task state
typedef enum task_state_t {
    TASK_INIT,     
    TASK_RUNNING,  
    TASK_READY,    
    TASK_BLOCKED,  
    TASK_SLEEPING, 
    TASK_WAITING,  
    TASK_DIED,     
} task_state_t;

typedef struct tss_t {
    u32 backlink; // The link of the previous task, which saves the segment selector of the previous state segment
    u32 esp0;     // ring0 top address of the stack
    u32 ss0;      // ring0 stack segment selector
    u32 esp1;     // ring1 top address of the stack
    u32 ss1;      // ring1 stack segment selector
    u32 esp2;     // ring2 top address of the stack
    u32 ss2;      // ring2 stack segment selector
    u32 cr3;
    u32 eip;
    u32 flags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldtr;          // local descriptor selector
    u16 trace : 1;     
    u16 reversed : 15; 
    u16 iobase;        
    u32 ssp;           
} __attribute__((packed)) tss_t;

typedef struct task_t {
    tss_t           tss;
    int             pid;
    int             ppid;
    char            name[32];
    task_state_t    state;
    int             exit_code;
    int             counter;
    int             priority;
    int             scheduling_times;       // scheduling times
    int             esp0;                   // At the beginning of creation, the active esp3 is saved in tss
    int             ebp0;
    int             esp3;
    int             ebp3;

    bool            resume_from_irq;        //Determine whether there is a disk interruption

    int             magic;
}task_t;

typedef union task_union_t {
    task_t task;
    char stack[PAGE_SIZE];
}task_union_t;

task_t* create_task(char* name, task_fun_t fun, int priority);
void task_init();

// 参数位置不可变，head.asm中有调用
void task_exit(int code, task_t* task);
void current_task_exit(int code);

void task_sleep(int ms);
void task_wakeup();

int inc_scheduling_times(task_t* task);

pid_t get_task_pid(task_t* task);
pid_t get_task_ppid(task_t* task);

task_t* create_child(char* name, task_fun_t fun, int priority);

int get_esp3(task_t* task);
void set_esp3(task_t* task, int esp);

void task_block(task_t* task);
void task_unblock(task_t* task);

void set_block(task_t* task);
bool is_blocked(task_t* task);

#endif 
