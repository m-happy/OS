#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int sum;
double mean; //data shared the threads
void *runner(void *param);
#define NUMTHREADS 10
int main(int argc, char *argv[])
{
	pthread_t tid;   		//the thread identifier
	pthread_attr_t attr; 	//set of thread attributes
	/*set the default thread attributes */
	pthread_attr_init(&attr);
	//create  the thread
	pthread_create(&tid, &attr, runner, argv[1]);
	/* an array of threads to be joined upon */
	//wait for the thread to complete
	pthread_join(tid, NULL);
	// pthread_t workers[NUMTHREADS];
	// for (int i = 0; i < NUMTHREADS; i++)
	// pthread_join (workers[i], NULL);
	printf("%lf\n", mean);
	// return 0;
}
void *runner(void *param){
	// 	int i;
	int upper = atoi(param);
	sum = 0;
	for (int i = 0; i <= upper; i++)
	{
		/* code */
		sum += i;
	}
	// sum=;
	mean = sum/upper;
	pthread_exit(0);
}