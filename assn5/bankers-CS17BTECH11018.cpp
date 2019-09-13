#include <iostream>
#include <fstream>
#include <mutex>
#include <pthread.h>
#include <random>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

FILE* file = fopen("lab-Log.txt","w");
ifstream in;
int CS_Session = 0;
int S;
int no_of_process;
int k;
bool flag = false;
bool flag1 = false;
int sessionCount = 0;
int time1, time2;
sem_t wait;
sem_t mut;
sem_t mut1;
default_random_engine generator;
exponential_distribution <double> distribution1 (1.0/time1);
exponential_distribution <double> distribution2 (1.0/time2);

//function to print current time
string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}

void *testCS(void *params){
	long long id = (long long)params;
	for(int i = 0; i < k; i++){
		int s = (rand() % S) + 1;
		time_t reqEnterTime = time(NULL);
		fprintf(file, "%dth CS Request at %s by thread %lld for session %d\n", i, getTime(reqEnterTime).c_str(), id, s);
		sem_wait(&mut1);
		while( s != CS_Session && sessionCount != 0)
			;
		if(sessionCount != 0)
			sem_post(&wait);
		sem_wait(&wait);
		CS_Session = s;
		sessionCount++;
		sem_post(&mut1);
		time_t actEnterTime = time(NULL);
		fprintf(file, "%dth CS Entry at %s by thread %lld for session %d\n", i, getTime(actEnterTime).c_str(), id, s);
		usleep(distribution1(generator) * 1e6);
		//exit section
		time_t exitTime = time(NULL);
		fprintf(file, "%dth CS Exit at %s by thread %lld for session %d\n", i, getTime(exitTime).c_str(), id, s);
		sessionCount--;
		sem_post(&wait);
		usleep(distribution2(generator) * 1e6);
	}
	pthread_exit(0);
}
int main(){
	in.open("inp-params.txt");
	in>>no_of_process>>k>>S>>time1>>time2;
	cout<<no_of_process<<k<<S<<time1<<time2<<endl;
	pthread_t Process[no_of_process];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	sem_init(&wait, 0, 1);
	sem_init(&mut, 0, 1);
	sem_init(&mut1, 0, 1);
	for(int i = 0; i < no_of_process; i++){
		pthread_create(&Process[i], &attr, testCS, (void*)i);
	}
	for(int i = 0; i < no_of_process; i++)
		pthread_join(Process[i], NULL);

	fclose(file);
}