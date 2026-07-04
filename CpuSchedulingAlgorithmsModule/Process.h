#ifndef __PROCESS__
#define __PROCESS__

// Process Structure and Utilities

/* Include standard headers used in the program */
#include <stdio.h>
#include <stdlib.h>

/* Define symbolic constants used in the program */
#define ID_LEN 20  // Maximum length of process ID

/* Define the Process structure */
typedef struct _process
{
    char id[ID_LEN];    // Process ID (e.g., "P0", "P1")
    int arrive_time;    // Arrival time of the process
    int burst;          // Burst time (execution time) of the process
    int priority;       // Priority of the process (lower number = higher priority, used in PPS and NPPS)
    int waiting;        // Waiting time of the process
    int turnaround;     // Turnaround time of the process
    int finish_time;    // Finish time of the process
} Process;  // Use "Process" as the type name

/* Define Quantum as an alias for int (used for Round Robin time quantum) */
typedef int Quantum;

#endif
