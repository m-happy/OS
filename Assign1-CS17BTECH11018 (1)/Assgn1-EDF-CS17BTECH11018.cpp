#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
//Process for
class Process{
    
    public:
        void set_attr(int process_no,int t,int p,int k,int i){
            process_no = process_no;
            this->t=t;
            this->period = p;
            this->k = k;
            this->deadline = p;
            this->time_left = t;
            this->index = i;
            this->queue_state = false;
        }
        int process_no;
        int t; //processing_time
        int period; //period of the process.
        int k; //number of time process repeats
        bool queue_state; //status in queue
        int deadline; //deadline
        int index; 
        int time_left;
        bool on_cpu; //whether it is executing on CPU or not
};
//function for sorting the queue
struct less_than_key 
{
    inline bool operator() (Process* struct1, Process* struct2)
    {
        return (struct1->deadline < struct2->deadline);
    }
};
//driver queue
int main(){
    ofstream write;
    ifstream in;
    int n;
    int t = 0;
    int missed = 0;

    int index1=0;
    double avg_wait_time =0;
    double turnaround_time=0;

    in.open("inp-params.txt");
    in>>n;
    vector<Process> processes(n);
    for(int i=0;i<n;i++){
        int process_no,t,p,k;
        in>>process_no>>t>>p>>k;
        processes[i].set_attr(process_no,t,p,k,i);
    }
    in.close();
    write.open("EDF-Log.txt");

    for(int i=0;i<n;i++)
         index1+=processes[i].k;
    for(int i=0;i<n;i++){
        write<<"Process P"<<processes[i].process_no<<": processing time="<<processes[i].t<<"; deadline:"
        <<processes[i].deadline<<"; period:"<<processes[i].period<<" joined the system at time 0\n";
    }
    int index = 0;
    //for calculating index1 no of process
    for(int i = 0; i < n; i++){
        index += processes[i].k;
    }

    vector <Process*> ready_queue;
    Process running;
    running.on_cpu = false;

    while(true){
        //keeps running loop till all the process gets completed
        if(index <= 0)
            break;
        //pushes process in queue
        if(running.on_cpu ==true && t % running.period==0) {
            running.on_cpu = false;
            if(processes[running.index].k>0){
                int x = running.index;
                processes[x].queue_state = true;
                ready_queue.push_back(&processes[x]);
            }
        }
        //checks if any process has any period 
        for(int i=0;i<n;i++){
            if(t%processes[i].period==0 && processes[i].k>0){
                if(processes[i].queue_state==false){
                    processes[i].queue_state=true;              

                    ready_queue.push_back(&processes[i]);
                }
                else{
                    index--;
                    write<<"Deadline miss by Process"<<processes[i].process_no<<" at time "<<t<<endl;
                    turnaround_time+=processes[i].period;                   
                    avg_wait_time+= t -  processes[i].deadline+processes[i].period - (processes[i].t-processes[i].time_left)+1;

                    missed++;
                    processes[i].time_left = processes[i].t;
                    processes[i].deadline+=processes[i].period;
                    processes[i].k--;
                }
            }
        }
        //sorts based on deadline
        sort(ready_queue.begin(),ready_queue.end(),less_than_key()); 
        
        if(running.on_cpu==false){
            if(!ready_queue.empty()){
                running = *ready_queue[0];
                if(processes[running.index].time_left == running.t){
                    write<<"Process P"<<running.process_no<<" starts execution at time "<<t<<".\n";
                }
                else{
                    write<<"Process P"<<running.process_no<<" resumes execution at time "<<t<<".\n";
                }
                ready_queue.erase(ready_queue.begin());
                processes[running.index].queue_state =false;
                running.on_cpu = true;
            }
            else{
                bool flag=true;
                while(flag==true){
                    t++;
                    int i=0;
                    while(i<n){
                        if(t%processes[i].period==0 && processes[i].k>0) {flag=false; break;}
                        i++;
                    }
                }
                write<<"CPU is idle till time "<<t<<".\n";

            }
        }        
        if(!ready_queue.empty() && ready_queue[0]->deadline < processes[running.index].deadline) { 
            Process r = *ready_queue[0];

            processes[r.index].queue_state = false;
            ready_queue.erase(ready_queue.begin());
            ready_queue.push_back(&processes[running.index]);
            write<<"Process P"<<running.process_no<<" is preempted by Process P"<<r.process_no
            <<" at time "<<t<<". Remaining processing time:"<<processes[running.index].time_left<<"\n";
            write<<"Process P"<<r.process_no<<" starts execution at time "<<t<<endl;
            processes[running.index].queue_state =true;
            running = r;
            running.on_cpu=true;
            processes[running.index].time_left--;
        }
        processes[running.index].time_left--; 
        if(processes[running.index].time_left<=0) { 
                index--;
                write<<"Process P"<<running.process_no<<" finishes execution at time "<<t<<".\n";
                turnaround_time+=t-processes[running.index].deadline+processes[running.index].period;
                avg_wait_time+= t -  processes[running.index].deadline+processes[running.index].period - processes[running.index].t+1;

                processes[running.index].time_left = running.t;           
                processes[running.index].k--;
                processes[running.index].queue_state=false;
                running.on_cpu = false;
                processes[running.index].deadline += processes[running.index].period;
        }
        t++;
    }
    write.close();
    write.open("EDF-Stats.txt");
    avg_wait_time = avg_wait_time/index1;
    turnaround_time = turnaround_time/index1;
    ofstream av;
    av.open("Average_times.txt",fstream::app | fstream::out | fstream::in);
    av<<"EDF :\n\tAverage wait time : "<<avg_wait_time<<"\n\tAverage turnaround time : "<<turnaround_time<<endl;

    write<<"Number of Processes entering the system : "<<index1<<endl;
    write<<"Number of Processes successfully completed : "<<index1-missed<<endl;
    write<<"Numer of Processes which missed the deadline : "<<missed<<endl; 
}