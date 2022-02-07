#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	pid_t pid, ppid;
	pid = fork();
	if (pid == -1) {
		printf("Error.");
		return 1;
	} else if (pid == 0) {
		printf("Child:\n\tpid: %d\n\tppid: %d\n", getpid(), getppid());
		printf("Program task1.c starts.\n");
		execl("./task1", "arg_A", "arg_B", "arg_C", (char*) 0);
	} else {
		printf("Parent:\n\tpid: %d\n\tppid: %d\n", getpid(), getppid());

	}
	return 0;
}

