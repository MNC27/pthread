#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

int n, p;
long long **d;

pthread_barrier_t bar;

typedef struct {
    int s;
    int e;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    for (int k = 0; k < n; k++) {

        for (int i = x->s; i < x->e; i++) {

            for (int j = 0; j < n; j++) {

                if (d[i][k] != LLONG_MAX &&
                    d[k][j] != LLONG_MAX &&
                    d[i][k] + d[k][j] < d[i][j]) {

                    d[i][j] = d[i][k] + d[k][j];
                }
            }
        }

        pthread_barrier_wait(&bar);
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    d = malloc(n * sizeof(long long *));

    for (int i = 0; i < n; i++) {

        d[i] = malloc(n * sizeof(long long));

        for (int j = 0; j < n; j++) {

            long long x;
            scanf("%lld", &x);

            if (i != j && x == 0)
                d[i][j] = LLONG_MAX;
            else
                d[i][j] = x;
        }
    }

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

    printf("All-Pairs Shortest Paths:\n");

    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n; j++) {

            if (d[i][j] == LLONG_MAX)
                printf("INF ");
            else
                printf("%lld ", d[i][j]);
        }

        printf("\n");
    }

    pthread_barrier_destroy(&bar);

    for (int i = 0; i < n; i++)
        free(d[i]);

    free(d);
    free(th);
    free(ar);

    return 0;
}