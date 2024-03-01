#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 100

// Global variables
long int *data;
int *bins;
int n, b, t;
long int m, M;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function declarations
void* compute_histogram(void* thread_id);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <n> <b> <t>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);
    b = atoi(argv[2]);
    t = atoi(argv[3]);

    if (n <= 0 || b <= 0 || t <= 0 || t > MAX_THREADS) {
        fprintf(stderr, "Invalid input values.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for data and bins
    data = (long int*)malloc(n * sizeof(long int));
    bins = (int*)calloc(b, sizeof(int));

    // Generate n long int numbers
    for (int i = 0; i < n; i++) {
        data[i] = rand();
    }

    // Determine minimum (m) and maximum (M) values
    m = data[0];
    M = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] < m) m = data[i];
        if (data[i] > M) M = data[i];
    }

    // Create threads
    pthread_t threads[MAX_THREADS];
    for (long int i = 0; i < t; i++) {
        pthread_create(&threads[i], NULL, compute_histogram, (void*)i);
    }

    // Join threads
    for (long int i = 0; i < t; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print global histogram
    for (int i = 0; i < b; i++) {
        printf("[%ld, %ld] %d\n", m + i * (M - m) / b, m + (i + 1) * (M - m) / b, bins[i]);
    }

    // Free allocated memory
    free(data);
    free(bins);

    return 0;
}

void* compute_histogram(void* thread_id) {
    long int tid = (long int)thread_id;
    int chunk_size = n / t;
    int start = tid * chunk_size;
    int end = (tid == t - 1) ? n : start + chunk_size;

    for (int i = start; i < end; i++) {
        // Determine the bin for the current data point
        int bin = (int)((data[i] - m) * b / (M - m + 1));  // Adjusted the bin computation

        // Protect the access to bins array with mutex
        pthread_mutex_lock(&mutex);
        bins[bin]++;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}
