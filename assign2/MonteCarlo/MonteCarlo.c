#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 100

// Global variables
long long int number_in_circle = 0;
long long int total_tosses;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function declarations
void* monte_carlo(void* arg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_tosses>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    total_tosses = atoll(argv[1]);

    if (total_tosses <= 0) {
        fprintf(stderr, "Invalid number of tosses.\n");
        exit(EXIT_FAILURE);
    }

    // Create threads
    pthread_t threads[MAX_THREADS];
    int num_threads = 4; // You can adjust the number of threads as needed

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, monte_carlo, NULL);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate the estimate for π
    double pi_estimate = 4.0 * number_in_circle / total_tosses;

    // Print the estimate
    printf("Estimate for π: %f\n", pi_estimate);

    return 0;
}

void* monte_carlo(void* arg) {
    long long int local_number_in_circle = 0;

    for (long long int toss = 0; toss < total_tosses; toss++) {
        double x = 2.0 * drand48() - 1.0;
        double y = 2.0 * drand48() - 1.0;
        double distance_squared = x * x + y * y;

        if (distance_squared <= 1) {
            local_number_in_circle++;
        }
    }

    // Protect the access to the global variable with mutex
    pthread_mutex_lock(&mutex);
    number_in_circle += local_number_in_circle;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}
