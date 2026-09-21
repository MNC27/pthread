#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, p;
int *a, *b;

int mx;

typedef struct {
    int s;
    int e;
} Arg;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    int cnt[10] = {0};

    for (int i = x->s; i < x->e; i++) {
        int d = (a[i] / mx) % 10;
        cnt[d]++;
    }

    return NULL;
}

void radix(int exp) {
    int *cnt = calloc(10, sizeof(int));

    for (int i = 0; i < n; i++) {
        int d = (a[i] / exp) % 10;
        cnt[d]++;
    }

    for (int i = 1; i < 10; i++)
        cnt[i] += cnt[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        int d = (a[i] / exp) % 10;
        b[cnt[d] - 1] = a[i];
        cnt[d]--;
    }

    for (int i = 0; i < n; i++)
        a[i] = b[i];

    free(cnt);
}

int main() {
    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(int));
    b = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    mx = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] > mx)
            mx = a[i];
    }

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    for (int i = 0; i < p; i++) {
        ar[i].s = i * n / p;
        ar[i].e = (i + 1) * n / p;
    }

    for (int exp = 1; mx / exp > 0; exp *= 10) {

        for (int i = 0; i < p; i++)
            pthread_create(&th[i], NULL, work, &ar[i]);

        for (int i = 0; i < p; i++)
            pthread_join(th[i], NULL);

        radix(exp);
    }

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    free(a);
    free(b);
    free(th);
    free(ar);

    return 0;
}