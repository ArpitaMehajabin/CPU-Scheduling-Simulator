#ifndef __FIRST__COME__FIRST__SERVED__
#define __FIRST__COME__FIRST__SERVED__

// First Come First Served (FCFS) Scheduling Algorithm

/* Include user-defined headers */
#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"

/**
 * Execute First Come First Served (FCFS) scheduling algorithm
 * @param p   Array of Process structs
 * @param len Number of processes
 */
void FCFS(Process *p, int len)
{
    int i;

    /* Sort processes by arrival time */
    merge_sort_by_arrive_time(p, 0, len - 1);

    /* Calculate completion, waiting, and turnaround times */
    int current_time = 0;
    for (i = 0; i < len; i++)
    {
        /* If current time is less than arrival time, wait until the process arrives */
        if (current_time < p[i].arrive_time)
        {
            current_time = p[i].arrive_time;
        }
        current_time += p[i].burst;
        p[i].finish_time = current_time;
        p[i].turnaround = p[i].finish_time - p[i].arrive_time;
        p[i].waiting = p[i].turnaround - p[i].burst;
    }

    /* Print the results using print_table */
    print_table(p, len, "FCFS", 0);//Quantam is ignored for FCFS
}

#endif
