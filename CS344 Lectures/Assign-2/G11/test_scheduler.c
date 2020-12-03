#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
	printf(1, "TESTCASE 1: NORMAL\n");
	printf(1, "Hi I am Parent Process [pid=%d]\n", getpid());
	for(int i = 0; i < 10; i++)
	{
		int f = fork();
		if( f < 0 )
		{
			printf(1, "Failed to create child\n");
		}
		else if( f == 0 )
		{
			printf(1, "Hi I am Child Process [pid=%d]\n", getpid());
			set_burst_time(19 - i);
			for(double j = 0; j <= 10000000.0; j++)
			{
				double k = 2;
				k = k + j - 3;
			}
			printf(1, "Child Process [pid=%d] [burstTime=%d] exiting....\n", getpid(), get_burst_time());
			exit();
		}
		else
			;
	}
	for(int i = 0; i < 10; i++)
	{
		int f = wait();
		printf(1,"Child Process [pid=%d] exited.\n", f); 
	}
	printf(1, "------------------------------------------------------------------------------\n");
	printf(1, "TESTCASE 2: WAITING\n");
	printf(1, "Hi I am Parent Process [pid=%d]\n", getpid());
	for(int i = 0; i < 7; i++)
	{
		int f = fork();
		if( f < 0 )
		{
			printf(1, "Failed to create child\n");
		}
		else if( f == 0 )
		{
			printf(1, "Hi I am Child Process [pid=%d]\n", getpid());
			set_burst_time(19 - i);
			if(i % 2 == 0)
			{
				char temp[20];
				printf(1, "Even Process, I should ask for input....\n");
				read(0, temp, sizeof(temp));
			}
			for(double j = 0; j <= 10000000.0; j++)
                        {
                                double k = 2;
                                k = k + j - 3;
                        }
                        printf(1, "Child Process [pid=%d] [burstTime=%d] exiting....\n", getpid(), get_burst_time());
			exit();
		}
		else
			;
	}	

	for(int i = 0; i < 7; i++)
        {
                int f = wait();
                printf(1,"Child Process [pid=%d] exited.\n", f);
        }	

	printf(1, "------------------------------------------------------------------------------\n");

	exit();
}
