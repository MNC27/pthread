#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

int n, p;
int **g;
int **d;

typedef struct {
    int s;
    int e;
} Arg;

void dij(int src) {
    int *ds = malloc(n * sizeof(int));
    int *vis = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        ds[i] = INT_MAX;

    ds[src] = 0;

    for (int it = 0; it < n; it++) {

        int u = -1;
        int mn = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!vis[i] && ds[i] < mn) {
                mn = ds[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        vis[u] = 1;

        for (int v = 0; v < n; v++) {

            if (!vis[v] && g[u][v] != 0 &&
                ds[u] != INT_MAX &&
                ds[u] + g[u][v] < ds[v]) {

                ds[v] = ds[u] + g[u][v];
            }
        }
    }

    for (int i = 0; i < n; i++)
        d[src][i] = ds[i];

    free(ds);
    free(vis);
}

void *work(void *arg) {
    Arg *x = (Arg *)arg;

    for (int src = x->s; src < x->e; src++)
        dij(src);

    return NULL;
}

int main() {
    scanf("%d %d", &n, &p);

    g = malloc(n * sizeof(int *));
    d = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        g[i] = malloc(n * sizeof(int));
        d[i] = malloc(n * sizeof(int));

        for (int j = 0; j < n; j++)
            scanf("%d", &g[i][j]);
    }

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

    printf("All-Pairs Shortest Path:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (d[i][j] == INT_MAX)
                printf("INF ");
            else
                printf("%d ", d[i][j]);
        }

        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        free(g[i]);
        free(d[i]);
    }

    free(g);
    free(d);
    free(th);
    free(ar);

    return 0;
}