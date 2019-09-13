#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <random>
#include <time.h>
using namespace std;


int BUFFER_SIZE;		//capacity of buffer
int *buffer;			//buffer is global but it is initialised dynamically as we don't have value of BUFFERSIZE
int count = 0;			//count of buffer
int consumer_freq, producer_freq;	
double producer_time, consumer_time;
double producer_avgerage_time = 0;
double consumer_average_time = 0;
int in = 0, out = 0;
pthread_mutex_t mutex1;	
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;
pthread_mutex_t mutex4;
FILE* file = fopen("prod_cons_sem.txt","w");	//file of output
//random generator funtion and their distributions
default_random_engine generator;
exponential_distribution <double> distribution1 (1.0/producer_time);
exponential_distribution <double> distribution2 (1.0/consumer_time);

//funtion for
string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}

//code for producer function
void *producer(void *param){
	long long id = (long long)param;		//id of the thread
	for(int i = 0; i < producer_freq; i++){
		//while loop keeps spining the thread if count = Buffer size 
		time_t entry_time = time(NULL);
		while(1){
			pthread_mutex_lock(&mutex1);	
			if(count == BUFFER_SIZE)	
				pthread_mutex_unlock(&mutex1);
			else
				break;
		}
		//mutex3 for preventing calculations
		pthread_mutex_lock(&mutex3);
		time_t prodTime = time(NULL);
		count++;
		buffer[in] = rand() % 100;
		fprintf(file, "%dth item : %d produced by thread %lld at %s into buffer location %d\n",
					 				count,buffer[in], id,getTime(prodTime).c_str(), in);
		in = (in + 1)%BUFFER_SIZE;
		pthread_mutex_unlock(&mutex3);
		pthread_mutex_unlock(&mutex1);
		time_t exit_time = time(NULL);
		usleep(distribution1(generator) *  1e6);
		pthread_mutex_lock(&mutex2);
		producer_avgerage_time += exit_time - entry_time;
		pthread_mutex_unlock(&mutex2);

	}
	pthread_exit(0);
}

//code for consumer functions
void *consumer(void *param){
	//id of thread
	long long id = (long long)param;
	for(int i = 0; i < consumer_freq; i++){
		//while loop keeps spining the thread if count = Buffer size 
		time_t entry_time = time(NULL);
		while(1){
			pthread_mutex_lock(&mutex1);
			if(count == 0)
				pthread_mutex_unlock(&mutex1);
			else
				break;
		}
		//mutex3 for mutual exclusion in calculations
		pthread_mutex_lock(&mutex3);		
		time_t consTime = time(NULL);
		// count--;
		fprintf(file, "%dth item : %d consumed by thread %lld at %s into buffer location %d\n",
									 count,buffer[out],id, getTime(consTime).c_str(), out);
		count--;
		out = (out + 1) % BUFFER_SIZE;
		pthread_mutex_unlock(&mutex3);		//release lock
		pthread_mutex_unlock(&mutex1);		//release lock
		usleep(distribution2(generator) *  1e6);
		time_t exit_time = time(NULL);
		pthread_mutex_lock(&mutex4);
		consumer_average_time += exit_time - entry_time;
		pthread_mutex_unlock(&mutex4);
	}
	pthread_exit(0);
}
int main(){
	ifstream inn;
	inn.open("inp-params.txt");			//input file
	int no_of_producer, no_of_consumer;
	inn>>BUFFER_SIZE>>no_of_producer>>no_of_consumer>>
			producer_freq>>consumer_freq>>producer_time>>consumer_time;
	buffer = new int [BUFFER_SIZE];
	//mutex initilization
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_mutex_init(&mutex3, NULL);
	pthread_mutex_init(&mutex4, NULL);
	pthread_t producers[no_of_producer], consumers[no_of_consumer];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	//producer thread creation
	for(int i = 0; i < no_of_producer; i++){
		pthread_create(&producers[i], &attr, producer, (void*)i);
	}
	//threads for consumers
	for(int j = 0; j < no_of_consumer; j++){
		pthread_create(&consumers[j], &attr, consumer, (void*)j);
	}
	//joining of threads
	for(int k = 0; k < no_of_producer;k++)
		pthread_join(producers[k], NULL);
	for(int l = 0; l < no_of_consumer; l++)
		pthread_join(consumers[l], NULL);
	fclose(file);
	printf("Average waiting time for producers : %lf\n", producer_avgerage_time/(no_of_producer * producer_freq));
	printf("Average waiting time for consumers : %lf\n", consumer_average_time/(no_of_consumer * consumer_freq));
	return 0;
}