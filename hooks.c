#include <linux/module.h>
#include "hooks.h"
#include "sys_hook.h"

extern struct sys_hook *lkh_sys_hook;

asmlinkage int
mkdir_hook(const char __user *path, int mode)
{
    sys_mkdir_t sys_mkdir;

    char path_buf[256] = {0};
    strncpy_from_user(path_buf, path, 256);

    printk(KERN_INFO "[kernel hook] path: %s\n", path_buf);
    
    sys_mkdir = (sys_mkdir_t)sys_hook_get_orig64(lkh_sys_hook, __NR_mkdir);

    return sys_mkdir(path, mode);
}

char * get_curr_proc_name(char* pathname)
{
  char *p = 0;
  struct mm_struct *mm = current->mm;
  if (mm) {
    down_read(&mm->mmap_sem);
    if (mm->exe_file) {
                //pathname = kmalloc(PATH_MAX, GFP_ATOMIC);
                //if (pathname) {
                      p = d_path(&mm->exe_file->f_path, pathname, PATH_MAX);
                    /*Now you have the path name of exe in p*/
                //}
            }
    up_read(&mm->mmap_sem);
  }
  return p;
}

asmlinkage int mkdir_hook_2(const struct pt_regs *regs)
{
    sys_mkdir_2_t sys_mkdir;

    char __user *path = (char*)regs->di;
    int mode = regs->si;

    char path_buf[256] = {0};
    long ncopy = strncpy_from_user(path_buf, path, 256);

    char proc_name[PATH_MAX]={0};

    if (ncopy > 0) {
      char *p = get_curr_proc_name(proc_name);
      printk(KERN_INFO "[khook] [%d] %s | mkdir(%s, %o)\n", current->pid, p, path_buf, mode);
    }
    
    sys_mkdir = (sys_mkdir_2_t)sys_hook_get_orig64(lkh_sys_hook, __NR_mkdir);

    return sys_mkdir(regs);
}
