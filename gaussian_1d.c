#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
double **a;

pthread_barrier_t bar;

typedef struct {
    int s;
    int e;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    for (int k = 0; k < n; k++) {

        pthread_barrier_wait(&bar);

        for (int i = x->s; i < x->e; i++) {

            if (i <= k)
                continue;

            double f = a[i][k] / a[k][k];

            for (int j = k; j <= n; j++)
                a[i][j] -= f * a[k][j];
        }

        pthread_barrier_wait(&bar);
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++) {
        a[i] = malloc((n + 1) * sizeof(double));

        for (int j = 0; j <= n; j++)
            scanf("%lf", &a[i][j]);
    }

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    pthread_barrier_init(&bar, NULL, p);

    int x = (n + p - 1) / p;

    for (int i = 0; i < p; i++) {

        ar[i].s = i * x;
        ar[i].e = (i + 1) * x;

        if (ar[i].e > n)
            ar[i].e = n;

        pthread_create(&th[i], NULL, work, &ar[i]);
    }

    for (int i = 0; i < p; i++)
        pthread_join(th[i], NULL);

    printf("Upper triangular matrix:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++)
            printf("%.2lf ", a[i][j]);

        printf("\n");
    }

    pthread_barrier_destroy(&bar);

    for (int i = 0; i < n; i++)
        free(a[i]);

    free(a);
    free(th);
    free(ar);

    return 0;
}