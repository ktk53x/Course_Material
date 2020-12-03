#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "processInfo.h"

int
main(int argc, char *argv[])
{
	printf(1, "The total number of active processes: %d\n\n", getNumProc());
	printf(1, "The maximum pid among the active processes: %d\n\n", getMaxPid());

	struct processInfo info;
	// Initialize to dummy values
	info.ppid = 0;
	info.psize = 0;
	info.numberContextSwitches = 0;

	printf(1, "Pid of the current process= %d\n", getpid());
	if( getProcInfo(getpid(), &info) == -1 )
	{
		printf(1, "Process Not Found ahahahhahahahahah\n\n");
	}
	else
	{
		printf(1, "Process Info of the current process\n");
		printf(1, "pid\tppid\tpsize\tnumberContextSwitches\n");
		printf(1, "%d\t%d\t%d\t%d\n\n", getpid(), info.ppid, info.psize, info.numberContextSwitches);
	}	

	printf(1, "Trying to get process info of a Non Existent Process with pid 100000\n");
	if( getProcInfo(100000, &info) == -1 )
        {
                printf(1, "Process Not Found ahahahahahahhahah\n\n");
        }
        else
        {
                printf(1, "Process Info of the current process\n");
                printf(1, "pid\tppid\tpsize\tnumberContextSwitches\n");
                printf(1, "%d\t%d\t%d\t%d\n\n", 100000,info.ppid, info.psize, info.numberContextSwitches);
        }

	printf(1, "Initial Burst Time %d\n", get_burst_time());
	char n[4];
	printf(1, "Please Enter BurstTime Value: \n");
	read(0, (void *)n, sizeof(n));
	printf(1, "Setting Burst Time to %s.....\n", n);
	if(set_burst_time(atoi(n)) == -1)
	{
		printf(1, "Please Enter burst Time between (1-20)\n\n");
	}
	else
	{
		printf(1, "Final Burst Time %d\n", get_burst_time());
	}
	exit();
}
