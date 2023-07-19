#ifndef ZIYA_OSKERNEL_TEACH_STDARG_H
#define ZIYA_OSKERNEL_TEACH_STDARG_H

typedef char* va_list;

#define va_start(p, count) (p = (va_list)&count + sizeof(char*))
#define va_arg(p, t) (*(t*)((p += sizeof(char*)) - sizeof(char*))) //modify p_args and fetch value
#define va_end(p) (p = 0)

#endif 
