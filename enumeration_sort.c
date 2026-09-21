#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
int *a, *b;

typedef struct {
    int s;
    int e;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    for (int i = x->s; i < x->e; i++) {

        int pos = 0;

        for (int j = 0; j < n; j++) {

            if (a[j] < a[i])
                pos++;

            else if (a[j] == a[i] && j < i)
                pos++;
        }

        b[pos] = a[i];
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(int));
    b = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

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
        printf("%d ", b[i]);

    printf("\n");

    free(a);
    free(b);
    free(th);
    free(ar);

    return 0;
}