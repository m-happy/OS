#include <stdio.h>

char ch;
void func()
{
  char var[1048576]; // 1MB
  scanf("%c",&ch);
}

int main()
{
  scanf("%c",&ch);
  func();
}

