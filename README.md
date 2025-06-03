# Operating Systems – Assignment 1 & Assignment 2

This repository contains two C programs created for the **Operating Systems** course. The tasks focus on signal handling, file operations, shared memory, and mutual exclusion.

## 📄 Contents

- `zadatak1.c` – Square number processing with signal handling
- `zadatak2.c` – Mutual exclusion using Peterson’s algorithm

---

## 🧠 Assignment 1: Signals and File Handling

### Description
The program in `zadatak1.c` continuously calculates squares of natural numbers and writes them to `obrada.txt` every 5 seconds. It uses `status.txt` to store the last processed number so that it can resume correctly after being terminated.

### Handled Signals:

- `SIGUSR1` – Prints the currently processed number
- `SIGTERM` – Writes the last number to `status.txt` and gracefully exits
- `SIGINT` – Exits immediately without saving state

### How to Run
```bash
gcc -o zadatak1 zadatak1.c -lm
./zadatak1
