#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
int *a;

pthread_barrier_t bar;

typedef struct {
    int id;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;
    int id = x->id;

    int pairs = n / 2;
    int z = (pairs + p - 1) / p;

    for (int ph = 0; ph < n; ph++) {

        int st = id * z;
        int en = (id + 1) * z;

        if (en > pairs)
            en = pairs;

        if (ph % 2 == 0) {

            for (int k = st; k < en; k++) {
                int i = 2 * k;

                if (i + 1 < n && a[i] > a[i + 1]) {
                    int t = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = t;
                }
            }

        } else {

            for (int k = st; k < en; k++) {
                int i = 2 * k + 1;

                if (i + 1 < n && a[i] > a[i + 1]) {
                    int t = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = t;
                }
            }
        }

        pthread_barrier_wait(&bar);
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    pthread_barrier_init(&bar, NULL, p);

    for (int i = 0; i < p; i++) {
        ar[i].id = i;
        pthread_create(&th[i], NULL, work, &ar[i]);
    }

    for (int i = 0; i < p; i++)
        pthread_join(th[i], NULL);

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    pthread_barrier_destroy(&bar);

    free(a);
    free(th);
    free(ar);

    return 0;
}