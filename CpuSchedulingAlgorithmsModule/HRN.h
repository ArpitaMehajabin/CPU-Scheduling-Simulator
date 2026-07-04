#ifndef __HIGHEST__RESPONSE__RATIO__NEXT__
#define __HIGHEST__RESPONSE__RATIO__NEXT__

// Highest Response Ratio Next (HRN) Scheduling Algorithm

/* Include user-defined headers */
#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"

/**
 * Execute Highest Response Ratio Next (HRN) scheduling algorithm
 * @param p   Array of Process structs
 * @param len Number of processes
 */
void HRN(Process *p, int len)
{
    int i, j;
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

    /* Track which processes are completed */
    int *is_completed = (int *)calloc(len, sizeof(int));

    /* Start with the earliest arrival time */
    current_time = p[0].arrive_time;

    /* Schedule processes until all are completed */
    while (completed < len)
    {
        float highest_ratio = -9999.0;
        int selected_idx = -1;

        /* Find the process with the highest response ratio among arrived processes */
        for (i = 0; i < len; i++)
        {
            if (!is_completed[i] && p[i].arrive_time <= current_time)
            {
                float ratio = (float)(current_time - p[i].arrive_time + p[i].burst) / p[i].burst;
                if (ratio > highest_ratio)
                {
                    highest_ratio = ratio;
                    selected_idx = i;
                }
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
    print_table(p, len, "HRN", 0);
}

#endif
