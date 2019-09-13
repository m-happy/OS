#include <stdio.h>

void func(int i) 
{
  printf("i=%d &i=%llx\n", i, &i);
  func(i+1);
}

int main() 
{
  char ch;
  scanf("%c",&ch);
  func(0);
}

