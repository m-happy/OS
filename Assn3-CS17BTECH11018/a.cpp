#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;
#define t 1000000
#define BUFFER_SIZE 100
default_random_engine eng1,eng2;
exponential_distribution<double> distribution1;
exponential_distribution<double> distribution2;
double t1,t2;
int cntp;
int cntc;
int buffer[BUFFER_SIZE];
int index=0;
int counts;
long double pavg;
long double cavg;
sem_t full,empt,b;
pthread_mutex_t mutex;
FILE* f=fopen("BufferLog.txt","w");

const string currenttime(time_t inp) {

    struct tm * tstruct;
    char       buf[80];
    tstruct = localtime(&inp);
    sprintf(buf,"%.2d:%.2d:%.2d",tstruct->tm_hour,tstruct->tm_min,tstruct->tm_sec);
    string tim(buf);
    return tim;
}

void* produce(void* arg){
    long int id=(long int)arg;
	for(int i=0;i<cntp;i++){
        time_t now1 = time(NULL);
		sem_wait(&empt);
		//pthread_mutex_lock(&mutex);
		sem_wait(&b);
		int item = rand()%100;
		buffer[index++] = item;
		counts++;
		time_t now = time(0);
		fprintf(f,"%d st item Produced by thread %d at %s into buffer location %d \n",i+1,id,(currenttime(now).c_str()),index);
		usleep(distribution1(eng1)*t);
		//pthread_mutex_unlock(&mutex);
		sem_post(&b);
		sem_post(&full);
        time_t now2 = time(NULL);
		pavg+=now2-now1;

	}
	pthread_exit(0);
}

void* consume(void* arg){
    long int id=(long int) arg;
	for(int i=0;i<cntc;i++){
        time_t now1 = time(NULL);
		sem_wait(&full);
		//pthread_mutex_lock(&mutex);
		sem_wait(&b);
		int item = buffer[--index];
		counts++;
		time_t now = time(0);
		fprintf(f,"%d st item Consumed by thread %d at %s from buffer location %d \n",i+1,id,(currenttime(now).c_str()),index+1);
		usleep(distribution2(eng2)*t);
		//pthread_mutex_unlock(&mutex);
		sem_post(&b);
		sem_post(&empt);
		time_t now2 = time(NULL);
		cavg+=now2-now1;

	}
	pthread_exit(0);
}

int main(){
    int np=10;
	int nc=15;
    double l1,l2;
	l1=8,l2=12;
    exponential_distribution<double> distribution1(1/l1);
    exponential_distribution<double> distribution2(1/l2);
	//distribution1((1/l1));
	//distribution2((1/l2));
	pthread_t producer[np],consumer[nc];
	sem_init(&empt,0,BUFFER_SIZE);
	sem_init(&full,0,0);
    sem_init(&b,0,1);
    cntp=5;
	cntc=5;

	pthread_mutex_init(&mutex,NULL);

    for(int i=0;i<np;i++){
	pthread_create(producer+i,NULL,produce,(void*)i);}

	for(int i=0;i<nc;i++){
	pthread_create(consumer+i,NULL,consume,(void*)i);}

	for(int i=0;i<np;i++){
        pthread_join(producer[i],NULL);
	}
	for(int i=0;i<nc;i++){
        pthread_join(consumer[i],NULL);
	}
	cout<<counts<<endl;
	pavg=pavg/(np*cntp);
	cavg=cavg/(nc*cntc);
	cout<<pavg<<endl<<cavg;
	pthread_exit(NULL);
}
