#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void initializeProcesses(Process proc[], int n);
void preemptivePriorityScheduling(Process proc[], int n);
void displayTable(Process proc[], int n);
void findavgTime(Process proc[], int n);
void printGanttChart(int gantt_chart[], int exec_times[], int gc_size);

int main() {
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];
    initializeProcesses(proc, n);

    preemptivePriorityScheduling(proc, n);
    displayTable(proc, n);
    findavgTime(proc, n);

    return 0;
}

void initializeProcesses(Process proc[], int n) {
	int i;
    for (i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("Enter arrival time, burst time, and priority for Process %d: ", proc[i].id);
        scanf("%d %d %d", &proc[i].arrival_time, &proc[i].burst_time, &proc[i].priority);
        proc[i].remaining_time = proc[i].burst_time;
    }
}

void preemptivePriorityScheduling(Process proc[], int n) {
    int time = 0, completed = 0, gc_index = 0;
    int *gantt_chart = malloc(n * 100 * sizeof(int));
    int *exec_times = malloc(n * 100 * sizeof(int));
    int last_process = -1;
	int i;
	
    while (completed < n) {
        int min_priority = INT_MAX, idx = -1;

        // Find the highest priority process available at current time
        for (i = 0; i < n; i++) {
            if (proc[i].arrival_time <= time && proc[i].remaining_time > 0 && proc[i].priority < min_priority) {
                min_priority = proc[i].priority;
                idx = i;
            }
        }

        if (idx == -1) { 
            // No process ready, advance time
            time++;
            continue;
        }

        if (idx != last_process) {
            gantt_chart[gc_index] = proc[idx].id;
            exec_times[gc_index++] = 1;  // New process starts
        } else {
            exec_times[gc_index - 1]++;  // Continue current process
        }

        proc[idx].remaining_time--;
        time++;

        if (proc[idx].remaining_time == 0) {
            proc[idx].completion_time = time;
            completed++;
        }

        last_process = idx;
    }

    // Calculate turnaround time and waiting time
    for (i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }

    printGanttChart(gantt_chart, exec_times, gc_index);

    free(gantt_chart);
    free(exec_times);
}

void displayTable(Process proc[], int n) {
	int i;
    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               proc[i].id, proc[i].arrival_time, proc[i].burst_time,
               proc[i].priority, proc[i].completion_time, proc[i].turnaround_time, proc[i].waiting_time);
    }
}

void findavgTime(Process proc[], int n) {
    int total_wt = 0, total_tat = 0;
	int i;
	
    for (i = 0; i < n; i++) {
        total_wt += proc[i].waiting_time;
        total_tat += proc[i].turnaround_time;
    }

    printf("Average waiting time: %.2f\n", (float)total_wt / n);
    printf("Average turnaround time: %.2f\n", (float)total_tat / n);
}

void printGanttChart(int gantt_chart[], int exec_times[], int gc_size) {
	int i;
    printf("\nGantt Chart:\n");

    for (i = 0; i < gc_size; i++) {
        printf(" -------");
    }
    printf("\n|");

    for (i = 0; i < gc_size; i++) {
        printf("  P%d   |", gantt_chart[i]);
    }
    printf("\n");

    for (i = 0; i < gc_size; i++) {
        printf(" -------");
    }
    printf("\n");

    int time = 0;
    printf("%d", time);

    for (i = 0; i < gc_size; i++) {
        time += exec_times[i];
        printf("%8d", time);
    }
    printf("\n");
}
