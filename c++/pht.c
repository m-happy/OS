#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
int sum; /* this data is shared by the thread(s) */
void *runner(void *param); /* threads call this function */
int main(int argc, char *argv[])
{
pthread t tid; /* the thread identifier */
pthread attr t attr; /* set of thread attributes */
/* set the default attributes of the thread */
pthread attr init (&attr);
/* create the thread */
pthread create (&tid, &attr, runner, argv[1]);
/* wait for the thread to exit */
pthread join (tid,NULL);
}
printf("sum = %d ∖ n",sum);
/* The thread will execute in this function */
void *runner(void *param)
{
int i, upper = atoi(param);
sum = 0;
for (i = 1; i <= upper; i++)
sum += i;
}