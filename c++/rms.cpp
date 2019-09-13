#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
class Process{
    
    public:
        void construct_process(int processNo,int t,int p,int k,int i){
            process_no = processNo; 
            this->t=t;
            this->period = p;
            this->k = k;
            this->deadline = p;
            this->time_left = t;
            this->index = i;
            this->in_queue = false;
        }
        int process_no;
        int t; //processing_time
        int period; //deadline
        int k; //number of time procenss repeats
        bool in_queue; //status in queue
        int deadline; //deadline
        int index; 
        int time_left;
        bool on_cpu; //whether it is executing on CPU or not
};

struct less_than_key //Sorting function comparator
{
    inline bool operator() (Process* struct1, Process* struct2)
    {
        return (struct1->period < struct2->period);
    }
};

int main(){
    ofstream rep1;
    ifstream in;
    int n;
    in.open("inp-params.txt");
    // in.open("input.txt"); // Reads from inp.txt
    in>>n;
    vector<Process> jobs(n);
    for(int i=0;i<n;i++){
        int processNo,t,p,k;
        in>>processNo>>t>>p>>k;
        jobs[i].construct_process(processNo,t,p,k,i);
    }
    in.close();
    rep1.open("RM-Log1.txt");

    vector<Process*> rq;
    Process running;
    running.on_cpu = false;
    int t=0;
    int fail=0;
    int total=0;
    int index = 0;
    double turnaround_time=0;
    for(int i=0;i<n;i++) total+=jobs[i].k;

    for(int i=0;i<n;i++){
        rep1<<"Process P"<<jobs[i].process_no<<": processing time="<<jobs[i].t<<"; deadline:"
        <<jobs[i].deadline<<"; period:"<<jobs[i].period<<" joined the system at time 0\n";
    }
    double avg_wait_time =0;
    int arr[n];
    for(int i = 0; i < n; i++){
        index += jobs[i].k;
        arr[i] = jobs[i].period;
    }
    sort(arr, arr + n);
    for(int i = 0; i < n; i++)
        cout<<arr[i]<<endl;
    // cout<<index;
    while(index >= 0){

        for(int i = 0; i < n; i++){
            if(t % jobs[i].period == 0 && jobs[i].k > 0){
                if(jobs[i].in_queue == false){
                    jobs[i].in_queue = true;
                    cout<<i<<" pushed in ready queue"<<endl;
                    rq.push_back(&jobs[i]);
                }
                else{
                    index--;
                    // cout<<i<<" process exited from queue"<<endl;
                }
            }

        }
        if(!rq.empty()){
            running = *rq[0];
            if()
        }

    }
}