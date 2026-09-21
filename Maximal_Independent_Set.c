#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, m, p;

int **g;
int *deg;

int *active;
int *mis;

int *win;
int *rem;

typedef struct {
    int s;
    int e;
} Arg;

pthread_barrier_t bar;

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    for (int u = x->s; u < x->e; u++) {

        if (!active[u])
            continue;

        int ok = 1;

        for (int i = 0; i < deg[u]; i++) {
            int v = g[u][i];

            if (active[v] && v < u) {
                ok = 0;
                break;
            }
        }

        win[u] = ok;
    }

    pthread_barrier_wait(&bar);

    for (int u = x->s; u < x->e; u++) {

        if (!win[u])
            continue;

        mis[u] = 1;
        rem[u] = 1;

        for (int i = 0; i < deg[u]; i++) {
            int v = g[u][i];
            rem[v] = 1;
        }
    }

    pthread_barrier_wait(&bar);

    for (int u = x->s; u < x->e; u++) {
        if (rem[u])
            active[u] = 0;
    }

    pthread_barrier_wait(&bar);

    return NULL;
}

int main() {
    scanf("%d %d %d", &n, &m, &p);

    g = malloc(n * sizeof(int *));
    deg = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        g[i] = malloc(n * sizeof(int));

    for (int i = 0; i < m; i++) {

        int u, v;

        scanf("%d %d", &u, &v);

        g[u][deg[u]++] = v;
        g[v][deg[v]++] = u;
    }

    active = malloc(n * sizeof(int));
    mis = calloc(n, sizeof(int));

    win = calloc(n, sizeof(int));
    rem = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        active[i] = 1;

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    pthread_barrier_init(&bar, NULL, p);

    int z = (n + p - 1) / p;

    int left = n;

    while (left > 0) {

        for (int i = 0; i < n; i++) {
            win[i] = 0;
            rem[i] = 0;
        }

        for (int i = 0; i < p; i++) {

            ar[i].s = i * z;
            ar[i].e = (i + 1) * z;

            if (ar[i].e > n)
                ar[i].e = n;

            pthread_create(&th[i], NULL, work, &ar[i]);
        }

        for (int i = 0; i < p; i++)
            pthread_join(th[i], NULL);

        left = 0;

        for (int i = 0; i < n; i++)
            if (active[i])
                left++;
    }

    printf("Maximal Independent Set:\n");

    for (int i = 0; i < n; i++) {
        if (mis[i])
            printf("%d ", i);
    }

    printf("\n");

    pthread_barrier_destroy(&bar);

    for (int i = 0; i < n; i++)
        free(g[i]);

    free(g);
    free(deg);
    free(active);
    free(mis);
    free(win);
    free(rem);
    free(th);
    free(ar);

    return 0;
}