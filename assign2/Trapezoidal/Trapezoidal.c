#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <stdint.h>

#define MAX_THREADS 100

// Global variables
double a, b;
long long int n;
double integral_sum = 0.0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;
pthread_barrier_t barrier;

// Function declarations
double f(double x);
void* trapezoidal_busy_waiting(void* arg);
void* trapezoidal_mutex(void* arg);
void* trapezoidal_semaphore(void* arg);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <a> <b> <n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoll(argv[3]);

    if (a <= 0 || b <= a || n <= 0) {
        fprintf(stderr, "Invalid input parameters.\n");
        exit(EXIT_FAILURE);
    }

    // Create threads
    pthread_t threads[MAX_THREADS];
    int num_threads = 4; // You can adjust the number of threads as needed

    // Initialize semaphore
    sem_init(&semaphore, 0, 1);

    // Initialize barrier
    pthread_barrier_init(&barrier, NULL, num_threads);

    // Launch threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, trapezoidal_semaphore, (void*)(intptr_t)i);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore and barrier
    sem_destroy(&semaphore);
    pthread_barrier_destroy(&barrier);

    // Calculate the approximation of the integral
    double h = (b - a) / n;
    double integral = (f(a) + f(b)) / 2.0;

    for (long long int k = 1; k < n; k++) {
        double x_k = a + k * h;
        integral += f(x_k);
    }

    integral *= h;

    // Print the result
    printf("Approximation of the integral: %f\n", integral);
    printf("Integration error: %f\n", fabs(integral - integral_sum));

    return 0;
}

double f(double x) {
    return 1.0 / x;
}

void* trapezoidal_busy_waiting(void* arg) {
    // Each thread computes its local sum
    double local_sum = 0.0;
    long long int thread_num = (long long int)arg;
    double h = (b - a) / n;

    // Compute the integral for the assigned range
    for (long long int k = thread_num; k < n; k += MAX_THREADS) {
        double x_k = a + k * h;
        local_sum += f(x_k);
    }

    // Busy-waiting section
    while (1) {
        pthread_mutex_lock(&mutex);
        if (thread_num == 0) {
            integral_sum += local_sum;
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* trapezoidal_mutex(void* arg) {
    // Each thread computes its local sum
    double local_sum = 0.0;
    long long int thread_num = (long long int)arg;
    double h = (b - a) / n;

    // Compute the integral for the assigned range
    for (long long int k = thread_num; k < n; k += MAX_THREADS) {
        double x_k = a + k * h;
        local_sum += f(x_k);
    }

    // Mutex section
    pthread_mutex_lock(&mutex);
    integral_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* trapezoidal_semaphore(void* arg) {
    // Each thread computes its local sum
    double local_sum = 0.0;
    long long int thread_num = (long long int)arg;
    double h = (b - a) / n;

    // Compute the integral for the assigned range
    for (long long int k = thread_num; k < n; k += MAX_THREADS) {
        double x_k = a + k * h;
        local_sum += f(x_k);
    }

    // Barrier synchronization
    pthread_barrier_wait(&barrier);

    // Semaphore section
    sem_wait(&semaphore);
    integral_sum += local_sum;
    sem_post(&semaphore);

    pthread_exit(NULL);
}
