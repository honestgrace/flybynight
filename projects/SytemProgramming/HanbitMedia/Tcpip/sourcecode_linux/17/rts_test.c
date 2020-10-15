#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	sigset_t set;
	siginfo_t info;
	int signo;
	printf("My pid %d\n", getpid());

	sigemptyset(&set);
	sigaddset(&set, SIGRTMIN);
	sigprocmask(SIG_BLOCK, &set, NULL);

	while(1)
	{
		sleep(5);
		signo = sigwaitinfo(&set, &info);
		printf("%d : %d\n", signo, info.si_pid);
	}
}
