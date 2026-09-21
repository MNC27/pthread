#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *a;
int n, p;

typedef struct {
    int l;
    int r;
} Arg;

void qs(int l, int r, int d);

int part(int l, int r) {
    int x = a[r];
    int i = l - 1;

    for (int j = l; j < r; j++) {
        if (a[j] <= x) {
            i++;

            int t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }

    int t = a[i + 1];
    a[i + 1] = a[r];
    a[r] = t;

    return i + 1;
}

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    qs(x->l, x->r, 1);

    free(x);

    return NULL;
}

void qs(int l, int r, int d) {
    if (l >= r)
        return;

    int m = part(l, r);

    pthread_t th;
    int created = 0;

    if (d < p && m - l > r - m) {

        Arg *x = malloc(sizeof(Arg));

        x->l = l;
        x->r = m - 1;

        pthread_create(&th, NULL, work, x);

        created = 1;

        qs(m + 1, r, d + 1);

    } else if (d < p) {

        Arg *x = malloc(sizeof(Arg));

        x->l = m + 1;
        x->r = r;

        pthread_create(&th, NULL, work, x);

        created = 1;

        qs(l, m - 1, d + 1);

    } else {

        qs(l, m - 1, d);
        qs(m + 1, r, d);
    }

    if (created)
        pthread_join(th, NULL);
}

int main() {
    scanf("%d %d", &n, &p);

    a = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    qs(0, n - 1, 0);

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");

    free(a);

    return 0;
}