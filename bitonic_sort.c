#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
int *a;

pthread_barrier_t bar;

typedef struct {
    int s;
    int e;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    for (int k = 2; k <= n; k *= 2) {

        for (int j = k / 2; j > 0; j /= 2) {

            for (int i = x->s; i < x->e; i++) {

                int y = i ^ j;

                if (y > i) {

                    if ((i & k) == 0) {
                        if (a[i] > a[y]) {
                            int t = a[i];
                            a[i] = a[y];
                            a[y] = t;
                        }
                    } else {
                        if (a[i] < a[y]) {
                            int t = a[i];
                            a[i] = a[y];
                            a[y] = t;
                        }
                    }
                }
            }

            pthread_barrier_wait(&bar);
        }
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

    int z = (n + p - 1) / p;

    for (int i = 0; i < p; i++) {

        ar[i].s = i * z;
        ar[i].e = (i + 1) * z;

        if (ar[i].e > n)
            ar[i].e = n;

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