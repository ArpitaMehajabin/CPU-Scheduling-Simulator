#ifndef __COMPARE__FUNCTION__
#define __COMPARE__FUNCTION__

// Sorting Functions for Process Scheduling

/* Include user-defined headers */
#include "./Process.h"

/**
 * Compare two processes by finish time for sorting
 * @param a First Process struct
 * @param b Second Process struct
 * @return Comparison result (-1 if a < b, 1 if a > b, 0 if equal)
 */
int compare_by_finish_time(const void *a, const void *b)
{
    /* Cast void pointers to Process pointers */
    Process *ptA = (Process *)a;
    Process *ptB = (Process *)b;

    /* If ptA's finish time is less than ptB's */
    if (ptA->finish_time < ptB->finish_time)
        return -1;
    /* If ptA's finish time is greater than ptB's */
    else if (ptA->finish_time > ptB->finish_time)
        return 1;
    /* If finish times are equal */
    else
        return 0;
}

/**
 * Sort processes by finish time using quicksort
 * @param p   Array of Process structs
 * @param len Number of processes
 */
void quick_sort_by_finish_time(Process p[], int len)
{
    /* Use the built-in qsort function with compare_by_finish_time */
    qsort(p, len, sizeof(Process), compare_by_finish_time);
}

/**
 * Merge two sorted subarrays into a single sorted array based on arrival time
 * @param arr   Array of Process structs
 * @param left  Leftmost index of the subarray
 * @param mid   Middle index of the subarray
 * @param right Rightmost index of the subarray
 */
void merge(Process arr[], int left, int mid, int right)
{
    int fIdx = left;  // Starting index of the left subarray
    int rIdx = mid + 1;  // Starting index of the right subarray
    int i;

    /* Allocate temporary array for merging */
    Process *sortArr = (Process *)malloc(sizeof(Process) * (right + 1));
    int sIdx = left;

    /* Merge the two subarrays by comparing arrival times */
    while (fIdx <= mid && rIdx <= right)
    {
        if (arr[fIdx].arrive_time <= arr[rIdx].arrive_time)
            sortArr[sIdx] = arr[fIdx++];
        else
            sortArr[sIdx] = arr[rIdx++];
        sIdx++;
    }

    /* Copy remaining elements from the left subarray, if any */
    if (fIdx > mid)
    {
        for (i = rIdx; i <= right; i++, sIdx++)
            sortArr[sIdx] = arr[i];
    }
    /* Copy remaining elements from the right subarray, if any */
    else
    {
        for (i = fIdx; i <= mid; i++, sIdx++)
            sortArr[sIdx] = arr[i];
    }

    /* Copy the sorted array back to the original array */
    for (i = left; i <= right; i++)
        arr[i] = sortArr[i];

    /* Free allocated memory */
    free(sortArr);
}

/**
 * Sort processes by arrival time using merge sort
 * @param arr   Array of Process structs
 * @param left  Leftmost index of the array
 * @param right Rightmost index of the array
 */
void merge_sort_by_arrive_time(Process arr[], int left, int right)
{
    int mid;

    /* Recursively divide the array if left < right */
    if (left < right)
    {
        mid = (left + right) / 2;
        merge_sort_by_arrive_time(arr, left, mid);
        merge_sort_by_arrive_time(arr, mid + 1, right);
        merge(arr, left, mid, right); // Removed duplicate call
    }
}

#endif
