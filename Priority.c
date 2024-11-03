#include <stdio.h>

typedef struct process{
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int  waiting_time;
}Process;

void sortProcesses(Process proc[], int n);
void findWaitingTime(Process proc[], int n);
void findTurnAroundTime(Process proc[], int n);
void findAvgTime(Process proc[], int n);
void printGanttChart(Process proc[], int n);

int main(){
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];

    for (int i = 0; i < n; i++){
        proc[i].id = i + 1; // Process ID starts from 1
        printf("Enter arrival time, burst time and priority for Process %d: ", proc[i].id);
        scanf("%d %d %d", &proc[i].arrival_time, &proc[i].burst_time, &proc[i].priority);
    }

    // Sort processes based on arrival time and priority
    sortProcesses(proc, n);

    // Calculate completion time
    for(int i = 0; i < n; i++){
        if (i == 0){
            proc[i].completion_time = proc[i].arrival_time + proc[i].burst_time;
        }
        else{
            proc[i].completion_time = proc[i - 1].completion_time + proc[i].burst_time;
        }
    }

    findWaitingTime(proc, n);
    findTurnAroundTime(proc, n);
    findAvgTime(proc, n);
    printGanttChart(proc, n);

    return 0;
}

// Function to sort processes based on arrival time and priority
void sortProcesses(Process proc[], int n){
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (proc[j].arrival_time > proc[j + 1].arrival_time || 
                (proc[j].arrival_time == proc[j + 1].arrival_time && proc[j].priority > proc[j + 1].priority)){
                Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }
}

// Function to calculate waiting time
void findWaitingTime(Process proc[], int n){
    proc[0].waiting_time = 0;

    for (int i = 1; i < n; i++){
        proc[i].waiting_time = proc[i - 1].completion_time - proc[i].arrival_time;
        if (proc[i].waiting_time < 0){
            proc[i].waiting_time = 0; // If process arrives after previous process completes
        }
    }
}

// Function to calculate turnaround time
void findTurnAroundTime(Process proc[], int n){
    for (int i = 0; i < n; i++){
        proc[i].turnaround_time = proc[i].burst_time + proc[i].waiting_time;
    }
}

// Function to calculate and display average waiting time and turnaround time
void findAvgTime(Process proc[], int n){
    int total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++){
        total_wt += proc[i].waiting_time;
        total_tat += proc[i].turnaround_time;
    }

    printf("Average waiting time: %.2f\n", (float)total_wt / n);
    printf("Average turnaround time: %.2f\n", (float)total_tat / n);
}

// Function to print Gantt Chart
void printGanttChart(Process proc[], int n) {
    int time = 0;

    printf("\nGantt Chart:\n");

    // Print top border with process IDs
    for (int i = 0; i < n; i++) {
        printf(" -------");
    }
    printf("\n|");

    for (int i = 0; i < n; i++) {
        printf("  P%d   |", proc[i].id);
    }
    printf("\n");

    // Print bottom border
    for (int i = 0; i < n; i++) {
        printf(" -------");
    }
    printf("\n");

    // Print time intervals below each process
    time = 0;
    printf("%d", time);
    for (int i = 0; i < n; i++) {
        time += proc[i].burst_time;
        printf("%8d", time);
    }
    printf("\n");
}