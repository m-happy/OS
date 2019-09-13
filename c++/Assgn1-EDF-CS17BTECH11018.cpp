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
            this->q_s = false;
        }
        int process_no;
        int t; //processing_time
        int period; //period of the process.
        int k; //number of time process repeats
        bool q_s; //status in queue
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
    in.open("inp-params.txt");
    in>>n;
    vector<Process> jobs(n);
    for(int i=0;i<n;i++){
        int process_no,t,p,k;
        in>>process_no>>t>>p>>k;
        jobs[i].set_attr(process_no,t,p,k,i);
    }
    in.close();
    write.open("EDF-Log.txt");
    vector<Process*> rq;
    Process running;
    running.on_cpu = false;
    int t=0;
    int fail=0;
    int total=0;
    double avg_wait_time =0,turnaround_time=0;
    for(int i=0;i<n;i++) total+=jobs[i].k;
    for(int i=0;i<n;i++){
        write<<"Process P"<<jobs[i].process_no<<": processing time="<<jobs[i].t<<"; deadline:"
        <<jobs[i].deadline<<"; period:"<<jobs[i].period<<" joined the system at time 0\n";
    }
    int index = 0;
    //for calculating total no of process
    for(int i = 0; i < n; i++){
        index += jobs[i].k;
    }
    while(true){
        //keeps running loop till all the process gets completed
        if(index <= 0)
            break;
        //pushes process in queue
        if(running.on_cpu ==true && t%running.period==0) {
            running.on_cpu = false;
            if(jobs[running.index].k>0){
                int x = running.index;
                jobs[x].q_s = true;
                rq.push_back(&jobs[x]);
            }
        }
        //checks if any process has any period 
        for(int i=0;i<n;i++){
            if(t%jobs[i].period==0 && jobs[i].k>0){
                if(jobs[i].q_s==false){
                    jobs[i].q_s=true;              

                    rq.push_back(&jobs[i]);
                }
                else{
                    index--;
                    write<<"Deadline miss by Process"<<jobs[i].process_no<<" at time "<<t<<endl;
                    turnaround_time+=jobs[i].period;                   
                    avg_wait_time+= t -  jobs[i].deadline+jobs[i].period - (jobs[i].t-jobs[i].time_left)+1;

                    fail++;
                    jobs[i].time_left = jobs[i].t;
                    jobs[i].deadline+=jobs[i].period;
                    jobs[i].k--;
                }
            }
        }
        //sorts based on deadline
        sort(rq.begin(),rq.end(),less_than_key()); 
        
        if(running.on_cpu==false){
            if(!rq.empty()){
                running = *rq[0];
                if(jobs[running.index].time_left == running.t){
                    write<<"Process P"<<running.process_no<<" starts execution at time "<<t<<".\n";
                }
                else{
                    write<<"Process P"<<running.process_no<<" resumes execution at time "<<t<<".\n";
                }
                rq.erase(rq.begin());
                jobs[running.index].q_s =false;
                running.on_cpu = true;
            }
            else{
                bool flag=true;
                while(flag==true){
                    t++;
                    int i=0;
                    while(i<n){
                        if(t%jobs[i].period==0 && jobs[i].k>0) {flag=false; break;}
                        i++;
                    }
                }
                write<<"CPU is idle till time "<<t<<".\n";
                continue;
            }
        }        
        if(!rq.empty() && rq[0]->deadline < jobs[running.index].deadline) { 
            Process r = *rq[0];

            jobs[r.index].q_s = false;
            rq.erase(rq.begin());
            rq.push_back(&jobs[running.index]);
            write<<"Process P"<<running.process_no<<" is preempted by Process P"<<r.process_no
            <<" at time "<<t<<". Remaining processing time:"<<jobs[running.index].time_left<<"\n";
            write<<"Process P"<<r.process_no<<" starts execution at time "<<t<<endl;
            jobs[running.index].q_s =true;
            running = r;
            running.on_cpu=true;
            jobs[running.index].time_left--;
        }
        jobs[running.index].time_left--; 
        if(jobs[running.index].time_left<=0) { 
                index--;
                write<<"Process P"<<running.process_no<<" finishes execution at time "<<t<<".\n";
                turnaround_time+=t-jobs[running.index].deadline+jobs[running.index].period;
                avg_wait_time+= t -  jobs[running.index].deadline+jobs[running.index].period - jobs[running.index].t+1;

                jobs[running.index].time_left = running.t;           
                jobs[running.index].k--;
                jobs[running.index].q_s=false;
                running.on_cpu = false;
                jobs[running.index].deadline += jobs[running.index].period;
        }
        t++;
    }
    write.close();
    write.open("EDF-Stats.txt");
    avg_wait_time = avg_wait_time/total;
    turnaround_time = turnaround_time/total;
    ofstream av;
    av.open("Average_times.txt",fstream::app | fstream::out | fstream::in);
    av<<"EDF :\n\tAverage wait time : "<<avg_wait_time<<"\n\tAverage turnaround time : "<<turnaround_time<<endl;

    write<<"Number of Processes entering the system : "<<total<<endl;
    write<<"Number of Processes successfully completed : "<<total-fail<<endl;
    write<<"Numer of Processes which missed the deadline : "<<fail<<endl; 
}