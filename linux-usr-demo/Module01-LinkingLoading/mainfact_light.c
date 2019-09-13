#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main()
{
	int n, f;

	printf("To calculate factorial, enter n: ");
	scanf("%d", &n);

	if (n < 0) {
		fprintf(stderr, "invalid n=%d\n", n);
		return 1;
	}
		
        void *handle;
        int (*fact)(int);
        char *error;

	handle = dlopen("libfact.so", RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		return 2;
	}

	dlerror();    /* Clear any existing error */
	*(void **) (&fact) = dlsym(handle, "fact");

	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return 3;
	}

	f = (*fact)(n);
	printf("n!=%d\n", f);

	dlclose(handle);

	return 0;
}

