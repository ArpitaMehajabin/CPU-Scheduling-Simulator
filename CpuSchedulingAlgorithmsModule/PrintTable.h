#ifndef __PRINT__TABLE__
#define __PRINT__TABLE__

// Print Table for Process Scheduling Results

/* Include user-defined headers */
#include "./Process.h"
#include "./SortingFunction.h"
#include <string.h>
#include <limits.h>

/**
 * Print a table of process scheduling results, including a Gantt Chart
 * @param p         Array of Process structs
 * @param n         Number of processes
 * @param algorithm Name of the scheduling algorithm (e.g., "RR", "FCFS")
 * @param quantum   Time quantum for Round Robin (ignored for other algorithms)
 */
void print_table(Process p[], int n, const char *algorithm, Quantum quantum)
{
    int i;

    /* Validate inputs */
    if (n <= 0 || p == NULL || algorithm == NULL)
    {
        printf("Error: Invalid input to print_table (n=%d, p=%p, algorithm=%p)\n", n, (void *)p, (void *)algorithm);
        return;
    }

    /* Print table header */
    printf("\n\t%s Scheduling Results:\n", algorithm);
    puts("\t+-----+------------+-------------+----------+-------------+--------------+-----------------+");
    puts("\t| PID | Burst Time | Arrive Time | Priority | Finish Time | Waiting Time | Turnaround Time |");
    puts("\t+-----+------------+-------------+----------+-------------+--------------+-----------------+");

    /* Print process details */
    for (i = 0; i < n; i++)
    {
        /* Display priority as "-" for algorithms that don't use it */
        const char *priority_str = (strcmp(algorithm, "PPS") == 0 || strcmp(algorithm, "NPPS") == 0)
                                   ? (char[4]){0} : "-";
        if (priority_str[0] != '-')
        {
            snprintf((char *)priority_str, 4, "%d", p[i].priority);
        }

        printf("\t| %3s |     %3d    |     %3d     |    %3s   |     %3d     |      %3d     |        %3d      |\n",
               p[i].id, p[i].burst, p[i].arrive_time, priority_str, p[i].finish_time, p[i].waiting, p[i].turnaround);
        puts("\t+-----+------------+-------------+----------+-------------+--------------+-----------------+");
    }

    /* Calculate and print averages */
    float avg_waiting = 0, avg_turnaround = 0;
    for (i = 0; i < n; i++)
    {
        avg_waiting += p[i].waiting;
        avg_turnaround += p[i].turnaround;
    }
    avg_waiting /= n;
    avg_turnaround /= n;

    printf("\n\tAverage Waiting Time: %.2f\n", avg_waiting);
    printf("\tAverage Turnaround Time: %.2f\n\n", avg_turnaround);

    /* Generate Gantt Chart based on the algorithm */
    printf("\tGantt Chart:\n");

    /* For Round Robin (RR), simulate the scheduling to show time slices */
    if (strcmp(algorithm, "RR") == 0)
    {
        /* Allocate arrays for simulation */
        int *remaining = (int *)malloc(sizeof(int) * n);
        if (remaining == NULL)
        {
            printf("Error: Memory allocation failed for remaining array\n");
            return;
        }
        for (i = 0; i < n; i++)
        {
            remaining[i] = p[i].burst;
        }

        int current_time = 0;
        int completed = 0;
        /* Allocate a larger queue to handle multiple time slices (e.g., 10 * n) */
        int queue_size = 10 * n;  // Arbitrary large size to handle all possible enqueues
        int *queue = (int *)malloc(sizeof(int) * queue_size);
        if (queue == NULL)
        {
            printf("Error: Memory allocation failed for queue array\n");
            free(remaining);
            return;
        }
        int front = 0, rear = -1;
        int *in_queue = (int *)calloc(n, sizeof(int));
        if (in_queue == NULL)
        {
            printf("Error: Memory allocation failed for in_queue array\n");
            free(remaining);
            free(queue);
            return;
        }

        /* Create a sorted array of indices based on arrival time */
        int *sorted_indices = (int *)malloc(sizeof(int) * n);
        if (sorted_indices == NULL)
        {
            printf("Error: Memory allocation failed for sorted_indices array\n");
            free(remaining);
            free(queue);
            free(in_queue);
            return;
        }
        for (i = 0; i < n; i++)
        {
            sorted_indices[i] = i;
        }

        /* Create a temporary array of processes to sort by arrival time */
        Process *temp_p = (Process *)malloc(sizeof(Process) * n);
        if (temp_p == NULL)
        {
            printf("Error: Memory allocation failed for temp_p array\n");
            free(remaining);
            free(queue);
            free(in_queue);
            free(sorted_indices);
            return;
        }
        for (i = 0; i < n; i++)
        {
            temp_p[i] = p[i];
        }

        /* Sort processes by arrival time using merge_sort_by_arrive_time */
        merge_sort_by_arrive_time(temp_p, 0, n - 1);

        /* Update sorted_indices based on the sorted order */
        for (i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (strcmp(temp_p[i].id, p[j].id) == 0)
                {
                    sorted_indices[i] = j;
                    break;
                }
            }
        }

        /* Free the temporary array */
        free(temp_p);

        /* Add processes that have arrived at time 0, in order of arrival */
        for (i = 0; i < n; i++)
        {
            int idx = sorted_indices[i];
            if (p[idx].arrive_time <= current_time && !in_queue[idx])
            {
                if (rear + 1 >= queue_size)
                {
                    printf("Error: Queue overflow in RR simulation\n");
                    free(remaining);
                    free(queue);
                    free(in_queue);
                    free(sorted_indices);
                    return;
                }
                rear++;
                queue[rear] = idx;
                in_queue[idx] = 1;
            }
        }

        /* Print Gantt Chart header */
        printf("\t|");

        /* Simulate Round Robin to generate Gantt Chart */
        int prev_idx = -1;
        while (completed < n)
        {
            /* If the queue is empty, increment time until a process arrives */
            if (front > rear)
            {
                int next_arrival = INT_MAX;
                for (i = 0; i < n; i++)
                {
                    if (!in_queue[i] && p[i].arrive_time < next_arrival)
                    {
                        next_arrival = p[i].arrive_time;
                    }
                }
                if (next_arrival == INT_MAX) break;
                if (prev_idx != -1)
                {
                    printf(" %s |", p[prev_idx].id);
                }
                printf(" Idle |");
                current_time = next_arrival;
                for (i = 0; i < n; i++)
                {
                    int idx = sorted_indices[i];
                    if (p[idx].arrive_time <= current_time && !in_queue[idx])
                    {
                        if (rear + 1 >= queue_size)
                        {
                            printf("Error: Queue overflow in RR simulation\n");
                            free(remaining);
                            free(queue);
                            free(in_queue);
                            free(sorted_indices);
                            return;
                        }
                        rear++;
                        queue[rear] = idx;
                        in_queue[idx] = 1;
                    }
                }
                prev_idx = -1;
                continue;
            }

            /* Process the next process in the queue */
            if (front >= queue_size)
            {
                printf("Error: Queue underflow in RR simulation\n");
                free(remaining);
                free(queue);
                free(in_queue);
                free(sorted_indices);
                return;
            }
            int idx = queue[front];
            front++;
            int time_slice = (remaining[idx] > quantum) ? quantum : remaining[idx];
            current_time += time_slice;
            remaining[idx] -= time_slice;

            /* Add processes that have arrived during this time slice, in order of arrival */
            for (i = 0; i < n; i++)
            {
                int idx2 = sorted_indices[i];
                if (p[idx2].arrive_time <= current_time && !in_queue[idx2])
                {
                    if (rear + 1 >= queue_size)
                    {
                        printf("Error: Queue overflow in RR simulation\n");
                        free(remaining);
                        free(queue);
                        free(in_queue);
                        free(sorted_indices);
                        return;
                    }
                    rear++;
                    queue[rear] = idx2;
                    in_queue[idx2] = 1;
                }
            }

            /* If the process is not finished, add it back to the queue */
            if (remaining[idx] > 0)
            {
                if (rear + 1 >= queue_size)
                {
                    printf("Error: Queue overflow in RR simulation\n");
                    free(remaining);
                    free(queue);
                    free(in_queue);
                    free(sorted_indices);
                    return;
                }
                rear++;
                queue[rear] = idx;
            }
            else
            {
                completed++;
            }

            /* Print the process if it changes or ends */
            if (prev_idx != idx && prev_idx != -1)
            {
                printf(" %s |", p[prev_idx].id);
            }
            prev_idx = idx;
        }

        /* Print the last process */
        if (prev_idx != -1)
        {
            printf(" %s |", p[prev_idx].id);
        }

        /* Print time markers below the Gantt Chart */
        printf("\n\t%d", p[sorted_indices[0]].arrive_time);
        completed = 0;
        current_time = 0;
        front = 0;
        rear = -1;
        for (i = 0; i < n; i++)
        {
            in_queue[i] = 0;
            remaining[i] = p[i].burst;
        }

        /* Add processes that have arrived at time 0, in order of arrival */
        for (i = 0; i < n; i++)
        {
            int idx = sorted_indices[i];
            if (p[idx].arrive_time <= current_time && !in_queue[idx])
            {
                if (rear + 1 >= queue_size)
                {
                    printf("Error: Queue overflow in RR simulation\n");
                    free(remaining);
                    free(queue);
                    free(in_queue);
                    free(sorted_indices);
                    return;
                }
                rear++;
                queue[rear] = idx;
                in_queue[idx] = 1;
            }
        }

        prev_idx = -1;
        while (completed < n)
        {
            if (front > rear)
            {
                int next_arrival = INT_MAX;
                for (i = 0; i < n; i++)
                {
                    if (!in_queue[i] && p[i].arrive_time < next_arrival)
                    {
                        next_arrival = p[i].arrive_time;
                    }
                }
                if (next_arrival == INT_MAX) break;
                if (prev_idx != -1)
                {
                    printf("%*d", 7, current_time);
                }
                current_time = next_arrival;
                for (i = 0; i < n; i++)
                {
                    int idx = sorted_indices[i];
                    if (p[idx].arrive_time <= current_time && !in_queue[idx])
                    {
                        if (rear + 1 >= queue_size)
                        {
                            printf("Error: Queue overflow in RR simulation\n");
                            free(remaining);
                            free(queue);
                            free(in_queue);
                            free(sorted_indices);
                            return;
                        }
                        rear++;
                        queue[rear] = idx;
                        in_queue[idx] = 1;
                    }
                }
                prev_idx = -1;
                continue;
            }

            if (front >= queue_size)
            {
                printf("Error: Queue underflow in RR simulation\n");
                free(remaining);
                free(queue);
                free(in_queue);
                free(sorted_indices);
                return;
            }
            int idx = queue[front];
            front++;
            int time_slice = (remaining[idx] > quantum) ? quantum : remaining[idx];
            current_time += time_slice;
            remaining[idx] -= time_slice;

            for (i = 0; i < n; i++)
            {
                int idx2 = sorted_indices[i];
                if (p[idx2].arrive_time <= current_time && !in_queue[idx2])
                {
                    if (rear + 1 >= queue_size)
                    {
                        printf("Error: Queue overflow in RR simulation\n");
                        free(remaining);
                        free(queue);
                        free(in_queue);
                        free(sorted_indices);
                        return;
                    }
                    rear++;
                    queue[rear] = idx2;
                    in_queue[idx2] = 1;
                }
            }

            if (remaining[idx] > 0)
            {
                if (rear + 1 >= queue_size)
                {
                    printf("Error: Queue overflow in RR simulation\n");
                    free(remaining);
                    free(queue);
                    free(in_queue);
                    free(sorted_indices);
                    return;
                }
                rear++;
                queue[rear] = idx;
            }
            else
            {
                completed++;
            }

            if (prev_idx != idx && prev_idx != -1)
            {
                printf("%*d", 7, current_time);
            }
            prev_idx = idx;
        }

        if (prev_idx != -1)
        {
            printf("%*d", 7, current_time);
        }
        printf("\n");

        /* Free allocated memory */
        free(remaining);
        free(queue);
        free(in_queue);
        free(sorted_indices);
    }
    /* For Shortest Remaining Time (SRT) and Preemptive Priority Scheduling (PPS), simulate preemptive scheduling */
    else if (strcmp(algorithm, "SRT") == 0 || strcmp(algorithm, "PPS") == 0)
    {
        /* Allocate arrays for simulation */
        int *remaining = (int *)malloc(sizeof(int) * n);
        if (remaining == NULL)
        {
            printf("Error: Memory allocation failed for remaining array\n");
            return;
        }
        int *is_completed = (int *)calloc(n, sizeof(int));
        if (is_completed == NULL)
        {
            printf("Error: Memory allocation failed for is_completed array\n");
            free(remaining);
            return;
        }
        for (i = 0; i < n; i++)
        {
            remaining[i] = p[i].burst;
        }

        int current_time = 0;
        int completed = 0;

        /* Print Gantt Chart header */
        printf("\t|");

        /* Simulate the scheduling to generate Gantt Chart */
        int prev_idx = -1;
        int slice_start = 0;
        while (completed < n)
        {
            int selected_idx = -1;
            int criterion = INT_MAX;  // Shortest remaining time for SRT, priority for PPS

            /* Find the next process to execute */
            for (i = 0; i < n; i++)
            {
                if (!is_completed[i] && p[i].arrive_time <= current_time && remaining[i] > 0)
                {
                    int value = (strcmp(algorithm, "SRT") == 0) ? remaining[i] : p[i].priority;
                    if (value < criterion)
                    {
                        criterion = value;
                        selected_idx = i;
                    }
                }
            }

            /* If no process is ready, increment time until the next process arrives */
            if (selected_idx == -1)
            {
                int next_arrival = INT_MAX;
                for (i = 0; i < n; i++)
                {
                    if (!is_completed[i] && p[i].arrive_time < next_arrival)
                    {
                        next_arrival = p[i].arrive_time;
                    }
                }
                if (next_arrival == INT_MAX) break;
                if (prev_idx != -1)
                {
                    printf(" %s |", p[prev_idx].id);
                }
                printf(" Idle |");
                current_time = next_arrival;
                prev_idx = -1;
                slice_start = current_time;
                continue;
            }

            /* Execute the selected process for 1 time unit */
            remaining[selected_idx]--;
            current_time++;

            /* If the process changes, print the previous process */
            if (prev_idx != selected_idx && prev_idx != -1)
            {
                printf(" %s |", p[prev_idx].id);
            }

            /* If the process is finished */
            if (remaining[selected_idx] == 0)
            {
                is_completed[selected_idx] = 1;
                completed++;
            }

            prev_idx = selected_idx;
        }

        /* Print the last process */
        if (prev_idx != -1)
        {
            printf(" %s |", p[prev_idx].id);
        }

        /* Print time markers below the Gantt Chart */
        printf("\n\t%d", p[0].arrive_time);
        completed = 0;
        current_time = 0;
        for (i = 0; i < n; i++)
        {
            remaining[i] = p[i].burst;
            is_completed[i] = 0;
        }

        prev_idx = -1;
        slice_start = 0;
        while (completed < n)
        {
            int selected_idx = -1;
            int criterion = INT_MAX;

            for (i = 0; i < n; i++)
            {
                if (!is_completed[i] && p[i].arrive_time <= current_time && remaining[i] > 0)
                {
                    int value = (strcmp(algorithm, "SRT") == 0) ? remaining[i] : p[i].priority;
                    if (value < criterion)
                    {
                        criterion = value;
                        selected_idx = i;
                    }
                }
            }

            if (selected_idx == -1)
            {
                int next_arrival = INT_MAX;
                for (i = 0; i < n; i++)
                {
                    if (!is_completed[i] && p[i].arrive_time < next_arrival)
                    {
                        next_arrival = p[i].arrive_time;
                    }
                }
                if (next_arrival == INT_MAX) break;
                if (prev_idx != -1)
                {
                    printf("%*d", 7, current_time);
                }
                current_time = next_arrival;
                prev_idx = -1;
                slice_start = current_time;
                continue;
            }

            remaining[selected_idx]--;
            current_time++;

            if (prev_idx != selected_idx && prev_idx != -1)
            {
                printf("%*d", 7, current_time);
            }

            if (remaining[selected_idx] == 0)
            {
                is_completed[selected_idx] = 1;
                completed++;
            }

            prev_idx = selected_idx;
        }

        if (prev_idx != -1)
        {
            printf("%*d", 7, current_time);
        }
        printf("\n");

        /* Free allocated memory */
        free(remaining);
        free(is_completed);
    }
    /* For non-preemptive algorithms (FCFS, SJF, HRN, NPPS), sort by finish time and display */
    else
    {
        /* Create a copy of the process array to sort */
        Process *sorted = (Process *)malloc(sizeof(Process) * n);
        if (sorted == NULL)
        {
            printf("Error: Memory allocation failed for sorted array\n");
            return;
        }
        for (i = 0; i < n; i++)
        {
            sorted[i] = p[i];
        }

        /* Sort by finish time */
        quick_sort_by_finish_time(sorted, n);

        /* Print Gantt Chart */
        printf("\t|");
        for (i = 0; i < n; i++)
        {
            printf(" %s |", sorted[i].id);
        }
        printf("\n\t%d", sorted[0].arrive_time);
        for (i = 0; i < n; i++)
        {
            printf("%*d", 7, sorted[i].finish_time);
        }
        printf("\n");

        /* Free allocated memory */
        free(sorted);
    }
}

#endif
