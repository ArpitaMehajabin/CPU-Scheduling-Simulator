# CPU Scheduling Simulator

A C program that simulates and compares classic CPU scheduling algorithms, showing Gantt charts, waiting time, turnaround time, and completion time for each process.

## Algorithms Implemented

- First Come First Served (FCFS)
- Shortest Job First (SJF)
- Shortest Remaining Time First (SRTF)
- Round Robin (RR)
- Priority Scheduling — Preemptive (PPS)
- Priority Scheduling — Non-Preemptive (NPPS)
- Highest Response Ratio Next (HRN)

## How It Works

1. Enter the number of processes.
2. Choose a scheduling algorithm from the menu.
3. Enter arrival time, burst time, and (if needed) priority for each process.
4. For Round Robin, enter a time quantum.
5. View the results: a formatted table with waiting/turnaround/finish times, averages, and a Gantt chart.

## Project Structure

- `CpuSchedulingSimulatorMain.c` — Entry point, menu, and input handling
- `CpuSchedulingAlgorithms.h` — Aggregates all algorithm headers
- `CpuSchedulingAlgorithmsModule/`
  - `Process.h` — Process struct definition
  - `SortingFunction.h` — Merge sort & quicksort helpers
  - `PrintTable.h` — Results table + Gantt chart rendering
  - `FCFS.h`
  - `SJF.h`
  - `SRT.h`
  - `RR.h`
  - `PPS.h`
  - `NPPS.h`
  - `HRN.h`

## Build & Run

Using GCC:

    gcc CpuSchedulingSimulatorMain.c -o scheduler
    ./scheduler

## Sample Output

The program prints a formatted table of process stats along with an ASCII Gantt chart showing execution order and idle time.

