#ifndef __Round__ROBIN__
#define __Round__ROBIN__

// Round Robin Scheduling

/* Include user-defined headers */
#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"
#include <limits.h>

/**
 * Execute Round Robin scheduling algorithm
 * @param p       Array of Process structs
 * @param len     Number of processes
 * @param quantum Time quantum for Round Robin
 */
void RR(Process *p, int len, Quantum quantum)
{
    int i;
    // Loop variable
    int current_time = 0;
    // Current time
    int completed = 0;
    // Number of completed processes

    /* Allocate arrays for remaining burst times and queue */
    int *remaining = (int *)malloc(sizeof(int) * len);
    if (remaining == NULL)
    {
        printf("Error: Memory allocation failed for remaining array\n");
        return;
    }

    /* Allocate a larger queue to handle multiple time slices (e.g., 10 * len) */
    int queue_size = 10 * len;  // Arbitrary large size to handle all possible enqueues
    int *queue = (int *)malloc(sizeof(int) * queue_size);
    if (queue == NULL)
    {
        printf("Error: Memory allocation failed for queue array\n");
        free(remaining);
        return;
    }

    int *in_queue = (int *)calloc(len, sizeof(int));
    if (in_queue == NULL)
    {
        printf("Error: Memory allocation failed for in_queue array\n");
        free(remaining);
        free(queue);
        return;
    }

    int front = 0, rear = -1;

    /* Initialize remaining burst times and process fields */
    for (i = 0; i < len; i++)
    {
        remaining[i] = p[i].burst;
        p[i].waiting = 0;
        p[i].turnaround = 0;
        p[i].finish_time = 0;
        in_queue[i] = 0;
    }

    /* Create a sorted array of indices based on arrival time */
    int *sorted_indices = (int *)malloc(sizeof(int) * len);
    if (sorted_indices == NULL)
    {
        printf("Error: Memory allocation failed for sorted_indices array\n");
        free(remaining);
        free(queue);
        free(in_queue);
        return;
    }
    for (i = 0; i < len; i++)
    {
        sorted_indices[i] = i;
    }

    /* Create a temporary array of processes to sort by arrival time */
    Process *temp_p = (Process *)malloc(sizeof(Process) * len);
    if (temp_p == NULL)
    {
        printf("Error: Memory allocation failed for temp_p array\n");
        free(remaining);
        free(queue);
        free(in_queue);
        free(sorted_indices);
        return;
    }
    for (i = 0; i < len; i++)
    {
        temp_p[i] = p[i];
    }

    /* Sort processes by arrival time using merge_sort_by_arrive_time */
    merge_sort_by_arrive_time(temp_p, 0, len - 1);

    /* Update sorted_indices based on the sorted order */
    for (i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
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
    for (i = 0; i < len; i++)
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

    /* Round Robin scheduling loop */
    while (completed < len)
    {
        /* If the queue is empty, increment time until a process arrives */
        if (front > rear)
        {
            int next_arrival = INT_MAX;
            for (i = 0; i < len; i++)
            {
                if (!in_queue[i] && p[i].arrive_time < next_arrival)
                {
                    next_arrival = p[i].arrive_time;
                }
            }
            if (next_arrival == INT_MAX) break; // No more processes to schedule
            current_time = next_arrival;
            /* Add processes that have arrived by the new current_time, in order of arrival */
            for (i = 0; i < len; i++)
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

        /* Execute for the quantum or remaining time, whichever is smaller */
        int time_slice = (remaining[idx] > quantum) ? quantum : remaining[idx];
        current_time += time_slice;
        remaining[idx] -= time_slice;

        /* Add processes that have arrived during this time slice, in order of arrival */
        for (i = 0; i < len; i++)
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
            /* Process is finished */
            p[idx].finish_time = current_time;
            p[idx].turnaround = p[idx].finish_time - p[idx].arrive_time;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            completed++;
        }
    }

    /* Free allocated memory */
    free(remaining);
    free(queue);
    free(in_queue);
    free(sorted_indices);

    /* Print the results using print_table */
    print_table(p, len, "RR", quantum);  // Pass the actual quantum for RR
}

#endif
