#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/wait.h>

#define ITER_K 5
#define ITER_M 5

volatile int *PRAVO;
volatile int *ZASTAVICA;

void udji_u_kriticni_odsjecak(int i, int j) {
    ZASTAVICA[i] = 1;
    while (ZASTAVICA[j] != 0) {
        if (*PRAVO == j) {
            ZASTAVICA[i] = 0;
            while (*PRAVO == j)
                ;
            ZASTAVICA[i] = 1;
        }
    }
}

void izadi_iz_kriticnog_odsjecka(int i, int j) {
    *PRAVO = j;  
    ZASTAVICA[i] = 0;  
}

int main() {

    PRAVO = mmap(NULL, sizeof *PRAVO, PROT_READ | PROT_WRITE,
                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    ZASTAVICA = mmap(NULL, 2 * sizeof *ZASTAVICA, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (PRAVO == MAP_FAILED || ZASTAVICA == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    *PRAVO = 0;
    ZASTAVICA[0] = 0;
    ZASTAVICA[1] = 0;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    int i = (pid == 0) ? 1 : 0;  
    int j = 1 - i;

    for (int k = 1; k <= ITER_K; k++) {
        udji_u_kriticni_odsjecak(i, j);

        for (int m = 1; m <= ITER_M; m++) {
            printf("Proces %d: k = %d, m = %d\n", i, k, m);
            fflush(stdout);
            usleep(100000);
        }

        izadi_iz_kriticnog_odsjecka(i, j);

        usleep(100000);
    }

    if (pid > 0) {
        wait(NULL);
    }

    return 0;
}