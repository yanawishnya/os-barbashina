#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	printf("UID = %d\nGID = %d\n", getuid(), getgid());
	return 0;
}
