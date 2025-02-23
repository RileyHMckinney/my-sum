#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>  //for usleep
#include <cstdlib>
#include "../include/barrier.h"

using namespace std;

// Create shared memory for barrier control variables (also used to create the temp array)
void shm_arr_create(int &shm_id, int *&arr, int size) {
    shm_id = shmget(IPC_PRIVATE, size * sizeof(int), IPC_CREAT | 0666);
    if (shm_id == -1) {
        cerr << "Error: Failed to allocate shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    arr = (int *)shmat(shm_id, NULL, 0);
    if (arr == (int *)-1) {
        cerr << "Error: Failed to attach shared memory." << endl;
        exit(EXIT_FAILURE);
    }

    memset(arr, 0, size * sizeof(int)); // Initialize shared memory to zero
}

// Function to enforce the barrier
void wall_use(int *barrier_vars, int process_number, int m) {
    if (m == 1) return;

    int *barrier = &barrier_vars[0];      // Shared barrier counter
    int *cycle_count = &barrier_vars[1];  // Shared cycle counter

    int local_cycle = *cycle_count; // Read current cycle count

    while (*barrier != process_number) {  
        usleep(100);  // Reduce CPU load while waiting
    }

    (*barrier)++; // Increment barrier count

    // Last process resets barrier and increments cycle_count
    if (process_number == m - 1) {
        *barrier = 0;
        (*cycle_count)++;
    }

    // Wait for `cycle_count` to increment before proceeding
    while (*cycle_count == local_cycle) {
        usleep(1000);
    }
}
