#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <random>
#include <time.h>
using namespace std;
std::atomic <int> lock(0);
//global variables 

FILE* file = fopen("CAS-Bounded-Log.txt","w");
ifstream in;
int no_of_process, k;
double t1, t2;
double waitTime, worstTime = 0, diffTime;
bool* waitingThread;

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

//testCS function to perform mutual exclution
void *testCS(void *param){
	int j;
	long long id = (long long)param;
	// cout<<param<<endl;	
	std::default_random_engine generator;
	std::exponential_distribution<double> distribution1(1.0/t1);
	std::exponential_distribution<double> distribution2(1.0/t2);
	for(int i = 0; i < k; i++){
		waitingThread[id] = true;
		bool key = false;
        time_t reqEnterTime = time(NULL);
        fprintf(file,"%dth CS request at %s by thread %lld\n", i+ 1, getTime(reqEnterTime).c_str(), id + 1);
        while(waitingThread[id] && !key){
        	int x = 0, y = 1;
        	key = lock.compare_exchange_strong(x ,y);
        }
        waitingThread[id] = false;
        //critical section
        time_t actEnterTime = time(NULL);

        diffTime = actEnterTime - reqEnterTime;
        waitTime += diffTime;
        fprintf(file,"%dth CS entry at %s by thread %lld\n", i+ 1, getTime(actEnterTime).c_str(), id + 1);
		usleep(distribution1(generator) * 1e6);
		j = (id + 1) % no_of_process;
		while((j != id) && !waitingThread[j])
			j = (j + 1) % no_of_process;
		if(worstTime < diffTime){
			worstTime = diffTime;
		}
        time_t exitTime = time(NULL);
        fprintf(file,"%dth CS exit at %s by thread %lld\n", i+ 1, getTime(exitTime).c_str(), id + 1);
		if(j == id)
			lock = 0;
		else
			waitingThread[j] = false;
        //remainder section
		usleep(distribution2(generator) * 1e6);
	
	}
	pthread_exit(0);

}

int main(){
	in.open("inp-params.txt");
	in>>no_of_process>>k>>t1>>t2;

	waitingThread = new bool[no_of_process];

	pthread_t thread[no_of_process];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	for(int i = 0; i < no_of_process; i++){
		pthread_create(&thread[i],&attr, testCS, (void*)i);
	}
	for(int j = 0; j < no_of_process; j++){
		pthread_join(thread[j], NULL);
	}

    fclose(file);
    file = fopen("Average_times.txt","w");
	fprintf(file,"Average wait time of CAS - Bounded : %lf\n",waitTime/(no_of_process*k));
	fprintf(file,"Worst time of CAS - Bounded : %lf\n", worstTime);
}