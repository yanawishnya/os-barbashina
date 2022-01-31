#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
	pid_t pid;
	pid = fork();
	if (pid == -1) {
		printf("Error.");
		return 1;
	} else if (pid == 0) {
		printf("Child:\n\tpid = %d\n\tppid = %d\n", getpid(),
							getppid());
	} else {
		printf("Parent:\n\tpid = %d\n\tppid = %d\n", getpid(),
							getppid());
	}
	return 0;
}

