#include <stdio.h>
#include "fact.h"

int main()
{
	int n, f;
	char ch;

	printf("To calculate factorial, enter n: ");
	scanf("%d", &n);

	f = fact(n);

	printf("n!=%d\n", f);
	scanf("%c", &ch);

	return 0;
}

