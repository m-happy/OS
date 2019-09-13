#include <iostream>
#include <random>
#include <pthread.h>
#include <time.h>
#include <condition_variable>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;
FILE* file = fopen("dining-log.txt","w");
int no_of_eatings;
int no_of_philosopher;
pthread_mutex_t lock;
pthread_cond_t *self;
enum STATES{THINKING, HUNGRY, EATING} state[20];
int cs_time, rem_time;
double average_waiting = 0, worst_waiting = 0;


//function for exponential distribution
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
//test function to test if the right or left is free or not
void *test(int philosopher){

	if((philosopher % 2) == 0){
		pthread_mutex_lock(&lock);
		if((state[philosopher] == HUNGRY) && (state[(philosopher + no_of_philosopher -1) % no_of_philosopher] != EATING)
			&& (state[(philosopher + 1)% no_of_philosopher] != EATING)){
			state[philosopher] = EATING; //state is eating
			pthread_cond_signal(&self[philosopher]);
		}
		pthread_mutex_unlock(&lock);
	}
	else{
		pthread_mutex_lock(&lock);
		if((state[philosopher] == HUNGRY) && (state[(philosopher + no_of_philosopher -1) % no_of_philosopher] != EATING)
			&& (state[(philosopher + 1)% no_of_philosopher] != EATING)){
			state[philosopher] = EATING; //eating
			pthread_cond_signal(&self[philosopher]);
		}
		pthread_mutex_unlock(&lock);
		return NULL;
	}
}
//pickup funtion for picking
void *pickup(int philosopher){
	// cout<<"1";
	pthread_mutex_lock(&lock);
	state[philosopher] = HUNGRY;
	pthread_mutex_unlock(&lock);
	test(philosopher);
	pthread_mutex_lock(&lock);
	if(state[philosopher] != EATING){
		pthread_cond_wait(&self[philosopher], &lock);
	}
	pthread_mutex_unlock(&lock);
}
//putdown the forks after eating
void *putdown(int philosopher){
	pthread_mutex_lock(&lock);
	state[philosopher] = THINKING;
	pthread_mutex_unlock(&lock);
	test((philosopher + no_of_philosopher - 1) % no_of_philosopher);
	test((philosopher + 1) % no_of_philosopher);
}
//Philosopher's function
void* Philosopher(void *param){
	// long long id = (long long)param;
	int *temp = (int*)param;
	int id = *temp;
	for(int i = 0; i < no_of_eatings; i++){
		// cout<<"hello";
		time_t requestTime = time(NULL);
		clock_t request = clock();
		fprintf(file, "%dth eat request by philosopher thread %d at %s\n", i + 1, id, getTime(requestTime).c_str());
		//CS entry section
		pickup(id);
		time_t entryTime = time(NULL);
		clock_t entry = clock();
		fprintf(file, "%dth CS Entry by philosopher Thread %d at %s\n", i+1, id, getTime(entryTime).c_str());
		// usleep(distribution1(generator) * 1e6);
		double diff_time = entry - request;
		average_waiting += diff_time;
		if(worst_waiting < diff_time)
			worst_waiting = diff_time;
		usleep(distribution1(generator) * 1e3);
		time_t exitTime = time(NULL);
		fprintf(file, "%dth CS exit by philosopher thread %d at %s\n", i + 1, id, getTime(exitTime).c_str());
		putdown(id);
		usleep(distribution2(generator) * 1e3);
	}
}
int main(){
	// int no_of_philosopher;
	ifstream inn;
	inn.open("input.txt");
	inn>>no_of_philosopher>>no_of_eatings>>cs_time>>rem_time;
	// cout<<no_of_philosopher<<no_of_eatings<<cs_time<<rem_time<<endl;
	// state = new  STATES[no_of_philosopher];
	self = new pthread_cond_t[no_of_philosopher];
	pthread_t philosophers[no_of_philosopher];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_mutex_init(&lock, NULL);
	// cout<<"1";
	for(int i = 0; i < no_of_philosopher; i++){
		state[i] = THINKING;
		pthread_cond_init(&self[i], NULL);
	}
	// cout<<"intilization done";
	for(int i = 0; i < no_of_philosopher; i++){
		pthread_create(&philosophers[i], &attr, Philosopher, &i);
	}
	for(int i = 0; i < no_of_philosopher; i++){
		pthread_join(philosophers[i], NULL);
	}
	// file.close();
	file = fopen("Time.txt","w");
	fprintf(file,"The average time for waiting it %f\n", average_waiting * 1000/(no_of_philosopher*no_of_eatings*CLOCKS_PER_SEC));
	fprintf(file, "The worst time for philosopher is %f\n",worst_waiting * 1000/CLOCKS_PER_SEC);
	return 0;
}