#include "CpuSchedulingAlgorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Clear the terminal screen (UNIX-compatible)
 */
void clearScreen()
{
    printf("\033[H\033[J"); // ANSI escape code to clear the screen
}

/**
 * Display the scheduling method menu and get user choice
 * @return The user's choice (1-8)
 */
int selectSchedulingMethod()
{
    int choice;
    printf("\nSelect Scheduling Method:\n");
    printf("-------------------------\n");
    printf("1. First Come First Served (FCFS)\n");
    printf("2. Shortest Job First (SJF)\n");
    printf("3. Shortest Remaining Time First (SRTF)\n");
    printf("4. Round Robin (RR)\n");
    printf("5. Priority Scheduling (Preemptive)\n");
    printf("6. Priority Scheduling (Non-Preemptive)\n");
    printf("7. Highest Response Ratio Next (HRN)\n");
    printf("8. Exit\n");
    printf("-------------------------\n");
    printf("Enter your choice (1-8): ");

    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 8)
    {
        printf("Invalid choice! Please enter a number between 1 and 8: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    return choice;
}

/**
 * Get process details from the user
 * @param n             Number of processes
 * @param processes     Array of Process structs to store the input
 * @param needsPriority Flag indicating if priority input is needed (for PPS/NPPS)
 * @param quantum       Pointer to store the time quantum (for RR), NULL if not needed
 */
void inputProcesses(int n, Process *processes, int needsPriority, Quantum *quantum)
{
    printf("\nEnter Process Details:\n");
    printf("-------------------------\n");
    for (int i = 0; i < n; i++)
    {
        // Process ID (auto-assigned as P0, P1, etc.)
        snprintf(processes[i].id, sizeof(processes[i].id), "P%d", i);

        // Arrival Time
        printf("Process %s:\n", processes[i].id);
        printf("  Arrival Time: ");
        while (scanf("%d", &processes[i].arrive_time) != 1 || processes[i].arrive_time < 0)
        {
            printf("  Invalid input! Enter a non-negative integer for Arrival Time: ");
            while (getchar() != '\n');
        }

        // Burst Time
        printf("  Burst Time: ");
        while (scanf("%d", &processes[i].burst) != 1 || processes[i].burst <= 0)
        {
            printf("  Invalid input! Enter a positive integer for Burst Time: ");
            while (getchar() != '\n');
        }

        // Priority (if needed)
        if (needsPriority)
        {
            printf("  Priority (lower number = higher priority): ");
            while (scanf("%d", &processes[i].priority) != 1 || processes[i].priority < 0)
            {
                printf("  Invalid input! Enter a non-negative integer for Priority: ");
                while (getchar() != '\n');
            }
        }
        else
        {
            processes[i].priority = 0; // Set to 0 for algorithms that don't use priority
        }
    }

    // Quantum for Round Robin (if needed)
    if (quantum != NULL)
    {
        printf("\nEnter Time Quantum for Round Robin: ");
        while (scanf("%d", quantum) != 1 || *quantum <= 0)
        {
            printf("Invalid input! Enter a positive integer for Time Quantum: ");
            while (getchar() != '\n');
        }
    }
}

/**
 * Main function to run the CPU Scheduling Simulator
 */
int main()
{
    int process_count;
    Quantum quantum = 0; // For Round Robin

    // Welcome message
    clearScreen();
    printf("=====================================\n");
    printf("   Welcome to CPU Scheduling Simulator\n");
    printf("=====================================\n");

    while (1)
    {
        // Get number of processes
        printf("\nEnter the number of processes (1-20): ");
        while (scanf("%d", &process_count) != 1 || process_count < 1 || process_count > 20)
        {
            printf("Invalid input! Enter a number between 1 and 20: ");
            while (getchar() != '\n');
        }

        // Allocate memory for processes
        Process *process = (Process *)malloc(sizeof(Process) * process_count);
        if (process == NULL)
        {
            printf("Memory allocation failed!\n");
            return 1;
        }

        // Select scheduling method
        int choice = selectSchedulingMethod();
        if (choice == 8)
        {
            printf("\nExiting the simulator. Goodbye!\n");
            free(process);
            break;
        }

        // Determine if the method needs priority or quantum
        int needsPriority = (choice == 5 || choice == 6); // Priority Scheduling
        int needsQuantum = (choice == 4);                 // Round Robin

        // Get process details
        inputProcesses(process_count, process, needsPriority, needsQuantum ? &quantum : NULL);

        // Display input summary
        printf("\nInput Summary:\n");
        printf("-------------------------\n");
        printf("PID\tArrival\tBurst\t%s\n", needsPriority ? "Priority" : "");
        for (int i = 0; i < process_count; i++)
        {
            if (needsPriority)
            {
                printf("%s\t%d\t%d\t%d\n", process[i].id, process[i].arrive_time,
                       process[i].burst, process[i].priority);
            }
            else
            {
                printf("%s\t%d\t%d\t-\n", process[i].id, process[i].arrive_time,
                       process[i].burst);
            }
        }
        printf("-------------------------\n");
        printf("Press Enter to continue...");
        while (getchar() != '\n');
        getchar(); // Wait for Enter key

        // Clear screen before displaying results
        clearScreen();
        printf("=====================================\n");
        printf("           Simulation Results\n");
        printf("=====================================\n");

        // Run the selected scheduling algorithm
        printf("------------------------------------------------------------\n\n");
        switch (choice)
        {
        case 1:
            printf("First Come First Served (FCFS) Scheduling\n");
            FCFS(process, process_count);
            break;
        case 2:
            printf("Shortest Job First (SJF) Scheduling\n");
            SJF(process, process_count);
            break;
        case 3:
            printf("Shortest Remaining Time First (SRTF) Scheduling\n");
            SRT(process, process_count);
            break;
        case 4:
            printf("Round Robin (RR) Scheduling with Quantum %d\n", quantum);
            RR(process, process_count, quantum);
            break;
        case 5:
            printf("Priority Scheduling (Preemptive)\n");
            PPS(process, process_count);
            break;
        case 6:
            printf("Priority Scheduling (Non-Preemptive)\n");
            NPPS(process, process_count);
            break;
        case 7:
            printf("Highest Response Ratio Next (HRN) Scheduling\n");
            HRN(process, process_count);
            break;
        }
        printf("\n------------------------------------------------------------\n");

        // Free memory
        free(process);

        // Ask if the user wants to run another simulation
        printf("\nDo you want to run another simulation? (y/n): ");
        char runAgain;
        while (scanf(" %c", &runAgain) != 1 || (runAgain != 'y' && runAgain != 'n'))
        {
            printf("Invalid input! Enter 'y' for yes or 'n' for no: ");
            while (getchar() != '\n');
        }
        if (runAgain == 'n')
        {
            printf("\nExiting the simulator. Goodbye!\n");
            break;
        }
        clearScreen();
    }

    return 0;
}
