#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define SHMSZ     4096

int main()
{
    char ch;
    int i, fd;
    char *shm;

    printf("press any key to do shm_open()\n");
    scanf("%c", &ch);

    if ((fd = shm_open("/myshm", O_CREAT|O_RDWR, 0600)) < 0) {
        perror("shm_open() failed");
        exit(1);
    }

    printf("Press any key to do ftruncate\n");
    scanf("%c", &ch);

    if (ftruncate(fd, SHMSZ) != 0) {
        perror("ftruncate() failed");
        exit(2);
    }

    printf("Press any key to do mmap\n");
    scanf("%c", &ch);

    if ((shm = mmap(NULL, SHMSZ, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("mmap");
        exit(3);
    }

    printf("Press any key to do write\n");
    scanf("%c", &ch);
    
    for (i=0; i<SHMSZ; i++) {
        shm[i] = 65 + i%26;
    }

    printf("Press any key to do unmap\n");
    scanf("%c", &ch);

    munmap(shm, SHMSZ);

#if 0
    printf("press any key to do shm_unlink()\n");
    scanf("%c", &ch);

    if (shm_unlink("/myshm") == -1) {
        perror("shm_unlink() failed");
        exit(2);
    }
#endif

    printf("press any key to exit\n");
    scanf("%c", &ch);

    close(fd);

    exit(0);
}

