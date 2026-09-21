#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int n, q, bs;
int **a, **b, **c;

pthread_barrier_t bar;

typedef struct {
    int id;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    int id = x->id;
    int r = id / q;
    int col = id % q;

    int **aa = malloc(bs * sizeof(int *));
    int **bb = malloc(bs * sizeof(int *));
    int **cc = malloc(bs * sizeof(int *));

    for (int i = 0; i < bs; i++) {
        aa[i] = calloc(bs, sizeof(int));
        bb[i] = calloc(bs, sizeof(int));
        cc[i] = calloc(bs, sizeof(int));
    }

    int ar = r * bs;
    int ac = ((col + r) % q) * bs;

    int br = ((r + col) % q) * bs;
    int bc = col * bs;

    for (int i = 0; i < bs; i++) {
        for (int j = 0; j < bs; j++) {
            aa[i][j] = a[ar + i][ac + j];
            bb[i][j] = b[br + i][bc + j];
        }
    }

    for (int step = 0; step < q; step++) {

        for (int i = 0; i < bs; i++) {
            for (int j = 0; j < bs; j++) {
                for (int k = 0; k < bs; k++) {
                    cc[i][j] += aa[i][k] * bb[k][j];
                }
            }
        }

        pthread_barrier_wait(&bar);

        int *tmp = malloc(bs * bs * sizeof(int));

        for (int i = 0; i < bs; i++)
            for (int j = 0; j < bs; j++)
                tmp[i * bs + j] = aa[i][j];

        int left = r * q + (col - 1 + q) % q;
        int right = r * q + (col + 1) % q;

        pthread_barrier_wait(&bar);

        for (int i = 0; i < bs; i++)
            for (int j = 0; j < bs; j++)
                aa[i][j] = tmp[i * bs + j];

        free(tmp);

        pthread_barrier_wait(&bar);

        int *tmp2 = malloc(bs * bs * sizeof(int));

        for (int i = 0; i < bs; i++)
            for (int j = 0; j < bs; j++)
                tmp2[i * bs + j] = bb[i][j];

        pthread_barrier_wait(&bar);

        for (int i = 0; i < bs; i++)
            for (int j = 0; j < bs; j++)
                bb[i][j] = tmp2[i * bs + j];

        free(tmp2);

        pthread_barrier_wait(&bar);
    }

    for (int i = 0; i < bs; i++) {
        for (int j = 0; j < bs; j++) {
            c[r * bs + i][col * bs + j] = cc[i][j];
        }
    }

    for (int i = 0; i < bs; i++) {
        free(aa[i]);
        free(bb[i]);
        free(cc[i]);
    }

    free(aa);
    free(bb);
    free(cc);

    return NULL;
}

int main() {
    scanf("%d", &n);

    q = (int)sqrt(n);

    if (q * q != n) {
        printf("n must be a perfect square\n");
        return 0;
    }

    bs = n / q;

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

    pthread_t *th = malloc(n * sizeof(pthread_t));
    Arg *ar = malloc(n * sizeof(Arg));

    pthread_barrier_init(&bar, NULL, n);

    for (int i = 0; i < n; i++) {
        ar[i].id = i;
        pthread_create(&th[i], NULL, work, &ar[i]);
    }

    for (int i = 0; i < n; i++)
        pthread_join(th[i], NULL);

    pthread_barrier_destroy(&bar);

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