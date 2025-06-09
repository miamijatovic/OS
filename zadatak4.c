#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  
#define UKUPNO 15  

sem_t prazna_mjesta;  
sem_t pun;    
sem_t zavrseno;    
sem_t mutex;      

int broj = 0;    

void pokreni_vrtuljak() {
    printf("Vrtuljak se pokreće!\n");
    sleep(2);
}

void zaustavi_vrtuljak() {
    printf("Vrtuljak se zaustavlja!\n");
}

void* posjetitelj(void* arg) {
    int id = *(int*)arg;
    free(arg);

    sem_wait(&prazna_mjesta);
    sem_wait(&mutex);

    broj++;
    printf("Posjetitelj %d se ukrcava. Trenutno na vrtuljku: %d\n", id, broj);

    if (broj == N) {
        printf("Vrtuljak je pun! Obavještavam vrtuljak.\n");
        sem_post(&pun);
    }
    sem_post(&mutex);
    sem_wait(&zavrseno);
    sem_wait(&mutex);

    broj--;
    printf("Posjetitelj %d silazi. Preostalo na vrtuljku: %d\n", id, broj);

    if (broj == 0) {
        printf("Svi posjetitelji su sišli. Slobodno mjesta su opet dostupna.\n");
        for (int i = 0; i < N; i++) {
            sem_post(&prazna_mjesta);  
        }
    }

    sem_post(&mutex);
    pthread_exit(NULL);
}

void* vrtuljak(void* arg) {
    (void)arg;

    while (1) {
        sem_wait(&pun);

        pokreni_vrtuljak();

        for (int i = 0; i < N; i++) {
            sem_post(&zavrseno);
        }

        zaustavi_vrtuljak();
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t dretva_vrtuljak;
    pthread_t posjetitelji[UKUPNO];

    sem_init(&prazna_mjesta, 0, N);
    sem_init(&pun, 0, 0);
    sem_init(&zavrseno, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&dretva_vrtuljak, NULL, vrtuljak, NULL);

    for (int i = 0; i < UKUPNO; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&posjetitelji[i], NULL, posjetitelj, id);
        usleep(200000);  
    }

    for (int i = 0; i < UKUPNO; i++) {
        pthread_join(posjetitelji[i], NULL);
    }

    printf("Svi posjetitelji su odvozili.\n");
    pthread_cancel(dretva_vrtuljak);
    pthread_join(dretva_vrtuljak, NULL);

    sem_destroy(&prazna_mjesta);
    sem_destroy(&pun);
    sem_destroy(&zavrseno);
    sem_destroy(&mutex);

    return 0;
}