# Operating Systems Tasks Repository

This repository contains 5 C-language projects demonstrating various Operating Systems concepts:
1. Signal handling and file persistence
2. Process synchronization using Peterson's algorithm
3. Thread synchronization with Bakery algorithm for resource allocation
4. Semaphore-based producer-consumer (carousel simulation)
5. Dining Philosophers problem with monitor and condition variables

---

## 📁 Task 1: Signal-based Square Generator

**Source file:** `task1.c`

**Description:**  
Continuously calculates and appends successive square numbers to `obrada.txt`, every 5 seconds.  
- Handles `SIGUSR1` to print current processing number.  
- On `SIGTERM`, persists the last processed number to `status.txt` and exits cleanly.  
- On `SIGINT`, exits without updating status (default status = 0).  
- On startup, resumes from last square based on `obrada.txt` and optionally `status.txt`.

**Usage:**  
```bash
gcc task1.c -o task1 -lm
./task1
kill -USR1 <pid>
kill -TERM <pid>

📁 Task 2: Peterson’s Algorithm for Two Processes

Source file: task2.c

Description:
Demonstrates mutual exclusion between a parent and child process using Peterson's algorithm.

    Alternates entering a critical section guarded by shared memory flags and who’s turn (PRAVO).

    Each process loops 5 iterations with 5 inner steps, printing its ID in the critical section.

Usage:

gcc task2.c -o task2
./task2

📁 Task 3: Bakery Algorithm for Threaded Table Reservation

Source file: task3.c

Description:
Multiple threads attempt to reserve a limited number of tables using the Bakery algorithm to ensure fairness.

    Threads choose a random table, enter critical section to reserve if free, then exit.

    Continues until no tables remain. Prints reservation attempts and current table states.

Usage:

gcc task3.c -o task3 -lpthread
./task3 <num_threads> <num_tables>

📁 Task 4: Carousel (Bounded Buffer) with Semaphores

Source file: task4.c

Description:
Simulates a carousel ride with 15 visitors (producer‑consumer model) and capacity N=5.

    posjetitelj threads wait for empty seats, board, notify when full, ride, unboard.

    vrtuljak thread waits for full ride, runs, signals when done.

    Uses four semaphores: prazna_mjesta, pun, zavrseno, and mutex.

Usage:

gcc task4.c -o task4 -lpthread
./task4

📁 Task 5: Dining Philosophers with Monitor

Source file: task5.c

Description:
Classic Dining Philosophers problem implemented using a monitor (mutex + condition variables).

    Five philosophers alternate thinking and eating.

    Wait until both forks (represented as vilice[]) are available, then eat.

    Uses pthread_mutex_t and pthread_cond_t to prevent deadlock and ensure fairness.

Usage:

gcc task5.c -o task5 -lpthread
./task5

🔧 Compilation & Running

Each task can be compiled and run independently:

# Example for Task 2
gcc task2.c -o task2
./task2

For threaded programs, remember to link with pthread:

gcc task3.c -o task3 -lpthread
./task3 4 3  # 4 threads, 3 tables

🧠 Concepts Covered

    Signal handling & state persistence (Task 1)

    Process synchronization: Peterson’s algorithm (Task 2)

    Thread synchronization: Bakery algorithm (Task 3)

    Semaphore-based synchronization: producer-consumer simulation (Task 4)

    Monitor-based concurrency: Dining Philosophers problem (Task 5)

📝 Notes

    File names, constants, and behavior match original code.

    Error handling is basic—focus is on demonstrating OS synchronization primitives.

    For more robust usage, consider adding argument parsing, logging, or timeout handling.

📚 References

These implementations follow standard OS synchronization patterns:

    Peterson’s and Bakery algorithms (Dekker, Lamport)

    Semaphore-based bounded buffer

    Monitor solution for Dining Philosophers (Hoare-style)
