#ifndef ZIYA_OS_RESEARCH_YA_SHELL_H
#define ZIYA_OS_RESEARCH_YA_SHELL_H

#include "linux/types.h"

bool ya_shell_is_active();
void active_ya_shell();
void close_ya_shell();

void run_ya_shell(char ch);
void exec_ya_shell();
void del_ya_shell();


#endif 
