#include <stdio.h>
#include <stdlib.h>

int main()
{
  char ch;
  void *ptr;
  int i;

  scanf("%c", &ch);
  for(i=1; ;i++) {
    printf("i=%d\n", i);
    ptr = malloc(100048576); // 100MB
    if (ptr==NULL) {
      printf("malloc() failed\n");
      break;
    }
  }
  scanf("%c", &ch);
}

