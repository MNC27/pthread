#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
int **a, **b, **c;

typedef struct {
    int s;
    int e;
} Arg;

void *mul(void *arg) {
    Arg *x = (Arg *)arg;

    for (int i = x->s; i < x->e; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;

            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
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
        c[i] = malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &a[i][j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &b[i][j]);

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    int x = n / p;

    for (int i = 0; i < p; i++) {
        ar[i].s = i * x;
        ar[i].e = (i == p - 1) ? n : (i + 1) * x;

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