#ifndef __SHORTEST__REMAINING__TIME
#define __SHORTEST__REMAINING__TIME

// Shortest Remaining Time (SRT) Scheduling Algorithm

/* Include user-defined headers */
#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"
#include <limits.h>

/**
 * Execute Shortest Remaining Time (SRT) scheduling algorithm
 * @param p   Array of Process structs
 * @param len Number of processes
 */
void SRT(Process *p, int len)
{
    int i;
    int current_time = 0;
    int completed = 0;
    int total_burst_time = 0;

    /* Calculate total burst time */
    for (i = 0; i < len; i++)
    {
        total_burst_time += p[i].burst;
    }

    /* Sort processes by arrival time */
    merge_sort_by_arrive_time(p, 0, len - 1);

    /* Allocate arrays for remaining burst times and completion tracking */
    int *remaining = (int *)malloc(sizeof(int) * len);
    int *is_completed = (int *)calloc(len, sizeof(int));

    /* Initialize remaining burst times */
    for (i = 0; i < len; i++)
    {
        remaining[i] = p[i].burst;
    }

    /* Schedule processes until all are completed */
    while (completed < len)
    {
        int shortest_remaining = INT_MAX;
        int selected_idx = -1;

        /* Find the process with the shortest remaining time among arrived processes */
        for (i = 0; i < len; i++)
        {
            if (!is_completed[i] && p[i].arrive_time <= current_time && remaining[i] < shortest_remaining && remaining[i] > 0)
            {
                shortest_remaining = remaining[i];
                selected_idx = i;
            }
        }

        /* If no process is ready, increment time until the next process arrives */
        if (selected_idx == -1)
        {
            int next_arrival = INT_MAX;
            for (i = 0; i < len; i++)
            {
                if (!is_completed[i] && p[i].arrive_time > current_time && p[i].arrive_time < next_arrival)
                {
                    next_arrival = p[i].arrive_time;
                }
            }
            if (next_arrival == INT_MAX) break;
            current_time = next_arrival;
            continue;
        }

        /* Execute the selected process for 1 time unit */
        remaining[selected_idx]--;
        current_time++;

        /* If the process is finished */
        if (remaining[selected_idx] == 0)
        {
            p[selected_idx].finish_time = current_time;
            p[selected_idx].turnaround = p[selected_idx].finish_time - p[selected_idx].arrive_time;
            p[selected_idx].waiting = p[selected_idx].turnaround - p[selected_idx].burst;
            is_completed[selected_idx] = 1;
            completed++;
        }
    }

    /* Free allocated memory */
    free(remaining);
    free(is_completed);

    /* Print the results using print_table */
    print_table(p, len, "SRT", 0);
}

#endif
