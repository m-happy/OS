#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int countt = 0;
    while (true) {
        int *ptr = (int*) malloc(100 * 1024 * 1024);
        memset(ptr, 1, 100 * 1024 * 1024);
        count++;
        printf("%d\n", count);
        sleep(1);
    }
    return 0;
}