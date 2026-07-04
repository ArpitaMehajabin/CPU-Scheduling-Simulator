#ifndef __NON__PREEMPTIVE__PRIORITY__SCHEDULING__
#define __NON__PREEMPTIVE__PRIORITY__SCHEDULING__

// Non-Preemptive Priority Scheduling (NPPS) Algorithm

/* Include user-defined headers */
#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"
#include <limits.h>

/**
 * Execute Non-Preemptive Priority Scheduling (NPPS) algorithm
 * @param p   Array of Process structs
 * @param len Number of processes
 */
void NPPS(Process *p, int len)
{
    int i;
    int current_time = 0;
    int completed = 0;

    /* Sort processes by arrival time */
    merge_sort_by_arrive_time(p, 0, len - 1);

    /* Track which processes are completed */
    int *is_completed = (int *)calloc(len, sizeof(int));

    /* Start with the earliest arrival time */
    current_time = p[0].arrive_time;

    /* Schedule processes until all are completed */
    while (completed < len)
    {
        int highest_priority = INT_MAX;
        int selected_idx = -1;

        /* Find the process with the highest priority (lowest priority value) among arrived processes */
        for (i = 0; i < len; i++)
        {
            if (!is_completed[i] && p[i].arrive_time <= current_time && p[i].priority < highest_priority)
            {
                highest_priority = p[i].priority;
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

        /* Execute the selected process */
        current_time += p[selected_idx].burst;
        p[selected_idx].finish_time = current_time;
        p[selected_idx].turnaround = p[selected_idx].finish_time - p[selected_idx].arrive_time;
        p[selected_idx].waiting = p[selected_idx].turnaround - p[selected_idx].burst;
        is_completed[selected_idx] = 1;
        completed++;
    }

    /* Free allocated memory */
    free(is_completed);

    /* Print the results using print_table */
    print_table(p, len, "NPPS", 0);
}

#endif
