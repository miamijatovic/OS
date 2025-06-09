#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BROJ_FILOZOFA 5

#define MISLI 'O'
#define GLADAN 'o'
#define JEDE 'X'

char stanje[BROJ_FILOZOFA];
int vilice[BROJ_FILOZOFA];  

pthread_mutex_t monitor = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t red[BROJ_FILOZOFA];

void ispis_stanja() {
    printf("Stanje filozofa: ");
    for (int i = 0; i < BROJ_FILOZOFA; i++) {
        printf("%c ", stanje[i]);
    }
    printf("\n");
}

void misliti() {
    sleep(3);
}

void jesti(int n) {
    pthread_mutex_lock(&monitor);

    stanje[n] = GLADAN;
    while (vilice[n] == 0 || vilice[(n + 1) % BROJ_FILOZOFA] == 0) {
        pthread_cond_wait(&red[n], &monitor);
    }

    vilice[n] = 0;
    vilice[(n + 1) % BROJ_FILOZOFA] = 0;
    stanje[n] = JEDE;
    ispis_stanja();

    pthread_mutex_unlock(&monitor);

    sleep(2);

    pthread_mutex_lock(&monitor);

    stanje[n] = MISLI;
    vilice[n] = 1;
    vilice[(n + 1) % BROJ_FILOZOFA] = 1;

    pthread_cond_signal(&red[(n + 4) % BROJ_FILOZOFA]);
    pthread_cond_signal(&red[(n + 1) % BROJ_FILOZOFA]);
    ispis_stanja();

    pthread_mutex_unlock(&monitor);
}

void* filozof(void* broj) {
    int n = *(int*)broj;
    while (1) {
        misliti();
        jesti(n);
    }
    return NULL;
}

int main() {
    pthread_t dretve[BROJ_FILOZOFA];
    int indeksi[BROJ_FILOZOFA];

    for (int i = 0; i < BROJ_FILOZOFA; i++) {
        stanje[i] = MISLI;
        vilice[i] = 1;
        pthread_cond_init(&red[i], NULL);
        indeksi[i] = i;
    }

    ispis_stanja();

    for (int i = 0; i < BROJ_FILOZOFA; i++) {
        pthread_create(&dretve[i], NULL, filozof, &indeksi[i]);
    }

    for (int i = 0; i < BROJ_FILOZOFA; i++) {
        pthread_join(dretve[i], NULL);
    }

    return 0;
}