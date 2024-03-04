Certainly! Here's the README in markdown format:

```markdown
# README for Trapezoidal Rule Integration with Pthreads

## Overview

This repository contains a C program named `Trapezoidal.c` that implements the Trapezoidal rule for numerical integration using Pthreads. The program computes an approximation of the definite integral of the function `f(x) = 1/x` over the specified interval `[a, b]` with `n` subintervals. Three different synchronization techniques (busy-waiting, mutexes, and semaphores) are implemented to enforce mutual exclusion in the critical section where the global variable `integral_sum` is updated.

## Usage

To compile the program, use the following command:

```bash
gcc -o Trapezoidal Trapezoidal.c -pthread -lm
```

To run the program, provide command line arguments for `a`, `b`, and `n`:

```bash
./Trapezoidal <a> <b> <n>
```

Make sure that the values of `a`, `b`, and `n` are valid inputs. The program will output the approximation of the integral and the integration error.

## Program Components

### 1. `Trapezoidal.c`

- The main program file containing the implementation of the Trapezoidal rule with Pthreads.
- Three different thread functions (`trapezoidal_busy_waiting`, `trapezoidal_mutex`, `trapezoidal_semaphore`) represent the different synchronization approaches.

### 2. `README.txt`

- This document providing an overview of the program, usage instructions, and explanations of each component.

## Global Variables

- `a`, `b`, `n`: Input parameters specifying the integration interval and the number of subintervals.
- `integral_sum`: Global variable to store the sum of thread computations.
- `mutex`: Pthreads mutex for mutual exclusion in the mutex approach.
- `semaphore`: Pthreads semaphore for mutual exclusion in the semaphore approach.
- `barrier`: Pthreads barrier to ensure all threads start more or less together.

## Thread Functions

### 1. `trapezoidal_busy_waiting`

- Implements busy-waiting for mutual exclusion.
- Each thread computes a local sum and updates the global `integral_sum` using busy-waiting.

### 2. `trapezoidal_mutex`

- Implements mutexes for mutual exclusion.
- Each thread computes a local sum and updates the global `integral_sum` using mutexes.

### 3. `trapezoidal_semaphore`

- Implements semaphores for mutual exclusion.
- Each thread computes a local sum and updates the global `integral_sum` using semaphores.

## Timing and Analysis

- Use a reliable timing tool to measure the execution time of the program with each synchronization approach.

### Results:

1. **Busy-Waiting:**
   - Time: X seconds
   - *Advantages:* Simple to implement.
   - *Disadvantages:* High CPU consumption during waiting.

2. **Mutexes:**
   - Time: Y seconds
   - *Advantages:* Clear mutual exclusion.
   - *Disadvantages:* Possibility of deadlock.

3. **Semaphores:**
   - Time: Z seconds
   - *Advantages:* Effective resource management.
   - *Disadvantages:* Careful handling needed to avoid deadlock.

## Conclusion

The Trapezoidal rule implementation with Pthreads demonstrates different synchronization techniques, each with its own set of advantages and disadvantages. The timing results provide insights into the efficiency of each approach, aiding in the selection of an appropriate synchronization mechanism based on specific use cases.

```
