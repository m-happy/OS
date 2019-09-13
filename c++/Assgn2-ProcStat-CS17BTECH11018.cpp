#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
using namespace std;
//structure to store the values of mean, median, mode
struct values
{
	double mean;
	double median;
	double standard_deviation;
};
int n;
int *arr;
int sum;
//function to calculate the mean value 
double mean_cal(){
	sum = 0;
	for(int i = 0; i < n; i++){
		sum += arr[i];
	}
	double mean = (double)sum/(double)n;
	return mean;
}
//function to create the standard deviation
double std_dev(){
	double s = 0;
	for(int i = 0; i < n; i++)
		s += arr[i];
	double m = s/(double)n;
	double temp=0;
	for(int i = 0; i < n ; i++)
	{
		temp += ((arr[i]*arr[i]) - m*m)/n;
	}
	double standard_deviation = sqrt(temp);
	return standard_deviation;
}
//function to calculate the median
double median_cal(){
	double median = (double)((arr[(n-1)/2] + arr[n/2])/2);
	return median;
}
//main function
int main(){
	cin>>n;
	//name of the shared memory object
	const char *name = "/process";
	//memory allocation for elements of array
	arr=(int *)malloc(n*sizeof(int));
	for(int i = 0; i < n; i++)
	{
		cin>>*(arr + i);
	}
	//size of the shared memeory
	const int SIZE = 4096;

	int fd;
	struct values *ptr;
	//open the shared memoory object
	fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	//configure the size of the shared memory object
	ftruncate(fd, SIZE);
	//time before the process starts
	clock_t before = clock();
	sort(arr, arr + n);
	//memory map to shared object
	ptr = (struct values *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	//process ids
	pid_t pid1, pid2, pid3;
	//child creation
	pid1 = fork();
	if(pid1 < 0){
		fprintf(stderr,"forking failed");
		return 1;
	}
	else if(pid1 == 0){
		ptr->mean = mean_cal();
	}
	else{
		//second child/process
		wait(NULL);
		pid2 = fork();
		if(pid2 < 0){
			fprintf(stderr, "forking failed 2");
		}
		else if (pid2 == 0){
			wait(NULL);
			ptr->median = median_cal();
		}
		else{
			//third child/ process
			wait(NULL);
			pid3 = fork();
			if(pid3 < 0){
				fprintf(stderr,"forking failed 3\n" );
			}
			else if(pid3 == 0){
				ptr->standard_deviation = std_dev();
			}
			else{
				//parent process
				//wait for all the child process to complete
				wait(NULL);
				//time after all the child process has been completed
				clock_t t = clock();
				cout<<"The average value is "<<ptr->mean<<endl;
				cout<<"The standard deviation value is "<<ptr->standard_deviation<<endl;
				cout<<"The median value is "<<ptr->median<<endl;
				cout<<(double)(t - before)/CLOCKS_PER_SEC<<endl; //diff between the starttime and endtime
			}
		}
	}
	shm_unlink(name);
	return 0;
}