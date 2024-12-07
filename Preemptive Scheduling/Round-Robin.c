#include <stdio.h>

typedef struct process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time; // Time left for execution
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void roundRobinScheduling(Process proc[], int n, int time_quantum);
void displayTable(Process proc[], int n);
void findavgTime(Process proc[], int n);
void printGanttChart(int gantt_chart[], int exec_times[], int gc_size);

int main() {
    int n, time_quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];

    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1; // Process ID starts from 1
        printf("Enter arrival time and burst time for Process %d: ", proc[i].id);
        scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
        proc[i].remaining_time = proc[i].burst_time; // Initialize remaining time
    }

    printf("Enter the time quantum: ");
    scanf("%d", &time_quantum);

    roundRobinScheduling(proc, n, time_quantum);
    displayTable(proc, n);
    findavgTime(proc, n);

    return 0;
}

void roundRobinScheduling(Process proc[], int n, int time_quantum) {
    int time = 0, completed = 0, gantt_chart[1000], exec_times[1000], gc_index = 0;
    int queue[1000], front = 0, rear = 0;
    int visited[n];
    for (int i = 0; i < n; i++) visited[i] = 0;

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (proc[j].arrival_time > proc[j + 1].arrival_time) {
                Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }

    queue[rear++] = 0; 
    visited[0] = 1;

    while (completed < n) {
        if (front < rear) {
            int idx = queue[front++]; 
            gantt_chart[gc_index] = proc[idx].id;

            int exec_time = (proc[idx].remaining_time <= time_quantum)
                            ? proc[idx].remaining_time
                            : time_quantum;

            time += exec_time;
            exec_times[gc_index++] = exec_time;

            proc[idx].remaining_time -= exec_time;

            if (proc[idx].remaining_time == 0) {
                proc[idx].completion_time = time;
                completed++;
            }

            // Add new processes
            for (int i = 0; i < n; i++) {
                if (!visited[i] && proc[i].arrival_time <= time && proc[i].remaining_time > 0) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }

            if (proc[idx].remaining_time > 0) {
                queue[rear++] = idx; 
            }
        } else {
            time++; 
        }
    }

    // Calculate turnaround time and waiting time
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }

    printGanttChart(gantt_chart, exec_times, gc_index);
}

void displayTable(Process proc[], int n) {
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               proc[i].id, proc[i].arrival_time, proc[i].burst_time,
               proc[i].completion_time, proc[i].turnaround_time, proc[i].waiting_time);
    }
}

void findavgTime(Process proc[], int n) {
    int total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++) {
        total_wt += proc[i].waiting_time;
        total_tat += proc[i].turnaround_time;
    }

    printf("Average waiting time: %.2f\n", (float)total_wt / n);
    printf("Average turnaround time: %.2f\n", (float)total_tat / n);
}

void printGanttChart(int gantt_chart[], int exec_times[], int gc_size) {
    printf("\nGantt Chart:\n");

    // Print the top border
    for (int i = 0; i < gc_size; i++) {
        printf(" -------");
    }
    printf("\n|");

    // Print each process in the Gantt chart
    for (int i = 0; i < gc_size; i++) {
        printf("  P%d   |", gantt_chart[i]);
    }
    printf("\n");

    // Print the bottom border
    for (int i = 0; i < gc_size; i++) {
        printf(" -------");
    }
    printf("\n");

    // Print time intervals below each process
    int time = 0;
    printf("%d", time); // Print the initial time (0)
    for (int i = 0; i < gc_size; i++) {
        time += exec_times[i]; // Add actual execution time for each process slice
        printf("%8d", time); 
    }
    printf("\n");
}