#include "fact.h"

int fact(int n)
{
	int i, fact;
  
	for(fact=1, i=1; i<=n; i++) {
		fact = fact * i;
	}
	return fact;
}

