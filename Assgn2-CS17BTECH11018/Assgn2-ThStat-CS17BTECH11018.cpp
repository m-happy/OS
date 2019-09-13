#include <iostream>
#include <pthread.h>
#include <math.h>
#include <algorithm>
#include <time.h>
using namespace std;
int n;		//no of elements in array
int *arr;	//arr pointer
double mean;	
double standard_deviation;
double median;
long int sum;
void *mean_cal(void *param);
void *median_cal(void *param);
void *std_dev(void *param);

int main(){
	cin>>n;
	arr=(int *)malloc(n*sizeof(int));
	for(int i = 0; i < n; i++)
	{
		cin>>*(arr + i);
	}
	//thred ids
	pthread_t thread1, thread2, thread3;
	clock_t t1;
	//time before the thread has been started
	clock_t before = clock();
	sort(arr, arr + n);
	pthread_create(&thread1, NULL, mean_cal, (void *)&arr[0]);		//first thread created
	pthread_create(&thread2, NULL, median_cal, (void *)&arr[0]);	//second thread created
	pthread_create(&thread3, NULL, std_dev, (void *)&arr[0]);		//third thread created
	pthread_join(thread1, NULL);	//first thread joined
	pthread_join(thread2, NULL);	//second thread joined
	pthread_join(thread3, NULL);	//third thread joined
	//time after the thread has been joined
	t1 = clock();
	cout<<"The average value is "<< mean<<endl;
	cout<<"The standard deviation value is "<<standard_deviation<<endl;
	cout<<"The median value is "<<median<<endl;
	// cout<<(double)(t1 - before)/CLOCKS_PER_SEC<<endl;	//diff of startime and endtime
	exit(EXIT_SUCCESS);
}
//function to calculate the mean
void *mean_cal(void *param){
	//int l = atoi(param);
	sum = 0;
	mean = 0;
	for(int i = 0; i < n; i++){
		// sum += arr[i];
		mean += (double)arr[i]/double(n);
	}
	// mean = (double)sum/(double)n;
	pthread_exit(0);
}
//function to calcuate the median
void *median_cal(void *param){
	//int l = atoi(param);	// int z;
	median = (double)((arr[(n-1)/2] + arr[n/2])/2);
	pthread_exit(0);
}
//function to calculate the standard deviation
void *std_dev(void *param){
	//int l = atoi(param);
	long int s = 0;
	double m = 0;
	for(int i = 0; i < n; i++){
		// s += arr[i];
		m += (double)arr[i]/double(n);
	}
	// cout<<s<<endl;
	// float m = (float)s/(float)n;
	double temp=0;
	for(int i = 0; i < n ; i++)
	{
		temp += (double)((arr[i]*arr[i]) - m*m)/(double)n;
	}
	standard_deviation = sqrt(temp);
	pthread_exit(0);
}