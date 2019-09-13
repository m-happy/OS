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

FILE* file = fopen("CAS-Log.txt","w");
ifstream in ;
int no_of_process, k;
double t1, t2;
double waitTime, worstTime, diffTime;

string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  static char output[10];
  sprintf(output,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(output);
  return tim;
}

void *testCS(void *param){
	// cout<<thread[i].getID();
	long long id = (long long)param;
	// cout<<param<<endl;	
	default_random_engine generator;
	exponential_distribution <double> distribution1 (1.0/t1);
	exponential_distribution <double> distribution2 (1.0/t2);
	for(int i = 0; i < k; i++){
		
        time_t reqEnterTime = time(NULL);
        fprintf(file,"%dth CS request at %s by thread %lld\n", i+ 1, getTime(reqEnterTime).c_str(), id + 1);
        while(1){
        	int x = 0, y = 1;
            if(lock.compare_exchange_strong(x ,y)) break;
		}
        //critical section
        time_t actEnterTime = time(NULL);
        diffTime = actEnterTime - reqEnterTime;
        waitTime += diffTime;
        fprintf(file,"%dth CS entry at %s by thread %lld\n", i+ 1, getTime(actEnterTime).c_str(), id + 1);
		usleep(distribution1(generator) * 1e6);		
		if(worstTime < diffTime){
			worstTime = diffTime;
		}
        time_t exitTime = time(NULL);
        fprintf(file,"%dth CS exit at %s by thread %lld\n", i+ 1, getTime(exitTime).c_str(), id + 1);
        lock = 0;
        //remainder section
		usleep(distribution2(generator) *  1e6);
	
	}
	pthread_exit(0);

}

int main(){
	in.open("inp-params.txt");
	in>>no_of_process>>k>>t1>>t2;
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
	fprintf(file,"Average wait time of CAS : %lf\n",waitTime/(no_of_process*k));
	fprintf(file,"Worst time of CAS - Bounded : %lf\n", worstTime);
}