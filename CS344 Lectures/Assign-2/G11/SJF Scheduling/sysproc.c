#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_wolfie(void)
{


  char *wolf = "        __.....__\n     .-'         '-.\n   .'               '.\n  /                   \\\n /        |\\           \\\n;        |V \\_          ;\n|        |  ' \\         ;\n;        )   ,_\\        |\n;       /    |          ;\n \\     /      \\         /\n  \\    |       \\       /\n   '.   \\       \\    .'\n     '-._|       \\.-'\n         | |\\     |    \n_________/ |_'.   /_______  \n";
  void *buff;
  argptr(0, (void *)&buff, sizeof(*buff));
  uint size;
  argptr(1, (void *)&size, sizeof(size));

  if (size < 365)
    return -1;
  else
  {
    for (int i = 0; i < 365; i++)
    {
      ((char *)buff)[i] = (char)wolf[i];
    }
    return 365;
  }
}


int
sys_getNumProc(void)
{
	return getNumProc();
}

int
sys_getMaxPid(void)
{
	return getMaxPid();
}

int
sys_getProcInfo(void)
{
	int pid;
	struct processInfo* info;	
	argptr(0, (void *)&pid, sizeof(pid));
	argptr(1, (void *)&info, sizeof(info));
	return getProcInfo(pid, info);
}

int
sys_set_burst_time(void)
{
	int n;
	argptr(0, (void *)&n, sizeof(n));
	return set_burst_time(n);
}

int
sys_get_burst_time(void)
{
	return get_burst_time();
}
