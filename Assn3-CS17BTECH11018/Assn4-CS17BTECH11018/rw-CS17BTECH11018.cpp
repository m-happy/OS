/* Reader - Writers Problem
	Assignment 4 - Operating system 2
	Dr. Sathya peri
	Happy kumar CS17BTECH11018 */
#include <iostream>
#include <random>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;

//global variables
FILE* file = fopen("reader_writer_sem.txt","w");
int writer_freq, reader_freq, read_count = 0;
double average_reader_waiting_time, average_writer_waiting_time;
double worst_reader_time, worst_writer_time;
double cs_time, rem_time;
sem_t mutex;
sem_t rw_mutex;
default_random_engine generator;
exponential_distribution <double> distribution1 (1.0/ cs_time);
exponential_distribution <double> distribution2 (1.0/	rem_time);

//function to get time
string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}

//writers function
void* Writer(void *param){
	// cout<<"writer";
	long long id = (long long)param;
	for(int i = 0; i < writer_freq; i++){
		time_t request_time = time(NULL);
		clock_t request = clock();
		fprintf(file, "%dth CS request by Writer thread %lld at %s\n", i + 1, id, getTime(request_time).c_str());
		sem_wait(&rw_mutex);
		//writing is performed
		time_t entry_time = time(NULL);
		clock_t entry = clock();
		fprintf(file, "%dth CS entry by Writer thread %lld at %s\n", i + 1, id, getTime(entry_time).c_str());
		usleep(distribution1(generator) * 1e6);
		time_t exit_time = time(NULL);
		fprintf(file, "%dth CS exit by Writer thread %lld at %s\n", i + 1, id , getTime(exit_time).c_str());
		double diff_time = entry - request;
		average_writer_waiting_time += diff_time;
		if(diff_time > worst_writer_time)
			worst_writer_time = diff_time;
		sem_post(&rw_mutex);
		usleep(distribution1(generator) * 1e6);
	}
}

//readers function
void *Reader(void *param){
	// cout<<"reader";
	long long id = (long long)param;
	for(int i = 0; i < reader_freq; i++){
		time_t request_time = time(NULL);
		clock_t request = clock();
		fprintf(file, "%dth CS request by Reader thread %lld at %s\n", i + 1, id, getTime(request_time).c_str());
		sem_wait(&mutex);
		read_count++;
		if(read_count == 1)
			sem_wait(&rw_mutex);
		sem_post(&mutex);
		time_t entry_time = time(NULL);
		clock_t entry = clock();
		fprintf(file, "%dth CS Entry by Reader thread %lld at %s\n", i + 1, id, getTime(entry_time).c_str());
		//reading is performed
		usleep(distribution2(generator) * 1e6);
		time_t exit_time = time(NULL);
		sem_wait(&mutex);
		read_count--;
		if(read_count == 0)
			sem_post(&rw_mutex);
		fprintf(file, "%dth CS Exit by Reader thread %lld at %s\n", i + 1,id, getTime(exit_time).c_str());
		double diff_time = entry - request;
		average_reader_waiting_time += diff_time;
		if(diff_time > worst_reader_time)
			worst_reader_time = diff_time;
		sem_post(&mutex);
		usleep(distribution2(generator) * 1e6);


	}
}
int main(){
	ifstream inn;
	inn.open("inp-params.txt");  //file input
	int no_of_readers, no_of_writers;
	inn>>no_of_writers>>no_of_readers>>writer_freq>>reader_freq>>cs_time>>rem_time;
	cout<<"no of writers : "<<no_of_writers <<", no of readers : "<<no_of_readers<<endl;
	pthread_t writers[no_of_writers], readers[no_of_readers];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	sem_init(&mutex, 0, 1);
	sem_init(&rw_mutex, 0, 1);
	//creation of thread for writers 
	for(int i = 0; i < no_of_writers; i++){
		pthread_create(&writers[i], &attr, *Writer, (void*)i);
	}
	for(int i = 0; i < no_of_readers; i++){
		pthread_create(&readers[i], &attr, *Reader, (void*)i);
	}
	//joining of threads
	for(int i = 0; i < no_of_writers; i++)
		pthread_join(writers[i], NULL);
	for(int i = 0; i < no_of_readers; i++)
		pthread_join(readers[i], NULL);
	//this gives time in second to change it milli second multiply it by 1000
	cout<<"average reader time : "<<(double)average_reader_waiting_time * 1000/(no_of_readers * reader_freq * CLOCKS_PER_SEC)<<endl;
	cout<<"average writer time : "<<(double)average_writer_waiting_time * 1000/(no_of_writers * writer_freq* CLOCKS_PER_SEC)<<endl;
	cout<<"worst reader time : "<<(double)worst_reader_time * 1000/ CLOCKS_PER_SEC<<endl;
	cout<<"worst writer time : "<<(double)worst_writer_time * 1000/ CLOCKS_PER_SEC<<endl;
	return 0;
}
