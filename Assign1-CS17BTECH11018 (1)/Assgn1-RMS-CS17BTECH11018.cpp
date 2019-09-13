#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
class Process{
    /* hello */
    
    public:
        void construct_process(int processNo, int time, int period, int k, int index){
            process_no = processNo; 
            this->t = time;
            this->period = period;
            this->k = k;
            this->deadline = period;
            this->time_left = time;
            this->index = index;
            this->queue_state = false;
        }
        int process_no;
        int t; //processing_time
        int period; //deadline
        int k; //number of time procenss repeats
        bool queue_state; //status in queue
        int deadline; //deadline
        int index; 
        int time_left;
        bool on_cpu; //whether it is executing on CPU or not
};
//struct function to compare periods
struct sorting_function
{
    inline bool operator() (Process* process1, Process* process2)
    {
        return (process1->period < process2->period);
    }
};


//Driver function
int main(){
	ofstream write;
	ifstream in;

    int n;
    vector<Process*> rq;
    Process running;
    running.on_cpu = false;
    int t=0;
    int missed=0;
    int index1=0;
    double turnaround_time = 0;
    double average_waiting_time  =0;
    bool preempt = false;
    in.open("inp-params.txt");
    in>>n;

    vector<Process> processes(n);

    for(int i=0;i<n;i++){
        int processNo,t,p,k;
        in>>processNo>>t>>p>>k;
        processes[i].construct_process(processNo,t,p,k,i);
    }
    in.close();

    write.open("RM-Log.txt");
    for(int i=0;i<n;i++)
         index1+=processes[i].k;

    for(int i=0;i<n;i++){
        write<<"Process P"<<processes[i].process_no<<": processing time="<<processes[i].t<<"; deadline:"
        <<processes[i].deadline<<"; period:"<<processes[i].period<<" joined the system at time 0\n";
    }
    int index = 0;
    for(int i = 0;i< n; i++){
    	index += processes[i].k;
    }
    while(true){
    	if(index <= 0)
    		break;
        if(running.on_cpu ==true && t%running.period==0) {
            running.on_cpu = false;
            if(processes[running.index].k>0){
                int x = running.index;
                processes[x].queue_state = true;
                rq.push_back(&processes[x]);
            }
        }
        for(int i=0;i<n;i++){
            if(t%processes[i].period==0 && processes[i].k>0){
                if(processes[i].queue_state==false){
                    processes[i].queue_state=true;
                    rq.push_back(&processes[i]);
                }
                else{
                    turnaround_time+=processes[i].period; 
                    average_waiting_time += t -  processes[i].deadline+processes[i].period - (processes[i].t-processes[i].time_left)+1;
                    index--;
   					write<<"Deadline miss by Process"<<processes[i].process_no<<" at time "<<t<<endl;
                    missed++;
                    processes[i].time_left = processes[i].t;
                    processes[i].deadline+=processes[i].period;
                    processes[i].k--;
                    // t--;
                }
            }
        }
        sort(rq.begin(),rq.end(),sorting_function()); 
        if(running.on_cpu==false){
            if(!rq.empty()){
                running = *rq[0];
                if(processes[running.index].time_left == running.t){
                    write<<"Process P"<<running.process_no<<" starts execution at time "<<t<<".\n";
                }
                else{
                    write<<"Process P"<<running.process_no<<" resumes execution at time "<<t<<".\n";
                }
                rq.erase(rq.begin()); 
                processes[running.index].queue_state =false;
                running.on_cpu = true;
                // t--;
            }
            else{
                bool flag=true;
                while(flag==true){
                    t++;
                    int i=0;
                    while(i<n){
                        if(t % processes[i].period==0 && processes[i].k > 0){
                            flag=false;
                            break;
                        }
                        i++;
                    }
                }
                write<<"CPU is idle till time "<<t<<".\n";
                // t--;
            }
        }
        if(!rq.empty() && rq[0]->period < processes[running.index].period) {
            Process r = *rq[0];
            processes[r.index].queue_state = false;
            rq.erase(rq.begin());
            rq.push_back(&processes[running.index]);
            write<<"Process P"<<running.process_no<<" is preempted by Process P"<<r.process_no
            <<" at time "<<t<<". Remaining processing time:"<<processes[running.index].time_left<<"\n";
            write<<"Process P"<<r.process_no<<" starts execution at time "<<t<<endl;
            processes[running.index].queue_state =true;
            running = r; 	
            running.on_cpu=true;
            preempt = true;
        }
        processes[running.index].time_left--;
        if(processes[running.index].time_left <= 0) {
        		index--;
                write<<"Process P"<<running.process_no<<" finishes execution at time "<<t + 1<<".\n";
                turnaround_time+=t-processes[running.index].deadline+processes[running.index].period;
                average_waiting_time += t -  processes[running.index].deadline+processes[running.index].period - processes[running.index].t+1;
                processes[running.index].time_left = running.t;           
                processes[running.index].k--;
                processes[running.index].queue_state = false;
                running.on_cpu = false;
                processes[running.index].deadline += processes[running.index].period;
                // t--;
        }
       	t++;
    }
    average_waiting_time  = average_waiting_time /index1;
    turnaround_time = turnaround_time/index1;

    write.open("Average_times.txt");
    write<<"RMS : \n\tAverage wait time : "<<average_waiting_time <<"\n\tAverage turnaround time : "<<turnaround_time<<endl;
    write.close();
    write.close();


    write.open("RM-Stats.txt");
    write<<"Number of Processes entering the system : "<<index1<<endl;
    write<<"Number of Processes successfully completed : "<<index1-missed<<endl;
    write<<"Numer of Processes which missed the deadline : "<<missed<<endl;
    write.close();
}