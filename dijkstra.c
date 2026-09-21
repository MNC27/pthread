#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

int n, p;
int **g;

int *d;
int *vis;

int mv;
int md;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int s;
    int e;
} Arg;

void *find_min(void *arg) {
    Arg *x = (Arg *)arg;

    int v = -1;
    int z = INT_MAX;

    for (int i = x->s; i < x->e; i++) {
        if (!vis[i] && d[i] < z) {
            z = d[i];
            v = i;
        }
    }

    if (v != -1) {
        pthread_mutex_lock(&mtx);

        if (d[v] < md) {
            md = d[v];
            mv = v;
        }

        pthread_mutex_unlock(&mtx);
    }

    return NULL;
}

void *relax(void *arg) {
    Arg *x = (Arg *)arg;

    for (int v = x->s; v < x->e; v++) {

        if (!vis[v] && g[mv][v] != 0) {

            if (d[mv] != INT_MAX &&
                d[mv] + g[mv][v] < d[v]) {

                d[v] = d[mv] + g[mv][v];
            }
        }
    }

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    g = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        g[i] = malloc(n * sizeof(int));

        for (int j = 0; j < n; j++)
            scanf("%d", &g[i][j]);
    }

    int src;
    scanf("%d", &src);

    d = malloc(n * sizeof(int));
    vis = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        d[i] = INT_MAX;

    d[src] = 0;

    pthread_t *th = malloc(p * sizeof(pthread_t));
    Arg *ar = malloc(p * sizeof(Arg));

    int z = (n + p - 1) / p;

    for (int it = 0; it < n; it++) {

        mv = -1;
        md = INT_MAX;

        for (int i = 0; i < p; i++) {

            ar[i].s = i * z;
            ar[i].e = (i + 1) * z;

            if (ar[i].e > n)
                ar[i].e = n;

            pthread_create(&th[i],
                           NULL,
                           find_min,
                           &ar[i]);
        }

        for (int i = 0; i < p; i++)
            pthread_join(th[i], NULL);

        if (mv == -1)
            break;

        vis[mv] = 1;

        for (int i = 0; i < p; i++)
            pthread_create(&th[i],
                           NULL,
                           relax,
                           &ar[i]);

        for (int i = 0; i < p; i++)
            pthread_join(th[i], NULL);
    }

    printf("Shortest distances from %d:\n", src);

    for (int i = 0; i < n; i++) {

        if (d[i] == INT_MAX)
            printf("%d -> %d = INF\n", src, i);
        else
            printf("%d -> %d = %d\n", src, i, d[i]);
    }

    for (int i = 0; i < n; i++)
        free(g[i]);

    free(g);
    free(d);
    free(vis);
    free(th);
    free(ar);

    pthread_mutex_destroy(&mtx);

    return 0;
}