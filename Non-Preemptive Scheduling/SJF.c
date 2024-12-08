#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int original_burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void initializeProcesses(Process proc[], int n);
void sjfScheduling(Process proc[], int n);
void displayTable(Process proc[], int n);
void findavgTime(Process proc[], int n);
void printGanttChart(int gantt_chart[], int exec_times[], int gc_size);

int main() {
    int n;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process proc[n];
    initializeProcesses(proc, n);

    sjfScheduling(proc, n);
    displayTable(proc, n);
    findavgTime(proc, n);

    return 0;
}

void initializeProcesses(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("Enter arrival time, burst time for Process %d: ", proc[i].id);
        scanf("%d %d", &proc[i].arrival_time, &proc[i].burst_time);
        proc[i].original_burst_time = proc[i].burst_time; // Store the original burst time
    }
}

void sjfScheduling(Process proc[], int n) {
    int time = 0, completed = 0, gc_index = 0;
    int *gantt_chart = malloc(n * 100 * sizeof(int));
    int *exec_times = malloc(n * 100 * sizeof(int));

    // Sort processes based on arrival time first
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (proc[j].arrival_time < proc[i].arrival_time) {
                Process temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    while (completed < n) {
        int idx = -1;
        int shortest_burst = 10000; // Arbitrary large number for comparison

        // Find the process that has arrived and has the shortest burst time
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= time && proc[i].burst_time > 0 && proc[i].burst_time < shortest_burst) {
                idx = i;
                shortest_burst = proc[i].burst_time;
            }
        }

        if (idx == -1) {
            // No process is ready, advance time
            time++;
            continue;
        }

        gantt_chart[gc_index] = proc[idx].id;
        exec_times[gc_index++] = proc[idx].burst_time; // Execute the entire burst time of the selected process

        time += proc[idx].burst_time;
        proc[idx].completion_time = time;
        proc[idx].burst_time = 0; // Mark the process as completed
        completed++;
    }

    // Calculate turnaround time and waiting time
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].original_burst_time; // Use original burst time
    }

    printGanttChart(gantt_chart, exec_times, gc_index);

    free(gantt_chart);
    free(exec_times);
}

void displayTable(Process proc[], int n) {
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               proc[i].id, proc[i].arrival_time, proc[i].original_burst_time,
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
        time += exec_times[i]; // Add execution time for each process
        printf("%8d", time);
    }
    printf("\n");
}