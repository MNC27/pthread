#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
double **a;
double *x;

pthread_barrier_t bar;

typedef struct {
    int s;
    int e;
} Arg;

void *work(void *arg) {
    Arg *v = (Arg *)arg;

    for (int i = n - 1; i >= 0; i--) {

        double sum = 0.0;

        for (int j = v->s; j < v->e; j++) {
            if (j > i)
                sum += a[i][j] * x[j];
        }

        static double *sums;
        static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

        pthread_mutex_lock(&mtx);

        if (sums == NULL)
            sums = calloc(p, sizeof(double));

        int id = v->s / ((n + p - 1) / p);

        sums[id] = sum;

        pthread_mutex_unlock(&mtx);

        pthread_barrier_wait(&bar);

        if (v->s == 0) {
            double z = 0.0;

            for (int k = 0; k < p; k++)
                z += sums[k];

            x[i] = (a[i][n] - z) / a[i][i];

            for (int k = 0; k < p; k++)
                sums[k] = 0.0;
        }

        pthread_barrier_wait(&bar);
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(double *));
    x = calloc(n, sizeof(double));

    for (int i = 0; i < n; i++) {
        a[i] = malloc((n + 1) * sizeof(double));

        for (int j = 0; j <= n; j++)
            scanf("%lf", &a[i][j]);
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

    printf("Solution:\n");

    for (int i = 0; i < n; i++)
        printf("x[%d] = %.4lf\n", i, x[i]);

    pthread_barrier_destroy(&bar);

    for (int i = 0; i < n; i++)
        free(a[i]);

    free(a);
    free(x);
    free(th);
    free(ar);

    return 0;
}