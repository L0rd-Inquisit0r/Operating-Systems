#include <stdio.h>

typedef struct process{
    int id;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
}Process;

void findWaitingTime(Process proc[], int n);
void findTurnAroundTime(Process proc[], int n);
void displayTable(Process proc[], int n); 
void findavgTime(Process proc[], int n);
void printGanttChart(Process proc[], int n);

int main(){
    int i, j, n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];

    for (i = 0; i < n; i++){
        proc[i].id = i + 1; // Process ID starts from 1
        printf("Enter arrival time and burst time for Process %d: ", proc[i].id);
        scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
    }

    // Sorting processes by arrival time (FCFS)
    for (i = 0; i < n - 1; i++){
        for (j = 0; j < n - i - 1; j++){
            if (proc[j].arrival_time > proc[j + 1].arrival_time){
                Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }

    // Calculate completion time
    for (i = 0; i < n; i++){
        if (i == 0){
            proc[i].completion_time = proc[i].arrival_time + proc[i].burst_time;
        }
        else{
            // If the arrival time of the current process is less than or equal to the completion time of the previous process
            if (proc[i].arrival_time <= proc[i - 1].completion_time) {
                proc[i].completion_time = proc[i - 1].completion_time + proc[i].burst_time;
            } else {
                proc[i].completion_time = proc[i].arrival_time + proc[i].burst_time;
            }
        }
    }

    findWaitingTime(proc, n);
    findTurnAroundTime(proc, n);
    displayTable(proc, n);
    findavgTime(proc, n);
    printGanttChart(proc, n);

    return 0;
}

void findWaitingTime(Process proc[], int n){
	int i;
    proc[0].waiting_time = 0;

    for (i = 1; i < n; i++){
        // If the process arrives after the previous process finishes, it doesn't wait
        if (proc[i].arrival_time >= proc[i - 1].completion_time) {
            proc[i].waiting_time = 0;
        } else {
            proc[i].waiting_time = proc[i - 1].completion_time - proc[i].arrival_time;
        }
    }
}

void findTurnAroundTime(Process proc[], int n){
	int i;
    for (i = 0; i < n; i++){
        proc[i].turnaround_time = proc[i].burst_time + proc[i].waiting_time;
    }
}

void displayTable(Process proc[], int n){
	int i;
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               proc[i].id, proc[i].arrival_time, proc[i].burst_time,
               proc[i].completion_time, proc[i].turnaround_time, proc[i].waiting_time);
    }
}

void findavgTime(Process proc[], int n){
    int total_wt = 0, total_tat = 0;
	int i;
	
    for (i = 0; i < n; i++){
        total_wt += proc[i].waiting_time;
        total_tat += proc[i].turnaround_time;
    }

    printf("Average waiting time: %.2f\n", (float)total_wt / n);
    printf("Average turnaround time: %.2f\n", (float)total_tat / n);
}

// Function to print Gantt Chart
void printGanttChart(Process proc[], int n) {
	int i;
    int time = 0;

    printf("\nGantt Chart:\n");

    // Print top border with process IDs
    for (i = 0; i < n; i++) {
        printf(" -------");
    }
    printf("\n|");

    for (i = 0; i < n; i++) {
        printf("  P%d   |", proc[i].id);
    }
    printf("\n");

    // Print bottom border
    for (i = 0; i < n; i++) {
        printf(" -------");
    }
    printf("\n");

    // Print time intervals below each process
    time = 0;
    printf("%d", time);
    for (i = 0; i < n; i++) {
        time += proc[i].burst_time;
        printf("%8d", time);
    }
    printf("\n");
}
