#pragma once

#include <linux/syscalls.h>
#include "types.h"

typedef asmlinkage int (*sys_mkdir_t)(const char *, int);

asmlinkage int
mkdir_hook(const char *, int);



/* new hooks */


typedef asmlinkage int (*sys_mkdir_2_t)(const struct pt_regs *regs);

asmlinkage int mkdir_hook_2(const struct pt_regs *);


char* get_curr_proc_name(char* p);
