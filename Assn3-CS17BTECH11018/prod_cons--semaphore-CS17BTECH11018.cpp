#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <random>
#include <time.h>
#include <semaphore.h>
using namespace std;
	
int BUFFER_SIZE;		//buffer size
int *buffer;			//dynamic buffer allocation	
int count = 0;			
int consumer_freq, producer_freq;
double producer_time, consumer_time;
double producer_average_time = 0, consumer_average_time = 0;
int in = 0, out = 0;
sem_t full, empty;
sem_t mut;
sem_t lock_p, lock_c;
FILE* file = fopen("prod_cons_sem.txt","w");

default_random_engine generator;
exponential_distribution <double> distribution1 (1.0/producer_time);
exponential_distribution <double> distribution2 (1.0/consumer_time);

string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}

void *producer(void *params){
	// cout<<"p";
	long long id = (long long)params;
	for(int i = 0; i < producer_freq; i++){
			// cout<<"p";
		clock_t entry_time = clock();
		sem_wait(&empty);
			// cout<<"inside semaphore empty"<<endl;
		sem_wait(&mut);
		time_t prodTime = time(NULL);
		count++;
		buffer[in] = rand() % 100;
		fprintf(file, "%dth item : %d produced by thread %lld at %s into buffer location %d\n",
					 				count,buffer[in], id,getTime(prodTime).c_str(), in);
		in = (in + 1)%BUFFER_SIZE;				
		sem_post(&mut);
		sem_post(&full);	
		usleep(distribution1(generator) * 1e6);
		clock_t exit_time = clock();
		sem_wait(&lock_p);
		producer_average_time += exit_time - entry_time;
		sem_post(&lock_p);
			// pthread_exit(0);
	}
	pthread_exit(0);
}

void *consumer(void *param){
	// cout<<"c";
	long long id_c = (long long)param;

	for(int i = 0;i < consumer_freq; i++){
		clock_t entry_time = clock();
		sem_wait(&full);
		// sem_wait(&mut);
		sem_wait(&mut);
		time_t consTime = time(NULL);
		// count--;
		fprintf(file, "%dth item : %d consumed by thread %lld at %s into buffer location %d\n",
									 count,buffer[out],id_c, getTime(consTime).c_str(), out);
		count--;
		out = (out + 1) % BUFFER_SIZE;
		sem_post(&mut);
		sem_post(&empty);
		usleep(distribution2(generator) * 1e6);
		clock_t exit_time = clock();
		sem_wait(&lock_c);
		consumer_average_time += exit_time - entry_time;
		sem_post(&lock_c);
			// pthread_exit(0);
	}
	pthread_exit(0);
}
int main(){
	ifstream inn;
	inn.open("inp-params.txt");
	int no_of_producer, no_of_consumer;
	inn>>BUFFER_SIZE>>no_of_producer>>no_of_consumer>>
			producer_freq>>consumer_freq>>producer_time>>consumer_time;
	// cout<<BUFFER_SIZE<<no_of_consumer<<endl;
	//semaphore initilization
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	sem_init(&mut, 0, 1);
	sem_init(&lock_p, 0, 1);
	sem_init(&lock_c, 0, 1);
	buffer = new int [BUFFER_SIZE];
	// cout<<consumer_freq<<" cons"<<endl;
	// cout<<producer_freq<<" prod"<<endl;
	pthread_t producers[no_of_producer], consumers[no_of_consumer];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	//producer and consumer thread creation
	for(int i = 0; i < no_of_producer; i++){
		pthread_create(&producers[i], &attr, producer, (void*)i);
	}
	for(int j = 0; j < no_of_consumer; j++){
		pthread_create(&consumers[j], &attr, consumer, (void*)j);
	}
	//thread joining
	for(int k = 0; k < no_of_producer;k++)
		pthread_join(producers[k], NULL);
	for(int l = 0; l < no_of_consumer; l++)
		pthread_join(consumers[l], NULL);
	fclose(file);
	printf("Average waiting time for producer : %lf\n", producer_average_time/(no_of_producer * producer_freq * CLOCKS_PER_SEC));
	printf("Average waiting time for consumer : %lf\n", consumer_average_time/ (no_of_consumer * consumer_freq * CLOCKS_PER_SEC));
	return 0;
}