#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p, bs;
int **a, **b, **c;

typedef struct {
    int s;
    int e;
} Arg;

void *mul(void *arg) {
    Arg *x = (Arg *)arg;

    for (int bi = x->s; bi < x->e; bi++) {
        int br = bi / (n / bs);
        int bc = bi % (n / bs);

        int r1 = br * bs;
        int r2 = r1 + bs;
        int c1 = bc * bs;
        int c2 = c1 + bs;

        if (r2 > n) r2 = n;
        if (c2 > n) c2 = n;

        for (int i = r1; i < r2; i++) {
            for (int j = c1; j < c2; j++) {
                c[i][j] = 0;

                for (int k = 0; k < n; k++)
                    c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return NULL;
}

int main() {
    scanf("%d %d %d", &n, &p, &bs);

    a = malloc(n * sizeof(int *));
    b = malloc(n * sizeof(int *));
    c = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        a[i] = malloc(n * sizeof(int));
        b[i] = malloc(n * sizeof(int));
        c[i] = malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &a[i][j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &b[i][j]);

    int nb = (n + bs - 1) / bs;
    int tot = nb * nb;

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    int x = (tot + p - 1) / p;

    for (int i = 0; i < p; i++) {
        ar[i].s = i * x;
        ar[i].e = (i + 1) * x;

        if (ar[i].e > tot)
            ar[i].e = tot;

        pthread_create(&th[i], NULL, mul, &ar[i]);
    }

    for (int i = 0; i < p; i++)
        pthread_join(th[i], NULL);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", c[i][j]);
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }

    free(a);
    free(b);
    free(c);
    free(th);
    free(ar);

    return 0;
}