#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
	printf("Number of arguments: %d\n", argc);
	printf("List of arguments:\n");
	for (int i = 0; i < argc; ++i) {
		printf("\targv[%d]: %s\n", i, argv[i]);
	}
	printf("Environment property:\n");
	for (int i = 0; envp[i] != 0; ++i) {
		printf("\tenvp[%d]: %s\n", i, envp[i]);
	}
	return 0;
}

