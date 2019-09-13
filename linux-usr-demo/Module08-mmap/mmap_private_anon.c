#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SZ (1024*1024)
char *buf;

int main()
{
	int i;
	char ch;

	printf("Press any key to do mmap\n");
	scanf("%c", &ch);

	if ((buf = mmap(NULL, SZ, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0)) == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	
	printf("Press any key to do write\n");
	scanf("%c", &ch);

	for (i=0; i<SZ; i++) {
		buf[i] = 97 + i%26;
	}

	printf("Press any key to do unmap\n");
	scanf("%c", &ch);

	munmap(buf, SZ);

	printf("Press any key to exit\n");
	scanf("%c", &ch);
}

