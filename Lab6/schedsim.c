// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"


// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    int *rem_bt = (int *)malloc(n * sizeof(int));
    if (!rem_bt) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    int *arrival_queue = (int *)malloc(n * sizeof(int));
    if (!arrival_queue) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(rem_bt);
        exit(1);
    }
    
    int *in_queue = (int *)calloc(n, sizeof(int));
    if (!in_queue) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(rem_bt);
        free(arrival_queue);
        exit(1);
    }
    
    int t = 0;
    int completed = 0;
    int queue_front = 0, queue_rear = 0;
    
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
    }
    
    for (int i = 0; i < n; i++) {
        if (plist[i].art <= t) {
            arrival_queue[queue_rear++] = i;
            in_queue[i] = 1;
        }
    }
    
    while (completed < n) {
        if (queue_front == queue_rear) {
            int next_arrival = -1;
            int min_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (rem_bt[i] > 0 && !in_queue[i] && plist[i].art < min_arrival) {
                    min_arrival = plist[i].art;
                    next_arrival = i;
                }
            }
            if (next_arrival != -1) {
                t = plist[next_arrival].art;
                arrival_queue[queue_rear++] = next_arrival;
                in_queue[next_arrival] = 1;
            } else {
                break;
            }
        }
        
        int idx = arrival_queue[queue_front++];
        
        if (rem_bt[idx] > quantum) {
            t += quantum;
            rem_bt[idx] -= quantum;
        } else {
            t += rem_bt[idx];
            plist[idx].wt = t - plist[idx].art - plist[idx].bt;
            if (plist[idx].wt < 0)
                plist[idx].wt = 0;
            rem_bt[idx] = 0;
            completed++;
        }
        
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && rem_bt[i] > 0 && plist[i].art <= t) {
                arrival_queue[queue_rear++] = i;
                in_queue[i] = 1;
            }
        }
        
        if (rem_bt[idx] > 0) {
            arrival_queue[queue_rear++] = idx;
        }
    }
    
    free(rem_bt);
    free(arrival_queue);
    free(in_queue);
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
    int *rem_bt = (int *)malloc(n * sizeof(int));
    if (!rem_bt) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    int *completion_time = (int *)malloc(n * sizeof(int));
    if (!completion_time) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(rem_bt);
        exit(1);
    }
    
    int completed = 0, current_time = 0, min_idx;
    int min_bt;
    
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
    }
    
    while (completed != n) {
        min_idx = -1;
        min_bt = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (plist[i].art <= current_time && rem_bt[i] > 0) {
                if (rem_bt[i] < min_bt) {
                    min_bt = rem_bt[i];
                    min_idx = i;
                }
                else if (rem_bt[i] == min_bt && min_idx != -1) {
                    if (plist[i].art < plist[min_idx].art) {
                        min_idx = i;
                    }
                }
            }
        }
        
        if (min_idx == -1) {
            current_time++;
            continue;
        }
        
        rem_bt[min_idx]--;
        current_time++;
        
        if (rem_bt[min_idx] == 0) {
            completed++;
            completion_time[min_idx] = current_time;
            plist[min_idx].wt = completion_time[min_idx] - plist[min_idx].art - plist[min_idx].bt;
            
            if (plist[min_idx].wt < 0)
                plist[min_idx].wt = 0;
        }
    }
    
    free(rem_bt);
    free(completion_time);
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    int *service_time = (int *)malloc(n * sizeof(int));
    if (!service_time) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    int *completion_time = (int *)malloc(n * sizeof(int));
    if (!completion_time) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(service_time);
        exit(1);
    }
    
    service_time[0] = plist[0].art;
    completion_time[0] = plist[0].art + plist[0].bt;
    plist[0].wt = 0;
    
    for (int i = 1; i < n; i++) {
        if (completion_time[i-1] < plist[i].art) {
            service_time[i] = plist[i].art;
        } else {
            service_time[i] = completion_time[i-1];
        }
        plist[i].wt = service_time[i] - plist[i].art;
        if (plist[i].wt < 0)
            plist[i].wt = 0;
        completion_time[i] = service_time[i] + plist[i].bt;
    }
    
    free(service_time);
    free(completion_time);
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
    ProcessType *p1 = (ProcessType *)this;
    ProcessType *p2 = (ProcessType *)that;
    
    if (p1->pri > p2->pri)
        return -1;
    else if (p1->pri < p2->pri)
        return 1;
    else
        return 0;
} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
    qsort(plist, n, sizeof(ProcessType), my_comparer);
    
    int *service_time = (int *)malloc(n * sizeof(int));
    if (!service_time) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    int *completion_time = (int *)malloc(n * sizeof(int));
    if (!completion_time) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(service_time);
        exit(1);
    }
    
    service_time[0] = plist[0].art;
    completion_time[0] = plist[0].art + plist[0].bt;
    plist[0].wt = 0;
    
    for (int i = 1; i < n; i++) {
        if (completion_time[i-1] < plist[i].art) {
            service_time[i] = plist[i].art;
        } else {
            service_time[i] = completion_time[i-1];
        }
        plist[i].wt = service_time[i] - plist[i].art;
        if (plist[i].wt < 0)
            plist[i].wt = 0;
        completion_time[i] = service_time[i] + plist[i].bt;
    }
    
    free(service_time);
    free(completion_time);
    
    findTurnAroundTime(plist, n);
    
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
    
    free(proc_list);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
    
    free(proc_list);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
    free(proc_list);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    free(proc_list);
    
    return 0; 
}