#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void initializeProcesses(Process proc[], int n);
void roundRobinScheduling(Process proc[], int n, int time_quantum);
void displayTable(Process proc[], int n);
void findavgTime(Process proc[], int n);
void printGanttChart(int gantt_chart[], int exec_times[], int gc_size);

int main() {
    int n, time_quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];
    initializeProcesses(proc, n);

    printf("Enter the time quantum: ");
    scanf("%d", &time_quantum);

    roundRobinScheduling(proc, n, time_quantum);
    displayTable(proc, n);
    findavgTime(proc, n);

    return 0;
}

void initializeProcesses(Process proc[], int n) {
	int i;
    for (i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("Enter arrival time and burst time for Process %d: ", proc[i].id);
        scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
        proc[i].remaining_time = proc[i].burst_time; 
    }
}

void roundRobinScheduling(Process proc[], int n, int time_quantum) {
    int time = 0, completed = 0, front = 0, rear = 0, gc_index = 0;
    int *queue = malloc(n * sizeof(int));
    int *gantt_chart = malloc(n * 100 * sizeof(int));
    int *exec_times = malloc(n * 100 * sizeof(int));
    int visited[n];
    int i, j, idx;
    memset(visited, 0, sizeof(visited));

    // Sort by arrival time
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
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
            idx = queue[front++];
            int exec_time = (proc[idx].remaining_time <= time_quantum) ? proc[idx].remaining_time : time_quantum;

            time += exec_time;
            proc[idx].remaining_time -= exec_time;

            if (gc_index > 0 && gantt_chart[gc_index - 1] == proc[idx].id) {
                exec_times[gc_index - 1] += exec_time;
            } else {
                gantt_chart[gc_index] = proc[idx].id;
                exec_times[gc_index++] = exec_time;
            }

            if (proc[idx].remaining_time == 0) {
                proc[idx].completion_time = time;
                completed++;
            }

            for (i = 0; i < n; i++) {
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

    for (i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }

    printGanttChart(gantt_chart, exec_times, gc_index);

    free(queue);
    free(gantt_chart);
    free(exec_times);
}

void displayTable(Process proc[], int n) {
	int i;
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", proc[i].id, proc[i].arrival_time, proc[i].burst_time, proc[i].completion_time, proc[i].turnaround_time, proc[i].waiting_time);
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
