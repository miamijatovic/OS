#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int *stolovi;      
int n_stolova;
int n_dretvi;

volatile int *ULAZ;
volatile int *BROJ;

int maxBroj() {
    int max = 0;
    for (int i = 0; i < n_dretvi; i++) {
        if (BROJ[i] > max) max = BROJ[i];
    }
    return max;
}

void uđi_u_kritični_odsječak(int i) {
    ULAZ[i] = 1;
    __sync_synchronize();

    BROJ[i] = maxBroj() + 1;

    __sync_synchronize();
    ULAZ[i] = 0;

    for (int j = 0; j < n_dretvi; j++) {
        if (j == i) continue;

        while (ULAZ[j] != 0) {}

        while (BROJ[j] != 0 &&
               (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j < i))) {}
    }
}

void izađi_iz_kritičnog_odsječka(int i) {
    BROJ[i] = 0;
}

int ima_slobodnih() {
    for (int i = 0; i < n_stolova; i++) {
        if (stolovi[i] == 0) return 1;
    }
    return 0;
}

void ispis_stolova() {
    for (int i = 0; i < n_stolova; i++) {
        if (stolovi[i] == 0) printf("-");
        else printf("%d", stolovi[i]);
    }
    printf("\n");
}

void *dretva_funkcija(void *arg) {
    int i = *(int *)arg;
    free(arg);

    while (1) {
        sleep(1);

        if (!ima_slobodnih()) break;

        int odabrani_stol = rand() % n_stolova;

        printf("Dretva %d: odabirem stol %d\n", i + 1, odabrani_stol + 1);

        uđi_u_kritični_odsječak(i);

        if (stolovi[odabrani_stol] == 0) {
            stolovi[odabrani_stol] = i + 1;
            printf("Dretva %d: rezerviram stol %d, stanje:\n", i + 1, odabrani_stol + 1);
            ispis_stolova();
        } else {
            printf("Dretva %d: neuspjela rezervacija stola %d, stanje:\n", i + 1, odabrani_stol + 1);
            ispis_stolova();
        }

        izađi_iz_kritičnog_odsječka(i);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Upotreba: %s <broj_dretvi> <broj_stolova>\n", argv[0]);
        return 1;
    }

    n_dretvi = atoi(argv[1]);
    n_stolova = atoi(argv[2]);

    if (n_dretvi < 1 || n_stolova < 1) {
        fprintf(stderr, "Broj dretvi i stolova mora biti pozitivan.\n");
        return 1;
    }

    stolovi = calloc(n_stolova, sizeof(int));
    ULAZ = calloc(n_dretvi, sizeof(int));
    BROJ = calloc(n_dretvi, sizeof(int));

    pthread_t *dretve = malloc(n_dretvi * sizeof(pthread_t));

    srand(time(NULL));

    for (int i = 0; i < n_dretvi; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&dretve[i], NULL, dretva_funkcija, id);
    }

    for (int i = 0; i < n_dretvi; i++) {
        pthread_join(dretve[i], NULL);
    }

    free(stolovi);
    free((void *)ULAZ);
    free((void *)BROJ);
    free(dretve);

    return 0;
}
