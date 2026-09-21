#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
int **a, **b, **c;

pthread_mutex_t *mtx;

typedef struct {
    int s;
    int e;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    for (int id = x->s; id < x->e; id++) {

        int i = id / (n * n);
        int r = id % (n * n);

        int j = r / n;
        int k = r % n;

        int v = a[i][k] * b[k][j];

        pthread_mutex_lock(&mtx[i * n + j]);

        c[i][j] += v;

        pthread_mutex_unlock(&mtx[i * n + j]);
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(int *));
    b = malloc(n * sizeof(int *));
    c = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        a[i] = malloc(n * sizeof(int));
        b[i] = malloc(n * sizeof(int));
        c[i] = calloc(n, sizeof(int));
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &a[i][j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &b[i][j]);

    mtx = malloc(n * n * sizeof(pthread_mutex_t));

    for (int i = 0; i < n * n; i++)
        pthread_mutex_init(&mtx[i], NULL);

    int tot = n * n * n;

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    int x = (tot + p - 1) / p;

    for (int i = 0; i < p; i++) {

        ar[i].s = i * x;
        ar[i].e = (i + 1) * x;

        if (ar[i].e > tot)
            ar[i].e = tot;

        pthread_create(&th[i], NULL, work, &ar[i]);
    }

    for (int i = 0; i < p; i++)
        pthread_join(th[i], NULL);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", c[i][j]);

        printf("\n");
    }

    for (int i = 0; i < n * n; i++)
        pthread_mutex_destroy(&mtx[i]);

    for (int i = 0; i < n; i++) {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }

    free(a);
    free(b);
    free(c);
    free(mtx);
    free(th);
    free(ar);

    return 0;
}