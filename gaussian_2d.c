#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int n, q, p;
double **a;

pthread_barrier_t bar;

typedef struct {
    int id;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    int id = x->id;

    int pr = id / q;
    int pc = id % q;

    int rs = pr * n / q;
    int re = (pr + 1) * n / q;

    if (pr == q - 1)
        re = n;

    for (int k = 0; k < n; k++) {

        pthread_barrier_wait(&bar);

        for (int i = rs; i < re; i++) {

            if (i <= k)
                continue;

            double f = a[i][k] / a[k][k];

            int cs = pc * (n + 1) / q;
            int ce = (pc + 1) * (n + 1) / q;

            if (pc == q - 1)
                ce = n + 1;

            for (int j = cs; j < ce; j++)
                a[i][j] -= f * a[k][j];
        }

        pthread_barrier_wait(&bar);
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &q);

    p = q * q;

    a = malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++) {
        a[i] = malloc((n + 1) * sizeof(double));

        for (int j = 0; j <= n; j++)
            scanf("%lf", &a[i][j]);
    }

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    pthread_barrier_init(&bar, NULL, p);

    for (int i = 0; i < p; i++) {
        ar[i].id = i;

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