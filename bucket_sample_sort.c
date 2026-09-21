#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
int *a;
int **b;
int *sz;

typedef struct {
    int s;
    int e;
    int id;
} Arg;

int cmp(const void *x, const void *y) {
    int a = *(int *)x;
    int b = *(int *)y;

    return (a > b) - (a < b);
}

void *sample(void *arg) {
    Arg *x = (Arg *)arg;

    int id = x->id;

    for (int i = x->s; i < x->e; i++)
        a[i] = a[i];

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    qsort(a, n, sizeof(int), cmp);

    int *sp = malloc((p - 1) * sizeof(int));

    for (int i = 1; i < p; i++) {
        int pos = (long long)i * n / p;

        if (pos >= n)
            pos = n - 1;

        sp[i - 1] = a[pos];
    }

    b = malloc(p * sizeof(int *));

    sz = calloc(p, sizeof(int));

    for (int i = 0; i < p; i++)
        b[i] = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {

        int l = 0;
        int r = p - 1;

        while (l < r) {
            int m = (l + r) / 2;

            if (a[i] <= sp[m])
                r = m;
            else
                l = m + 1;
        }

        b[l][sz[l]++] = a[i];
    }

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    for (int i = 0; i < p; i++) {

        ar[i].id = i;

        pthread_create(&th[i], NULL, sample, &ar[i]);
    }

    for (int i = 0; i < p; i++)
        pthread_join(th[i], NULL);

    for (int i = 0; i < p; i++)
        qsort(b[i], sz[i], sizeof(int), cmp);

    int k = 0;

    for (int i = 0; i < p; i++) {
        for (int j = 0; j < sz[i]; j++)
            a[k++] = b[i][j];
    }

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    for (int i = 0; i < p; i++)
        free(b[i]);

    free(a);
    free(b);
    free(sz);
    free(sp);
    free(th);
    free(ar);

    return 0;
}