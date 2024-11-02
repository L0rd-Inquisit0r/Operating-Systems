#include <stdio.h>

typedef struct process{
    int id;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
}Process;

void findWaitingTime(Process proc[], int in);
void findTurnAroundTime(Process proc[], int n);
void findavgTime(Process proc[], int n);
void printGanttChart(Process proc[], int n);

int main(){
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];

    for (int i = 0; i < n; i++){
        proc[i].id = i + 1; // Process ID starts from 1
        printf("Enter arrival time and burst time for Process %d: ", proc[i].id);
        scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
    }

    // Sorting processes by arrival time (FCFS)
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (proc[j].arrival_time > proc[j + 1].arrival_time){
                Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }

    // Calculate completion time
    for (int i = 0; i < n; i++){
        if (i == 0){
            proc[i].completion_time = proc[i].arrival_time + proc[i].burst_time;
        }
        else{
            proc[i].completion_time = proc[i - 1].completion_time + proc[i].burst_time;
        }
    }

    findWaitingTime(proc, n);
    findTurnAroundTime(proc, n);
    findavgTime(proc, n);
    printGanttChart(proc, n);

    return 0;
}

void findWaitingTime(Process proc[], int n){
    proc[0].waiting_time = 0;

    for (int i = 1; i < n; i++){
        proc[i].waiting_time = proc[i - 1].completion_time - proc[i].arrival_time;
        if (proc[i].waiting_time < 0){
            proc[i].waiting_time = 0; // If process arrives after previous process completes
        }
    }
}

void findTurnAroundTime(Process proc[], int n){
    for (int i = 0; i < n; i++){
        proc[i].turnaround_time = proc[i].burst_time + proc[i].waiting_time;
    }
}

void findavgTime(Process proc[], int n){
    int total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++){
        total_wt += proc[i].waiting_time;
        total_tat += proc[i].turnaround_time;
    }

    printf("Average waiting time: %.2f\n", (float)total_wt / n);
    printf("Average turnaround time: %.2f\n", (float)total_tat / n);
}

void printGanttChart(Process proc[], int n){
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; i++){
        printf("| P%d ", proc[i].id);
    }
    printf("|\n");

    int time = 0;
    for (int i = 0; i < n; i++){
        time += proc[i].burst_time;
        printf("%5d ", time);
    }
    printf("\n");
}